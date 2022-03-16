// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TargetLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "ReturnoftheGorm/Enemy.h"
#include "Kismet/GameplayStatics.h"


UBTTask_TargetLocation::UBTTask_TargetLocation()
{
	NodeName = TEXT("Target Location");
}

EBTNodeResult::Type UBTTask_TargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) { return EBTNodeResult::Failed; }
	AEnemy* Character = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	if (Character == nullptr) { return EBTNodeResult::Failed; }
	OwnerComp.GetAIOwner()->MoveToLocation(Character->TargetLocation,100);
	OwnerComp.GetAIOwner()->SetFocus(UGameplayStatics::GetPlayerCharacter(this,0));
	return EBTNodeResult::Succeeded;
}
