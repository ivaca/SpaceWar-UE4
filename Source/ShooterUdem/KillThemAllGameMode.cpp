// Fill out your copyright notice in the Description page of Project Settings.


#include "KillThemAllGameMode.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "ShooterAIController.h"

void AKillThemAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);
	auto PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if (PlayerController)
	{
		EndGame(false);
	}

	for (AShooterAIController* AIController : TActorRange<AShooterAIController>(GetWorld()))
	{
		if (!AIController->IsDead())
			return;
	}
	EndGame(true);
}

void AKillThemAllGameMode::EndGame(bool bIsPlayerWon)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		auto bIsPlayerController = Controller->IsPlayerController() == bIsPlayerWon;
		Controller->GameHasEnded(Controller->GetPawn(), bIsPlayerController);
	}
}
