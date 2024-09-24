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

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USlider> ContentSlider;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ContentProgressBar;
};
