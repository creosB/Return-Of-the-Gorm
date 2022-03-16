// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
	NodeName = TEXT("Update Player Location If Seen");
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AActor* Character = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (OwnerComp.GetAIOwner() == nullptr) { return; }
	if (Character == nullptr)
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		return;
	}

	RangeToPlayer = Cast<AActor>(OwnerComp.GetAIOwner())->GetDistanceTo(Character);

	if (OwnerComp.GetAIOwner()->LineOfSightTo(Character) && RangeToPlayer <= 900.0f)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(
			GetSelectedBlackboardKey(),
			Character);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(
			TEXT("LastKnownPlayerLocation"), Character->GetTargetLocation());
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
}
