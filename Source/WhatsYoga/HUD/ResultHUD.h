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

	UPROPERTY(EditAnywhere, Category = "WidgetClass")
	TSubclassOf<class UUserWidget> YogaResultWidgetClass;

	UPROPERTY()
	TObjectPtr<class UYogaResult> YogaResultWidget;

	void AddResultWidget();
	void AddYogaResultWidget();
};
