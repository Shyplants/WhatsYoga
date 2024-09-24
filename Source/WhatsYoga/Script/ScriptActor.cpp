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
	LastProcessedTextEventIndex = -1;
	LastProcessedScoreEventIndex = -1;
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
		UE_LOG(LogTemp, Log, TEXT("ContentIndex : [%d]"), ContentIndex);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ContentIndex is unvalid"));
	}

	if (ScoreEventArray.IsValidIndex(ContentIndex))
	{
		ScoreEvents = ScoreEventArray[ContentIndex].InnerArray;
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

	check(ContentRunningTime != 0.0f);
	if (AccumulatedTime > 0.5f)
	{
		YogaInfo->SetContentProgressBarPercent(AccumulatedTime / ContentRunningTime);
	}

	if (LastProcessedTextEventIndex + 1 < TextEvents.Num())
	{
		FTextEvent& Event = TextEvents[LastProcessedTextEventIndex + 1];
		if (!Event.bIsActive)
		{
			if (AccumulatedTime >= Event.TriggerTime)
			{
				Event.bIsActive = true;
				ShowYogaTextBlock(Event.Text);
				// SetCountdownText(FString::Printf(TEXT("%d"), static_cast<int32>(Event.ClearTime - AccumulatedTime)));
			}
		}
		else if (AccumulatedTime >= Event.ClearTime)
		{
			ClearYogaTextBlock();
			LastProcessedTextEventIndex++;
		}
		else
		{
			// SetCountdownText(FString::Printf(TEXT("%d"), static_cast<int32>(Event.ClearTime - AccumulatedTime)));
		}
	}

	if (LastProcessedScoreEventIndex + 1 < ScoreEvents.Num())
	{
		FScoreEvent& Event = ScoreEvents[LastProcessedScoreEventIndex + 1];
		if (!Event.bIsActive)
		{
			if (AccumulatedTime >= Event.TriggerTime)
			{
				Event.bIsActive = true;
				YogaInfo->SetCountdownVisible(true);
				SetCountdownTextBlock(FString::Printf(TEXT("%d"), static_cast<int32>(Event.ClearTime - AccumulatedTime)));
			}
		}
		else if (AccumulatedTime >= Event.ClearTime)
		{
			YogaInfo->SetCountdownVisible(false);
			LastProcessedScoreEventIndex++;
		}
		else
		{
			SetCountdownTextBlock(FString::Printf(TEXT("%d"), static_cast<int32>(Event.ClearTime - AccumulatedTime)));
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
	// YogaInfo->SetCountdownVisible(false);
}

void AScriptActor::ShowYogaTextBlock(const FString& Text)
{
	if (nullptr == YogaInfo)
	{
		YogaInfo = MainHUD->GetYogaInfo();
	}

	YogaInfo->SetYogaText(Text);
	// YogaInfo->SetCountdownVisible(true);
}

void AScriptActor::ClearCountdownTextBlock()
{
	if (nullptr == YogaInfo)
	{
		YogaInfo = MainHUD->GetYogaInfo();
	}

	YogaInfo->SetCountdownText(TEXT(""));
}

void AScriptActor::SetCountdownTextBlock(const FString& Text)
{
	if (nullptr == YogaInfo)
	{
		YogaInfo = MainHUD->GetYogaInfo();
	}

	UE_LOG(LogTemp, Log, TEXT("Countdown Called : [%d]"), FCString::Atoi(*Text));
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

	ContentRunningTime = static_cast<float>(SelectedSequence->GetMovieScene()->GetPlaybackRange().GetUpperBound().GetValue().Value / 24000.0f);
	
	TObjectPtr<UWYGameInstance> WYGameInstance = CastChecked<UWYGameInstance>(GetGameInstance());
	WYGameInstance->TCPSendMessage(JsonData);
}