// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ResultPawn.generated.h"

UCLASS()
class WHATSYOGA_API AResultPawn : public APawn
{
	GENERATED_BODY()

public:
	AResultPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void ExitGame();
};
