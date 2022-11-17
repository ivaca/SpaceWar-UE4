// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERUDEM_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LoseWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WinWidget;
	UPROPERTY(EditAnywhere)
	float RestartDelay = 5.0f;

	FTimerHandle RestartTimerHandle;

	
};
