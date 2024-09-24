#include "YogaInfo.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "UObject/ObjectPtr.h"
#include "Gauge.h"
#include "ContentProgressBar.h"

void UYogaInfo::NativeConstruct()
{
	Super::NativeConstruct();

	SetCountdownVisible(false);
}

void UYogaInfo::SetYogaText(const FString& InText)
{
	if (YogaExplanationTextBlock)
	{
		YogaExplanationTextBlock->SetText(FText::FromString(InText));
	}
}

void UYogaInfo::SetGaugePercent(float Percent)
{
	check(WB_ComettailGauge != nullptr);

	WB_ComettailGauge->SetGaugePercent(Percent);
}

void UYogaInfo::SetCountdownText(const FString& InText)
{
	if (CountDownTextBlock)
	{
		CountDownTextBlock->SetText(FText::FromString(InText));
	}
}

void UYogaInfo::SetCountdownVisible(bool bVisible)
{
	if (CountDownTextBlock)
	{
		CountDownTextBlock->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UYogaInfo::SetContentProgressBarPercent(float Percent)
{
	UE_LOG(LogTemp, Log, TEXT("YogaInfo::SetContentProgressBarPercent called, percent = [%f]"), Percent);

	if (WB_ContentProgressBar)
	{
		WB_ContentProgressBar->SetPercent(Percent);
	}
}
