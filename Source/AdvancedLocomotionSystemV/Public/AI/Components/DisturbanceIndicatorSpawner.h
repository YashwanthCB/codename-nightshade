

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Disturbable.h"
#include "DisturbanceIndicatorSpawner.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVANCEDLOCOMOTIONSYSTEMV_API UDisturbanceIndicatorSpawner : public UActorComponent
{
	GENERATED_BODY()

public:
	UDisturbanceIndicatorSpawner();
	void SetSpawnPoint(FTransform InSpawnPoint);
	
	
public:
	UFUNCTION(BlueprintCallable)
	void SpawnDisturbanceIndicator(FVector InInvestigationLocation,TScriptInterface<IDisturbable> InSpawnerOfDisturbanceActor);

	UFUNCTION(BLueprintCallable)
	void DestroyDisturbanceIndicator();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class ASimpleDisturbanceIndicator> DisturbanceIndicatorClass;

	UPROPERTY()
	ASimpleDisturbanceIndicator* DisturbanceIndicator;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform SpawnPoint;
};
