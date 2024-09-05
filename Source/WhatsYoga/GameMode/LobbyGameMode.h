#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WHATSYOGA_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void LoadYogaMap();

	UFUNCTION()
	void OpenYogaMap();

private:
	TObjectPtr<class ALobbyHUD> LobbyHUD;
	
};
