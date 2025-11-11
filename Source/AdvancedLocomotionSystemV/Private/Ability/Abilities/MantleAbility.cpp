


#include "Ability/Abilities/MantleAbility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"

UMantleAbility::UMantleAbility()
{
	MantleTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MantleTimeline"));
}


void UMantleAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData)
{
	if (!ActorInfo->OwnerActor->GetClass()->ImplementsInterface(UCharacterAbilityHelperInterface::StaticClass()))
	{
		EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , true);
	}


	if (!MantleTimeline->IsRegistered())
	{
		MantleTimeline->RegisterComponent();
	}
	ProgressFunction.BindUFunction(this , "HandleInterpolationProgress");
	FinishedFunction.BindUFunction(this , "HandleInterpolationCompleted");
	MantleTimeline->AddInterpFloat(MantleCurve ,ProgressFunction );
	MantleTimeline->SetTimelineFinishedFunc(FinishedFunction);
	MantleTimeline->SetLooping(false);
	CharacterCapsuleComponent = Cast<UCapsuleComponent>(ActorInfo->OwnerActor->GetRootComponent());
	

		if (TriggerEventData)
		{
			// FVector LedgeLocation = ICharacterAbilityHelperInterface::Execute_GetActiveLedge(Cast<UObject>(ActorInfo->OwnerActor));
			float maxclimbheight = ICharacterAbilityHelperInterface::Execute_GetMaxClimbHeight(Cast<UObject>(ActorInfo->OwnerActor));
			float maxledgegrabheight = ICharacterAbilityHelperInterface::Execute_GetMaxLedgeGrabHeight(Cast<UObject>(ActorInfo->OwnerActor));
			float CharacterCapsuleHalfHeight = CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight();
			float CharacterFootLocation = ActorInfo->OwnerActor->GetActorLocation().Z - CharacterCapsuleHalfHeight;
			FVector LedgeVector = ICharacterAbilityHelperInterface::Execute_GetActiveLedge(Cast<UObject>(ActorInfo->OwnerActor));
			if (LedgeVector == FVector(0,0,0) || LedgeVector.Z - CharacterFootLocation > maxclimbheight || LedgeVector.Z < CharacterFootLocation)
				{
					EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , true);
					return;
				}

			// LedgeVector = LedgeVector + ActorInfo->OwnerActor->GetActorForwardVector() * 20;
			// float LedgeZ = LedgeVector.Z + CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight() + 1;
			// LedgeVector.Z = LedgeZ;
			LedgeVector = CalculateLandingPoint(LedgeVector);
			if (CheckLandingClearance(LedgeVector))
			{
				// LedgeVector = ActorInfo->OwnerActor->GetActorLocation() + (ActorInfo->OwnerActor->GetActorForwardVector() * 80);
				// LedgeVector.X = 
				MantleUp(LedgeVector);
			}
			else
			{
				// SendGameplayEvent(FGameplayTag::RequestGameplayTag("Character.Status.LedgeGrab") , FGameplayEventData());
				UE_LOG(LogTemp, Warning, TEXT("ending mantle ability"));
				EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , true);
			}
		}
		else
		{
			// FVector LandPoint = ICharacterAbilityHelperInterface::Execute_GetMantleLandLocation(Cast<UObject>(ActorInfo->OwnerActor));
			FVector LandPoint = ActorInfo->OwnerActor->GetActorLocation();
			ICharacterAbilityHelperInterface* CharacterAbilityHelperInterface = Cast<ICharacterAbilityHelperInterface>(ActorInfo->OwnerActor);
			LandPoint = CalculateLandingPoint(LandPoint , CharacterAbilityHelperInterface->GetGrappleOffset() ,ICharacterAbilityHelperInterface::Execute_GetHitGrappleSurfaceImpactNormal(Cast<UObject>(ActorInfo->OwnerActor))* -1	);
			LandPoint.Z +=30;
			if (LandPoint != FVector(0,0,0))
			{
				if (CheckLandingClearance(LandPoint))
				{
					
					MantleUp(LandPoint);
				}
				else
				{
					// FGameplayEventData EventData;
					// EventData.InstigatorTags.AddTag(FGameplayTag::RequestGameplayTag("Character.Status.Mantle"));
					// SendGameplayEvent(FGameplayTag::RequestGameplayTag("Character.Status.LedgeGrab") ,EventData);
					EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , true);
				}
			}
		}

 	
		

	// if ability triggered by gameplay event
	// if (TriggerEventData)
	// {
	// 	FVector CharacterEyesLocation;
	// 	FRotator CharacterEyesRotation;
	// 	ActorInfo->OwnerActor->GetActorEyesViewPoint(CharacterEyesLocation , CharacterEyesRotation);
	// 	FVector Start = (ActorInfo->OwnerActor->GetActorForwardVector() * 100)  + CharacterEyesLocation;
	// 	FVector End   (Start.X , Start.Y , Start.Z - CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight());
	// 	TArray<AActor*> ActorsToIgnore;
	// 	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	// 	FHitResult HitResult;

	// 	if (!UKismetSystemLibrary::LineTraceSingle(GetWorld() , Start , End ,  UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false , ActorsToIgnore ,EDrawDebugTrace::None , HitResult , true))
	// 	{
	// 		EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , true);
	// 		return;
	// 	}
		
	// 	Start = FVector (HitResult.Location.X , HitResult.Location.Y , CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight() + HitResult.Location.Z + 40 + MaxMantleReachHeight);
	// 	End = FVector (HitResult.Location.X , HitResult.Location.Y , HitResult.Location.Z + 70 + MaxMantleReachHeight);
		
	// 	if(UKismetSystemLibrary::SphereTraceSingle(GetWorld() , Start  , End , CharacterCapsuleComponent->GetUnscaledCapsuleRadius() , UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility) , false , ActorsToIgnore , EDrawDebugTrace::None , HitResult , true))
	// 	{
	// 		//if sphere trace hit, means there isnt enough space above ledge , hang onto ledge in this case
	// 		FGameplayEventData Payload;
	// 		SendGameplayEvent(FGameplayTag::RequestGameplayTag("Character.Status.LedgeGrab") , Payload);
	// 		EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , true);
	// 		return;
	// 	}
	// 	else
	// 	{
	// 		//if no hit, space above ledge is big enough for character, climb up in that case
	// 		MantleUp(End);
	// 	}	
	// }
	// else
	// {
	// 	if (!ActorInfo->OwnerActor->GetClass()->ImplementsInterface(UCharacterAbilityHelperInterface::StaticClass()))
	// 	{
	// 		EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , false);
	// 	}

	// 	FVector LandPoint = ICharacterAbilityHelperInterface::Execute_GetMantleLandLocation(Cast<UObject>(ActorInfo->OwnerActor));
	// 	LandPoint = FVector(LandPoint.X , LandPoint.Y , LandPoint.Z + CharacterCapsuleComponent->GetUnscaledCapsuleRadius());
	// 	if (LandPoint != FVector(0,0,0))
	// 	{
	// 		MantleUp(LandPoint);
	// 	}
	// }
}

void UMantleAbility::MantleUp(FVector _MantleLandLocation)
{
	MantleLandLocation = _MantleLandLocation;
	MantleTimeline->PlayFromStart();
}

void UMantleAbility::HandleInterpolationProgress(float Value)
{
	CurrentActorInfo->OwnerActor->SetActorLocation(FMath::Lerp(CurrentActorInfo->OwnerActor->GetActorLocation() , MantleLandLocation , Value));
}

void UMantleAbility::HandleInterpolationCompleted()
{
	UE_LOG(LogTemp, Warning, TEXT("Mantle Interpolation completed"));
	Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent)->SetMovementMode(EMovementMode::MOVE_Walking	);
	Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent)->StopMovementImmediately();
	Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent)->GravityScale = 1;
	Cast<ACharacter>(CurrentActorInfo->OwnerActor)->bUseControllerRotationYaw = true;
	// Cast<ACharacter>(CurrentActorInfo->OwnerActor)->StopJumping();
	if (GetAvatarActorFromActorInfo() && DefaultMappingContext)
	{
		ICharacterAbilityHelperInterface::Execute_UpdateMappingContext(Cast<UObject>(GetAvatarActorFromActorInfo()) , DefaultMappingContext);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("failed to change mapping context mantleability.cpp (94)"));
	}
	EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , false);
}

bool UMantleAbility::CheckLandingClearance(FVector LandPoint)
{		
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	FHitResult HitResult;

		
	if(!UKismetSystemLibrary::CapsuleTraceSingle(GetWorld() , LandPoint , LandPoint , CharacterCapsuleComponent->GetUnscaledCapsuleRadius() , CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight() ,UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility) , false , ActorsToIgnore , EDrawDebugTrace::ForDuration , HitResult , true , FLinearColor::Red , FLinearColor::Green , 10))
		{
			// UE_LOG(LogTemp, Warning, TEXT("%s"), *End.ToString());
			return true;
		}
	else
		{
			return false;
		}
}


void UMantleAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle , ActorInfo , ActivationInfo , bReplicateEndAbility , bWasCancelled);
	// UE_LOG(LogTemp, Warning, TEXT("Mantle Ability Ended"));
	// UE_LOG(LogTemp, Warning, TEXT("%d") , bWasCancelled);
}

FVector UMantleAbility::CalculateLandingPoint(FVector LedgeLocation , FVector Offset  , FVector SurfaceNormal)
{
	TSoftObjectPtr<AActor> Owner = GetAvatarActorFromActorInfo();
	FVector OwnerForwardVector = Owner->GetActorForwardVector();
	if (SurfaceNormal == FVector(0,0,0))
	{
		bool UseRotationYaw  = Cast<ACharacter>(CurrentActorInfo->OwnerActor)->bUseControllerRotationYaw;
		if (UseRotationYaw == false)
		{
			FVector LedgeVector = FVector(LedgeLocation.X + (CharacterCapsuleComponent->GetForwardVector() * 50).X, LedgeLocation.Y + (CharacterCapsuleComponent->GetForwardVector() * 50).Y  , LedgeLocation.Z + CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight() + 1);
			return LedgeVector;
		}
		FVector LedgeVector = FVector(LedgeLocation.X , LedgeLocation.Y , LedgeLocation.Z + CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight() + 1);
		return LedgeVector;
		// LedgeVector = FVector (LedgeVector)
	}
	
	FVector LedgeVector = FVector(Owner->GetActorLocation().X + (SurfaceNormal.X * Offset.X) , Owner->GetActorLocation().Y + (SurfaceNormal.Y * Offset.Y), LedgeLocation.Z + CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight() + 1);
	return LedgeVector;

}