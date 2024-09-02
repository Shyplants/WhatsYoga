#include "Gauge.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"

void UGauge::NativeConstruct()
{
	Super::NativeConstruct();

	if (GaugeImage && GaugeImage->Brush.GetResourceObject())
	{
		Material = CastChecked<UMaterialInterface>(GaugeImage->Brush.GetResourceObject());

		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		GaugeImage->SetBrushFromMaterial(DynamicMaterial);
	}

	ElapsedTime = 0.0f;
	
	PastGaugePercent = 100.0f;
	SetGaugePercent(PastGaugePercent);
}

void UGauge::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);

	if (bIsUpdating)
	{
		UpdateGauge(DeltaTime);
	}
}

void UGauge::UpdateGauge(float DeltaTime)
{
	ElapsedTime += DeltaTime;

	check(UpdateDuration != 0.0f);
	
	float Alpha = ElapsedTime / UpdateDuration;
	CurrentGaugePercent = FMath::Lerp(PastGaugePercent, TargetGaugePercent, Alpha);
	CurrentGaugePercent = FMath::Clamp(CurrentGaugePercent, MinGaugePercent, MaxGaugePercent);

	GaugeValue->SetText(FText::AsNumber(FMath::RoundToInt(CurrentGaugePercent)));

	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(FName("FillPercent"), CurrentGaugePercent * 0.75f);
	}

	if (ElapsedTime >= UpdateDuration)
	{
		bIsUpdating = false;
	}
}

void UGauge::SetGaugePercent(float InPercent, float InUpdateDuration)
{
	if (GaugeImage)
	{
		PastGaugePercent = CurrentGaugePercent;
		TargetGaugePercent = InPercent;
		MinGaugePercent = FMath::Min(PastGaugePercent, TargetGaugePercent);
		MaxGaugePercent = FMath::Max(PastGaugePercent, TargetGaugePercent);

		UpdateDuration = InUpdateDuration;
		ElapsedTime = 0.0f;
		bIsUpdating = true;
	}
}
