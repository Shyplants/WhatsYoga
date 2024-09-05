#include "MainGameMode.h"
#include "GameInstance/WYGameInstance.h"
#include "HUD/MainHUD.h"

AMainGameMode::AMainGameMode()
{

}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		MainHUD = CastChecked<AMainHUD>(PlayerController->GetHUD());

		MainHUD->AddYogaInfoWidget();
	}
}

void AMainGameMode::RegisterGstTexture(UTexture2D* InTexture)
{
	if (OnGstTextureCreated.IsBound())
	{
		OnGstTextureCreated.Execute(InTexture);
	}
}

void AMainGameMode::SetGaugePercent(float Percent)
{
	check(MainHUD != nullptr);

	MainHUD->SetGaugePercent(Percent);
}
