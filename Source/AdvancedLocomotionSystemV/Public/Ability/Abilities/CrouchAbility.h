

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CrouchAbility.generated.h"

/**
 * 
 */
class UCapsuleComponent;

UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API UCrouchAbility : public UGameplayAbility
{
	GENERATED_BODY()

	public:
	UCrouchAbility();

	private:
		UPROPERTY()
		UCapsuleComponent *CharacterCapsuleComponent;

		UPROPERTY(EditDefaultsOnly)
		FGameplayTag CrouchTag;	

		float TargetHalfHeight;
		UPROPERTY(EditDefaultsOnly , BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
		float InterpolationSpeed = 10;
		float halfheightdiff = 0;
		bool CanUnCrouch;
		


	private:
		void Crouch();
		void Uncrouch();
		void InterpolateCrouch();
		void InterpolateUncrouch();
		void SetUnCrouch();

	UFUNCTION()
	void OnAbilityCancelled();
	UFUNCTION()
	void OnAbilityEnded(UGameplayAbility* EndedAbility);


	private:
		FTimerHandle CrouchInterpolationTimer;
		FTimerHandle UnCrouchInterpolationTimer;
		FTimerHandle UnCrouchCheckTimerHandle;

	float PlayerCameraRelativeZ;


	public:
		virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const 	FGameplayEventData* TriggerEventData) override;

};
