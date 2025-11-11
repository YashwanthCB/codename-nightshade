

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Interfaces/Interactable.h"
#include "InteractionComponent.generated.h"

UCLASS( ClassGroup=(Nightshade), meta=(BlueprintSpawnableComponent) )
class ADVANCEDLOCOMOTIONSYSTEMV_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	DECLARE_EVENT_TwoParams(UInteractionComponent, FOnInteractionReady, IInteractable*, bool);
	FOnInteractionReady& OnInteractionAvailable() { return InteractionAvailable; }
	void Interact();
	
private:
	void HandleInteraction();

protected:
	UPROPERTY(EditAnywhere, Category = Nightshade)
	float InteractionDistance;

	UPROPERTY(BlueprintReadWrite, Category = Nightshade)
	USceneComponent* EyeTransform;
	
private:
	IInteractable* LastInteractedActor;
	FOnInteractionReady	InteractionAvailable;

	FCollisionQueryParams CollisionQueryParams;
	FHitResult HitResult;
};
