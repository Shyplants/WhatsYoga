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

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void RegisterGstTexture(UTexture2D* InTexture);

	FOnGstTextureCreated OnGstTextureCreated;

private:
	class AMainHUD* MainHUD;
};
