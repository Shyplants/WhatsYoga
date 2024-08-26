#include "SocketClientTCP.h"

USocketClientTCPClient::USocketClientTCPClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	this->AddToRoot();
	onSocketClientTCPConnectionEventDelegate.AddDynamic(this, &USocketClientTCPClient::connectionEvent);
}

void USocketClientTCPClient::socketClientTCPConnectionEventDelegate(const bool success, const FString message, const FString clientConnectionID)
{
}

void USocketClientTCPClient::receiveTCPMessageEventDelegate(const FString message, const TArray<uint8>& byteArray, const FString clientConnectionID)
{
}

void USocketClientTCPClient::connectionEvent(bool success, FString message, FString clientConnectionID)
{
	connected = success;
}

void USocketClientTCPClient::connect(USocketClientBPLibrary* InMainLib, FString InDomainOrIP, ESocketClientIPType ipType, int32 port, EReceiveFilterClient receiveFilter,
	ESocketClientTCPSeparator InMessageWrapping, FString InConnectionID, UTCPConnectAsyncNode* InTcpConnectAsyncNode, bool noPacketDelay, bool noPacketBlocking)
{
	mainLib = InMainLib;
	connectionID = InConnectionID;
	messageWrapping = InMessageWrapping;
	tcpConnectAsyncNode = InTcpConnectAsyncNode;
	USocketClientBPLibrary::socketClientBPLibrary->getTcpSeparator(tcpByteSeparator, tcpStringSeparator);
	tcpReceiveDataThread = new FSocketClientTCPReceiveDataThread(mainLib, connectionID, receiveFilter, InDomainOrIP, ipType, port, this, noPacketDelay, noPacketBlocking);
}

void USocketClientTCPClient::sendMessage(FString message, TArray<uint8> byteArray)
{
	if (run && tcpSendThread != nullptr)
	{
		tcpSendThread->sendMessage(message, byteArray);
	}
}

void USocketClientTCPClient::closeConnection()
{
	setRun(false);

	if (tcpSendThread != nullptr)
	{
		tcpSendThread->pauseThread(false);
	}

	if (mainLib != nullptr)
	{
		FSocketClientPluginSession connectionSession = FSocketClientPluginSession();
		connectionSession.tcpReceiveThread = tcpReceiveDataThread;
		connectionSession.tcpSendThread = tcpSendThread;
		connectionSession.socket = socket;
		connectionSession.clientID = connectionID;
		mainLib->cleanConnection(connectionSession);
	}
}

bool USocketClientTCPClient::isRun()
{
	return run;
}

void USocketClientTCPClient::setRun(bool InRun)
{
	run = InRun;
}

FString USocketClientTCPClient::getConnectionID()
{
	return connectionID;
}

void USocketClientTCPClient::setSocket(FSocket* InSocket)
{
	socket = InSocket;
}

FSocket* USocketClientTCPClient::getSocket()
{
	return socket;
}

USocketClientBPLibrary* USocketClientTCPClient::getMainLib()
{
	return mainLib;
}

void USocketClientTCPClient::createSendThread()
{
	tcpSendThread = new FSocketClientTCPSendDataThread(mainLib, this, connectionID);
}

void USocketClientTCPClient::getTcpSeparator(FString& OutStringSeparator, uint8& byteSeparator, ESocketClientTCPSeparator& OutMessageWrapping)
{
	OutMessageWrapping = messageWrapping;
	OutStringSeparator = tcpStringSeparator;
	tcpByteSeparator = byteSeparator;

}

void USocketClientTCPClient::readDataLength(TArray<uint8>& byteDataArray, int32& byteLength)
{
	if (FGenericPlatformProperties::IsLittleEndian() && byteDataArray[0] == 0x00)
	{
		FMemory::Memcpy(&byteLength, byteDataArray.GetData() + 1, 4);
	}
	else
	{
		byteDataArray.SwapMemory(1, 4);
		byteDataArray.SwapMemory(2, 3);
		FMemory::Memcpy(&byteLength, byteDataArray.GetData() + 1, 4);
	}
}

bool USocketClientTCPClient::hasResume()
{
	return resume;
}

bool USocketClientTCPClient::isConnected()
{
	return connected;
}