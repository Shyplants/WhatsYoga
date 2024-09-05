#include "LobbyHUD.h"
#include "UI/Intro.h"
#include "UI/PlayerInfo.h"

void ALobbyHUD::AddIntroWidget()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && IntroWidgetClass)
	{
		IntroWidget = CreateWidget<UIntro>(PlayerController, IntroWidgetClass);
		if (IntroWidget)
		{
			IntroWidget->AddToViewport();
			IntroWidget->OnClosed.BindUObject(this, &ALobbyHUD::AddPlayerInfoWidget);
		}
	}
}

void ALobbyHUD::AddPlayerInfoWidget()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && PlayerInfoWidgetClass)
	{
		PlayerInfoWidget = CreateWidget<UPlayerInfo>(PlayerController, PlayerInfoWidgetClass);
	}
	if (PlayerInfoWidget)
	{
		PlayerInfoWidget->AddToViewport();
	}
}
