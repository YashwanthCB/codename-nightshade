

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API ANSPlayerController : public APlayerController
{
	GENERATED_BODY()

	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Interact();

	class UInteractionComponent* PlayerInteractionComponent;
};
