


#include "AI/Components/MisplacedActorIndicator.h"

TArray<struct FInvestigationAction> AMisplacedActorIndicator::GetActionItem()
{
	TArray<FInvestigationAction> InvestigationActions(Super::GetActionItem());
	FInvestigationAction InteractionAction(EInvestigationActionType::Interact, FVector::ZeroVector, SpawnerOfDisturbableActor);
	InvestigationActions.Add(InteractionAction);
	return InvestigationActions;
}
