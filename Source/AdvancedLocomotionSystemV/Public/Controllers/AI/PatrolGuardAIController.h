

#pragma once

#include "CoreMinimal.h"
#include "Controllers/AI/Base/BaseAIController.h"
#include "Character/Components/NSAIHearingProcessor.h"
#include "PatrolGuardAIController.generated.h"


/**
 * 
 */
class UBehaviorTreeComponent;
UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API APatrolGuardAIController : public ABaseAIController
{
	GENERATED_BODY()
public:
	APatrolGuardAIController();

protected:	
	virtual void OnPossess(APawn* InPawn) override;

public:
	class UInvestigationPlanComponent* GetInvestigationPlanComponent() const { return InvestigationPlanComponent; }
	class UNSAIHearingProcessor* GetHearingProcessor() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "AI | Investigation")
	class UVisionSenseComponent* GetVisionSenseComponent();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector InitialPawnLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector DestinationLocation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float VisibilityPercentageThreshold;

	UPROPERTY(EditAnywhere)
	class UInvestigationPlanComponent* InvestigationPlanComponent;

	UPROPERTY()
	class UNSAIHearingProcessor* HearingProcessor;

	bool ReadyToConsume; // <- dummy variable
};
 