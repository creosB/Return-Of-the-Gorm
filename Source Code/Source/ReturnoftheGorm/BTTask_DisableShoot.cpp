// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DisableShoot.h"
#include "AIController.h"
#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_DisableShoot::UBTTask_DisableShoot()
{
	NodeName = TEXT("Disable Shoot");
}

EBTNodeResult::Type UBTTask_DisableShoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) { return EBTNodeResult::Failed; }
	AEnemy* Character = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	if (Character == nullptr) { return EBTNodeResult::Failed; }
	// It is deleting selected blackboard key in memory
	Character->StopShooting();
	return EBTNodeResult::Succeeded;
}
