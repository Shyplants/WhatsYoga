#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ResultGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WHATSYOGA_API AResultGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AResultGameMode();

	virtual void BeginPlay() override;

private:
	TObjectPtr<class AResultHUD> ResultHUD;
	
};

