#include "ContentProgressBar.h"
#include "Components/Slider.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "SliderBar.h"

void UContentProgressBar::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackgroundImage)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		if (DynamicMaterial)
		{
			FSlateBrush Brush;
			Brush.SetResourceObject(DynamicMaterial);

			BackgroundImage->SetBrush(Brush);
		}
	}
}

void UContentProgressBar::SetPercent(float Percent)
{
	if (ContentProgressBar)
	{
		ContentProgressBar->SetPercent(Percent);
	}

	/*if (ContentSlider)
	{
		ContentSlider->SetValue(Percent);
	}*/

	if (SliderBar)
	{
		SliderBar->UpdateImagePosition(Percent);
	}

	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(FName("FillPercent"), Percent);
	}
}