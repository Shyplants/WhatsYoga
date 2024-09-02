#include "ScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/MainHUD.h"
#include "UI/YogaInfo.h"


AScriptActor::AScriptActor()
{
	PrimaryActorTick.bCanEverTick = true;
	AccumulatedTime = 0.0f;
	LastProcessedEventIndex = -1;
}

void AScriptActor::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		MainHUD = CastChecked<AMainHUD>(PlayerController->GetHUD());
	}
}

void AScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AccumulatedTime += DeltaTime;
	if (LastProcessedEventIndex + 1 < TextEvents.Num())
	{
		FTextEvent& Event = TextEvents[LastProcessedEventIndex + 1];
		if (!Event.bIsActive)
		{
			if (AccumulatedTime >= Event.TriggerTime)
			{
				Event.bIsActive = true;
				ShowYogaTextBlock(Event.Text);
				SetCountdownText(FString::Printf(TEXT("%d"), static_cast<int32>(Event.ClearTime - AccumulatedTime)));
			}
		}
		else if (AccumulatedTime >= Event.ClearTime)
		{
			ClearYogaTextBlock();
			LastProcessedEventIndex++;
		}
		else
		{
			SetCountdownText(FString::Printf(TEXT("%d"), static_cast<int32>(Event.ClearTime - AccumulatedTime)));
		}
	}
}

void AScriptActor::ClearYogaTextBlock()
{
	if (nullptr == YogaInfo)
	{
		YogaInfo = MainHUD->GetYogaInfo();
	}

	YogaInfo->SetYogaText(TEXT(""));
	YogaInfo->SetCountdownVisible(false);
}

void AScriptActor::ShowYogaTextBlock(const FString& Text)
{
	if (nullptr == YogaInfo)
	{
		YogaInfo = MainHUD->GetYogaInfo();
	}

	YogaInfo->SetYogaText(Text);
	YogaInfo->SetCountdownVisible(true);
}

void AScriptActor::SetCountdownText(const FString& Text)
{
	if (nullptr == YogaInfo)
	{
		YogaInfo = MainHUD->GetYogaInfo();
	}

	YogaInfo->SetCountdownText(Text);
}

