

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Interfaces/CharacterAbilityHelperInterface.h"
#include "WallRunAbility.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API UWallRunAbility : public UGameplayAbility
{
	GENERATED_BODY()

	private:
	UFUNCTION()
	void EndAbilityInternal();

	public:
		virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const 	FGameplayEventData* TriggerEventData) override;


};
