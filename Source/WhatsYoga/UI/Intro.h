#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Intro.generated.h"

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

private:
	void PlayIntroAnimation();

};
