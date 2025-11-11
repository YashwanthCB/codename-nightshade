

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Disturbable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UDisturbable : public UInterface
{
	GENERATED_BODY()
};

class ADVANCEDLOCOMOTIONSYSTEMV_API IDisturbable
{
	GENERATED_BODY()

public:
	virtual void Fix(class AAIController* InAIController) =0; // TODO: use recursion to trace back from light class to switch class. can be done when migrating all critical logics of these 2 classes to C++.
	virtual FTransform GetDisturbedActorTransform() =0;
	virtual FTransform GetIdealInteractionTransform() =0;
	//virtual int GetPriority() =0;
};
