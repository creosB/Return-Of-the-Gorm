// Fill out your copyright notice in the Description page of Project Settings.

#include "ContraAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ContraCharacter.h"

void AContraAIController::BeginPlay()
{
	Super::BeginPlay();
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	// Run different AI for various enemies.
	if (ContraAI != nullptr && SoliderAI != nullptr && JumpAI != nullptr)
	{
		if (GetPawn()->ActorHasTag("ContraAI"))
		{
			RunBehaviorTree(ContraAI);
		}
		else if (GetPawn()->ActorHasTag("SoliderAI"))
		{
			RunBehaviorTree(SoliderAI);
		}
		else if (GetPawn()->ActorHasTag("JumpAI"))
		{
			RunBehaviorTree(JumpAI);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Tag not found"));
			RunBehaviorTree(ContraAI);
		}

		// enemy spawn location
		GetBlackboardComponent()->SetValueAsVector(
			TEXT("StartLocation"),
			GetPawn()->GetActorLocation()
		);
	}
}

void AContraAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

bool AContraAIController::IsDead() const
{
	// When enemy was dead, it will give information to controller.
	AContraCharacter* ControlledCharacter = Cast<AContraCharacter>(GetPawn());
	if (ControlledCharacter != nullptr)
	{
		return ControlledCharacter->IsDead();
	}

	return true;
}
