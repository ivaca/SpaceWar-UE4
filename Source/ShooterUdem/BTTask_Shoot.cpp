// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"

#include "AIController.h"
#include "Character/ShooterCharacter.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = "Shoot";
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	auto AIOwner = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (AIOwner)
	{
		AIOwner->Shoot();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
