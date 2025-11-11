

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TrainGameInstance.h"
#include "BTService_VariableHandler.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ADVANCEDLOCOMOTIONSYSTEMV_API UBTService_VariableHandler : public UBTService
{
	GENERATED_BODY()

	UBTService_VariableHandler(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
		bNotifyBecomeRelevant = true;
	}
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
public:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Investigation")
	FBlackboardKeySelector HasInvestigationPlan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Investigation")
	FBlackboardKeySelector AIAwarenessState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Investigation")
	FBlackboardKeySelector SuspiciousActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Investigation")
	FBlackboardKeySelector DestinationLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Investigation")
	FBlackboardKeySelector HeardSomething;

private:
	void PlayerRemainsHidden(bool HasRemainedHidden) const;
	
private:
	TUniquePtr<NSBoolVariableMonitor> IsPlayerNotInFOV;
	bool PreviousIsPlayerInFOV;
	UPROPERTY()
	UBehaviorTreeComponent* OwnerCompCache;
};
