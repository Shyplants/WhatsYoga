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
	if (ScreenImage && InTexture && RoundedMaterial)
	{
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(RoundedMaterial, this);

		// Set the texture parameter
		DynamicMaterial->SetTextureParameterValue(FName("InputTexture"), InTexture);

		// Set the corner radius parameter
		// DynamicMaterial->SetScalarParameterValue(FName("CornerRadius"), 0.1f);

		// Create a brush from the material
		FSlateBrush Brush;
		Brush.SetResourceObject(DynamicMaterial);

		// Set the image size
		Brush.ImageSize = ScreenImage->GetDesiredSize();

		// Apply the brush to the image
		ScreenImage->SetBrush(Brush);


		/*FSlateBrush Brush;
		Brush.SetResourceObject(InTexture);
		Brush.ImageSize = FVector2D(ScreenImage->GetDesiredSize());

		ScreenImage->SetBrush(Brush);

		if (BackgroundImage)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			if (DynamicMaterial)
			{
				FSlateBrush Brush;
				Brush.SetResourceObject(DynamicMaterial);

				BackgroundImage->SetBrush(Brush);
			}
		}*/
	}
}