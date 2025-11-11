

#pragma once

#include "CoreMinimal.h"
#include "AI/Components/InvestigationPlanComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ExtractInvestigationPlan.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API UExtractInvestigationPlan : public UBTTaskNode
{
	GENERATED_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Investigation")
	FBlackboardKeySelector AIAwarenessState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Investigation")
	FBlackboardKeySelector OutInvestigationActionContainer;
private:
	void ProcessInvestigationPlan(FInvestigationAction& InInvestigationAction);
};
