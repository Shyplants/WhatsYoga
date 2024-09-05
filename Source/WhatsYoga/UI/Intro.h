#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Intro.generated.h"

DECLARE_DELEGATE(FIntroOnClosed)

/**
 * 
 */
UCLASS()
class WHATSYOGA_API UIntro : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> IntroAnim;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> TextAnim;

	UFUNCTION()
	void OnIntroAnimationFinished();

	FIntroOnClosed OnClosed;

private:
	void PlayIntroAnimation();

};
