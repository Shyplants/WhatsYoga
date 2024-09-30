#include "ResultGameMode.h"
#include "HUD/ResultHUD.h"
#include "GameInstance/WYGameInstance.h"

AResultGameMode::AResultGameMode()
{
}

void AResultGameMode::BeginPlay()
{
	TObjectPtr<UWYGameInstance> WYGameInstance = CastChecked<UWYGameInstance>(GetGameInstance());

	TObjectPtr<APlayerController> PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		ResultHUD = CastChecked<AResultHUD>(PlayerController->GetHUD());
		ResultHUD->AddYogaResultWidget();
	}
}
