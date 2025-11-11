

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "LaserSecurity.generated.h"

UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API ALaserSecurity : public AActor
{
	GENERATED_BODY()
	
public:	
	ALaserSecurity();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION()
	void ResetLaser();

public:	
	virtual void Tick(float DeltaTime) override;
	DECLARE_EVENT(ULaserComponent, FLaserHitSomethingEvent);
	FORCEINLINE FLaserHitSomethingEvent& OnLaserSenseSomething() { return LaserHitSomethingEvent; }
	
private:
	void InternalLaserSenseSomethingCallback();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nightshade")
	USceneComponent* SceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nightshade")
	UNiagaraComponent* NiagaraLaserComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nightshade")
	UStaticMeshComponent* LaserProducerComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nightshade")
	UStaticMeshComponent* LaserReceiverComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nightshade")
	UAudioComponent* AlarmAudioComponent;

	UPROPERTY(BlueprintReadWrite, meta = (BlueprintProtected))
	bool IsSwitchedOn;
	
private:
	bool DidHitAnything;
	FLaserHitSomethingEvent LaserHitSomethingEvent;
	FVector LaserProducerLocation;
	FVector LaserReceiverLocation;
};
