


#include "AI/LightTurnOffIndicator.h"
#include "AI/Components/InvestigationPlanComponent.h"

TArray<struct FInvestigationAction> ALightTurnOffIndicator::GetActionItem()
{
	TArray<FInvestigationAction> InvestigationActions(Super::GetActionItem());
	FInvestigationAction InteractionAction(EInvestigationActionType::Interact, FVector::ZeroVector, SpawnerOfDisturbableActor);
	InvestigationActions.Add(InteractionAction);
	return InvestigationActions;
}
