


#include "Ability/Abilities/CrouchAbility.h"
#include "Components/CapsuleComponent.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interfaces/CharacterAbilityHelperInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"

UCrouchAbility::UCrouchAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	OnGameplayAbilityCancelled.AddUObject(this , &UCrouchAbility::OnAbilityCancelled);
	OnGameplayAbilityEnded.AddUObject(this,&UCrouchAbility::OnAbilityEnded);
	
}

void UCrouchAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData)
{
	Super::ActivateAbility(Handle , ActorInfo , ActivationInfo , TriggerEventData);
	CharacterCapsuleComponent = Cast<UCapsuleComponent>(ActorInfo->OwnerActor->GetRootComponent());
	if (!CharacterCapsuleComponent || GetAvatarActorFromActorInfo() == nullptr) 
	{
		UE_LOG(LogTemp, Warning, TEXT("ability cancelled : nullptr ref"));
		EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , true);
	}
	// UE_LOG(LogTemp, Warning, TEXT("%f") , CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight());

	FGameplayTagContainer AbilitySystemTagContainer;
	ActorInfo->AbilitySystemComponent->GetOwnedGameplayTags(AbilitySystemTagContainer);

	GetWorld()->GetTimerManager().SetTimer(UnCrouchCheckTimerHandle,this, &UCrouchAbility::SetUnCrouch , 0.1 , true);

	if(AbilitySystemTagContainer.HasTag(CrouchTag))
	{
		Uncrouch();
	}
	else
	{
		Crouch();
	}
}


void UCrouchAbility::Crouch()
{
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(GetAvatarActorFromActorInfo() , FGameplayTagContainer(CrouchTag));
	Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent)->MaxWalkSpeed = Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent)->MaxWalkSpeedCrouched;

	TargetHalfHeight = Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent)->GetCrouchedHalfHeight();
	
	auto PlayerCameraComponent = CurrentActorInfo->OwnerActor->GetComponentByClass<UCameraComponent>();
	PlayerCameraRelativeZ = PlayerCameraComponent->GetRelativeLocation().Z;
	float NewCameraRelativeZ = TargetHalfHeight * PlayerCameraRelativeZ / CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight();
	
	halfheightdiff = CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight() - TargetHalfHeight;
	CharacterCapsuleComponent->SetCapsuleHalfHeight(TargetHalfHeight);
	PlayerCameraComponent->SetRelativeLocation(FVector(0, 0, NewCameraRelativeZ));
	
	// GetWorld()->GetTimerManager().SetTimer(CrouchInterpolationTimer,this,&UCrouchAbility::InterpolateCrouch,0.01,true);
	// UE_LOG(LogTemp, Warning, TEXT("Current Height is =  %f") , CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight());
	// UE_LOG(LogTemp, Warning, TEXT("Target Height is =  %f") , TargetHalfHeight);
}

void UCrouchAbility::Uncrouch()
{
	if (!CanUnCrouch)
	{
		return;
	}
	// UKismetSystemLibrary::LineTraceSingle(GetWorld() , Start , End, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4), false , ActorsToIgnore , EDrawDebugTrace::ForDuration , UnCrouchHitResult , true);

	auto PlayerCameraComponent = CurrentActorInfo->OwnerActor->GetComponentByClass<UCameraComponent>();
	
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo() , FGameplayTagContainer(CrouchTag));
	TargetHalfHeight = CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight() + halfheightdiff;
	CharacterCapsuleComponent->SetCapsuleHalfHeight(TargetHalfHeight);
	PlayerCameraComponent->SetRelativeLocation(FVector(0, 0, PlayerCameraRelativeZ));
	Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent)->MaxWalkSpeed = 300;
	EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , false);
	// EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , false);
	// GetWorld()->GetTimerManager().SetTimer(CrouchInterpolationTimer,this,&UCrouchAbility::InterpolateUncrouch,0.01,true);
	// GetWorld()->GetTimerManager().SetTimer(UnCrouchInterpolationTimer,this,&UCrouchAbility::InterpolateUncrouch,0.01,true);
}

void UCrouchAbility::InterpolateCrouch()
{
	CharacterCapsuleComponent->SetCapsuleHalfHeight(FMath::FInterpTo(CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight() , TargetHalfHeight , GetWorld()->GetDeltaSeconds() , InterpolationSpeed));

	if (UKismetMathLibrary::NearlyEqual_FloatFloat(CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight(),TargetHalfHeight ,1))
	{
		CharacterCapsuleComponent->SetCapsuleHalfHeight(TargetHalfHeight);
		GetWorld()->GetTimerManager().ClearTimer(CrouchInterpolationTimer);
	}
}


void UCrouchAbility::InterpolateUncrouch()
{
	CharacterCapsuleComponent->SetCapsuleHalfHeight(FMath::FInterpTo(CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight() , TargetHalfHeight , GetWorld()->GetDeltaSeconds() , InterpolationSpeed));

	if (UKismetMathLibrary::NearlyEqual_FloatFloat(CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight(),TargetHalfHeight ,1))
	{
		CharacterCapsuleComponent->SetCapsuleHalfHeight(TargetHalfHeight);
		GetWorld()->GetTimerManager().ClearTimer(UnCrouchInterpolationTimer);
		EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , false);
	}
}

void UCrouchAbility::OnAbilityCancelled()
{
	Uncrouch();
	// UE_LOG(LogTemp, Warning, TEXT("Ability Cancelled"));
}

void UCrouchAbility::OnAbilityEnded(UGameplayAbility* EndedAbility)
{
	// UE_LOG(LogTemp, Warning, TEXT("Ability ended"));
}

void UCrouchAbility::SetUnCrouch()
{
	FVector OwnerActorLocation = CurrentActorInfo->OwnerActor->GetActorLocation();
	FVector Start = OwnerActorLocation;
	FVector End = FVector(OwnerActorLocation.X , OwnerActorLocation.Y , OwnerActorLocation.Z + CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight()*2.5 + 10);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	FHitResult UnCrouchHitResult;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld() , Start , End ,CharacterCapsuleComponent->GetUnscaledCapsuleRadius(), UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4),false , ActorsToIgnore , EDrawDebugTrace::None , UnCrouchHitResult , true);
	CanUnCrouch = !UnCrouchHitResult.bBlockingHit;
	ICharacterAbilityHelperInterface::Execute_SetCanUncrouch(Cast<UObject>(CurrentActorInfo->OwnerActor) , CanUnCrouch);


}