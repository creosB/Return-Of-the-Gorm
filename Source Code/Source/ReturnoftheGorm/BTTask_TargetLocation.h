// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TargetLocation.generated.h"



UCLASS()
class RETURNOFTHEGORM_API UBTTask_TargetLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TargetLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
