


#include "GameModes/Missions/LightTurnOffCondition.h"

#include "Devices/SwitchBase.h"

void ULightTurnOffCondition::BeginPlay()
{
	Super::BeginPlay();
	SwitchBase = Cast<ASwitchBase>(GetOwner());
	if (!SwitchBase)
		return;
	SwitchBase->OnEventSwitch().AddUObject(this, &ULightTurnOffCondition::OnSwitchCallback);
}

void ULightTurnOffCondition::OnSwitchCallback(bool InIsSwitchedOn)
{
	if (!InIsSwitchedOn)
	{
		NotifyObjectiveConditionReached();
	}
}
