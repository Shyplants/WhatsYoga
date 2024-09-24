#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "YogaInfo.generated.h"

/**
 * 
 */
UCLASS()
class WHATSYOGA_API UYogaInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetYogaText(const FString& InText);

	UFUNCTION()
	void SetGaugePercent(float Percent);

	UFUNCTION()
	void SetCountdownText(const FString& InText);

	void SetCountdownVisible(bool bVisible);

	void SetContentProgressBarPercent(float Percent);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> YogaExplanationTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UGauge> WB_ComettailGauge;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CountDownTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UContentProgressBar> WB_ContentProgressBar;
};
