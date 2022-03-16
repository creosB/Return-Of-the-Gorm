// Fill out your copyright notice in the Description page of Project Settings.


#include "ContraPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AContraPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);
	// creating losee screen and adding to viewport
	UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
	UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);

	// showing lose or win widgets.
	if (bIsWinner)
	{
		if (WinScreen != nullptr)
		{
			WinScreen->AddToViewport();
		}
	}
	else
	{
		if (LoseScreen != nullptr)
		{
			LoseScreen->AddToViewport();
		}
	}
}

void AContraPlayerController::CreatePause()
{
	UUserWidget* PauseScreen = CreateWidget(this, PauseScreenClass);
	if(PauseScreen)
	{
		PauseScreen->AddToViewport();
	}
}

