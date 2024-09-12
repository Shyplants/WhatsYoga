#include "ResultHUD.h"
#include "UI/Result.h"


void AResultHUD::AddResultWidget()
{
	TObjectPtr<APlayerController> PlayerController = GetOwningPlayerController();

	if (PlayerController && ResultWidgetClass)
	{
		ResultWidget = CreateWidget<UResult>(PlayerController, ResultWidgetClass);

		ResultWidget->AddToViewport();
	}
}
