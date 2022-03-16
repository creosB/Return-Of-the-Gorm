#include "BTTask_Shoot.h"
#include "AIController.h"
#include "Enemy.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = "Shoot";
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) { return EBTNodeResult::Failed; }

	AEnemy* Character = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	if (Character == nullptr) { return EBTNodeResult::Failed; }
	Character->Shoot();

	return EBTNodeResult::Succeeded;
}
