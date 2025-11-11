

#pragma once

#include "CoreMinimal.h"
#include "AI/DisturbanceIndicator.h"
#include "SimpleDisturbanceIndicator.generated.h"

/**
 * Provides action to go to a location to the AI  
 */
UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API ASimpleDisturbanceIndicator : public ADisturbanceIndicator
{
	GENERATED_BODY()

public:
	virtual TArray<struct FInvestigationAction> GetActionItem() override;
	void SetInvestigationLocation(FVector InInvestigationLocation);
	inline FVector GetInvestigationLocation() const { return InvestigationLocation; }
	
private:
	FVector InvestigationLocation;
};
