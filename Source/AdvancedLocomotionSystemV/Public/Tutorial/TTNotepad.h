

#pragma once

#include "CoreMinimal.h"
#include "Character/Interfaces/Interactable.h"
#include "GameFramework/Actor.h"
#include "TTNotepad.generated.h"

UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API ATTNotepad : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:
	ATTNotepad();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual bool CanInteract() const;
	virtual FName GetDisplayText() const;
	virtual FTransform GetInteractionTransform() const;
	virtual void Interact(AActor* InInstigator);
};
