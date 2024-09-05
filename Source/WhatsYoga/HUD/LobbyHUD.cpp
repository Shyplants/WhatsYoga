#include "LobbyHUD.h"
#include "UI/Intro.h"

void ALobbyHUD::AddIntro()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && IntroClass)
	{
		Intro = CreateWidget<UIntro>(PlayerController, IntroClass);
		if (Intro)
		{
			Intro->AddToViewport();
		}
	}
}
