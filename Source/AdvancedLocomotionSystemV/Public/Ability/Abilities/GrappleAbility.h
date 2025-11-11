

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Interfaces/CharacterAbilityHelperInterface.h"
#include "Components/TimelineComponent.h"
#include "GrappleAbility.generated.h"

/**
 * 
 */
class UCurveFloat;
class UInputMappingContext;

UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API UGrappleAbility : public UGameplayAbility
{
	GENERATED_BODY()

	UGrappleAbility();

	private:
	bool IsA=false;

	private:
	UPROPERTY(EditDefaultsOnly)
	float CableWindInSpeed = 1;
	UPROPERTY(EditDefaultsOnly)
	float BeforeWindingInCableDelay = 0.2;
	UPROPERTY(EditDefaultsOnly)
	float AfterCableWindDelay = 0.3;
	UPROPERTY(EditDefaultsOnly)
	float PlayerHoldTime = 1;
	UPROPERTY(EditDefaultsOnly)
	float VibrationForce = 20000;


	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext *DefaultMappingContext;
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext *GrappleMappingContext;
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext *NoKeyBoardInputGrappleMappingContext;

	private:
	FVector GrappleMeshStartLocation;
	FVector CharacterInitialLocation;
	FVector GrappleEndLocation;
	FVector GrapplePlayerLocation;
	FName GrappleSocketName;

	//components
	UPROPERTY()
	USkeletalMeshComponent *CharacterSkeletalMesh;
	UPROPERTY()
	UStaticMeshComponent *GrappleEndMesh;


	//curves and timelines
	private:
	UPROPERTY(EditDefaultsOnly)
	UCurveFloat *GrappleExpansionCurve;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat *GrappleRetractionCurve;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat*PlayerZForceCurve;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat *PlayerMovementCurve;

	UPROPERTY()
    UTimelineComponent* GrappleExpansionTimeline;
	UPROPERTY()
    UTimelineComponent* GrappleRetractionTimeline;
	UPROPERTY()
    UTimelineComponent* PlayerZForceTimeline;
	UPROPERTY()
    UTimelineComponent* PlayerMovementTimeline;


	UPROPERTY()
	FOnTimelineFloat ProgressFunction;
	UPROPERTY()
	FOnTimelineFloat PlayerZForceProgressFunction;
	UPROPERTY()
	FOnTimelineEvent FinishedFunction;


	float PlayerZForce=0;

///////////////////////////////////////////////////////////////////////////////
	FTimerHandle CableWindInTimerHandle;
	FTimerHandle CableVibrateTimerHandle;
	FTimerHandle AfterWindInDelayHandle;
	FTimerHandle PlayerFallDelayHandle;


	//timeline callback functions
	UFUNCTION()
	void HandleGrappleExpansion(float Value);
	UFUNCTION()
	void OnGrappleExpansionCompleted();

	UFUNCTION()
	void HandleGrappleMeshRetraction(float Value);
	UFUNCTION()
	void OnGrappleMeshRetractionCompleted();

	UFUNCTION()
	void HandlePlayerMovement(float Value);
	UFUNCTION()
	void OnPlayerMovementCompleted();

	UFUNCTION()
	void HandlePlayerZForce(float Value);

	void WindInCable();
	void VibrateCable();
	void AttachGrappleEndMesh();
	void OnAfterWindInDelayFinished();
	void OnGrappleEnd();
	void ChangeMappingContext(UInputMappingContext *NewContext);
	//1 - forward, -1 - backward 
	// Impulse Direction travels from particle position
	void AddRopeImpulseInternal(int32 ImpulseDirection,float Intensity , int32 ImpulseParticlePosition , bool UseDistributedForce , int32 UnfoldingRange);


	protected:
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const 	FGameplayEventData* TriggerEventData) override;
	
};
