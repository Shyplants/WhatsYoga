#pragma once

#include "SocketClient.h"

class SOCKETCLIENT_API FSocketClientTCPReceiveDataThread : public FRunnable
{
public:
	FSocketClientTCPReceiveDataThread() {}
	FSocketClientTCPReceiveDataThread(USocketClientBPLibrary* InSocketClientBPLibrary, FString InClientConnectionID, EReceiveFilterClient InReceiveFilter,
		FString InIPorDomain, ESocketClientIPType InIPType, int32 InPort, USocketClientTCPClient* InTcpClient, bool InNoPacketDelay, bool InNoPacketBlocking);
	~FSocketClientTCPReceiveDataThread();

	virtual uint32 Run() override;


	void triggerConnectionEvent(bool success, FString clientConnectionIDGlobal, FString serverMessage, USocketClientTCPClient* tcpClientGlobal,
		USocketClientBPLibrary* socketClinetGlobal);
	void triggerMessageEvent(TArray<uint8>& byteDataArray, FString& clientConnectionIDGlobal, USocketClientTCPClient* tcpClientGlobal,
		USocketClientBPLibrary* socketClientGlobal, bool addNullTerminator = true);

private:
	USocketClientBPLibrary* socketClientBPLibrary = nullptr;
	FString clientConnectionID;
	FString originalIP;
	EReceiveFilterClient receiveFilter;
	FString ipOrDomain;
	ESocketClientIPType ipType;
	int32 port;
	USocketClientTCPClient* tcpClient = nullptr;
	bool noPacketDelay = false;
	bool noPacketBlocking = false;


	FRunnableThread* thread = nullptr;
};