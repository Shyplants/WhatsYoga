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
	void SetYogaNameText(const FString& InText);

	UFUNCTION()
	void SetYogaEfficacyText(const FString& InText);

	UFUNCTION()
	void SetYogaExplanationText(const FString& InText);

	UFUNCTION()
	void SetGaugePercent(float Percent);

	UFUNCTION()
	void SetCountdownText(const FString& InText);

	void SetCountdownVisible(bool bVisible);

	void SetContentProgressBarPercent(float Percent);

	void SetStarFillTexture(int32 index);

	int32 GetStarState();
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> YogaNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> YogaEfficacyTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> YogaExplanationTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UGauge> WB_ComettailGauge;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CountDownTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UContentProgressBar> WB_ContentProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UOverlay> StarOverlay;

	TArray<TObjectPtr<class UImage>> StarImageArray;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> StarBlankTexture;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> StarFillTexture;

	UPROPERTY()
	TObjectPtr<class UWYGameInstance> WYGameInstance;

private:
	UPROPERTY()
	int32 StarState;

	UPROPERTY()
	int32 ScoreCount;
};
