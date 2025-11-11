

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Components/TimelineComponent.h"
#include "Interfaces/CharacterAbilityHelperInterface.h"
#include "MantleAbility.generated.h"

/**
 * 
 */
class UCapsuleComponent;
class UCurveFloat;
class UInputMappingContext;


UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API UMantleAbility : public UGameplayAbility
{
	GENERATED_BODY()


	protected:
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const 	FGameplayEventData* TriggerEventData) override;


	public:
	UMantleAbility();

	private:
	UPROPERTY()
	UCapsuleComponent *CharacterCapsuleComponent;
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext *DefaultMappingContext;
	// UPROPERTY(EditDefaultsOnly)
	// float MaxMantleReachHeight;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat *MantleCurve;
	UPROPERTY()
	UTimelineComponent *MantleTimeline;

	UPROPERTY()
	FOnTimelineFloat ProgressFunction;
	UPROPERTY()
	FOnTimelineEvent FinishedFunction;



	private:
	void MantleUp(FVector _MantleLandLocation);
	FVector CalculateLandingPoint(FVector LedgeLocation , FVector Offset = FVector(0,0,0) , FVector SurfaceNormal = FVector(0,0,0));
	bool CheckLandingClearance(FVector LandPoint);
	UFUNCTION()
	void HandleInterpolationProgress(float Value);
	UFUNCTION()
	void HandleInterpolationCompleted();

	private:
	FVector MantleLandLocation;
	
};
