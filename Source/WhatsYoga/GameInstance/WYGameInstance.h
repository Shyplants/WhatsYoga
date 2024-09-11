#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WYGameInstance.generated.h"

DECLARE_DELEGATE_OneParam(FOnNameReceivedDelegate, const FString&);

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

// Player Section
public:
	FOnNameReceivedDelegate OnNameReceived;
	FORCEINLINE void SetPlayerName(const FString& InPlayerName) { PlayerName = InPlayerName; OnNameReceived.ExecuteIfBound(InPlayerName); }
	FORCEINLINE FString GetPlayerName() const { return PlayerName; }

	FORCEINLINE void SetContentName(const FString& InContentName) { ContentName = InContentName; }
	FORCEINLINE FString GetContentName() const { return ContentName; }

private:
	FString PlayerName;
	FString ContentName;
};
