

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "VisionSenseComponent.generated.h"

// Utility class for UVisionSenseComponent
class VisibilityChecker
{
public:
	VisibilityChecker(USceneComponent* InCallingSceneComponent, AActor* InOwnerActor, AActor* PointOfInterest, float InHalfFOV); // add bCalculateLightExposure as default argument?
	void Process();
	FORCEINLINE float GetVisibility() const { return PointOfInterestVisibilityPercentage; }
	FORCEINLINE bool IsInFOV() const { return IsPointOfInterestInFOV; }
	

private:
	void DetectPointOfInterest();
	void CacheVisibilityProbes();
	void ShootLineTraces();
	bool DoesSingleLineTraceHitActorAtVector(FVector InVector);
	bool IsVectorInFOV(FVector InVector, float InHalfFOV);
	float GetLightExposureFromActor(AActor* InActor);
	int GetProbeHitCount();

private:
	float HalfFOV;
	float PointOfInterestVisibilityPercentage;
	bool IsPointOfInterestInFOV;

	FCollisionQueryParams LineTraceParams;
	TArray<class UVisibilityProbeComponent*> VisibilityProbes;// Cache

	USceneComponent* CallingSceneComponent;
	AActor* OwnerActor;
	AActor* ActorToTrackVisibility;
};

// Attached to all AI Pawns that can see. Percives any actor that has implemented IVisibleToAI (or has LightInfluenceComponent in it)
UCLASS( ClassGroup=(Nightshade), meta=(BlueprintSpawnableComponent),Blueprintable)
class ADVANCEDLOCOMOTIONSYSTEMV_API UVisionSenseComponent : public UArrowComponent
{
	GENERATED_BODY()

public:
	UVisionSenseComponent();
protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const TMap<AActor*, float> GetVisibilityMap() { return VisibilityMap; }

	FORCEINLINE float GetVisibilityPercentage(AActor* InActor);
	FORCEINLINE bool IsActorInFOV(AActor* InActor);
	//UFUNCTION(BlueprintCallable, BlueprintPure) // <- TODO: We may not need UFUNCTION tag for this function
	FORCEINLINE float GetPlayerPawnVisibilityPercentage();
	FORCEINLINE bool IsPlayerInFOV();
	
private:
	void MeasureVisibility(AActor* InActor);
	void MeasureVisibility();
	void PrintVisibility();

	void OnVisibleActorSpawnedOrDestroyedRaw(AActor* InActor, bool IsSpawned);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float HalfFOV; // future plans: horizontal and vertical FOVs

private:
	UPROPERTY()
	TMap<AActor*, float> VisibilityMap;
	
	UPROPERTY()
	TMap<AActor*, bool> FOVCheckMap;
	
	TArray<class UVisibilityProbeComponent*> VisibilityProbes;
};
