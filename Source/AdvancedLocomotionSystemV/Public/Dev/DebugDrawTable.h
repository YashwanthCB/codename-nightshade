

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DebugDrawTable.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct ADVANCEDLOCOMOTIONSYSTEMV_API FDebugDrawStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bShouldDrawDebug=false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EDrawDebugTrace::Type> TraceDebug;
};
