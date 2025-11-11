

#pragma once

#include "CoreMinimal.h"
#include "AI/SimpleDisturbanceIndicator.h"
#include "LightTurnOffIndicator.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API ALightTurnOffIndicator : public ASimpleDisturbanceIndicator
{
	GENERATED_BODY()
public:
	virtual TArray<struct FInvestigationAction> GetActionItem() override; 
};
