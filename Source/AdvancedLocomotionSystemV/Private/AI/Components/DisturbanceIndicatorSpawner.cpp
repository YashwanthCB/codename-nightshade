


#include "AI/Components/DisturbanceIndicatorSpawner.h"
#include "AI/SimpleDisturbanceIndicator.h"

UDisturbanceIndicatorSpawner::UDisturbanceIndicatorSpawner() : DisturbanceIndicator(nullptr), SpawnPoint(FVector::ZeroVector)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDisturbanceIndicatorSpawner::SetSpawnPoint(FTransform InSpawnPoint)
{
	SpawnPoint = InSpawnPoint;
}

void UDisturbanceIndicatorSpawner::SpawnDisturbanceIndicator(FVector InInvestigationLocation,TScriptInterface<IDisturbable> InSpawnerOfDisturbanceActor)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	DisturbanceIndicator = GetWorld()->SpawnActor<ASimpleDisturbanceIndicator>(DisturbanceIndicatorClass, SpawnPoint, SpawnParameters);
	
	//checkf(!DisturbanceIndicator,  TEXT("UDisturbanceIndicatorSpawner::SpawnDisturbanceIndicator, Either DisturbanceIndicatorClass is null or unable to spawn actor. %p"), &DisturbanceIndicator);
	DisturbanceIndicator->SetInvestigationLocation(InInvestigationLocation);
	DisturbanceIndicator->SetSpawnerOfDisturbanceActor(InSpawnerOfDisturbanceActor);
}

void UDisturbanceIndicatorSpawner::DestroyDisturbanceIndicator()
{
	if (!IsValid(DisturbanceIndicator))
		return;
	
	DisturbanceIndicator->Destroy();
}
