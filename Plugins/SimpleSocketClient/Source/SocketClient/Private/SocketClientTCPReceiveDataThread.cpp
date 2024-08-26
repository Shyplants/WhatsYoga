#include "SocketClientTCPReceiveDataThread.h"

FSocketClientTCPReceiveDataThread::FSocketClientTCPReceiveDataThread(USocketClientBPLibrary* InSocketClientBPLibrary, FString InClientConnectionID, EReceiveFilterClient InReceiveFilter,
	FString InIPorDomain, ESocketClientIPType InIPType, int32 InPort, USocketClientTCPClient* InTcpClient, bool InNoPacketDelay, bool InNoPacketBlocking) :
	socketClientBPLibrary(InSocketClientBPLibrary),
	clientConnectionID(InClientConnectionID),
	receiveFilter(InReceiveFilter),
	ipOrDomain(InIPorDomain),
	ipType(InIPType),
	port(InPort),
	tcpClient(InTcpClient),
	noPacketDelay(InNoPacketDelay),
	noPacketBlocking(InNoPacketBlocking)
{
	FString threadName = "FServerConnectionThread" + FGuid::NewGuid().ToString();
	thread = FRunnableThread::Create(this, *threadName, 0, EThreadPriority::TPri_Normal);
}

FSocketClientTCPReceiveDataThread::~FSocketClientTCPReceiveDataThread()
{
	if (tcpClient != nullptr && tcpClient->tcpConnectAsyncNode != nullptr)
	{
		tcpClient->tcpConnectAsyncNode = nullptr;
	}
	
	delete thread;
}

uint32 FSocketClientTCPReceiveDataThread::Run()
{
	FString ip = socketClientBPLibrary->resolveDomain(ipOrDomain, ipType);
	int32 portGlobal = port;
	FString clientConnectionIDGlobal = clientConnectionID;
	USocketClientBPLibrary* socketClientGlobal = socketClientBPLibrary;
	USocketClientTCPClient* tcpClientGlobal = tcpClient;

	// message wrapping
	FString stringSeparator = FString();
	uint8 byteSeparator = 0x00;
	ESocketClientTCPSeparator messageWrapping = ESocketClientTCPSeparator::E_None;
	tcpClient->getTcpSeparator(stringSeparator, byteSeparator, messageWrapping);

	TArray<TCHAR> stringSeparatorArray = stringSeparator.GetCharArray();
	if (stringSeparatorArray.Num() > 0 && stringSeparatorArray.Last() == 0x00)
	{
		stringSeparatorArray.RemoveAt(stringSeparatorArray.Num() - 1, 1, true);
	}

	if (messageWrapping == ESocketClientTCPSeparator::E_LengthSeparator && stringSeparatorArray.Num() == 0)
	{
		messageWrapping = ESocketClientTCPSeparator::E_None;
		UE_LOG(LogTemp, Warning, TEXT("Socket Client Plugin: Separator mode is set to String but there is no String Separator."));
	}

	ISocketSubsystem* sSS = USocketClientBPLibrary::getSocketSubSystem();
	if (sSS == nullptr)
	{
		FString info = "Connection failed(1). SocketSubSystem does not exist.:" + FString::FromInt(portGlobal) + " " + clientConnectionIDGlobal;
		triggerConnectionEvent(false, clientConnectionID, info, tcpClientGlobal, socketClientGlobal);
		return 0;
	}

	TSharedRef<FInternetAddr> addr = sSS->CreateInternetAddr();
	bool bIsValid;
	addr->SetIp(*ip, bIsValid);
	addr->SetPort(port);

	if (bIsValid)
	{
		// create the socket
		FSocket* socket = sSS->CreateSocket(NAME_Stream, TEXT("socketClient"), addr->GetProtocolType());
		tcpClient->setSocket(socket);

		// socket options
		if (socket != nullptr)
		{
			socket->SetNoDelay(noPacketDelay);
			socket->SetNonBlocking(noPacketBlocking);

			tcpClient->setRun(true);
		}

		// try to connect to the server
		if (socket == nullptr || socket->Connect(*addr) == false)
		{
			const TCHAR* socketErr = sSS->GetSocketError(SE_GET_LAST_ERROR_CODE);
			FString info = "Connection failed(2):" + FString(socketErr) + "|" + ip + ":" + FString::FromInt(portGlobal) + "|" + clientConnectionIDGlobal;
			triggerConnectionEvent(false, clientConnectionID, info, tcpClientGlobal, socketClientGlobal);
		}
		else
		{
			FString info = "Connection successful" + ip + ":" + FString::FromInt(portGlobal) + "|" + clientConnectionIDGlobal;
			triggerConnectionEvent(true, clientConnectionID, info, tcpClientGlobal, socketClientGlobal);

			tcpClient->createSendThread();
			int64 ticks1;
			int64 ticks2;
			TArray<uint8> byteDataArray;
			TArray<uint8> byteDataArrayCache;
			FString mainMessage;
			bool isCollectMessageStatus = false;
			int32 lastDataLengthFromHeader = 0;


			uint32 dataSize;
			while (socket != nullptr && tcpClient->isRun())
			{
				ticks1 = FDateTime::Now().GetTicks();
				socket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromSeconds(1));
				ticks2 = FDateTime::Now().GetTicks();

				bool hasData = socket->HasPendingData(dataSize);
				if (!hasData && ticks1 == ticks2)
				{
					UE_LOG(LogTemp, Display, TEXT("Socket Client: Connection aborted or closed by the server: %s"), *clientConnectionID);
					break;
				}

				if (hasData)
				{
					TArray<uint8> dataFromSocket;
					dataFromSocket.SetNumUninitialized(dataSize);
					int32 BytesRead = 0;
					if (socket->Recv(dataFromSocket.GetData(), dataFromSocket.Num(), BytesRead))
					{
						switch (messageWrapping)
						{
						case ESocketClientTCPSeparator::E_None:
							triggerMessageEvent(dataFromSocket, clientConnectionIDGlobal, tcpClientGlobal, socketClientGlobal);
							break;

						case ESocketClientTCPSeparator::E_ByteSeparator:
							for (int32 i = 0; i < dataFromSocket.Num(); ++i)
							{
								byteDataArrayCache.Add(dataFromSocket[i]);
								if (dataFromSocket[i] == byteSeparator)
								{
									triggerMessageEvent(byteDataArrayCache, clientConnectionIDGlobal, tcpClientGlobal, socketClientGlobal, false);
									byteDataArrayCache.Empty();
								}
							}
							break;

						case ESocketClientTCPSeparator::E_StringSeparator:
							for (int32 i = 0; i < dataFromSocket.Num(); ++i)
							{
								if ((TCHAR)dataFromSocket[i] == stringSeparatorArray[0])
								{
									bool found = true;

									if (i + stringSeparatorArray.Num() > dataFromSocket.Num())
									{
										found = false;
									}
									
									for (int32 j = 1; found && j < stringSeparatorArray.Num(); ++j)
									{
										if ((TCHAR)dataFromSocket[i+j] != stringSeparatorArray[j])
										{
											found = false;
										}
									}

									if (found)
									{
										triggerMessageEvent(byteDataArrayCache, clientConnectionIDGlobal, tcpClientGlobal, socketClientGlobal, true);
										byteDataArrayCache.Empty();
									}
									else
									{
										byteDataArrayCache.Add(dataFromSocket[i]);
									}
								}
								else
								{
									byteDataArrayCache.Add(dataFromSocket[i]);
								}
							}
							break;

						case ESocketClientTCPSeparator::E_LengthSeparator:
							if (lastDataLengthFromHeader == 0 && dataFromSocket.Num() >= 5) {
								
								tcpClient->readDataLength(dataFromSocket, lastDataLengthFromHeader);

								if (dataFromSocket.Num() == 5)
								{
									dataFromSocket.Empty();
									continue;
								}

								byteDataArrayCache.Append(dataFromSocket.GetData() + 5, dataFromSocket.Num() - 5);
								dataFromSocket.Empty();
							}
							else
							{
								byteDataArrayCache.Append(dataFromSocket.GetData(), dataFromSocket.Num());
							}

							int32 maxLoops = 1000;

							while (byteDataArrayCache.Num() > 0 && byteDataArrayCache.Num() >= lastDataLengthFromHeader && maxLoops > 0)
							{
								--maxLoops;

								byteDataArray.Append(byteDataArrayCache.GetData(), lastDataLengthFromHeader);
								byteDataArrayCache.RemoveAt(0, lastDataLengthFromHeader, true);

								triggerMessageEvent(byteDataArray, clientConnectionIDGlobal, tcpClientGlobal, socketClientGlobal);
								byteDataArray.Empty();

								if (byteDataArrayCache.Num() == 0)
								{
									lastDataLengthFromHeader = 0;
									break;
								}

								if (byteDataArrayCache.Num() > 5)
								{
									tcpClient->readDataLength(byteDataArrayCache, lastDataLengthFromHeader);
									byteDataArrayCache.RemoveAt(0, 5, true);
								}
							}
							break;
						}
					}
					mainMessage.Empty();
					byteDataArray.Empty();
					dataFromSocket.Empty();
				}
			}

			info = "Connection close:" + ip + ":" + FString::FromInt(portGlobal) + "|" + clientConnectionIDGlobal;
			triggerConnectionEvent(false, clientConnectionID, info, tcpClientGlobal, socketClientGlobal);
		}

		if (socket != nullptr)
		{
			socket->Close();
		}

		if (tcpClient->isRun())
		{
			socketClientBPLibrary->closeSocketClientConnectionTCPNonStatic(clientConnectionID);
		}
	}
	else
	{
		FString info = "Connection failed(3). IP not Valid:" + ip + ":" + FString::FromInt(portGlobal) + "|" + clientConnectionIDGlobal;
		triggerConnectionEvent(false, clientConnectionID, info, tcpClientGlobal, socketClientGlobal);
	}

	return 0;
}

void FSocketClientTCPReceiveDataThread::triggerConnectionEvent(bool success, FString clientConnectionIDGlobal, FString serverMessage, USocketClientTCPClient* tcpClientGlobal,
	USocketClientBPLibrary* socketClinetGlobal)
{
	AsyncTask(ENamedThreads::GameThread, [success, serverMessage, clientConnectionIDGlobal, tcpClientGlobal, socketClinetGlobal]() {
		if (socketClinetGlobal != nullptr)
		{
			socketClinetGlobal->onSocketClientTCPConnectionEventDelegate.Broadcast(success, serverMessage, clientConnectionIDGlobal);
		}

		if (tcpClientGlobal != nullptr)
		{
			tcpClientGlobal->onSocketClientTCPConnectionEventDelegate.Broadcast(success, serverMessage, clientConnectionIDGlobal);
			if (tcpClientGlobal->tcpConnectAsyncNode != nullptr)
			{
				tcpClientGlobal->tcpConnectAsyncNode->triggerConnectionEvent(success, clientConnectionIDGlobal, serverMessage);
			}
		}
	});
}

void FSocketClientTCPReceiveDataThread::triggerMessageEvent(TArray<uint8>& byteDataArray, FString& clientConnectionIDGlobal, USocketClientTCPClient* tcpClientGlobal,
	USocketClientBPLibrary* socketClientGlobal, bool addNullTerminator)
{
	FString mainMessage = FString();
	if (receiveFilter == EReceiveFilterClient::E_SAB || receiveFilter == EReceiveFilterClient::E_S)
	{
		if (addNullTerminator)
		{
			byteDataArray.Add(0x00); // null-terminator
		}
		mainMessage = FString(UTF8_TO_TCHAR((char*)byteDataArray.GetData()));
		if (receiveFilter == EReceiveFilterClient::E_S)
		{
			byteDataArray.Empty();
		}
	}

	AsyncTask(ENamedThreads::GameThread, [mainMessage, byteDataArray, clientConnectionIDGlobal, tcpClientGlobal, socketClientGlobal]()
		{
			if (socketClientGlobal != nullptr)
			{
				socketClientGlobal->onReceiveTCPMessageEventDelegate.Broadcast(mainMessage, byteDataArray, clientConnectionIDGlobal);
			}
			if (tcpClientGlobal != nullptr)
			{
				tcpClientGlobal->onReceiveTCPMessageEventDelegate.Broadcast(mainMessage, byteDataArray, clientConnectionIDGlobal);
				if (tcpClientGlobal->tcpConnectAsyncNode != nullptr)
				{
					tcpClientGlobal->tcpConnectAsyncNode->triggerMessageEvent(byteDataArray, clientConnectionIDGlobal, mainMessage);
				}
			}
		});
	mainMessage.Empty();
}