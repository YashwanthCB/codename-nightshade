#include "Ability/Abilities/GrappleAbility.h"
#include "Curves/CurveFloat.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"

UGrappleAbility::UGrappleAbility()
{
	GrappleExpansionTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("GrappleExpansionTimeline"));
	GrappleRetractionTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("GrappleRetractionTimeline"));
	PlayerZForceTimeline= CreateDefaultSubobject<UTimelineComponent>(TEXT("PlayerZForceTimeline"));
	PlayerMovementTimeline= CreateDefaultSubobject<UTimelineComponent>(TEXT("PlayerMovementTimeline"));
	// UE_LOG(LogTemp, Warning, TEXT("grapple ability initialized"));
	// UE_LOG(LogTemp, Warning, TEXT("expansion finished"));
	
}

void UGrappleAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData)
{

	if (GetAvatarActorFromActorInfo() == nullptr || !ActorInfo->OwnerActor->GetClass()->ImplementsInterface(UCharacterAbilityHelperInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("exiting ability GrappleAbility.cpp(22)"));
		EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , true);
	}

	//initializing default components and variables
	if (UObject *OwnerUObject = Cast<UObject>(ActorInfo->OwnerActor))
	{
		ICharacterAbilityHelperInterface::Execute_InitializeGrappleHook(OwnerUObject);
		CharacterSkeletalMesh = ICharacterAbilityHelperInterface::Execute_GetCharacterSkeletalMesh(OwnerUObject);
		GrappleEndMesh = ICharacterAbilityHelperInterface::Execute_GetGrappleEndMesh(OwnerUObject);
		GrappleEndLocation = ICharacterAbilityHelperInterface::Execute_GetGrappleEndLocation(OwnerUObject);
		GrapplePlayerLocation = ICharacterAbilityHelperInterface::Execute_GetDestinationLocaton(OwnerUObject);
		GrappleSocketName = ICharacterAbilityHelperInterface::Execute_GetGrappleParentSocketName(OwnerUObject);

		if(GrappleEndLocation == FVector(0,0,0))
		{
			// UE_LOG(LogTemp, Warning, TEXT("exiting ability GrappleAbility.cpp(37)"));
			EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , true);
			return;
		}

		if (GrappleSocketName == "")
		{
			FTransform3d LocalTransform (FRotator::ZeroRotator , ICharacterAbilityHelperInterface::Execute_GetGrappleRelativeInitialLocation(OwnerUObject) , FVector(1,1,1));
			GrappleMeshStartLocation = UKismetMathLibrary::TransformLocation(LocalTransform , CharacterSkeletalMesh->GetComponentLocation());
			// UE_LOG(LogTemp, Warning, TEXT("%s") , *GrappleMeshStartLocation.ToString());
		}
		else
		{
			GrappleMeshStartLocation = CharacterSkeletalMesh->GetBoneLocation(GrappleSocketName , EBoneSpaces::WorldSpace);
			
		}
		
		ICharacterAbilityHelperInterface::Execute_UpdateCableLength(OwnerUObject , FVector::Dist(GrappleEndMesh->GetComponentLocation() , GrappleEndLocation));
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("null ref : GrappleAbility.cpp(28)"));
		EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , true);
	}

	//adding timeline components to ability actor
	if (!GrappleExpansionTimeline->IsRegistered())
	{
		GrappleExpansionTimeline->RegisterComponent();
		GrappleRetractionTimeline->RegisterComponent();
		PlayerZForceTimeline->RegisterComponent();
		PlayerMovementTimeline->RegisterComponent();
	}
	ChangeMappingContext(NoKeyBoardInputGrappleMappingContext);
	ProgressFunction.BindUFunction(this, "HandleGrappleExpansion");
	FinishedFunction.BindUFunction(this, "OnGrappleExpansionCompleted");

	GrappleExpansionTimeline->AddInterpFloat(GrappleExpansionCurve , ProgressFunction);
	GrappleExpansionTimeline->SetTimelineFinishedFunc(FinishedFunction);
	GrappleExpansionTimeline->SetLooping(false);
	GrappleExpansionTimeline->PlayFromStart();
}


//throwing grapple
void UGrappleAbility::HandleGrappleExpansion(float Value)
{
	
	GrappleEndMesh->SetWorldLocation(FMath::Lerp(GrappleMeshStartLocation , GrappleEndLocation , Value));
}

//grapple reached destination
void UGrappleAbility::OnGrappleExpansionCompleted()
{
	AttachGrappleEndMesh();
	AddRopeImpulseInternal(-1 , 200 , 58 , false , 5);
	

	//winding in cable
	GetWorld()->GetTimerManager().SetTimer(CableWindInTimerHandle ,this , &UGrappleAbility::WindInCable , 0.0001 , true , BeforeWindingInCableDelay);
}


void UGrappleAbility::WindInCable()
{
	//winding in extra rope
	float length = FMath::FInterpTo(ICharacterAbilityHelperInterface::Execute_GetCurrentCableLength(Cast<UObject>(CurrentActorInfo->OwnerActor)) , 10 , GetWorld()->GetDeltaSeconds() , CableWindInSpeed);
	ICharacterAbilityHelperInterface::Execute_UpdateCableLength(Cast<UObject>(CurrentActorInfo->OwnerActor) , length);

	// UE_LOG(LogTemp, Warning, TEXT("%f") ,length );
	//checking if rope is winded	
	if (UKismetMathLibrary::NearlyEqual_FloatFloat(length , 10 , 2))
	{
		GetWorld()->GetTimerManager().ClearTimer(CableWindInTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(CableVibrateTimerHandle ,this , &UGrappleAbility::VibrateCable , 0.001 , true);
		//adding delay after winding
		GetWorld()->GetTimerManager().SetTimer(AfterWindInDelayHandle ,this , &UGrappleAbility::OnAfterWindInDelayFinished , 0.001 , false ,AfterCableWindDelay);
	}
}

//delay after winding finished
void UGrappleAbility::OnAfterWindInDelayFinished()
{
	Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent)->DisableMovement();
	Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent)->StopMovementImmediately();
	GetWorld()->GetTimerManager().ClearTimer(CableVibrateTimerHandle);
	ICharacterAbilityHelperInterface::Execute_SetCableForce(Cast<UObject>(CurrentActorInfo->OwnerActor), FVector(0,0,0));

	GrappleEndMesh->AttachToComponent(CharacterSkeletalMesh , FAttachmentTransformRules::KeepWorldTransform);
	ProgressFunction.BindUFunction(this, "HandleGrappleMeshRetraction");
	FinishedFunction.BindUFunction(this, "OnGrappleMeshRetractionCompleted");

	GrappleRetractionTimeline->AddInterpFloat(GrappleRetractionCurve , ProgressFunction);
	GrappleRetractionTimeline->SetTimelineFinishedFunc(FinishedFunction);
	GrappleRetractionTimeline->PlayFromStart();

	ChangeMappingContext(GrappleMappingContext);

	if (GetAvatarActorFromActorInfo())
	{
		// ACharacter * PlayerCharacter = Cast<ACharacter>(CurrentActorInfo->OwnerActor);
		// auto PlayerController = Cast<APlayerController>(PlayerCharacter->GetController());
		// // auto temp = 
		// if (PlayerController)
		// {
		// 	PlayerCharacter->EnableInput(PlayerController);
		// }
		// CurrentActorInfo->PlayerController->EnableInput(Cast<APlayerController>(CurrentActorInfo->OwnerActor->GetInstigatorController()));
		CharacterInitialLocation = CurrentActorInfo->OwnerActor->GetActorLocation();
		ProgressFunction.BindUFunction(this, "HandlePlayerMovement");
		FinishedFunction.BindUFunction(this, "OnPlayerMovementCompleted");
		PlayerZForceProgressFunction.BindUFunction(this, "HandlePlayerZForce");
		// UE_LOG(LogTemp, Error, TEXT("flying mode"));
		Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent)->SetMovementMode(EMovementMode::MOVE_Flying);
		PlayerZForceTimeline->AddInterpFloat(PlayerZForceCurve , PlayerZForceProgressFunction);
		PlayerMovementTimeline->AddInterpFloat(PlayerMovementCurve , ProgressFunction);
		PlayerMovementTimeline->SetTimelineFinishedFunc(FinishedFunction);
		PlayerMovementTimeline->SetPlayRate(0.9);
		PlayerZForceTimeline->SetPlayRate(0.9);
		PlayerZForceTimeline->PlayFromStart();
		PlayerMovementTimeline->PlayFromStart();
		// UE_LOG(LogTemp, Warning, TEXT("initial = %s") , *CharacterInitialLocation.ToString());
		// UE_LOG(LogTemp, Warning, TEXT("Destination = %s") , *GrappleDestinationLocation.ToString());
	}

}


void UGrappleAbility::HandleGrappleMeshRetraction(float Value)
{
	// UE_LOG(LogTemp, Warning, TEXT("retract"));
	if (ICharacterAbilityHelperInterface::Execute_GetGrappleParentSocketName(Cast<UObject>(CurrentActorInfo->OwnerActor)) == "")
	{
		GrappleEndMesh->SetRelativeLocation(FMath::Lerp(GrappleEndMesh->GetRelativeLocation() , ICharacterAbilityHelperInterface::Execute_GetGrappleRelativeInitialLocation(Cast<UObject>(CurrentActorInfo->OwnerActor)) , Value));
	}
	else
	{
		GrappleEndMesh->SetWorldLocation(FMath::Lerp(GrappleEndLocation , CharacterSkeletalMesh->GetBoneLocation(GrappleSocketName , EBoneSpaces::WorldSpace) , Value));
		// GrappleEndMesh->SetWorldLocation(FMath::Lerp(GrappleEndMesh->GetComponentLocation() , CharacterSkeletalMesh->GetBoneLocation(GrappleSocketName , EBoneSpaces::WorldSpace) , Value));
		// UE_LOG(LogTemp, Warning, TEXT("Your message"));
	}
}



//on grapple hook returned
void UGrappleAbility::OnGrappleMeshRetractionCompleted()
{
	// UE_LOG(LogTemp, Warning, TEXT("%s") , *GrappleEndMesh->GetRelativeLocation().ToString());
	//Todo : set grapple invisible if necessary

	// if (GetAvatarActorFromActorInfo())
	// {
	// 	ProgressFunction.BindUFunction(this, "HandlePlayerMovement");
	// 	FinishedFunction.BindUFunction(this, "OnPlayerMovementCompleted");
	// 	PlayerZForceProgressFunction.BindUFunction(this, "HandlePlayerZForce");
	// 	Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent)->SetMovementMode(EMovementMode::MOVE_Flying);
	// 	PlayerZForceTimeline->AddInterpFloat(PlayerZForceCurve , PlayerZForceProgressFunction);
	// 	PlayerMovementTimeline->AddInterpFloat(PlayerMovementCurve , ProgressFunction);
	// 	PlayerMovementTimeline->SetTimelineFinishedFunc(FinishedFunction);
	// 	PlayerZForceTimeline->PlayFromStart();
	// 	PlayerMovementTimeline->PlayFromStart();
	// }
	// UE_LOG(LogTemp, Warning, TEXT("retract complete"));
}


//pushing character to garpple point
void UGrappleAbility::HandlePlayerMovement(float Value)
{
	FVector LerpVector  = FMath::Lerp(CharacterInitialLocation , GrapplePlayerLocation , Value);
	LerpVector.Z += PlayerZForce;
	CurrentActorInfo->OwnerActor->SetActorLocation(LerpVector);
}

//when character reached deatination - ending ability 
void UGrappleAbility::OnPlayerMovementCompleted()
{
	/*// Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent)->Velocity = FVector(0,0,0);
	// Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent)->StopMovementImmediately();
	ChangeMappingContext(GrappleMappingContext);
	// UE_LOG(LogTemp, Warning, TEXT("Character Interp completed"));
	if (ICharacterAbilityHelperInterface::Execute_GetCanHoldPlayer(Cast<UObject>(CurrentActorInfo->OwnerActor)))
	{
		// UE_LOG(LogTemp, Warning, TEXT("Holding player"));
		GetWorld()->GetTimerManager().SetTimer(PlayerFallDelayHandle ,this , &UGrappleAbility::OnGrappleEnd , PlayerHoldTime , false);
	}
	else
	{
		OnGrappleEnd();
	}
	*/
	OnGrappleEnd();
	
}


void UGrappleAbility::OnGrappleEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("updating movement mode to falling"));
	Cast<UCharacterMovementComponent>(CurrentActorInfo->MovementComponent)->SetMovementMode(EMovementMode::MOVE_Walking);
	ChangeMappingContext(DefaultMappingContext);
  	EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , false);
}

void UGrappleAbility::HandlePlayerZForce(float Value)
{
	PlayerZForce = Value;
}

void UGrappleAbility::AttachGrappleEndMesh()
{
	FHitResult SphereHitresult;
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());

	UKismetSystemLibrary::SphereTraceSingle(GetWorld() , GrappleEndMesh->GetComponentLocation() , GrappleEndMesh->GetComponentLocation() , 150 ,UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4)  , false ,ActorsToIgnore ,EDrawDebugTrace::None , SphereHitresult , true);

	GrappleEndMesh->AttachToComponent(Cast<USceneComponent>(SphereHitresult.Component) , FAttachmentTransformRules::KeepWorldTransform);
}

void UGrappleAbility::AddRopeImpulseInternal(int32 ImpulseDirection , float Intensity , int32 ImpulseParticlePosition , bool UseDistributedForce , int32 UnfoldingRange)
{
	if (ImpulseDirection != -1 && ImpulseDirection != 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed To Add Rope Impulse see AddImpulseInternal declaration"));
		return;
	}

	ICharacterAbilityHelperInterface::Execute_AddImpulseToRope(Cast<UObject>(GetAvatarActorFromActorInfo()),UKismetMathLibrary::Multiply_VectorFloat((GrappleEndMesh->GetForwardVector()* ImpulseDirection) , Intensity) , ImpulseParticlePosition , UseDistributedForce , UnfoldingRange);
}

void UGrappleAbility::VibrateCable()
{
	// UE_LOG(LogTemp, Warning, TEXT("vibrate"));
	FVector Force = UKismetMathLibrary::Multiply_VectorFloat(GrappleEndMesh->GetRightVector() , VibrationForce);
	if (IsA == true)
	{
		ICharacterAbilityHelperInterface::Execute_SetCableForce(Cast<UObject>(CurrentActorInfo->OwnerActor), Force);
		IsA=false;
		return;
	}
	ICharacterAbilityHelperInterface::Execute_SetCableForce(Cast<UObject>(CurrentActorInfo->OwnerActor), (Force * -1));
	IsA=true;
}


void UGrappleAbility::ChangeMappingContext(UInputMappingContext *NewContext)
{
	if (GetAvatarActorFromActorInfo())
	{
		ICharacterAbilityHelperInterface::Execute_UpdateMappingContext(Cast<UObject>(GetAvatarActorFromActorInfo()) , NewContext);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("null ref : GrappleAbility.cpp(208)"));
		EndAbility(CurrentSpecHandle , CurrentActorInfo , CurrentActivationInfo , false , true);
	}
}


void UGrappleAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle , ActorInfo , ActivationInfo , bReplicateEndAbility , bWasCancelled);
	// UE_LOG(LogTemp, Error, TEXT("ending grapple ability"));
	
	
	GetWorld()->GetTimerManager().ClearTimer(AfterWindInDelayHandle);
	GetWorld()->GetTimerManager().ClearTimer(PlayerFallDelayHandle);
	GetWorld()->GetTimerManager().ClearTimer(CableVibrateTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(CableWindInTimerHandle);
	// AfterWindInDelayHandle.Invalidate();
	// PlayerFallDelayHandle.Invalidate();
	// CableVibrateTimerHandle.Invalidate();
	// CableWindInTimerHandle.Invalidate();
	GrappleExpansionTimeline->Stop();
	GrappleRetractionTimeline->Stop();
	PlayerZForceTimeline->Stop();
	PlayerMovementTimeline->Stop();

}