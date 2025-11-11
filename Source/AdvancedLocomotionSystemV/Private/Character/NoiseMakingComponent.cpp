


#include "Character/NoiseMakingComponent.h"

UNoiseMakingComponent::UNoiseMakingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNoiseMakingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UNoiseMakingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

