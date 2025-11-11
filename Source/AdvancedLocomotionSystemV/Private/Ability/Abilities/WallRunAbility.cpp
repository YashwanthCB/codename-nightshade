#include "Ability/Abilities/WallRunAbility.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemBlueprintLibrary.h"

void UWallRunAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData)
{
	if(GetAvatarActorFromActorInfo() == nullptr)
	{
		EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , true);
	}

	if (ActorInfo->OwnerActor->GetClass()->ImplementsInterface(UCharacterAbilityHelperInterface::StaticClass()))
		{
			ICharacterAbilityHelperInterface *CharacterAbilityHelperInterface = Cast<ICharacterAbilityHelperInterface>(Cast<UObject>(CurrentActorInfo->OwnerActor));
			CharacterAbilityHelperInterface->GetCharacterLandDelegate().BindUFunction(this,"EndAbilityInternal");
		}
	else
		{
			UE_LOG(LogTemp, Warning, TEXT("CharacterAbilityHelperInterface null"));
		}
}

void UWallRunAbility::EndAbilityInternal()
{
	UE_LOG(LogTemp, Warning, TEXT("ending wallrun ability"));
	EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , false);
}