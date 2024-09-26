#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "YogaSpectator.generated.h"

UCLASS()
class WHATSYOGA_API AYogaSpectator : public APawn
{
	GENERATED_BODY()

public:
	AYogaSpectator();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
