#include "SliderBar.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void USliderBar::NativeConstruct()
{
	Super::NativeConstruct();

	if (MovementRange == 0.0f)
	{
		MovementRange = 480.0f;
	}
	
	check(RunTextureArray.Num() > 0);
	RunTextureIndex = 0;

	if (SliderImage)
	{
		SliderImage->SetBrushFromTexture(RunTextureArray[RunTextureIndex]);

		CanvasSlot = Cast<UCanvasPanelSlot>(SliderImage->Slot);
		if (CanvasSlot)
		{
			StartXPosition = CanvasSlot->GetPosition().X;
		}
	}

	RunTextureAnimationDuration = 0.0f;
}

void USliderBar::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);

	RunTextureAnimationDuration += DeltaTime;
	if (RunTextureAnimationDuration > 0.5f)
	{
		RunTextureIndex++;
		RunTextureIndex %= RunTextureArray.Num();
		SliderImage->SetBrushFromTexture(RunTextureArray[RunTextureIndex]);

		RunTextureAnimationDuration = 0.0f;
	}
}

void USliderBar::UpdateImagePosition(float Percent)
{
	if (SliderImage && CanvasSlot)
	{
		Percent = FMath::Clamp(Percent, 0.0f, 1.0f);

		float XPosition = StartXPosition + (Percent * MovementRange);
		float YPosition = CanvasSlot->GetPosition().Y;

		CanvasSlot->SetPosition(FVector2D(XPosition, YPosition));
	}
}
