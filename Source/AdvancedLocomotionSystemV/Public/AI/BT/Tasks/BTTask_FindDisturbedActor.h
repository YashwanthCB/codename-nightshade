

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindDisturbedActor.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API UBTTask_FindDisturbedActor : public UBTTaskNode
{
	GENERATED_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Investigation")
	FBlackboardKeySelector InInvestigationActionContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Investigation")
	FBlackboardKeySelector OutInvestigationLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Investigation")
	FBlackboardKeySelector OutIdealInvestigationRotation;	
};
