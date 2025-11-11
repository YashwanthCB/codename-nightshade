

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Disturbable.h"
#include "LightBase.generated.h"



// Base class for all light actors
UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API ALightBase : public AActor, public IDisturbable
{
	GENERATED_BODY()

public:
	ALightBase();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = NightShade) /// <-- Migrate to C++
	float GetLightExposureOnVisibleActor(AActor* InActor);
	float GetLightExposureOn(AActor* InActor);

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	virtual void Fix(AAIController* InAIController) override;
	virtual FTransform GetDisturbedActorTransform() override;
	virtual FTransform GetIdealInteractionTransform() override;

	UFUNCTION(BlueprintImplementableEvent, meta=(BlueprintProtected = "true"))
	void OnPlayerBeginPlayCalled();
	
protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (BlueprintProtected = "true"), Category = NightShade)
	void FixLight();

	void SwitchLight(bool InIsSwitchedOn);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = NightShade)
	void SwitchLightBP(bool InIsSwitchedOn);

	UFUNCTION(BlueprintImplementableEvent, Category = NightShade)
	class ULightComponent* GetLightComponent();
	
private:
	void OnEventSwitchCallback(bool);
	
	FVector GetPrimitiveLightForwardVector();
	float GetLightIntensityAt(FVector InPosition);
	float GetAngularAttenuationAt(FVector InPosition);
	float GetLinearAttenuationAt(FVector InPosition);
	
	
protected:
	// paused development for now
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = NightShade)
	class ASwitchBase* SwitchReference;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = NightShade)
	USceneComponent* Root;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = NightShade)
	USceneComponent* LightIndicatorSpawnPoint;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = NightShade)
	class UDisturbanceIndicatorSpawner* DisturbanceIndicatorSpawner;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NightShade)
	bool IsTurnedOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NightShade, meta=(BlueprintProtected = "true"))
	float LightIntensityFactor;

private:
	bool IsInitiallyTurnedOn;
};


