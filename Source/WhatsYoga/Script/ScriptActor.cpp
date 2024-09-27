#include "ScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/MainHUD.h"
#include "UI/YogaInfo.h"
#include "Helper/JsonHelper.h"
#include "GameInstance/WYGameInstance.h"
#include "GameMode/MainGameMode.h"
#include "Audio/InteractiveAudioManager.h"
#include "Sound/SoundCue.h"
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
	LastProcessedExplanationTextEventIndex = -1;
	LastProcessedEfficacyTextEventIndex = -1;
	LastProcessedNameTextEventIndex = -1;
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

	if (YogaExplanationTextEventArray.IsValidIndex(ContentIndex))
	{
		YogaExplanationTextEvents = YogaExplanationTextEventArray[ContentIndex].InnerArray;
		// UE_LOG(LogTemp, Log, TEXT("ContentIndex : [%d]"), ContentIndex);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("YogaExplanationTextEventArray ContentIndex is unvalid"));
	}

	if (YogaEfficacyTextEventArray.IsValidIndex(ContentIndex))
	{
		YogaEfficacyTextEvents = YogaEfficacyTextEventArray[ContentIndex].InnerArray;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("YogaEfficacyTextEventArray ContentIndex is unvalid"));
	}

	if (YogaNameTextEventArray.IsValidIndex(ContentIndex))
	{
		YogaNameTextEvents = YogaNameTextEventArray[ContentIndex].InnerArray;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("YogaNameTextEventArray ContentIndex is unvalid"));
	}

	if (ScoreEventArray.IsValidIndex(ContentIndex))
	{
		ScoreEvents = ScoreEventArray[ContentIndex].InnerArray;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ContentIndex is unvalid"));
	}

	InteractiveAudioManager = AInteractiveAudioManager::GetInstance(GetWorld());
	
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

	// YogaExplanation
	if (LastProcessedExplanationTextEventIndex + 1 < YogaExplanationTextEvents.Num())
	{
		FTextEvent& Event = YogaExplanationTextEvents[LastProcessedExplanationTextEventIndex + 1];
		if (!Event.bIsActive)
		{
			if (AccumulatedTime >= Event.TriggerTime)
			{
				Event.bIsActive = true;
				ShowYogaExplanationTextBlock(Event.Text);
				// SetCountdownText(FString::Printf(TEXT("%d"), static_cast<int32>(Event.ClearTime - AccumulatedTime)));

				if (InteractiveAudioManager && YogaExplanationSoundArray.IsValidIndex(LastProcessedExplanationTextEventIndex + 1))
				{
					FAudioData AudioData(YogaExplanationSoundArray[LastProcessedExplanationTextEventIndex + 1], EAudioType::SFX, 0, true);
					InteractiveAudioManager->PlayAudio(AudioData);
				}
			}
		}
		else if (AccumulatedTime >= Event.ClearTime)
		{
			ClearYogaExplanationTextBlock();
			LastProcessedExplanationTextEventIndex++;
		}
		else
		{
			// SetCountdownText(FString::Printf(TEXT("%d"), static_cast<int32>(Event.ClearTime - AccumulatedTime)));
		}
	}

	// YogaEfficacy
	if (LastProcessedEfficacyTextEventIndex + 1 < YogaEfficacyTextEvents.Num())
	{
		FTextEvent& Event = YogaEfficacyTextEvents[LastProcessedEfficacyTextEventIndex + 1];
		if (!Event.bIsActive)
		{
			if (AccumulatedTime >= Event.TriggerTime)
			{
				Event.bIsActive = true;
				ShowYogaEfficacyTextBlock(Event.Text);
				// SetCountdownText(FString::Printf(TEXT("%d"), static_cast<int32>(Event.ClearTime - AccumulatedTime)));
			}
		}
		else if (AccumulatedTime >= Event.ClearTime)
		{
			LastProcessedEfficacyTextEventIndex++;
		}
		else
		{
			// SetCountdownText(FString::Printf(TEXT("%d"), static_cast<int32>(Event.ClearTime - AccumulatedTime)));
		}
	}

	// YogaName
	if (LastProcessedNameTextEventIndex + 1 < YogaNameTextEvents.Num())
	{
		FTextEvent& Event = YogaNameTextEvents[LastProcessedNameTextEventIndex + 1];
		if (!Event.bIsActive)
		{
			if (AccumulatedTime >= Event.TriggerTime)
			{
				Event.bIsActive = true;
				ShowYogaNameTextBlock(Event.Text);
			}
		}
		else if (AccumulatedTime >= Event.ClearTime)
		{
			LastProcessedNameTextEventIndex++;
		}
		else
		{
			// SetCountdownText(FString::Printf(TEXT("%d"), static_cast<int32>(Event.ClearTime - AccumulatedTime)));
		}
	}

	// YogaScore
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
			YogaInfo->SetStarFillTexture(LastProcessedScoreEventIndex + 1);
			LastProcessedScoreEventIndex++;
		}
		else
		{
			SetCountdownTextBlock(FString::Printf(TEXT("%d"), static_cast<int32>(Event.ClearTime - AccumulatedTime)));
		}
		
	}
}

void AScriptActor::OnYogaSequenceFinished()
{
	// UE_LOG(LogTemp, Log, TEXT("OnYogaSequenceFinished called"));

	AMainGameMode* MainGameMode = CastChecked<AMainGameMode>(GetWorld()->GetAuthGameMode());
	MainGameMode->LoadResultMap();
}

void AScriptActor::ClearYogaExplanationTextBlock()
{
	if (nullptr == YogaInfo)
	{
		YogaInfo = MainHUD->GetYogaInfo();
	}

	YogaInfo->SetYogaExplanationText(TEXT(""));
	// YogaInfo->SetCountdownVisible(false);
}

void AScriptActor::ShowYogaExplanationTextBlock(const FString& Text)
{
	if (nullptr == YogaInfo)
	{
		YogaInfo = MainHUD->GetYogaInfo();
	}

	YogaInfo->SetYogaExplanationText(Text);
	// YogaInfo->SetCountdownVisible(true);
}

void AScriptActor::ShowYogaEfficacyTextBlock(const FString& Text)
{
	if (nullptr == YogaInfo)
	{
		YogaInfo = MainHUD->GetYogaInfo();
	}

	YogaInfo->SetYogaEfficacyText(Text);
}

void AScriptActor::ShowYogaNameTextBlock(const FString& Text)
{
	if (nullptr == YogaInfo)
	{
		YogaInfo = MainHUD->GetYogaInfo();
	}

	YogaInfo->SetYogaNameText(Text);
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
	SequencePlayer->OnFinished.AddDynamic(this, &AScriptActor::OnYogaSequenceFinished);

	ContentRunningTime = static_cast<float>(SelectedSequence->GetMovieScene()->GetPlaybackRange().GetUpperBound().GetValue().Value / 24000.0f);
	
	TObjectPtr<UWYGameInstance> WYGameInstance = CastChecked<UWYGameInstance>(GetGameInstance());
	WYGameInstance->TCPSendMessage(JsonData);
}