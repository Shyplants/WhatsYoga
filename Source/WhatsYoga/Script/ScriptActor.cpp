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
#include "Tracks/MovieSceneSkeletalAnimationTrack.h"
#include "Sections/MovieSceneSkeletalAnimationSection.h"

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

	TArray<AActor*> SequenceActor;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ALevelSequenceActor::StaticClass(), FName("Yoga"), SequenceActor);
	if (SequenceActor.IsValidIndex(0))
	{
		ALevelSequenceActor* LevelSequenceActor = Cast<ALevelSequenceActor>(SequenceActor[0]);
		if (LevelSequenceActor)
		{
			YogaSequencePlayer = LevelSequenceActor->GetSequencePlayer();
			// check(YogaSequencePlayer != nullptr);

			YogaSequence = LevelSequenceActor->GetSequence();
			check(YogaSequence != nullptr);

			YogaMovieScene = YogaSequence->GetMovieScene();
		}
	}

	for (UMovieSceneTrack* Track : YogaMovieScene->GetBindings()[0].GetTracks())
	{
		if (UMovieSceneSkeletalAnimationTrack* AnimTrack = Cast<UMovieSceneSkeletalAnimationTrack>(Track))
		{
			int SectionIndex = 0;
			for (UMovieSceneSection* Section : AnimTrack->GetAllSections())
			{
				YogaAnimationSections.Add(Cast<UMovieSceneSkeletalAnimationSection>(Section));
			}
		}
	}

	PlaySelectedAnimation(1);
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

void AScriptActor::PlaySelectedAnimation(int32 AnimationIndex)
{
	FString JsonData = JsonHelper::CreateEventJson(TEXT("start"));

	check(AnimationIndex < YogaAnimationSections.Num());

	for (int32 i = 0; i < YogaAnimationSections.Num(); ++i)
	{
		YogaAnimationSections[i]->SetIsActive(AnimationIndex == i);

		if (i == AnimationIndex)
		{
			FFrameNumber AnimationStartFrame = YogaAnimationSections[i]->GetInclusiveStartFrame();
			FFrameNumber AnimationEndFrame = YogaAnimationSections[i]->GetExclusiveEndFrame();
			YogaSequencePlayer->SetTimeRange(AnimationStartFrame.Value, AnimationEndFrame.Value);
		}
	}

	UWYGameInstance* WYGameInstance = CastChecked<UWYGameInstance>(GetGameInstance());
	WYGameInstance->TCPSendMessage(JsonData);

	check(YogaSequencePlayer);
	YogaSequencePlayer->Play();
}

