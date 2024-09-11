#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WYGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WHATSYOGA_API UWYGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UWYGameInstance();

	virtual void Init() override;

// TCP Section
public:
	UFUNCTION(BlueprintCallable)
	void HandleReceivedTCPMessage(FString Message);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "TCPConnect"))
	void TCPConnect();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "TCPSend"))
	void TCPSendMessage(const FString& Message);
	
	UFUNCTION(BlueprintCallable)
	FString GetLocalIPAddress();

protected:
	TMap<FString, FString> ConfigOptionsMap;

	UPROPERTY(BlueprintReadOnly, Category = "TCP/IP")
	FString IP;

	UPROPERTY(BlueprintReadOnly, Category = "TCP/IP")
	int32 Port;

private:
	void SetGaugePercent(float Percent);
};
