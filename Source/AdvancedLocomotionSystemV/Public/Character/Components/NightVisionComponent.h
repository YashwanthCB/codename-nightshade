

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/PostProcessComponent.h"
#include "Engine/PostProcessVolume.h"
#include "NightVisionComponent.generated.h"

UCLASS( ClassGroup=(Nightshade), meta=(BlueprintSpawnableComponent) )
class ADVANCEDLOCOMOTIONSYSTEMV_API UNightVisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNightVisionComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool ToggleVision();
	
private:
	void EnableNightVision();
	void DisableNightVision();

protected:
	UPROPERTY(EditAnywhere, Category = Nightshade)
	float HeatupTime;
	
	UPROPERTY(EditAnywhere, Category = Nightshade)
	float CooldownTime;

	UPROPERTY(BlueprintReadWrite, Category = Nightshade)
	TSoftObjectPtr<UCameraComponent> FirstPersonCameraComponent;

	UPROPERTY()
	TSoftObjectPtr<UPostProcessComponent> DefaultPostProcess;

	UPROPERTY()
	TSoftObjectPtr<APostProcessVolume> LevelPostProcessVolume;

	UPROPERTY(BlueprintReadWrite, Category = Nightshade)
	FPostProcessSettings NightPostProcessSettings;

	UPROPERTY(BlueprintReadWrite, Category = Nightshade)
	TSoftObjectPtr<UAudioComponent> TurnOnAudio;

	UPROPERTY(BlueprintReadWrite, Category = Nightshade)
	TSoftObjectPtr<UAudioComponent> TurnOffAudio;
	
	UPROPERTY(BlueprintReadWrite, Category = Nightshade)
	TSoftObjectPtr<ULightComponent> PointLight;
	
private:
	FPostProcessSettings DefaultPostProcessSettings;

	float HeatupRate;
	float CooldownRate;

	float HeatLevel;

	bool bIsNightVisionActive;
	bool bIsOverheated;
	const float MIN_HEAT = 0.0f;
	const float MAX_HEAT = 100.0f;
};
