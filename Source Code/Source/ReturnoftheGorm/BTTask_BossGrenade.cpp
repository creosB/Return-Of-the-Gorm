#include "BTTask_BossGrenade.h"
#include "AIController.h"
#include "Enemy.h"

UBTTask_BossGrenade::UBTTask_BossGrenade()
{
	NodeName = "Boss Grenade";
}

EBTNodeResult::Type UBTTask_BossGrenade::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) { return EBTNodeResult::Failed; }

	AEnemy* Character = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	if (Character == nullptr) { return EBTNodeResult::Failed; }
	Character->BossAIGrenade();
	return EBTNodeResult::Succeeded;
}
