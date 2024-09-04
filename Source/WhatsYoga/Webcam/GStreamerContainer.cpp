#include "GStreamerContainer.h"
#include "GameMode/MainGameMode.h"
#include "Kismet/GameplayStatics.h"

AGStreamerContainer::AGStreamerContainer()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AGStreamerContainer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGStreamerContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGStreamerContainer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AGStreamerContainer::SetWebcamTexture(UTexture2D* InTexture)
{
	check(InTexture != nullptr);

	AMainGameMode* MainGameMode = CastChecked<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameMode->RegisterGstTexture(InTexture);
}

