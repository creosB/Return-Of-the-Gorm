// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossGrenade.generated.h"


UCLASS()
class RETURNOFTHEGORM_API UBTTask_BossGrenade : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_BossGrenade();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
