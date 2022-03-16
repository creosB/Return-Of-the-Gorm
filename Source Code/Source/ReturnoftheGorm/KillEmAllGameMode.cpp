// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ContraAIController.h"
#include "ContraPlayerController.h"
#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKiled)
{
	Super::PawnKilled(PawnKiled);

	APlayerController* PlayerController = Cast<APlayerController>(PawnKiled->GetController());
	if (PlayerController != nullptr)
	{
		EndGame(false);
	}

	for (AContraAIController* Controller : TActorRange<AContraAIController>(GetWorld()))
	{
		if (Controller != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Controller is not found!"));
		}
	}

	if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(PawnKiled))
	{
		ScoreText();
	}
	if (PawnKiled->ActorHasTag("Boss"))
	{
		EndGame(true);
		ShowScore();
	}
}


void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	// it will call all controller in game
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		// if player is winner, returning true value
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}

void AKillEmAllGameMode::CallPause()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	ContraController = Cast<AContraPlayerController>(GetWorld()->GetFirstPlayerController());
	if(ContraController)
	{
		ContraController->CreatePause();
	}
}


int32 AKillEmAllGameMode::ScoreText()
{
	return Score += 100;
}
