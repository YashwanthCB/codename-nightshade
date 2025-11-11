

#pragma once

#include "CoreMinimal.h"
#include "GameModes/Missions/ObjectiveCondition.h"
#include "LightTurnOffCondition.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Nightshade), meta=(BlueprintSpawnableComponent) )
class ADVANCEDLOCOMOTIONSYSTEMV_API ULightTurnOffCondition : public UObjectiveCondition
{
	GENERATED_BODY()

public:
	void BeginPlay() override;
private:
	void OnSwitchCallback(bool InIsSwitchedOn);
	
	UPROPERTY()
	class ASwitchBase* SwitchBase;
};
