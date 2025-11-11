


#include "Character/Components/NightVisionComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/LightComponent.h"
#include "HUD/PlayerHUD.h"
#include "Kismet/GameplayStatics.h"

UNightVisionComponent::UNightVisionComponent(): HeatupTime(50000.0f), CooldownTime(3.0f), bIsOverheated(false), bIsNightVisionActive(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNightVisionComponent::BeginPlay()
{
	Super::BeginPlay();
	LevelPostProcessVolume = Cast<APostProcessVolume>(UGameplayStatics::GetActorOfClass(this, APostProcessVolume::StaticClass()));
	DefaultPostProcessSettings = FirstPersonCameraComponent->PostProcessSettings;
	HeatupRate = MAX_HEAT / HeatupTime;
	CooldownRate = MAX_HEAT / CooldownTime;
	HeatLevel = MIN_HEAT;
}

void UNightVisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsNightVisionActive)
	{
		HeatLevel += HeatupRate * DeltaTime;
		HeatLevel = FMath::Clamp(HeatLevel, MIN_HEAT, MAX_HEAT);
		if (HeatLevel == MAX_HEAT)
		{
			DisableNightVision();
			bIsOverheated = true;
		}
	}
	else
	{
		HeatLevel -= CooldownRate * DeltaTime;
		HeatLevel = FMath::Clamp(HeatLevel, MIN_HEAT, MAX_HEAT);
	}
	if (HeatLevel == MIN_HEAT)
	{
		bIsOverheated = false;
	}
	APlayerHUD::GetInstance()->SetNightVisionHeatNormalized(HeatLevel / MAX_HEAT);
	APlayerHUD::GetInstance()->SetNightVisionOverheated(bIsOverheated);
	APlayerHUD::GetInstance()->SetNightVisionActive(bIsNightVisionActive);
	UE_LOG(LogTemp, Log, TEXT("UNightVisionComponent::TickComponent. HeatLevel: %f. NightVisionActive? %s"), HeatLevel, bIsNightVisionActive ? TEXT("true") : TEXT("false"));
}

bool UNightVisionComponent::ToggleVision()
{
	if (bIsNightVisionActive)
	{
		DisableNightVision();
	}
	else
	{
		if (HeatLevel == MIN_HEAT)
		{
			EnableNightVision();
		}
	}
	return bIsNightVisionActive;
}

void UNightVisionComponent::EnableNightVision()
{
	bIsNightVisionActive = true;
	LevelPostProcessVolume->bEnabled = false;
	FirstPersonCameraComponent->PostProcessSettings = NightPostProcessSettings;
	TurnOnAudio->Play();
	TurnOffAudio->Stop();
	PointLight->Intensity = 1000.0f;
}

void UNightVisionComponent::DisableNightVision()
{
	bIsNightVisionActive = false;
	LevelPostProcessVolume->bEnabled = true;
	FirstPersonCameraComponent->PostProcessSettings = DefaultPostProcessSettings;
	TurnOffAudio->Play();
	TurnOnAudio->Stop();
	PointLight->Intensity = 0.0f;
}
