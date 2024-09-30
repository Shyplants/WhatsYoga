#include "ResultHUD.h"
#include "UI/Result.h"
#include "UI/YogaResult.h"


void AResultHUD::AddResultWidget()
{
	TObjectPtr<APlayerController> PlayerController = GetOwningPlayerController();

	if (PlayerController && ResultWidgetClass)
	{
		ResultWidget = CreateWidget<UResult>(PlayerController, ResultWidgetClass);

		ResultWidget->AddToViewport();
	}
}

void AResultHUD::AddYogaResultWidget()
{
	TObjectPtr<APlayerController> PlayerController = GetOwningPlayerController();

	if (PlayerController && YogaResultWidgetClass)
	{
		YogaResultWidget = CreateWidget<UYogaResult>(PlayerController, YogaResultWidgetClass);

		YogaResultWidget->AddToViewport();
	}
}
