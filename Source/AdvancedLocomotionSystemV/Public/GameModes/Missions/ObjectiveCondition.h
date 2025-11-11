

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectiveCondition.generated.h"


UCLASS( ClassGroup=(Nightshade), meta=(BlueprintSpawnableComponent) )
class ADVANCEDLOCOMOTIONSYSTEMV_API UObjectiveCondition : public UActorComponent
{
	GENERATED_BODY()
public:	
	UObjectiveCondition();
	
protected:
	/**
	 * TODO: When the player actually sets the correct condition for the objective that is not current and that objective comes current,
	 * TODO: the game mode should move forward to the next objective considering the current one as success.
	 */
	void NotifyObjectiveConditionReached();
};
