#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "YogaResult.generated.h"

/**
 * 
 */
UCLASS()
class WHATSYOGA_API UYogaResult : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetYogaPoseEvaluationTextBlock(const FString& InText, const FLinearColor& InColor);

	void SetStarFillTexture(int32 index);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> YogaPoseEvaluationTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UOverlay> StarOverlay;

	TArray<TObjectPtr<class UImage>> StarImageArray;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> StarBlankTexture;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> StarFillTexture;

	UPROPERTY(EditAnywhere)
	TArray<FLinearColor> FontColorArray;

private:
	UPROPERTY()
	int32 StarCount;
};
