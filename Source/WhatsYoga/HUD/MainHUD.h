#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class WHATSYOGA_API AMainHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void AddYogaInfoWidget();

	class UYogaInfo* GetYogaInfo() { return YogaInfo; }

private:
	UPROPERTY(EditAnywhere, Category = "WidgetClass")
	TSubclassOf<class UUserWidget> YogaInfoClass;

	UPROPERTY()
	class UYogaInfo* YogaInfo;


};
