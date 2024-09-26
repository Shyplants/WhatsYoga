#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WebcamScreen.generated.h"

/**
 * 
 */
UCLASS()
class WHATSYOGA_API UWebcamScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetScreenImageTexture(UTexture2D* InTexture);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UImage* ScreenImage;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Webcam")
	UMaterialInterface* RoundedMaterial;
	
};
