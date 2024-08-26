#include "SocketClientBPLibrary.h"
#include "SocketClient.h"

USocketClientBPLibrary* USocketClientBPLibrary::socketClientBPLibrary;

USocketClientBPLibrary::USocketClientBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) 
{

	socketClientBPLibrary = this;

	if (socketClientCleanerThread == nullptr)
	{
		socketClientCleanerThread = new FSocketClientCleanerThread();
	}
}

USocketClientBPLibrary::~USocketClientBPLibrary() {

}

void USocketClientBPLibrary::SocketClientTCPConnectionEventDelegate(const bool success, const FString message, const FString clientConnectionID)
{
}

void USocketClientBPLibrary::ReceiveTCPMessageEventDelegate(const FString message, const TArray<uint8>& byteArray, const FString clientConnectionID)
{
}

USocketClientBPLibrary* USocketClientBPLibrary::GetSocketClientTarget()
{
	return socketClientBPLibrary;
}

ISocketSubsystem* USocketClientBPLibrary::getSocketSubSystem()
{
	switch (USocketClientBPLibrary::GetSocketClientTarget()->systemSocketPlatform)
	{
	case ESocketPlatformClient::E_SSC_SYSTEM:
		return ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

	case ESocketPlatformClient::E_SSC_WINDOWS:
		return ISocketSubsystem::Get(FName(TEXT("WINDOWS")));

	default:
		return ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	}
}

void USocketClientBPLibrary::getTcpSeparator(uint8& byteSeparator, FString& stringSeparator)
{
	stringSeparator = tcpStringSeparator;
	byteSeparator = tcpByteSeparator;
}

FString USocketClientBPLibrary::getLocalIP()
{
	bool canBind = false;
	TSharedRef<FInternetAddr> localIp = USocketClientBPLibrary::GetSocketClientTarget()->getSocketSubSystem()->GetLocalHostAddr(*GLog, canBind);

	if (localIp->IsValid())
	{
		FString localIP = localIp->ToString(false);
		if (localIP.Equals("127.0.0.1"))
		{
			UE_LOG(LogTemp, Error, TEXT("Could not detect the local IP."));
			return "0.0.0.0";
		}
		return localIP;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not detect the local IP."));
	}

	return "0.0.0.0";
}

void USocketClientBPLibrary::connectSocketClientTCP(FString domainOrIP, ESocketClientIPType ipType, int32 port, EReceiveFilterClient receiveFilters,
	ESocketClientTCPSeparator messageSeparator, FString& connectionID, bool disableNaglesAlgorithm)
{
	USocketClientBPLibrary::GetSocketClientTarget()->connectSocketClientTCPNonStatic(domainOrIP, ipType, port, receiveFilters,
		messageSeparator, connectionID, nullptr, disableNaglesAlgorithm);
}

void USocketClientBPLibrary::connectSocketClientTCPNonStatic(FString domainOrIP, ESocketClientIPType ipType, int32 port, EReceiveFilterClient receiveFilters, ESocketClientTCPSeparator messageSeparator, FString& connectionID, UTCPConnectAsyncNode* tcpConnectAsyncNode, bool disableNaglesAlgorithm)
{
	USocketClientTCPClient* tcpClient = NewObject<USocketClientTCPClient>(USocketClientTCPClient::StaticClass());

	connectionID = FGuid::NewGuid().ToString();
	if (tcpClients.Find(connectionID) != nullptr)
	{
		FString serverMessage = "An existing connection with this ID was found. Connection establishment was aborted.";
		UE_LOG(LogTemp, Warning, TEXT("%s"), *serverMessage);
		onSocketClientTCPConnectionEventDelegate.Broadcast(false, serverMessage, connectionID);

		tcpClient->onSocketClientTCPConnectionEventDelegate.Broadcast(false, serverMessage, connectionID);
		if (tcpConnectAsyncNode != nullptr)
		{
			tcpConnectAsyncNode->triggerConnectionEvent(false, connectionID, serverMessage);
		}

		return;
	}

	tcpClients.Add(connectionID, tcpClient);
	tcpClient->connect(this, domainOrIP, ipType, port, receiveFilters, messageSeparator, connectionID, tcpConnectAsyncNode);
}

void USocketClientBPLibrary::socketClientSendTCP(FString connectionID, FString message, TArray<uint8> byteArray, bool addLineBreak)
{
	USocketClientBPLibrary::GetSocketClientTarget()->socketClientSendTCPNonStatic(connectionID, message, byteArray, addLineBreak);
}

void USocketClientBPLibrary::socketClientSendTCPNonStatic(FString connectionID, FString message, TArray<uint8> byteArray, bool addLineBreak)
{
	if (connectionID.IsEmpty() || tcpClients.Find(connectionID) == nullptr)
	{
		if (((FDateTime::Now().GetTicks()) - lastErrorMessageTime) >= 10000000)
		{
			UE_LOG(LogTemp, Error, TEXT("Connection not found (socketClientSendTCPMessage). %s"), *connectionID);
			lastErrorMessageTime = FDateTime::Now().GetTicks();
		}
		return;
	}

	if (message.Len() > 0)
	{
		if (addLineBreak)
		{
			message.Append("\r\n");
		}
	}

	USocketClientTCPClient* tcpClient = *tcpClients.Find(connectionID);
	tcpClient->sendMessage(message, byteArray);
}

void USocketClientBPLibrary::closeSocketClientConnectionTCP(FString connectionID)
{
	USocketClientBPLibrary::GetSocketClientTarget()->closeSocketClientConnectionTCPNonStatic(connectionID);
}

void USocketClientBPLibrary::closeSocketClientConnectionTCPNonStatic(FString connectionID)
{
	if (connectionID.IsEmpty())
	{
		return;
	}

	if (tcpClients.Find(connectionID) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Connection not found (closeSocketClientTCPConnection). %s"), *connectionID);
		return;
	}

	USocketClientTCPClient* tcpClient = *tcpClients.Find(connectionID);
	if (tcpClient != nullptr)
	{
		if (tcpClient->isRun())
		{
			tcpClient->closeConnection();
		}
	}
	tcpClients.Remove(connectionID);
	tcpClient = nullptr;
}

void USocketClientBPLibrary::closeAllSocketClientConnectionsTCP()
{
	USocketClientBPLibrary::GetSocketClientTarget()->closeAllSocketClientConnectionsTCPNonStatic();
}

void USocketClientBPLibrary::closeAllSocketClientConnectionsTCPNonStatic()
{
	TArray<FString> tmpArray;
	tcpClients.GetKeys(tmpArray);

	for (int32 i = 0; i < tmpArray.Num(); ++i)
	{
		closeSocketClientConnectionTCPNonStatic(tmpArray[i]);
	}
}

void USocketClientBPLibrary::getTCPConnectionByConnectionID(FString connectionID, bool& found, USocketClientTCPClient*& connection)
{
	USocketClientBPLibrary::GetSocketClientTarget()->getTCPConnectionByConnectionIDNonStatic(connectionID, found, connection);
}

void USocketClientBPLibrary::getTCPConnectionByConnectionIDNonStatic(FString connectionID, bool& found, USocketClientTCPClient*& connection)
{
	if (connectionID.IsEmpty() || tcpClients.Find(connectionID) == nullptr)
	{
		found = false;
		connection = nullptr;
		return;
	}
	
	found = true;
	connection = *tcpClients.Find(connectionID);
}

bool USocketClientBPLibrary::isTCPConnected(FString connectionID)
{
	return USocketClientBPLibrary::GetSocketClientTarget()->isTCPConnectedNonStatic(connectionID);
}

bool USocketClientBPLibrary::isTCPConnectedNonStatic(FString connectionID)
{
	if (connectionID.IsEmpty() || tcpClients.Find(connectionID) == nullptr)
	{
		return false;
	}

	USocketClientTCPClient* tcpClient = *tcpClients.Find(connectionID);
	if (tcpClient->isRun() && tcpClient->isConnected())
	{
		return true;
	}

	return false;
}

FString USocketClientBPLibrary::resolveDomain(FString domain, ESocketClientIPType ipType)
{
	FString* cachedDomainPointer = domainCache.Find(domain);
	if (cachedDomainPointer != nullptr)
	{
		return *cachedDomainPointer;
	}

	if (ipType == ESocketClientIPType::E_ipv4)
	{
		TArray<FString> ipNumbers;
		int32 lineCount = domain.ParseIntoArray(ipNumbers, TEXT("."), true);
		if (lineCount == 4 && domain.Len() <= 15 && domain.Len() >= 7)
		{
			domainCache.Add(domain, domain);
			return domain;
		}
	}

	if (ipType == ESocketClientIPType::E_ipv6)
	{
		TArray<FString> ipNumbers;
		int32 lineCount = domain.ParseIntoArray(ipNumbers, TEXT(":"), true);
		if (lineCount >= 2)
		{
			domainCache.Add(domain, domain);
			return domain;
		}
	}

	return domain;
}

void USocketClientBPLibrary::cleanConnection(FSocketClientPluginSession& session)
{
	socketClientCleanerThread->addSession(session);
}