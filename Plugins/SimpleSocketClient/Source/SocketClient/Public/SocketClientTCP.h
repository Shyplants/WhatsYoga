#pragma once

#include "SocketClient.h"

#include "SocketClientTCPReceiveDataThread.h"
#include "SocketClientTCPSendDataThread.h"

#include "SocketClientTCP.generated.h"

UCLASS(Blueprintable, BlueprintType)
class SOCKETCLIENT_API USocketClientTCPClient : public UObject
{
	GENERATED_BODY()

public:

	USocketClientTCPClient(const FObjectInitializer& ObjectInitializer);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSocketClientTCPConnectionEventDelegate, bool, success, FString, message, FString, clientConnectionID);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceiveTCPMessageEvenetDelegate, FString, message, const TArray<uint8>&, byteArray, FString, clientConnectionID);

	UFUNCTION()
	void socketClientTCPConnectionEventDelegate(const bool success, const FString message, const FString clientConnectionID);

	UPROPERTY(BlueprintAssignable, Category = "SocketClient|TCP|Events|ConnectionInfo")
	FSocketClientTCPConnectionEventDelegate onSocketClientTCPConnectionEventDelegate;

	UFUNCTION()
	void receiveTCPMessageEventDelegate(const FString message, const TArray<uint8>& byteArray, const FString clientConnectionID);

	UPROPERTY(BlueprintAssignable, Category = "SocketClient|TCP|Events|ReceiveMessage")
	FReceiveTCPMessageEvenetDelegate onReceiveTCPMessageEventDelegate;

	UFUNCTION()
	void connectionEvent(bool success, FString message, FString clientConnectionID);


	void connect(USocketClientBPLibrary* InMainLib, FString InDomainOrIP, ESocketClientIPType ipType, int32 port, EReceiveFilterClient receiveFilter,
		ESocketClientTCPSeparator InMessageWrapping, FString InConnectionID, UTCPConnectAsyncNode* InTcpConnectAsyncNode, bool noPacketDelay = false, bool noPacketBlocking = false);
	void sendMessage(FString message, TArray<uint8> byteArray);

	void closeConnection();

	bool isRun();
	void setRun(bool InRun);
	FString getConnectionID();
	
	void setSocket(FSocket* InSocket);
	FSocket* getSocket();
	USocketClientBPLibrary* getMainLib();

	void createSendThread();
	void readDataLength(TArray<uint8>& byteDataArray, int32& byteLength);
	bool hasResume();
	bool isConnected();
	void getTcpSeparator(FString& stringSeparator, uint8& byteSeparator, ESocketClientTCPSeparator& messageWrapping);

	UTCPConnectAsyncNode* tcpConnectAsyncNode = nullptr;


private:

	bool run = false;
	bool connected = false;
	bool resume = false;
	int32 sendOrReceive = -1;
	FString connectionID = FString();
	
	FSocket* socket = nullptr;

	
	FSocketClientTCPReceiveDataThread* tcpReceiveDataThread = nullptr;
	FSocketClientTCPSendDataThread* tcpSendThread = nullptr;
	

	USocketClientBPLibrary* mainLib = nullptr;

	ESocketClientTCPSeparator messageWrapping;
	FString tcpStringSeparator = "(~{";
	uint8 tcpByteSeparator = 0x00;
};