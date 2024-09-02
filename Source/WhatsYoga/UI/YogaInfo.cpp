#include "YogaInfo.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"

void UYogaInfo::NativeConstruct()
{
	Super::NativeConstruct();
}

void UYogaInfo::SetYogaText(const FString& InText)
{
	if (YogaExplanationTextBlock)
	{
		YogaExplanationTextBlock->SetText(FText::FromString(InText));
	}
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
