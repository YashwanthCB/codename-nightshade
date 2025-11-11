

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AIController.h"
#include "BTTaskPatrol.generated.h"

enum EPatrolDirection
{
	FORWARD,
	BACKWARD
};

/**
 * 
 */
class UBehaviourTreeComponent;
class AIController;
UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API UBTTaskPatrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTaskPatrol();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UFUNCTION()
	void OnDestinationReached(FAIRequestID RequestID, EPathFollowingResult::Type Result);

private:
	UBehaviorTreeComponent *OwnerComponent;
	FVector DestinationLocation;
	EPatrolDirection PatrolDirection;
};
