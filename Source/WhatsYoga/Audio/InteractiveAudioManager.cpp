#include "InteractiveAudioManager.h"
#include "EngineUtils.h"

AInteractiveAudioManager* AInteractiveAudioManager::Instance = nullptr;

AInteractiveAudioManager* AInteractiveAudioManager::GetInstance(UWorld* World)
{
	if (!Instance)
	{
		for (TActorIterator<AInteractiveAudioManager> It(World); It; ++It)
		{
			Instance = *It;
			break;
		}

		if (!Instance)
		{
			Instance = World->SpawnActor<AInteractiveAudioManager>();
		}
	}

	return Instance;
}

bool AInteractiveAudioManager::PlayAudio(const FAudioData& AudioData)
{
	if (PriorityQueues[static_cast<int32>(AudioData.AudioType)].Num() > 1 && !AudioData.bForcePlay)
	{
		return false;
	}

	AddToPriorityQueue(AudioData);
	if (PriorityQueues[static_cast<int32>(AudioData.AudioType)].Num() == 1)
	{
		PlayFromPriorityQueue(AudioData.AudioType);
	}
	return true;
}

AInteractiveAudioManager::AInteractiveAudioManager()
{
	PrimaryActorTick.bCanEverTick = false;

	AudioComponents.Reserve(static_cast<int32>(EAudioType::End));
	PriorityQueues.SetNum(static_cast<int32>(EAudioType::End));

	for (int32 i = 0; i < static_cast<int32>(EAudioType::End); ++i)
	{
		UAudioComponent* AudioComponent = CreateDefaultSubobject<UAudioComponent>(*FString::Printf(TEXT("AudioComponent_%d"), i));
		AudioComponent->bAutoActivate = false;
		AudioComponent->bAutoDestroy = false;
		AudioComponent->SetupAttachment(RootComponent);

		AudioComponent->OnAudioFinished.AddDynamic(this, &AInteractiveAudioManager::OnAudioFinished);

		AudioComponents.Emplace(AudioComponent);
	}
}

void AInteractiveAudioManager::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractiveAudioManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Instance = nullptr;

	for (UAudioComponent* AudioComponent : AudioComponents)
	{
		if (AudioComponent)
		{
			AudioComponent->OnAudioFinished.RemoveDynamic(this, &AInteractiveAudioManager::OnAudioFinished);
		}
	}
}

void AInteractiveAudioManager::AddToPriorityQueue(const FAudioData& AudioData)
{
	int32 Index = static_cast<int32>(AudioData.AudioType);
	PriorityQueues[Index].HeapPush(AudioData, [](const FAudioData& A, const FAudioData& B) {
		return A.Priority > B.Priority;
		});
}

void AInteractiveAudioManager::PlayFromPriorityQueue(EAudioType AudioType)
{
	int32 Index = static_cast<int32>(AudioType);
	if (PriorityQueues[Index].Num() == 0)
	{
		return;
	}

	FAudioData TopAudioData;
	PriorityQueues[Index].HeapPop(TopAudioData, [](const FAudioData& A, const FAudioData& B) {
		return A.Priority > B.Priority;
		});

	UAudioComponent* AudioComponent = AudioComponents[Index];
	if (AudioComponent && TopAudioData.Sound)
	{
		AudioComponent->SetSound(TopAudioData.Sound);
		AudioComponent->Play();
	}
}

void AInteractiveAudioManager::OnAudioFinished()
{
	for (int32 i = 0; i < static_cast<int32>(EAudioType::End); ++i)
	{
		PlayFromPriorityQueue(static_cast<EAudioType>(i));
	}
}
