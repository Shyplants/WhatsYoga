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
	PlayerNameTextBox->SetText(FText::FromString(WYGameInstance->GetPlayerName()));
	YogaContentTextBox->SetText(FText::FromString(WYGameInstance->GetContentName()));
}

void UPlayerInfo::LoadYogaMap()
{
	ALobbyGameMode* LobbyGameMode = CastChecked<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	LobbyGameMode->LoadYogaMap();
}
