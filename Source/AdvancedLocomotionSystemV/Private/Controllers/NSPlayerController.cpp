


#include "Controllers/NSPlayerController.h"
#include "Character/Components/InteractionComponent.h"

void ANSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	PlayerInteractionComponent = GetPawn()->GetComponentByClass<UInteractionComponent>();
}

void ANSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ANSPlayerController::Interact);
}

void ANSPlayerController::Interact()
{
	PlayerInteractionComponent->Interact();
}
