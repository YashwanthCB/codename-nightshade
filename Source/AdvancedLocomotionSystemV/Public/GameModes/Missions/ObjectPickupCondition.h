

#pragma once

#include "CoreMinimal.h"
#include "GameModes/Missions/ObjectiveCondition.h"
#include "ObjectPickupCondition.generated.h"

/**
 * For now whatever object that gets pickedup will be destroyed eventually destroying this component.
 * So we don't need to separate create conditions for checking. 
 */
UCLASS( ClassGroup=(Nightshade), meta=(BlueprintSpawnableComponent) )
class ADVANCEDLOCOMOTIONSYSTEMV_API UObjectPickupCondition : public UObjectiveCondition
{
	GENERATED_BODY()
	
};
