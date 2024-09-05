#include "Intro.h"
#include "Animation/WidgetAnimation.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/LobbyGameMode.h"

void UIntro::NativeConstruct()
{
	Super::NativeConstruct();

	if (TextAnim)
	{
		PlayAnimation(TextAnim, 0.0f, 0);
	}

	if (IntroAnim)
	{
		FTimerHandle IntroAnimTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(IntroAnimTimerHandle, this, &UIntro::PlayIntroAnimation, 2.0f, false);

	}
}

void UIntro::OnIntroAnimationFinished()
{
	OnClosed.ExecuteIfBound();
}

void UIntro::PlayIntroAnimation()
{
	if (IntroAnim)
	{
		FWidgetAnimationDynamicEvent IntroAnimationFinishedDelegate;
		IntroAnimationFinishedDelegate.BindUFunction(this, FName("OnIntroAnimationFinished"));
		BindToAnimationFinished(IntroAnim, IntroAnimationFinishedDelegate);

		PlayAnimation(IntroAnim);
	}
}
