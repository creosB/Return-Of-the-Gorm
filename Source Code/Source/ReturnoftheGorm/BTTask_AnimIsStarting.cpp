// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AnimIsStarting.h"
#include "AIController.h"
#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_AnimIsStarting::UBTTask_AnimIsStarting()
{
	NodeName = TEXT("Anim Is Starting");
}

EBTNodeResult::Type UBTTask_AnimIsStarting::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) { return EBTNodeResult::Failed; }
	AEnemy* Character = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	if (Character == nullptr) { return EBTNodeResult::Failed; }
	// It is deleting selected blackboard key in memory
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("AnimIsStarting"), Character->AnimIsPlaying);
	return EBTNodeResult::Succeeded;
}
