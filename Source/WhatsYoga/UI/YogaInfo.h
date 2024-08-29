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
	void SetCountdownText(const FString& InText);
	
protected:
	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* YogaExplanationTextBlock;

	/*UPROPERTY(meta = (BindWidget))
	class UGauge* WB_ComettailGauge;*/

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CountDownTextBlock;
};
