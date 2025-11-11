


#include "AI/BT/Tasks/ExtractInvestigationPlan.h"
#include "AI/SimpleDisturbanceIndicator.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controllers/AI/PatrolGuardAIController.h"

EBTNodeResult::Type UExtractInvestigationPlan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto PatrolAIController = Cast<APatrolGuardAIController>(OwnerComp.GetAIOwner());
	bool HasPlan = PatrolAIController->GetInvestigationPlanComponent()->HasInvestigationActionPlan();

	if (!HasPlan)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AIAwarenessState.SelectedKeyName, static_cast<uint8>(EAIAwarenessState::Roaming));
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(OutInvestigationActionContainer.SelectedKeyName, nullptr);
		return EBTNodeResult::Failed;
	}	
	FInvestigationAction InvestigationAction = PatrolAIController->GetInvestigationPlanComponent()->ConsumeInvestigationPlan();
	UInvestigationActionContainer* Container = NewObject<UInvestigationActionContainer>();
	Container->AddToRoot(); // memory stagnation issues. 
	Container->InvestigationAction = InvestigationAction;
	UE_LOG(LogTemp, Log, TEXT("ACTIONPLAN_SEQUENCE UExtractInvestigationPlan::ExecuteTask. %d"), InvestigationAction.InvestigationActionType);
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(OutInvestigationActionContainer.SelectedKeyName, Container);
	
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UExtractInvestigationPlan::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UExtractInvestigationPlan::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

/**
 * TODO: Idea
 * Get the investigationAction and segregate them into atomic components
 * based on what they are set the variables accordingly
 * setup the BT in such a way that necessary tasks will be triggerred when setting the variable like that
 * configure the SERVICE somehow to abort the existing tasks when a new investigation plan is made available
 * TODO: Do this in indicator classes or BTs itself.
 */
void UExtractInvestigationPlan::ProcessInvestigationPlan(FInvestigationAction& InInvestigationAction)
{
	
}
