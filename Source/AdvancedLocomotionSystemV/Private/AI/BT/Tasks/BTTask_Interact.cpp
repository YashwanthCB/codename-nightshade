


#include "AI/BT/Tasks/BTTask_Interact.h"

#include "AI/Components/InvestigationPlanComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_Interact::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto InvestigationActionContainer = Cast<UInvestigationActionContainer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(InInvestigationActionContainer.SelectedKeyName));

	if (!IsValid(InvestigationActionContainer)|| InvestigationActionContainer->InvestigationAction.InvestigationActionType != EInvestigationActionType::Interact)
		return EBTNodeResult::Failed;

	auto Disturbable = InvestigationActionContainer->InvestigationAction.DisturbableActor;
	OwnerComp.GetAIOwner()->GetPawn()->SetActorRotation(Disturbable->GetIdealInteractionTransform().GetRotation());
	Disturbable->Fix(OwnerComp.GetAIOwner());
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_Interact::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_Interact::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
