


#include "AI/BT/Tasks/BTTask_FindDisturbedActor.h"

#include "AI/Components/InvestigationPlanComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_FindDisturbedActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	const auto Container = Cast<UInvestigationActionContainer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(InInvestigationActionContainer.SelectedKeyName));
	if (Container == nullptr)
		return EBTNodeResult::Failed;
	
	const auto& InvestigationAction = Container->InvestigationAction;
	if(InvestigationAction.InvestigationActionType != EInvestigationActionType::Interact)
		return EBTNodeResult::Failed;
	
	const auto Location = InvestigationAction.DisturbableActor->GetIdealInteractionTransform().GetLocation();
	const auto Rotation = InvestigationAction.DisturbableActor->GetIdealInteractionTransform().GetRotation();

	UE_LOG(LogTemp, Log, TEXT("ACTIONPLAN_SEQUENCE. Actor: %s, Location: %s")
		, *Cast<AActor>(InvestigationAction.DisturbableActor.GetInterface())->GetActorNameOrLabel(), *InvestigationAction.DisturbableActor->GetDisturbedActorTransform().GetLocation().ToString());
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(OutInvestigationLocation.SelectedKeyName, Location);
	OwnerComp.GetBlackboardComponent()->SetValueAsRotator(OutIdealInvestigationRotation.SelectedKeyName, Rotation.Rotator());
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_FindDisturbedActor::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_FindDisturbedActor::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
