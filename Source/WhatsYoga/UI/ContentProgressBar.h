#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ContentProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class WHATSYOGA_API UContentProgressBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetPercent(float Percent);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USlider> ContentSlider;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ContentProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USliderBar> SliderBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> BackgroundImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TObjectPtr<class UMaterialInterface> Material;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> DynamicMaterial;
};
