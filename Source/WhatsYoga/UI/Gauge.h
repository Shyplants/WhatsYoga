#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Gauge.generated.h"

/**
 * 
 */
UCLASS()
class WHATSYOGA_API UGauge : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

	UFUNCTION()
	void UpdateGauge(float DeltaTime);

	UFUNCTION()
	void SetGaugePercent(float InPercent, float InUpdateDuration = 2.0f);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> GaugeValue;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> GaugeImage;

	UPROPERTY()
	TObjectPtr<class UMaterialInterface> Material;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> DynamicMaterial;

	float PastGaugePercent;
	float CurrentGaugePercent;
	float TargetGaugePercent;
	float MinGaugePercent;
	float MaxGaugePercent;

	float UpdateDuration;
	float ElapsedTime;

	UPROPERTY()
	bool bIsUpdating;
};
