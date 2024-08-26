#pragma once

#include "SocketClient.h"

class SOCKETCLIENT_API FSocketClientTCPSendDataThread : public FRunnable
{
public:

	FSocketClientTCPSendDataThread() {}
	FSocketClientTCPSendDataThread(USocketClientBPLibrary* InSocketClientLib, USocketClientTCPClient* InTcpClient, FString InClientConnectionID);
	~FSocketClientTCPSendDataThread();

	virtual uint32 Run() override;
	void sendMessage(FString message, TArray<uint8> byteArray);
	void pauseThread(bool pause);

private:
	TQueue<FString> messageQueue;
	TQueue<TArray<uint8>> byteArrayQueue;
	USocketClientBPLibrary* socketClientLib;
	USocketClientTCPClient* tcpClient = nullptr;
	FString clientConnectionID;
	FRunnableThread* thread = nullptr;

	bool run = true;
	bool paused = false;
	bool blah = true;

};