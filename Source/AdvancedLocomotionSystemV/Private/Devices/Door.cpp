


#include "Devices/Door.h"
#include "Components/ArrowComponent.h"

ADoor::ADoor() : IsOpen(false)
{
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	InteractionTransformArrow = CreateDefaultSubobject<UArrowComponent>("InteractionTransformArrow");
	DoorPivot = CreateDefaultSubobject<USceneComponent>("DoorPivot1");
	RootComponent = SceneComponent;
	DoorPivot->SetupAttachment(SceneComponent);
	InteractionTransformArrow->SetupAttachment(SceneComponent);
	InteractionTransformArrow->ArrowColor = FColor::Green;
}

void ADoor::Fix(AAIController* InAIController)
{
	FixDoor(InAIController);
}

FTransform ADoor::GetDisturbedActorTransform()
{
	return GetActorTransform();
}

FTransform ADoor::GetIdealInteractionTransform()
{
	return InteractionTransformArrow->GetComponentTransform();
}

bool ADoor::CanInteract() const
{
	return !bIsAnimating; // can not interact while animating
}

FName ADoor::GetDisplayText() const
{
	if (IsOpen)
		return FName("Close door");
	return FName("Open door");
}

FTransform ADoor::GetInteractionTransform() const
{
	return GetActorTransform();
}

void ADoor::Interact(AActor* InInstigator)
{
	//BPInteract(InInstigator);
	if (!InInstigator)
	{
		// Safety check
		return;
	}

	FVector DoorLocation = GetActorLocation();
	FVector PlayerLocation = InInstigator->GetActorLocation();

	// Door forward vector (direction door considers "front")
	FVector DoorForward = GetActorForwardVector();

	// Vector from door to player
	FVector DoorToPlayer = (PlayerLocation - DoorLocation).GetSafeNormal();

	// Calculate dot product to check if player is in front or behind
	float Dot = FVector::DotProduct(DoorForward, DoorToPlayer);

	// Determine signed open angle based on player position
	// If player is in front (dot > 0), open backwards (negative angle)
	// If player is behind (dot <= 0), open forwards (positive angle)
	float DesiredOpenAngle = (Dot > 0.f) ? -OpenAngle : OpenAngle;

	if (bIsAnimating)
	{
		// If animating, reverse direction smoothly
		TargetOpenAlpha = (TargetOpenAlpha > 0.5f) ? 0.f : 1.f;
		SignedOpenAngle = (TargetOpenAlpha > 0.5f) ? DesiredOpenAngle : 0.f;
	}
	else
	{
		if (IsOpen)
		{
			// Close door
			TargetOpenAlpha = 0.f;
		}
		else
		{
			// Open door with signed angle
			TargetOpenAlpha = 1.f;
			SignedOpenAngle = DesiredOpenAngle;
		}

		bIsAnimating = true;
	}
}

void ADoor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UE_LOG(LogTemp, Display, TEXT("ADoor::Tick. bIsAnimating: %s"), bIsAnimating ? TEXT("true") : TEXT("false"));
	if (bIsAnimating)
	{
		UpdateDoorRotation(DeltaSeconds);
	}
}

void ADoor::UpdateDoorRotation(float DeltaTime)
{
	CurrentOpenAlpha = FMath::FInterpConstantTo(CurrentOpenAlpha, TargetOpenAlpha, DeltaTime, DoorOpenSpeed);

	// Use signed open angle for interpolation
	float CurrentYaw = FMath::Lerp(0.f, SignedOpenAngle, CurrentOpenAlpha);

	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw = CurrentYaw;

	if (FMath::IsNearlyEqual(CurrentOpenAlpha, TargetOpenAlpha, 0.01f))
	{
		bIsAnimating = false;
		IsOpen = (TargetOpenAlpha > 0.5f);
		if (!IsOpen)
		{
			SignedOpenAngle = 0.f;
			NewRotation = FRotator::ZeroRotator;
		}
		else
		{
			NewRotation.Yaw = SignedOpenAngle;
		}
	}
	
	DoorPivot->SetRelativeRotation(NewRotation);
}

