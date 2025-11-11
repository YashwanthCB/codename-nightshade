


#include "Character/VisibilityProbeComponent.h"

UVisibilityProbeComponent::UVisibilityProbeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UVisibilityProbeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UVisibilityProbeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

