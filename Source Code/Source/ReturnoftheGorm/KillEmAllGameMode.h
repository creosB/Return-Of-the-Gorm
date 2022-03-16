// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReturnoftheGormGameMode.h"
#include "ContraPlayerController.h"
#include "KillEmAllGameMode.generated.h"

/**
*
*/
UCLASS()
class RETURNOFTHEGORM_API AKillEmAllGameMode : public AReturnoftheGormGameMode
{
	GENERATED_BODY()

public:
	virtual void PawnKilled(APawn* PawnKiled) override;

	UFUNCTION (BlueprintImplementableEvent)
	void ShowScore();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 ScoreText();
	int32 Score = 0;

	void CallPause();

private:
	void EndGame(bool bIsPlayerWinner);

	AContraPlayerController* ContraController;
};
