#include "ScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/MainHUD.h"
#include "UI/YogaInfo.h"
#include "Helper/JsonHelper.h"
#include "GameInstance/WYGameInstance.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieScene.h"
#include "MovieSceneTrack.h"
#include "MovieSceneSection.h"
#include "MovieSceneSequencePlayer.h"
#include "MovieSceneSequence.h"
#include "Tracks/MovieSceneSkeletalAnimationTrack.h"
#include "Sections/MovieSceneSkeletalAnimationSection.h"

AScriptActor::AScriptActor()
{
	PrimaryActorTick.bCanEverTick = true;
	AccumulatedTime = 0.0f;
	LastProcessedEventIndex = -1;
	ContentIndex = 0;
}

void AScriptActor::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		MainHUD = CastChecked<AMainHUD>(PlayerController->GetHUD());
	}


	TObjectPtr<UWYGameInstance> WYGameInstance = CastChecked<UWYGameInstance>(GetGameInstance());
	ContentIndex = WYGameInstance->GetContentIndex();

	if (TextEventArray.IsValidIndex(ContentIndex))
	{
		TextEvents = TextEventArray[ContentIndex].InnerArray;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ContentIndex is unvalid"));
	}
	
	PlaySelectedAnimation(ContentIndex);
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

void AScriptActor::SetContentIndex(int32 InContentInedx)
{
	if (TextEventArray.IsValidIndex(InContentInedx))
	{
		ContentIndex = InContentInedx;
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

void AScriptActor::PlaySelectedAnimation(int32 AnimationIndex)
{
	FString JsonData = JsonHelper::CreateEventJson(TEXT("start"));

	ULevelSequence* SelectedSequence = YogaAnimationSequences[AnimationIndex];
	
	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	ALevelSequenceActor* OutActor = nullptr;

	ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		SelectedSequence,
		PlaybackSettings,
		OutActor
	);

	SequencePlayer->Play();

	TObjectPtr<UWYGameInstance> WYGameInstance = CastChecked<UWYGameInstance>(GetGameInstance());
	WYGameInstance->TCPSendMessage(JsonData);
}

