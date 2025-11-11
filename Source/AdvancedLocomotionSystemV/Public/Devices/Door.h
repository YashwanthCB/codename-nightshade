

#pragma once

#include "CoreMinimal.h"
#include "Character/Interfaces/Interactable.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Disturbable.h"
#include "Door.generated.h"

UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API ADoor : public AActor, public IInteractable, public IDisturbable
{
	GENERATED_BODY()

public:
	ADoor();
	virtual void Fix(AAIController* InAIController) override;
	virtual FTransform GetDisturbedActorTransform() override;
	virtual FTransform GetIdealInteractionTransform() override;

public:
	virtual bool CanInteract() const override;
	virtual FName GetDisplayText() const override;
	virtual FTransform GetInteractionTransform() const override;
	virtual void Interact(AActor* InInstigator) override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (BlueprintProtected = "true"), Category = NightShade)
	void FixDoor(AAIController* InAIController);

	UFUNCTION(BlueprintImplementableEvent, meta = (BlueprintProtected = "true"), Category = NightShade)
	void BPInteract(AActor* InInstigator);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nightshade")
	class USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nightshade")
	class USceneComponent* DoorPivot;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nightshade")
	class UArrowComponent* InteractionTransformArrow;

	UPROPERTY(BlueprintReadWrite, Category = "Nightshade", meta = (BlueprintProtected))
	bool IsOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nightshade")
	float OpenAngle = 70.f;

	// Speed at which door opens/closes (alpha per second)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nightshade")
	float DoorOpenSpeed = 1.0f;

	// Current interpolation alpha (0 closed, 1 open)
	float CurrentOpenAlpha = 0.f;

	// Target alpha we want to reach (0 or 1)
	float TargetOpenAlpha = 0.f;

	// Whether door is currently animating
	bool bIsAnimating = false;

	// Target open angle with sign (+/-)
	float SignedOpenAngle = 0.f;
	
	// Function to update door rotation each tick
	void UpdateDoorRotation(float DeltaTime);
};
