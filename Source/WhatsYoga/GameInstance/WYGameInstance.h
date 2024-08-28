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

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "TCPConnect"))
	void TCPConnect();

protected:
	TMap<FString, FString> ConfigOptionsMap;

	UPROPERTY(BlueprintReadOnly, Category = "TCP/IP")
	FString IP;

	UPROPERTY(BlueprintReadOnly, Category = "TCP/IP")
	int32 Port;
};
