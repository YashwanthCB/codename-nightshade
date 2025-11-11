


#include "Ability/Abilities/LedgeGrabAbility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InputMappingContext.h"
#include "Curves/CurveFloat.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Components/CapsuleComponent.h"

ULedgeGrabAbility::ULedgeGrabAbility()
{
	LedgeGrabTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("LedgeGrabTimeline"));
}

void ULedgeGrabAbility::OnDropInputPressed(float TimeWaited)
{
	DropDown();
}


void ULedgeGrabAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData)
{
	if (!ActorInfo->OwnerActor->GetClass()->ImplementsInterface(UCharacterAbilityHelperInterface::StaticClass()) || ActorInfo->OwnerActor == nullptr || ICharacterAbilityHelperInterface::Execute_GetIsOutOfLedgeTolerenceRange(Cast<UObject>(ActorInfo->OwnerActor)))
	{
		EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , true);
		return;
	}

	if (TriggerEventData->InstigatorTags.GetByIndex(0) == FGameplayTag::RequestGameplayTag("Character.Status.Mantle"))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s") , *TriggerEventData->InstigatorTags.GetByIndex(0).GetTagName().ToString());
	}

	UAbilityTask_WaitInputPress* WaitInputPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this, false);
	WaitInputPressTask->OnPress.AddDynamic(this, &ULedgeGrabAbility::OnDropInputPressed);
	WaitInputPressTask->Activate();

	OwnerObject = Cast<UObject>(ActorInfo->OwnerActor);

	LedgeLocation = ICharacterAbilityHelperInterface::Execute_GetFinalLedgeVector(OwnerObject);
	float maxclimbheight = ICharacterAbilityHelperInterface::Execute_GetMaxClimbHeight(OwnerObject);
	float maxledgegrabheight = ICharacterAbilityHelperInterface::Execute_GetMaxLedgeGrabHeight(OwnerObject);
	CharacterCapsuleComponent = Cast<UCapsuleComponent>(ActorInfo->OwnerActor->GetRootComponent());
	float CharacterCapsuleHalfHeight = CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight();
	float CharacterFootLocation = ActorInfo->OwnerActor->GetActorLocation().Z - CharacterCapsuleHalfHeight;
	
	
	ICharacterAbilityHelperInterface::Execute_SetActiveLedge(OwnerObject , LedgeLocation);

	if (LedgeLocation.Z - CharacterFootLocation  < maxclimbheight && LedgeLocation.Z - CharacterFootLocation < maxledgegrabheight)
	{
		SendGameplayEvent(FGameplayTag::RequestGameplayTag("Character.Status.Mantle") , FGameplayEventData());
		EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , false);
		return;
	}

	if (LedgeLocation.Z - CharacterFootLocation  > maxledgegrabheight)
	{
		EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , true);
		return;
	}

	if (!LedgeGrabTimeline->IsRegistered())
	{
		LedgeGrabTimeline->RegisterComponent();
	}

	StartLocation = ActorInfo->OwnerActor->GetActorLocation();
	EndLocation = LedgeLocation - FVector(0,0,CharacterCapsuleHalfHeight);

	ProgressFunction.BindUFunction(this , "HandleInterpolationProgress");
	FinishedFunction.BindUFunction(this , "HandleInterpolationCompleted");
	LedgeGrabTimeline->AddInterpFloat(LedgeGrabCurve ,ProgressFunction);
	LedgeGrabTimeline->SetTimelineFinishedFunc(FinishedFunction);
	LedgeGrabTimeline->SetLooping(false);
	LedgeGrabTimeline->PlayFromStart();
}



void ULedgeGrabAbility::HandleInterpolationProgress(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Ledge Grab Interpolation progress"));
	GetAvatarActorFromActorInfo()->SetActorLocation((FMath::Lerp(StartLocation , FVector(StartLocation.X , StartLocation.Y , EndLocation.Z) , Value)));
}

void ULedgeGrabAbility::HandleInterpolationCompleted()
{
	UE_LOG(LogTemp, Warning, TEXT("Ledge Grab Interpolation completed"));
	ICharacterAbilityHelperInterface::Execute_UpdateMappingContext(Cast<UObject>(GetAvatarActorFromActorInfo()) , LedgeGrabMappingContext);
	Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent)->GravityScale = 0;
	Cast<ACharacter>(CurrentActorInfo->OwnerActor)->bUseControllerRotationYaw = false;
	Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent)->StopMovementImmediately();
	Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent)->SetMovementMode(EMovementMode::MOVE_Flying);

	GetWorld()->GetTimerManager().SetTimer(LedgeClimbTimerHandle , this , &ULedgeGrabAbility::TraceDetectEnvironment , 0.01 , true);

}

void ULedgeGrabAbility::TraceDetectEnvironment()
{
	FHitResult HitRight;
	FHitResult HitLeft;
	FHitResult HitFront;

	FVector StartFront (GetAvatarActorFromActorInfo()->GetActorLocation().X , GetAvatarActorFromActorInfo()->GetActorLocation().Y , LedgeLocation.Z);

	FVector StartLeftRight = GetAvatarActorFromActorInfo()->GetActorLocation();
	FVector EndRight = StartLeftRight + GetAvatarActorFromActorInfo()->GetActorRightVector() * 45;
	FVector EndLeft = StartLeftRight - GetAvatarActorFromActorInfo()->GetActorRightVector() * 45;
	FVector EndFront = FVector(StartFront.X , StartFront.Y , LedgeLocation.Z) + GetAvatarActorFromActorInfo()->GetActorForwardVector() * 100;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());

	UKismetSystemLibrary::LineTraceSingle(GetWorld() , StartLeftRight , EndRight , UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility) , false , ActorsToIgnore , EDrawDebugTrace::None , HitRight , true);

	UKismetSystemLibrary::LineTraceSingle(GetWorld() , StartLeftRight , EndLeft , UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility) , false , ActorsToIgnore , EDrawDebugTrace::None , HitLeft , true);

	UKismetSystemLibrary::LineTraceSingle(GetWorld() , StartFront , EndFront , UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility) , false , ActorsToIgnore , EDrawDebugTrace::None , HitFront , true);


	if (!HitFront.bBlockingHit || ICharacterAbilityHelperInterface::Execute_GetFinalLedgeVector(OwnerObject) == FVector::ZeroVector)
	{
		DropDown();
		return;
	}

	FVector NewCharacterPosition (HitFront.Location + ((Cast<ACharacter>(CurrentActorInfo->OwnerActor)
	->GetActorForwardVector() * 50) * -1));

	NewCharacterPosition.Z = LedgeLocation.Z - CharacterCapsuleComponent->GetUnscaledCapsuleHalfHeight();

	Cast<ACharacter>(CurrentActorInfo->OwnerActor)->SetActorLocation(NewCharacterPosition);
	
}

void ULedgeGrabAbility::DropDown()
{
		ICharacterAbilityHelperInterface::Execute_UpdateMappingContext(Cast<UObject>(GetAvatarActorFromActorInfo()) , DefaultMappingContext);
		Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent)->SetMovementMode(EMovementMode::MOVE_Walking);
		Cast<ACharacter>(CurrentActorInfo->OwnerActor)->bUseControllerRotationYaw = true;
		Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent)->GravityScale = 1;
	
		// UE_LOG(LogTemp, Warning, TEXT("Drop Down"));
		EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , false);
}

void ULedgeGrabAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle , ActorInfo , ActivationInfo , bReplicateEndAbility , bWasCancelled);
	// UE_LOG(LogTemp, Warning, TEXT("Ledge Grab Ability Ended"));
	// if (bWasCancelled)
	// {
	// 	ICharacterAbilityHelperInterface::Execute_SetActiveLedge(OwnerObject , FVector::ZeroVector);
	// }
}




