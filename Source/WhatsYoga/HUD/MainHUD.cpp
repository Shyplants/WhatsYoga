#include "MainHUD.h"
#include "UI/YogaInfo.h"

void AMainHUD::AddYogaInfoWidget()
{
	APlayerController* PlayerController = GetOwningPlayerController();

	if (PlayerController && YogaInfoClass)
	{
		YogaInfo = CreateWidget<UYogaInfo>(PlayerController, YogaInfoClass);
		if (YogaInfo)
		{
			YogaInfo->AddToViewport();
		}
	}
}

void AMainHUD::SetGaugePercent(float Percent)
{
	check(YogaInfo != nullptr);

	YogaInfo->SetGaugePercent(Percent);
}
