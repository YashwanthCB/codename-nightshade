

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ADVANCEDLOCOMOTIONSYSTEMV_API IAIInterface
{
	GENERATED_BODY()




	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FVector GetAIInitialLocation() = 0;
	virtual FRotator GetAIInitialRotation() = 0;
	virtual FVector GetDestinationLocation() = 0;

	UFUNCTION(BlueprintNativeEvent)
	float GetWalkDistance();
};
