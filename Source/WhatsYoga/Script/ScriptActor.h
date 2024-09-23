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

USTRUCT()
struct FTextEventArray
{
	GENERATED_BODY()

	FTextEventArray() {}

	UPROPERTY(EditAnywhere, Category="Text Events")
	TArray<FTextEvent> InnerArray;
};

USTRUCT()
struct FScoreEvent
{
	GENERATED_BODY()

	FScoreEvent() :
		bIsActive(false) {}

	UPROPERTY(EditAnywhere)
	float TriggerTime;

	UPROPERTY(EditAnywhere)
	float ClearTime;

	UPROPERTY()
	bool bIsActive;
};

USTRUCT()
struct FScoreEventArray
{
	GENERATED_BODY()

	FScoreEventArray() {}

	UPROPERTY(EditAnywhere, Category = "Score Events")
	TArray<FScoreEvent> InnerArray;
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

public:
	UFUNCTION()
	void SetContentIndex(int32 InContentInedx);

private:
	void ClearYogaTextBlock();
	void ShowYogaTextBlock(const FString& Text);
	void ClearCountdownTextBlock();
	void SetCountdownTextBlock(const FString& Text);

	void PlaySelectedAnimation(int32 AnimationIndex);

private:

	UPROPERTY(EditAnywhere, Category = "Sequencer")
	TArray<TObjectPtr<class ULevelSequence>> YogaAnimationSequences;

private:
	float AccumulatedTime;

	UPROPERTY(EditAnywhere, Category = "Text Events")
	TArray<FTextEventArray> TextEventArray;

	UPROPERTY()
	TArray<FTextEvent> TextEvents;

	UPROPERTY(EditAnywhere, Category = "Score Events")
	TArray<FScoreEventArray> ScoreEventArray;

	UPROPERTY()
	TArray<FScoreEvent> ScoreEvents;
	
	int32 LastProcessedTextEventIndex;
	int32 LastProcessedScoreEventIndex;
	int32 ContentIndex;

private:
	TObjectPtr<class AMainHUD> MainHUD;
	TObjectPtr<class UYogaInfo> YogaInfo;
};
