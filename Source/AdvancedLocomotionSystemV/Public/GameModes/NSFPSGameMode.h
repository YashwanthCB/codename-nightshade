

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModes/Missions/ObjectiveCondition.h"
#include "NSFPSGameMode.generated.h"

USTRUCT(BlueprintType)
struct FMissionObjective
{
	GENERATED_BODY()
public:
	FMissionObjective();
	bool AreObjectivesComplete();
	bool DoesBelongToCurrentMission(UObjectiveCondition* InObjectiveCondition);

private:
	bool IsObjectiveComplete(UObjectiveCondition* InObjectiveCondition);
	
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Mission | Objective")
	FString MissionID;	
	UPROPERTY(BlueprintReadWrite, Category = "Mission | Objective")
	FString Objective;
	UPROPERTY(BlueprintReadWrite, Category = "Mission | Objective")
	FString Description;

	UPROPERTY(BlueprintReadWrite, Category = "Mission | Objective")
	TArray<UObjectiveCondition*> ObjectiveConditions;
};

UCLASS(BlueprintType)
class UMission : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Mission")
	TArray<FMissionObjective> MissionObjectives;
};

/**
 * 
 */
UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API ANSFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANSFPSGameMode();
	
	void NotepadPickedup(); // Maybe move to game instance if needed
	void NotifyNotepadPlacedInLevel();

	UFUNCTION(BlueprintCallable)
	void SetMission(UMission* InMission);

	bool DoesBelongToCurrentMission(UObjectiveCondition* InObjectiveCondition);
	
protected:

	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateUIModel();
	
	UPROPERTY(BlueprintReadWrite, Category = "Mission", meta = (BlueprintProtected="true"))
	UMission* Mission;
	
	
private:
	uint8 TotalNotepadCount;
	uint8 NotepadPickedupCount;
};

inline bool ANSFPSGameMode::DoesBelongToCurrentMission(UObjectiveCondition* InObjectiveCondition)
{
	if (Mission->MissionObjectives.Num() == 0)
		return false;
	
	return Mission->MissionObjectives[0].DoesBelongToCurrentMission(InObjectiveCondition);
}
