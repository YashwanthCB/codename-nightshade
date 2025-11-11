

#pragma once

#include "CoreMinimal.h"
#include "GameModes/Missions/ObjectiveCondition.h"
#include "TriggerOverlapCondition.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Nightshade), meta=(BlueprintSpawnableComponent) )
class ADVANCEDLOCOMOTIONSYSTEMV_API UTriggerOverlapCondition : public UObjectiveCondition
{
	GENERATED_BODY()

public:
	UTriggerOverlapCondition();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnComponentBeginOverlapCallback(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& OverlapInfo);

	class ATriggerBox* TriggerBox;
	bool bConditionReached;
};
