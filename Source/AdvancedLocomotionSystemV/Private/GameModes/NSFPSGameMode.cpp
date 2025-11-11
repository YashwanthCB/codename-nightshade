


#include "GameModes/NSFPSGameMode.h"

FMissionObjective::FMissionObjective()
{
}

bool FMissionObjective::AreObjectivesComplete()
{
	//bool ObjectivesCompletionStatus = false;
	for (const auto& ObjectiveCondition : ObjectiveConditions)
	{
		if (IsValid(ObjectiveCondition))
		{
			return false;
		}
	}
	return true;
}

bool FMissionObjective::DoesBelongToCurrentMission(UObjectiveCondition* InObjectiveCondition)
{
	for (auto& ObjectiveCondition : ObjectiveConditions)
	{
		if (ObjectiveCondition == InObjectiveCondition)
			return true;
	}
	return false;
}

bool FMissionObjective::IsObjectiveComplete(UObjectiveCondition* InObjectiveCondition)
{
	return !IsValid(InObjectiveCondition); // if the objective component is destroyed that means the objective is complete
}

ANSFPSGameMode::ANSFPSGameMode() : TotalNotepadCount(0), NotepadPickedupCount(0), Mission(nullptr)
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ANSFPSGameMode::NotifyNotepadPlacedInLevel()
{
	++TotalNotepadCount;
}

void ANSFPSGameMode::SetMission(UMission* InMission)
{
	Mission = InMission;
	UpdateUIModel();
}

void ANSFPSGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!Mission || !IsValid(Mission) || Mission->MissionObjectives.Num() == 0)
		return;
	
	UE_LOG(LogTemp, Warning, TEXT("ANSFPSGameMode::Tick. Called"));
	
	FMissionObjective ObjectiveToTrack = Mission->MissionObjectives[0];

	if (ObjectiveToTrack.AreObjectivesComplete())
	{
		UE_LOG(LogTemp, Warning, TEXT("ANSFPSGameMode::Tick. CurrentObjectiveCompleted"));
		Mission->MissionObjectives.RemoveAt(0);
		UpdateUIModel();
	}
}

void ANSFPSGameMode::NotepadPickedup()
{
	++NotepadPickedupCount;
	if (NotepadPickedupCount >= TotalNotepadCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANSFPSGameMode::NotepadPickedup. All notepads collected"));
	}
}
