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

	if (SliderImage)
	{
		CanvasSlot = Cast<UCanvasPanelSlot>(SliderImage->Slot);
		if (CanvasSlot)
		{
			StartXPosition = CanvasSlot->GetPosition().X;
		}
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
