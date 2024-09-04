// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScriptActor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogScriptActor, Log, All);

USTRUCT()
struct FTextEvent
{
	GENERATED_BODY()

	FTextEvent() :
		bIsActive(false) {}

	UPROPERTY(EditAnywhere)
	float TriggerTime;

	UPROPERTY(EditAnywhere)
	float ClearTime;

	UPROPERTY(EditAnywhere)
	FString Text;

	UPROPERTY()
	bool bIsActive;
};

UCLASS()
class WHATSYOGA_API AScriptActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AScriptActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	void ClearYogaTextBlock();
	void ShowYogaTextBlock(const FString& Text);
	void SetCountdownText(const FString& Text);

	void PlaySelectedAnimation(int32 AnimationIndex);

private:

	/*UPROPERTY(EditAnywhere)
	TObjectPtr<class ULevelSequence> YogaSequence;

	UPROPERTY()
	TObjectPtr<class ALevelSequenceActor> YogaSequenceActor;*/

	UPROPERTY()
	TObjectPtr<class ULevelSequencePlayer> YogaSequencePlayer;

	UPROPERTY()
	TObjectPtr<class ULevelSequence> YogaSequence;

	UPROPERTY()
	TObjectPtr<class UMovieScene> YogaMovieScene;
	
	UPROPERTY()
	TArray<TObjectPtr<class UMovieSceneSkeletalAnimationSection>> YogaAnimationSections;
	

private:
	float AccumulatedTime;

	UPROPERTY(EditAnywhere)
	TArray<FTextEvent> TextEvents;
	
	int32 LastProcessedEventIndex;

private:
	TObjectPtr<class AMainHUD> MainHUD;
	TObjectPtr<class UYogaInfo> YogaInfo;
};
