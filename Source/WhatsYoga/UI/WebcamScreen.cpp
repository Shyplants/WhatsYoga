#include "WebcamScreen.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Slate/SlateBrushAsset.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/MainGameMode.h"

void UWebcamScreen::NativeConstruct()
{
	Super::NativeConstruct();

	AMainGameMode* MainGameMode = CastChecked<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameMode->OnGstTextureCreated.BindUFunction(this, TEXT("SetScreenImageTexture"));
}

void UWebcamScreen::SetScreenImageTexture(UTexture2D* InTexture)
{
	if (ScreenImage && InTexture)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(InTexture);
		Brush.ImageSize = FVector2D(ScreenImage->GetDesiredSize());

		ScreenImage->SetBrush(Brush);
	}
}
