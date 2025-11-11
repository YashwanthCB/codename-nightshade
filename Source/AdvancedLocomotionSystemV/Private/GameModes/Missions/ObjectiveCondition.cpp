


#include "GameModes/Missions/ObjectiveCondition.h"

#include "GameModes/NSFPSGameMode.h"
#include "Kismet/GameplayStatics.h"

UObjectiveCondition::UObjectiveCondition()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UObjectiveCondition::NotifyObjectiveConditionReached()
{
	auto GameMode = Cast<ANSFPSGameMode>(UGameplayStatics::GetGameMode(this));

	if (GameMode && GameMode->DoesBelongToCurrentMission(this))
	{
		DestroyComponent();
	}
}


