#include "LobbyGameMode.h"
#include "HUD/LobbyHUD.h"
#include "GameInstance/WYGameInstance.h"
#include "Kismet/GameplayStatics.h"

ALobbyGameMode::ALobbyGameMode()
{
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWYGameInstance* WYGameInstance = CastChecked<UWYGameInstance>(GetGameInstance());

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		LobbyHUD = CastChecked<ALobbyHUD>(PlayerController->GetHUD());
		LobbyHUD->AddIntroWidget();
	}
}

void ALobbyGameMode::LoadYogaMap()
{
	FTimerHandle OpenYogaMapTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(OpenYogaMapTimerHandle, this, &ALobbyGameMode::OpenYogaMap, 5.0f, false);
}

void ALobbyGameMode::OpenYogaMap()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("YogaMap"));
}
