// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ContraAIController.generated.h"

UCLASS()
class RETURNOFTHEGORM_API AContraAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* ContraAI;
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* SoliderAI;
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* JumpAI;

public:
	virtual void Tick(float DeltaSeconds) override;
	bool IsDead() const;

protected:
	virtual void BeginPlay() override;
};
