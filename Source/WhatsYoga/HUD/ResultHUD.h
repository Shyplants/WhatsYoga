#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ResultHUD.generated.h"

/**
 * 
 */
UCLASS()
class WHATSYOGA_API AResultHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "WidgetClass")
	TSubclassOf<class UUserWidget> ResultWidgetClass;

	UPROPERTY()
	TObjectPtr<class UResult> ResultWidget;

	void AddResultWidget();
};
