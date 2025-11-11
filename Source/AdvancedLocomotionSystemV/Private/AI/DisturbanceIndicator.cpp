


#include "AI/DisturbanceIndicator.h"
#include "Interfaces/Disturbable.h"
#include "AI/Components/InvestigationPlanComponent.h"

ADisturbanceIndicator::ADisturbanceIndicator() : SpawnerOfDisturbableActor(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADisturbanceIndicator::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("ADisturbanceIndicator::BeginPlay. %p, %s"), &SpawnerOfDisturbableActor, *GetOwner()->GetActorNameOrLabel());
}

TArray<struct FInvestigationAction> ADisturbanceIndicator::GetActionItem()
{
	return TArray<struct FInvestigationAction>();
}
