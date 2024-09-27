#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "InteractiveAudioManager.generated.h"

UENUM(BlueprintType)
enum class EAudioType : uint8
{
	BGM UMETA(Displayname = "Background Music"),
	SFX UMETA(DisplayName = "Sound Effect"),
	End UMETA(DisplayName = "End"),
};

USTRUCT(BlueprintType)
struct FAudioData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	EAudioType AudioType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	int32 Priority;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	bool bForcePlay;

	FAudioData()
		: Sound(nullptr)
		, AudioType(EAudioType::SFX)
		, Priority(0)
		, bForcePlay(false)
	{}

	FAudioData(USoundBase* InSound, EAudioType InAudioType, int32 InPriority, bool InForcePlay = false)
		: Sound(InSound)
		, AudioType(InAudioType)
		, Priority(InPriority)
		, bForcePlay(InForcePlay)
	{}
};


UCLASS()
class WHATSYOGA_API AInteractiveAudioManager : public AActor
{
	GENERATED_BODY()

public:
	static AInteractiveAudioManager* GetInstance(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	bool PlayAudio(const FAudioData& AudioData);


private:
	AInteractiveAudioManager();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	static AInteractiveAudioManager* Instance;

	TArray<UAudioComponent*> AudioComponents;
	TArray<TArray<FAudioData>> PriorityQueues;

	void AddToPriorityQueue(const FAudioData& AudioData);
	void PlayFromPriorityQueue(EAudioType AudioType);

	UFUNCTION()
	void OnAudioFinished();
};
