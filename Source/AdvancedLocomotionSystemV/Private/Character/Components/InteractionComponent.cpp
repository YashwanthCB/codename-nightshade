


#include "Character/Components/InteractionComponent.h"

#include "TrainGameInstance.h"
#include "Character/Interfaces/Interactable.h"

UInteractionComponent::UInteractionComponent() : InteractionDistance(75.0f)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	LastInteractedActor = nullptr;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	HandleInteraction();
}

void UInteractionComponent::Interact()
{
	if (LastInteractedActor && LastInteractedActor->CanInteract())
		LastInteractedActor->Interact(GetOwner());
}

void UInteractionComponent::HandleInteraction()
{
	FVector EndLocation = EyeTransform->GetComponentLocation() + EyeTransform->GetForwardVector() * InteractionDistance;
	bool HitSomething = GetWorld()->LineTraceSingleByChannel(HitResult, EyeTransform->GetComponentLocation(), EndLocation, ECC_GameTraceChannel8, CollisionQueryParams);
	IInteractable* Interactable = HitSomething ? Cast<IInteractable>(HitResult.GetActor()) : nullptr;
	
	if (LastInteractedActor != Interactable)
	{
		InteractionAvailable.Broadcast(LastInteractedActor, false);

		if (LastInteractedActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInteractionComponent::FindInteractable. Not available: %s"), *LastInteractedActor->_getUObject()->GetName());
		}
    
		if (Interactable)
		{
			InteractionAvailable.Broadcast(Interactable, true);
			UE_LOG(LogTemp, Warning, TEXT("UInteractionComponent::FindInteractable. Found Something: %s"), *Interactable->_getUObject()->GetName());
		}

		LastInteractedActor = Interactable;
	}
	/*
	if (!HitSomething)
	{
		if (LastInteractedActor)
		{
			InteractionAvailable.Broadcast(LastInteractedActor, false);
			UE_LOG(LogTemp, Warning, TEXT("UInteractionComponent::FindInteractable. Not available: %s"), *LastInteractedActor->_getUObject()->GetName());			
		}
		LastInteractedActor = nullptr;
		return;
	}

	if (auto Interactable = Cast<IInteractable>(HitResult.GetActor()))
	{
		if (LastInteractedActor != Interactable)
		{
			InteractionAvailable.Broadcast(Interactable, true);
			UE_LOG(LogTemp, Warning, TEXT("UInteractionComponent::FindInteractable. Found Something: %s"), *Interactable->_getUObject()->GetName());
		}
		LastInteractedActor = Interactable;
	}
	else
	{
		if (LastInteractedActor)
		{
			InteractionAvailable.Broadcast(Interactable, false);
			UE_LOG(LogTemp, Warning, TEXT("UInteractionComponent::FindInteractable. Not available: %s"), *LastInteractedActor->_getUObject()->GetName());				
		}
		LastInteractedActor = nullptr;
	}*/
}
