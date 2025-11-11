

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Components/TimelineComponent.h"
#include "Interfaces/CharacterAbilityHelperInterface.h"
#include "LedgeGrabAbility.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UCurveFloat;
UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API ULedgeGrabAbility : public UGameplayAbility
{
	GENERATED_BODY()

	ULedgeGrabAbility();

	public:
		virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const 	FGameplayEventData* TriggerEventData) override;
		virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	private:
	UPROPERTY()
	UObject *OwnerObject;

	UPROPERTY()
	class UCapsuleComponent *CharacterCapsuleComponent;

	UPROPERTY()
	UTimelineComponent *LedgeGrabTimeline;

	UPROPERTY()
	FOnTimelineFloat ProgressFunction;
	UPROPERTY()
	FOnTimelineEvent FinishedFunction;


	UPROPERTY(EditDefaultsOnly)
	UCurveFloat *LedgeGrabCurve;

	FVector LedgeLocation;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext *DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext *LedgeGrabMappingContext;


	FTimerHandle LedgeClimbTimerHandle;


	private:
	FVector StartLocation;
	FVector EndLocation;


	private:
	void TraceDetectEnvironment();
	void DropDown();
	UFUNCTION()
	void HandleInterpolationProgress(float Value);
	UFUNCTION()
	void HandleInterpolationCompleted();
	UFUNCTION()
	void OnDropInputPressed(float TimeWaited);


};
