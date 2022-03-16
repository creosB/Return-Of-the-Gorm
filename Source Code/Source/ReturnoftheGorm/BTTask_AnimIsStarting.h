// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_AnimIsStarting.generated.h"

UCLASS()
class RETURNOFTHEGORM_API UBTTask_AnimIsStarting : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	
	UBTTask_AnimIsStarting();

protected:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
