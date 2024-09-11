#include "GStreamerContainer.h"
#include "GameMode/MainGameMode.h"
#include "Kismet/GameplayStatics.h"

AGStreamerContainer::AGStreamerContainer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGStreamerContainer::BeginPlay()
{
	Super::BeginPlay();
}

void AGStreamerContainer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
	/*FString IpAddress = GetLocalIPAddress();

	GStreamerPipelineConfig = FString::Printf(TEXT("rtspsrc location=rtsp://%s:8562//test latency=100 name=source ! rtph264depay name=depayloader ! avdec_h264 name=decoder ! videoconvert name=convert ! video/x-raw,format=(string)RGBA ! videoconvert ! appsink name=sink"), *IpAddress);
	UE_LOG(LogTemp, Warning, TEXT("GStreamer Pipeline Config: %s"), *GStreamerPipelineConfig);*/
}

void AGStreamerContainer::SetWebcamTexture(UTexture2D* InTexture)
{
	check(InTexture != nullptr);

	AMainGameMode* MainGameMode = CastChecked<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameMode->RegisterGstTexture(InTexture);
}

