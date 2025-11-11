


#include "HUD/PlayerHUD.h"

#include "Character/Components/InteractionComponent.h"
#include "Kismet/GameplayStatics.h"

APlayerHUD::APlayerHUD()
{
	Instance = this;
}

void APlayerHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (InteractableActor)
	{
		if (CanInteract && InteractableActor->CanInteract())
		{
			InteractionDisplayText = FText::FromName(InteractableActor->GetDisplayText());
			InteractionPointTransform = InteractableActor->GetInteractionTransform();
		}
		else
		{
			InteractionDisplayText = FText::GetEmpty();			
		}
	}
	
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	InteractionComponent = GetOwningPawn()->GetComponentByClass<UInteractionComponent>();
	InteractionComponent->OnInteractionAvailable().AddUObject(this, &APlayerHUD::InteractionAvailableCallback);
}

void APlayerHUD::InteractionAvailableCallback(IInteractable* InInteractableActor, bool InCanInteract)
{
	InteractableActor = InInteractableActor;
	CanInteract = InCanInteract;
	/*if (CanInteract)
	{
		InteractionDisplayText = FText::FromName(InteractableActor->GetDisplayText());
		InteractionPointTransform = InteractableActor->GetInteractionTransform();
	}
	else
	{
		InteractionDisplayText = FText::GetEmpty();
	}*/
}

APlayerHUD* APlayerHUD::Instance;
