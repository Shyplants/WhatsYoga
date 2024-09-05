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
	TSubclassOf<class UUserWidget> IntroWidgetClass;

	UPROPERTY(EditAnywhere, Category = "WidgetClass")
	TSubclassOf<class UUserWidget> PlayerInfoWidgetClass;

	void AddIntroWidget();
	void AddPlayerInfoWidget();

private:
	UPROPERTY()
	TObjectPtr<class UIntro> IntroWidget;

	UPROPERTY()
	TObjectPtr<class UPlayerInfo> PlayerInfoWidget;
};
