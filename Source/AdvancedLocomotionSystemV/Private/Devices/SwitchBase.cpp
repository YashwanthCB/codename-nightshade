


#include "Devices/SwitchBase.h"
#include "Components/ArrowComponent.h"

ASwitchBase::ASwitchBase() : IsTurnedOn(false), IsInitiallyTurnedOn(false)
{
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneComponent;
	InteractionTransformArrow = CreateDefaultSubobject<UArrowComponent>("InteractionTransformArrow");
	InteractionTransformArrow->ArrowColor = FColor::Green;
	InteractionTransformArrow->SetupAttachment(SceneComponent);
}

void ASwitchBase::Fix(AAIController* InAIController)
{
	IsTurnedOn = IsInitiallyTurnedOn;
	OnEventSwitch().Broadcast(IsTurnedOn);
	FixSwitch();
}

FTransform ASwitchBase::GetDisturbedActorTransform()
{
	return GetActorTransform();
}

FTransform ASwitchBase::GetIdealInteractionTransform()
{
	return InteractionTransformArrow->GetComponentTransform();
}

bool ASwitchBase::CanInteract() const
{
	return true;
}

FName ASwitchBase::GetDisplayText() const
{
	if (IsTurnedOn)
		return FName("Switch Off");
	return FName("Switch On");
}

FTransform ASwitchBase::GetInteractionTransform() const
{
	return GetActorTransform();
}

void ASwitchBase::Interact(AActor* InInstigator)
{
	IsTurnedOn = !IsTurnedOn;
	OnEventSwitch().Broadcast(IsTurnedOn);
	BPInteract(InInstigator);
}

void ASwitchBase::BeginPlay()
{
	IsTurnedOn = IsInitiallyTurnedOn;
	OnEventSwitch().Broadcast(IsTurnedOn);
	Super::BeginPlay();
}
