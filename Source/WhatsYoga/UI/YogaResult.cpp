#include "YogaResult.h"
#include "GameInstance/WYGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

void UYogaResult::NativeConstruct()
{
	TObjectPtr<UWYGameInstance> WYGameInstance = CastChecked<UWYGameInstance>(GetGameInstance());
	int32 StarState = WYGameInstance->GetStartState();
	int32 ScoreCount = WYGameInstance->GetScoreCount();

	for (int32 i = 0; i < ScoreCount; ++i)
	{
		StarCount += ((StarState >> i) & 1);
	}

	check(FontColorArray.Num() >= 4);
	FLinearColor CurrentColor = FontColorArray[0];
	if (StarCount == ScoreCount)
	{
		SetYogaPoseEvaluationTextBlock(TEXT("PERFECT!!!"), FontColorArray[0]);
	}
	else if (StarCount > ScoreCount/3*2)
	{
		SetYogaPoseEvaluationTextBlock(TEXT("AWESOME!!"), FontColorArray[1]);
		CurrentColor = FontColorArray[1];
	}
	else if (StarCount > ScoreCount/3)
	{
		SetYogaPoseEvaluationTextBlock(TEXT("GREAT!"), FontColorArray[2]);
		CurrentColor = FontColorArray[2];
	}
	else
	{
		SetYogaPoseEvaluationTextBlock(TEXT("GOOD"), FontColorArray[3]);
		CurrentColor = FontColorArray[3];
	}

	StarImageArray.Empty();
	for (int32 i = 0; i < ScoreCount; ++i)
	{
		UImage* StarImage = NewObject<UImage>(this, UImage::StaticClass());
		if (StarImage)
		{
			UTexture2D* StarTexture = ((StarState >> i) & 1) ? StarFillTexture : StarBlankTexture;

			if (StarTexture)
			{
				StarImage->SetBrushFromTexture(StarTexture);
				StarImage->SetBrushTintColor(CurrentColor);

				StarImage->Brush.ImageSize = FVector2D(52.0f, 52.0f);
			}

			UOverlaySlot* OverlaySlot = StarOverlay->AddChildToOverlay(StarImage);

			if (OverlaySlot)
			{
				OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
				OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
			}

			StarImageArray.Add(StarImage);

			FWidgetTransform transform;
			float offset = -(ScoreCount / 2 - 0.5f) * 52.0f;
			transform.Translation = FVector2D(offset + 52.0f * i, 0.0f);
			StarImageArray[i]->SetRenderTransform(transform);
		}
	}
}

void UYogaResult::SetYogaPoseEvaluationTextBlock(const FString& InText, const FLinearColor& InColor)
{
	if (YogaPoseEvaluationTextBlock)
	{
		YogaPoseEvaluationTextBlock->SetText(FText::FromString(InText));
		YogaPoseEvaluationTextBlock->SetColorAndOpacity(InColor);
	}
}

void UYogaResult::SetStarFillTexture(int32 index)
{
	if (index < StarImageArray.Num())
	{
		if (StarFillTexture)
		{
			StarImageArray[index]->SetBrushFromTexture(StarFillTexture);
		}
	}
}
