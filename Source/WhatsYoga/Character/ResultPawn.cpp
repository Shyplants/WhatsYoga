#include "ResultPawn.h"
#include "Kismet/KismetSystemLibrary.h"

AResultPawn::AResultPawn()
{
}

void AResultPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AResultPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &AResultPawn::ExitGame);
	PlayerInputComponent->BindKey(EKeys::Q, IE_Pressed, this, &AResultPawn::ExitGame);
}

void AResultPawn::ExitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}

