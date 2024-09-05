#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

/**
 * 
 */
UCLASS()
class WHATSYOGA_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "WidgetClass")
	TSubclassOf<class UUserWidget> IntroClass;

	UPROPERTY()
	TObjectPtr<class UIntro> Intro;
	
	void AddIntro();
};
