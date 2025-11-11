


#include "AI/SimpleDisturbanceIndicator.h"
#include "AI/Components/InvestigationPlanComponent.h"

TArray<struct FInvestigationAction> ASimpleDisturbanceIndicator::GetActionItem()
{
	TArray<FInvestigationAction> InvestigationActions;
	FInvestigationAction GotoAction(EInvestigationActionType::GotoLocation, GetActorLocation());
	InvestigationActions.Add(GotoAction);
	return InvestigationActions;
}

void ASimpleDisturbanceIndicator::SetInvestigationLocation(FVector InInvestigationLocation)
{
	InvestigationLocation = InInvestigationLocation;
}
