#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GStreamerContainer.generated.h"

UCLASS()
class WHATSYOGA_API AGStreamerContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	AGStreamerContainer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void PostInitializeComponents() override;


	UFUNCTION(BlueprintCallable, Category = "Texture")
	void SetWebcamTexture(UTexture2D* InTexture);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	UTexture2D* WebcamTexture;

private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString GStreamerPipelineConfig;

};
