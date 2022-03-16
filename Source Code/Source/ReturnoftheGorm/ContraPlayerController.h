// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ContraPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RETURNOFTHEGORM_API AContraPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> PauseScreenClass;

public:
	void CreatePause();

};
