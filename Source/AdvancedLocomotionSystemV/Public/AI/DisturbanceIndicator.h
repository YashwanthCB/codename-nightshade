

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AI/Components/InvestigationPlanComponent.h"
#include "DisturbanceIndicator.generated.h"

UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API ADisturbanceIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	ADisturbanceIndicator();

protected:
	virtual void BeginPlay() override;

public:	
	virtual TArray<struct FInvestigationAction> GetActionItem();
	FORCEINLINE void SetSpawnerOfDisturbanceActor(TScriptInterface<class IDisturbable> InSpawnerOfDisturbableActor) { SpawnerOfDisturbableActor = InSpawnerOfDisturbableActor; }

protected:
	UPROPERTY()
	TScriptInterface<class IDisturbable> SpawnerOfDisturbableActor;
};
