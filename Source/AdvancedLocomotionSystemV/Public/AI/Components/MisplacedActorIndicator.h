

#pragma once

#include "CoreMinimal.h"
#include "AI/SimpleDisturbanceIndicator.h"
#include "MisplacedActorIndicator.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API AMisplacedActorIndicator : public ASimpleDisturbanceIndicator
{
	GENERATED_BODY()
public:
	virtual TArray<struct FInvestigationAction> GetActionItem() override;
};
