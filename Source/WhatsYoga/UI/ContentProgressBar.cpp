#include "ContentProgressBar.h"
#include "Components/Slider.h"
#include "Components/ProgressBar.h"

void UContentProgressBar::NativeConstruct()
{
	Super::NativeConstruct();

}

void UContentProgressBar::SetPercent(float Percent)
{
	if (ContentProgressBar)
	{
		ContentProgressBar->SetPercent(Percent);
	}

	if (ContentSlider)
	{
		ContentSlider->SetValue(Percent);
	}
}