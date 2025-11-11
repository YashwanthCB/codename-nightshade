

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class ADVANCEDLOCOMOTIONSYSTEMV_API IInteractable
{
	GENERATED_BODY()
public:
	virtual bool CanInteract() const =0;
	virtual FName GetDisplayText() const =0;
	virtual FTransform GetInteractionTransform() const =0;
	virtual void Interact(AActor* InInstigator) =0;
};
