


#include "Devices/LightInfluenceComponent.h"
#include "Devices/LightBase.h"

void ULightInfluenceComponent::BeginPlay()
{
	Super::BeginPlay();
	OnVisibleActorSpawnedOrDestroyed.Broadcast(GetOwner(), true);
}

void ULightInfluenceComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	OnVisibleActorSpawnedOrDestroyed.Broadcast(GetOwner(), false);
}

float ULightInfluenceComponent::GetLightExposure()
{
	float TotalLightExposure = 0.0f;
	UE_LOG(LogTemp, Warning, TEXT("ULightInfluenceComponent::GetLightExposure. Start"));
	for(auto RegisteredLightSource : RegisteredLightSources)
	{
			TotalLightExposure += RegisteredLightSource->GetLightExposureOn(GetOwner());
		UE_LOG(LogTemp, Warning, TEXT("ULightInfluenceComponent::GetLightExposure %s, %f"), *RegisteredLightSource->GetActorNameOrLabel(), TotalLightExposure);
	}
	TotalLightExposure = FMath::Clamp(TotalLightExposure, 0, 100.0f);
	UE_LOG(LogTemp, Warning, TEXT("ULightInfluenceComponent::GetLightExposure. End %f"), TotalLightExposure);
	return TotalLightExposure;
}

USceneComponent *ULightInfluenceComponent::GetComponentToCheckHit()
{
    return ComponentToCheckHit;
}

void ULightInfluenceComponent::SetComponentToCheckHit(USceneComponent *InComponentToCheckHit)
{
	ComponentToCheckHit = InComponentToCheckHit;
}

void ULightInfluenceComponent::RegisterLightSource(ALightBase *InLightBase)
{
	RegisteredLightSources.Add(InLightBase);
}

void ULightInfluenceComponent::UnregisterLightSource(ALightBase *InLightBase)
{
	RegisteredLightSources.Remove(InLightBase);
}

FOnVisibleActorSpawnedOrDestroyed ULightInfluenceComponent::OnVisibleActorSpawnedOrDestroyed;