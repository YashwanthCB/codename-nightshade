

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LightInfluenceComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnVisibleActorSpawnedOrDestroyed, AActor*, bool IsSpawned)


// Attached to any actors that is influenced by light and a point of interest for AI.
UCLASS(ClassGroup=(Nightshade), meta=(BlueprintSpawnableComponent),Blueprintable)
class ADVANCEDLOCOMOTIONSYSTEMV_API ULightInfluenceComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual float GetLightExposure();

	UFUNCTION(BlueprintCallable)
	virtual USceneComponent* GetComponentToCheckHit();

	UFUNCTION(BlueprintCallable)
	virtual void SetComponentToCheckHit(USceneComponent* InComponentToCheckHit);
	
	UFUNCTION(BlueprintCallable)
	virtual void RegisterLightSource(ALightBase* InLightBase);

	UFUNCTION(BlueprintCallable)
	virtual void UnregisterLightSource(ALightBase* InLightBase);

public:
	static FOnVisibleActorSpawnedOrDestroyed OnVisibleActorSpawnedOrDestroyed;

private:
	TArray<ALightBase*> RegisteredLightSources;
	USceneComponent* ComponentToCheckHit;
};
