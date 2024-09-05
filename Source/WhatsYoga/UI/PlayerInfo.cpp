#include "PlayerInfo.h"
#include "Components/EditableTextBox.h"
#include "GameMode/LobbyGameMode.h"
#include "GameInstance/WYGameInstance.h"

void UPlayerInfo::NativeConstruct()
{
	Super::NativeConstruct();

	LoadPlayerInfo();
	LoadYogaMap();
}

void UPlayerInfo::LoadPlayerInfo()
{
	UWYGameInstance* WYGameInstance = CastChecked<UWYGameInstance>(GetGameInstance());
	PlayerNameTextBox->SetText(FText::FromString(WYGameInstance->GetName()));
}

void UPlayerInfo::LoadYogaMap()
{
	ALobbyGameMode* LobbyGameMode = CastChecked<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	LobbyGameMode->LoadYogaMap();
}
