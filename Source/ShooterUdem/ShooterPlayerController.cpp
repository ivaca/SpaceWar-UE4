// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"

#include "Blueprint/UserWidget.h"

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (bIsWinner)
	{
		UUserWidget* WinWidgetRef = CreateWidget(this, WinWidget);
		WinWidgetRef->AddToViewport();
	}
	else
	{
		UUserWidget* LoseWidgetRef = CreateWidget(this, LoseWidget);
		if (LoseWidgetRef)
		{
			LoseWidgetRef->AddToViewport();
		}
	}


	GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &APlayerController::RestartLevel, RestartDelay);
}
