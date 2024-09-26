#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainGameMode.generated.h"

DECLARE_DELEGATE_OneParam(FOnGstTextureCreated, UTexture2D*);

/**
 * 
 */
UCLASS()
class WHATSYOGA_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AMainGameMode();

	UFUNCTION()
	void RegisterGstTexture(UTexture2D* InTexture);

	UFUNCTION()
	void LoadResultMap();

	UFUNCTION()
	void OpenResultMap();

public:
	UFUNCTION()
	void SetGaugePercent(float Percent);

protected:
	virtual void BeginPlay() override;

public:
	FOnGstTextureCreated OnGstTextureCreated;

private:
	class AMainHUD* MainHUD;
};
