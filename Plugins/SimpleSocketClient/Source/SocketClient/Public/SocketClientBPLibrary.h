// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SocketClient.h"
#include "SocketClientBPLibrary.generated.h"

class FSocketClientCleanerThread;

class USocketClientTCPClient;


/**
 * 
 */
UCLASS()
class SOCKETCLIENT_API USocketClientBPLibrary : public UObject
{
	GENERATED_BODY()
	
public:
	USocketClientBPLibrary(const FObjectInitializer& ObjectInitializer);

	~USocketClientBPLibrary();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSocketClientTCPConnectionEventDelegate, bool, success, FString, message, FString, clientConnectionID);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceiveTCPMessageEventDelegate, FString, message, const TArray<uint8>&, byteArray, FString, clientConnectionID);
	

	UFUNCTION()
	void SocketClientTCPConnectionEventDelegate(const bool success, const FString message, const FString clientConnectionID);

	UPROPERTY(BlueprintAssignable, Category = "SocketClient|TCP|Events|ConnectionInfo")
	FSocketClientTCPConnectionEventDelegate onSocketClientTCPConnectionEventDelegate;

	UFUNCTION()
	void ReceiveTCPMessageEventDelegate(const FString message, const TArray<uint8>& byteArray, const FString clientConnectionID);
	
	UPROPERTY(BlueprintAssignable, Category = "SocketClient|TCP|Events|ReceiveMessage")
	FReceiveTCPMessageEventDelegate onReceiveTCPMessageEventDelegate;


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SocketClient")
	static USocketClientBPLibrary* GetSocketClientTarget();
	static USocketClientBPLibrary* socketClientBPLibrary;

	// Connect to a TCP Server
	UFUNCTION(BlueprintCallable, Category = "SocketClient|TCP", meta = (AdvancedDisplay = 6))
	static void connectSocketClientTCP(FString domainOrIP, ESocketClientIPType ipType, int32 port, EReceiveFilterClient receiveFilters,
		ESocketClientTCPSeparator messageSeparator, FString& connectionID, bool disableNaglesAlgorithm = false);
	void connectSocketClientTCPNonStatic(FString domainOrIP, ESocketClientIPType ipType, int32 port, EReceiveFilterClient receiveFilters,
	
		ESocketClientTCPSeparator messageSeparator, FString& connectionID, UTCPConnectAsyncNode* tcpConnectAsyncNode, bool disableNaglesAlgorithm = false);

	UFUNCTION(BlueprintCallable, Category = "SocketClient|TCP", meta = (AutoCreateRefTerm = "byteArray"))
	static void socketClientSendTCP(FString connectionID, FString message, TArray<uint8> byteArray, bool addLineBreak = true);
	void socketClientSendTCPNonStatic(FString connectionID, FString message, TArray<uint8> byteArray, bool addLineBreak = true);
	
	// Terminate an existing connection
	UFUNCTION(BlueprintCallable, Category = "SocketClient|TCP")
	static void closeSocketClientConnectionTCP(FString connectionID);
	void closeSocketClientConnectionTCPNonStatic(FString connectionID);

	// Terminate all connections
	UFUNCTION(BlueprintCallable, Category = "SocketClient|TCP")
	static void closeAllSocketClientConnectionsTCP();
	void closeAllSocketClientConnectionsTCPNonStatic();

	UFUNCTION(BlueprintCallable, Category = "SocketClient|TCP")
	static void getTCPConnectionByConnectionID(FString connectionID, bool& found, USocketClientTCPClient*& connection);
	void getTCPConnectionByConnectionIDNonStatic(FString connectionID, bool& found, USocketClientTCPClient*& connection);

	UFUNCTION(BlueprintCallable, Category = "SocketClient|TCP")
	static bool isTCPConnected(FString connectionID);
	bool isTCPConnectedNonStatic(FString connectionID);

	UFUNCTION(BlueprintCallable, Category = "SocketClient|SpecialFunctions")
	static FString getLocalIP();


	void getTcpSeparator(uint8& byteSeparator, FString& stringSeparator);

	static ISocketSubsystem* getSocketSubSystem();

	FString resolveDomain(FString domain, ESocketClientIPType ipType);
	TMap<FString, FString> domainCache;

	void cleanConnection(FSocketClientPluginSession& session);


private:
	ESocketPlatformClient systemSocketPlatform;

	TMap<FString, USocketClientTCPClient*> tcpClients;

	int64 lastErrorMessageTime = 0;

	FString tcpStringSeparator = "(~{";
	uint8 tcpByteSeparator = 0x00;

	FSocketClientCleanerThread* socketClientCleanerThread = nullptr;
};
