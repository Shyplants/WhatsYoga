#include "SocketClientTCPSendDataThread.h"

FSocketClientTCPSendDataThread::FSocketClientTCPSendDataThread(USocketClientBPLibrary* InSocketClientLib, USocketClientTCPClient* InTcpClient, FString InClientConnectionID):
	socketClientLib(InSocketClientLib),
	tcpClient(InTcpClient),
	clientConnectionID(InClientConnectionID)
{
	FString threadName = "FSendDataToServerThread" + FGuid::NewGuid().ToString();
	thread = FRunnableThread::Create(this, *threadName, 0, EThreadPriority::TPri_Normal);
}

FSocketClientTCPSendDataThread::~FSocketClientTCPSendDataThread()
{
	delete thread;
}

uint32 FSocketClientTCPSendDataThread::Run()
{
	if (tcpClient == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Class is not initialized."));
		return 0;
	}

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
		UE_LOG(LogTemp, Warning, TEXT("Socket Client Plugin: Separtor mode is set to String but there is no String Separtor."));
	}


	FSocket* socket = tcpClient->getSocket();
	int32 sent = 0;

	while (tcpClient->isRun())
	{
		if (socket == NULL || socket == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Connection not exist."));
			break;
		}

		if (socket != nullptr && tcpClient->isRun())
		{
			while (!messageQueue.IsEmpty())
			{
				FString m;
				messageQueue.Dequeue(m);
				FTCHARToUTF8 Convert(*m);
				sent = 0;

				TArray<uint8> byteCache;

				switch (messageWrapping)
				{
				case ESocketClientTCPSeparator::E_None:
					byteCache.Append((uint8*)Convert.Get(), Convert.Length());
					break;

				case ESocketClientTCPSeparator::E_ByteSeparator:
					byteCache.Append((uint8*)Convert.Get(), Convert.Length());
					byteCache.Add(byteSeparator);
					break;
				case ESocketClientTCPSeparator::E_StringSeparator:
				{
					m += stringSeparator;
					FTCHARToUTF8 ConvertWithSeparator(*m);
					byteCache.Append((uint8*)ConvertWithSeparator.Get(), ConvertWithSeparator.Length());
				}
					break;

				case ESocketClientTCPSeparator::E_LengthSeparator:
					if (FGenericPlatformProperties::IsLittleEndian())
					{
						byteCache.Add(0x00);
					}
					else
					{
						byteCache.Add(0x01);
					}
					byteCache.SetNum(5);
					int32 dataLength = Convert.Length();
					FMemory::Memcpy(byteCache.GetData() + 1, &dataLength, 4);
					byteCache.Append((uint8*)Convert.Get(), Convert.Length());
					break;
				}

				socket->Send(byteCache.GetData(), byteCache.Num(), sent);
			}

			while (!byteArrayQueue.IsEmpty())
			{
				TArray<uint8> byteCache;
				byteArrayQueue.Dequeue(byteCache);
				sent = 0;

				switch (messageWrapping)
				{
				case ESocketClientTCPSeparator::E_ByteSeparator:
					byteCache.Add(byteSeparator);
					break;

				case ESocketClientTCPSeparator::E_StringSeparator:
				{
					FTCHARToUTF8 ConvertWithSeperator(*stringSeparator);
					byteCache.Append((uint8*)ConvertWithSeperator.Get(), ConvertWithSeperator.Length());
				}
				break;

				case ESocketClientTCPSeparator::E_LengthSeparator:
					byteCache.InsertZeroed(0, 5);
					if (!FGenericPlatformProperties::IsLittleEndian())
					{
						uint8 a = 0x01;
						FMemory::Memcpy(byteCache.GetData(), &a, 1);
					}
					int32 dataLength = byteCache.Num() - 5;
					FMemory::Memcpy(byteCache.GetData() + 1, &dataLength, 4);
					break;
				}
				
				socket->Send(byteCache.GetData(), byteCache.Num(), sent);
			}
		}

		if (tcpClient->isRun())
		{
			pauseThread(true);
			while (paused && tcpClient->isRun())
			{
				FPlatformProcess::Sleep(0.01f);
			}
		}
	}
	return 0;
}

void FSocketClientTCPSendDataThread::sendMessage(FString message, TArray<uint8> byteArray)
{
	if (message.Len() > 0)
	{
		messageQueue.Enqueue(message);
	}
	if (byteArray.Num() > 0)
	{
		byteArrayQueue.Enqueue(byteArray);
	}
	pauseThread(false);
}

void FSocketClientTCPSendDataThread::pauseThread(bool pause)
{
	paused = pause;
	if (thread != nullptr)
	{
		thread->Suspend(paused);
	}
}
