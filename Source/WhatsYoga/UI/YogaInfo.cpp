#include "YogaInfo.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "UObject/ObjectPtr.h"
#include "Gauge.h"
#include "ContentProgressBar.h"

void UYogaInfo::NativeConstruct()
{
	Super::NativeConstruct();

	SetCountdownVisible(false);

	StarImageArray.Empty();
	FLinearColor StarColor = FLinearColor(FColor::FromHex(TEXT("EE7D26FF")));
	for (int32 i = 0; i < 10; ++i)
	{
		UImage* StarImage = NewObject<UImage>(this, UImage::StaticClass());

		if (StarImage)
		{
			if (StarBlankTexture)
			{
				StarImage->SetBrushFromTexture(StarBlankTexture);
				StarImage->SetBrushTintColor(StarColor);
			}

			UOverlaySlot* OverlaySlot = StarOverlay->AddChildToOverlay(StarImage);
			
			if (OverlaySlot)
			{
				OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
				OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
			}

			StarImageArray.Add(StarImage);

			FWidgetTransform transform;
			transform.Translation = FVector2D(-189.0f + 42.0f * i, 0.0f);
			StarImageArray[i]->SetRenderTransform(transform);
		}
	}

	
}

void UYogaInfo::SetYogaNameText(const FString& InText)
{
	if (YogaNameTextBlock)
	{
		YogaNameTextBlock->SetText(FText::FromString(InText));
	}
}

void UYogaInfo::SetYogaEfficacyText(const FString& InText)
{
	if (YogaEfficacyTextBlock)
	{
		YogaEfficacyTextBlock->SetText(FText::FromString(InText));
	}
}

void UYogaInfo::SetYogaExplanationText(const FString& InText)
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

void UYogaInfo::SetStarFillTexture(int32 index)
{
	if (index < StarImageArray.Num())
	{
		// FLinearColor StarColor = FLinearColor(FColor::FromHex(TEXT("EE7D26FF")));

		if (StarFillTexture)
		{
			StarImageArray[index]->SetBrushFromTexture(StarFillTexture);
			// StarImageArray[i]->SetBrushTintColor(StarColor);
		}

		StarState |= (1 << index);
	}
}

int32 UYogaInfo::GetStarState()
{
	return StarState;
}
