

#pragma once

#include "CoreMinimal.h"
#include "GameModes/NSFPSGameMode.h"
#include "NSTutorialGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API ANSTutorialGameMode : public ANSFPSGameMode
{
	GENERATED_BODY()

public:
	ANSTutorialGameMode();

	void SetGrappledIntoBuilding();

private:
	// mission requirements
	bool bGrappledIntoBuilding;
};
