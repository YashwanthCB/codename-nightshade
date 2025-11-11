

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

UENUM(BLueprintType)
enum class EAIAwarenessState : uint8
{
	None,
	Roaming,
	Suspicion,
	Chase
};

/**
 * 
 */
UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
};
