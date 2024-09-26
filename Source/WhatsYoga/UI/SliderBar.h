#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SliderBar.generated.h"

/**
 * 
 */
UCLASS()
class WHATSYOGA_API USliderBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

	void UpdateImagePosition(float Percent);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> CanvasPanel;

	UPROPERTY()
	TObjectPtr<class UCanvasPanelSlot> CanvasSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SliderImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Slider")
	float MovementRange;

	UPROPERTY()
	float StartXPosition;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class UTexture2D>> RunTextureArray;

	int RunTextureIndex;
	float RunTextureAnimationDuration;
};
