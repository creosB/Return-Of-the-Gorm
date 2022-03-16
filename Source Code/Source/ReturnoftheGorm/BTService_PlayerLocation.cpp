// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"


UBTService_PlayerLocation::UBTService_PlayerLocation()
{
	NodeName = TEXT("Update Player");
}

void UBTService_PlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AActor* Character = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (Character == nullptr) { return; }
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(
		GetSelectedBlackboardKey(),
		Character->GetActorLocation());
}
