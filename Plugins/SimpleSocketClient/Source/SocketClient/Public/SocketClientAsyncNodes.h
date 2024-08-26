#pragma once

#include "SocketClient.h"

#ifndef __SocketClientBPLibrary
#define __SocketClientBPLibrary
#include "SocketClientBPLibrary.h"
#endif

#include "SocketClientAsyncNodes.generated.h"

UCLASS()
class SOCKETCLIENT_API UTCPConnectAsyncNode : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FTCPConnectDelegate, const FString, connectionInfo, const FString, clientConnectionID, const FString, messageFromServer, const TArray<uint8>&, byteArrayFromServer);

	UPROPERTY(BlueprintAssignable)
	FTCPConnectDelegate OnConnect;

	UPROPERTY(BlueprintAssignable)
	FTCPConnectDelegate OnDisconnect;

	UPROPERTY(BlueprintAssignable)
	FTCPConnectDelegate OnServerMessage;

	UFUNCTION(BlueprintCallable, Category = "SocketClient|TCP", meta = (BlueprintInternalUseOnly = "true", AdvancedDisplay = 6))
	static UTCPConnectAsyncNode* socketClientTCPConnectionAsyncNode(FString domainOrIP, ESocketClientIPType ipType, int32 port, EReceiveFilterClient receiveFilters,
		ESocketClientTCPSeparator messageWrapping, bool disableNaglesAlgorithm = false);

	virtual void Activate() override;

	void triggerConnectionEvent(bool success, FString clientConnectionID, FString serverMessage);
	void triggerMessageEvent(TArray<uint8> byteDataArray, FString clientConnectionID, FString serverMessage);


private:
	UTCPConnectAsyncNode* instance = nullptr;
	FString domainOrIP = "0.0.0.0";
	ESocketClientIPType ipType = ESocketClientIPType::E_ipv4;
	int32 port = 9804;
	EReceiveFilterClient receiveFilters = EReceiveFilterClient::E_SAB;
	ESocketClientTCPSeparator messageWrapping = ESocketClientTCPSeparator::E_None;
	FString connectionID = FString();
	bool disableNagleAlgorithm = false;
};