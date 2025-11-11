
#include "WallRunComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CollisionQueryParams.h"
#include "Kismet/KismetMathLibrary.h"

void UWallRunComponent::Initialize(ACharacter *Character)
{
	if (Character)	
	{
		OwnerCharacter = Character;
		OwnerCharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
		
		DefaultGravity = OwnerCharacterMovementComponent->GravityScale;
		DefaultAirControl = OwnerCharacterMovementComponent->AirControl;

		GetWorld()->GetTimerManager().SetTimer(WallRunUpdateTimerHandle,this,&UWallRunComponent::WallRunTick,0.01,true);
	}
}

void UWallRunComponent::WallRunTick()
{
	if (!bWallRunSupressed)
	{
		WallRunUpdate();
	}
	CameraUpdate();
}

void UWallRunComponent::CameraUpdate()
{
	if(bWallRunningLeft)
	{
		// UE_LOG(LogTemp, Warning, TEXT("left"));
		TiltCamera(CameraTiltAngle);
		return;
	}

	if(bWallRunningRight)
	{
		// UE_LOG(LogTemp, Warning, TEXT("right"));
		TiltCamera(-CameraTiltAngle);
		return;
	}
		// UE_LOG(LogTemp, Warning, TEXT("nothing"));
		TiltCamera(0);
}

void UWallRunComponent::TiltCamera(float Roll)
{
	FRotator CurrentRotation = OwnerCharacter->GetController()->GetControlRotation();
	FRotator TargetRotation = FMath::RInterpTo(CurrentRotation, FRotator(CurrentRotation.Pitch,CurrentRotation.Yaw,Roll), GetWorld()->GetDeltaSeconds(), 10);
	OwnerCharacter->GetController()->SetControlRotation(TargetRotation);
}

void UWallRunComponent::WallRunUpdate()
{
	// if (WallRunMovement(OwnerCharacter->GetActorLocation(), GetEndVector().Get().))
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetEndVector().Get<0>().ToString());
	if(WallRunMovement(OwnerCharacter->GetActorLocation(), GetEndVector().Get<1>(), -1.0))
	{
		bWallRunning = true;
		bWallRunningRight = true;
		bWallRunningLeft = false;
			OwnerCharacterMovementComponent->GravityScale = FMath::FInterpTo(OwnerCharacterMovementComponent->GravityScale , WallRunGravityScale , GetWorld()->GetDeltaSeconds() , 10);
			OwnerCharacterMovementComponent->AirControl = 0;
	}
	else
	{
		if(bWallRunningRight)
		{
			EndWallRun(WallRunResetTime);
		}
		else
		{
			if(WallRunMovement(OwnerCharacter->GetActorLocation(), GetEndVector().Get<0>(), 1.0))
			{
				bWallRunning = true;
				bWallRunningRight = false;
				bWallRunningLeft = true;
				OwnerCharacterMovementComponent->GravityScale = FMath::FInterpTo(OwnerCharacterMovementComponent->GravityScale , WallRunGravityScale , GetWorld()->GetDeltaSeconds() , 10);
				OwnerCharacterMovementComponent->AirControl = 0;
			}
			else
			{
				EndWallRun(WallRunResetTime);
			}
		}
	}
}

bool UWallRunComponent::WallRunMovement(FVector StartVector, FVector EndVector, float Direction)
{
	bool bHitDetected = GetWorld()->LineTraceSingleByObjectType(HitResult, StartVector, EndVector, FCollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel3));
	// DrawDebugLine(GetWorld(), StartVector, EndVector, FColor::Red, false, 0.5f);
	
	if(!bHitDetected)
	{
		return false;
	}

	if(OwnerCharacterMovementComponent->IsFalling() == true && IsValidWallRunVector() == true)
	{
		//launching character forward
		WallRunNormal = HitResult.ImpactNormal;
		FVector vec =  UKismetMathLibrary::Subtract_VectorVector(OwnerCharacter->GetActorLocation(),  WallRunNormal);
		float len = vec.Size();
		OwnerCharacter->LaunchCharacter(UKismetMathLibrary::Multiply_VectorFloat(WallRunNormal , len), false, false);


		//launching character upwards
		FVector Cross = UKismetMathLibrary::Cross_VectorVector(WallRunNormal , FVector(0,0,1));
		FVector multi = UKismetMathLibrary::Conv_DoubleToVector(Direction * WallRunSpeed);
		OwnerCharacter->LaunchCharacter(UKismetMathLibrary::Multiply_VectorVector(Cross, multi) , true , !bEnableGravity);

		return true;
	}
		return false;
}


void UWallRunComponent::EndWallRun(float ResetTime)
{
	if(bWallRunning)
	{
		// UE_LOG(LogTemp, Warning, TEXT("end wallrun"));
		bWallRunning = false;
		bWallRunningLeft = false;
		bWallRunningRight = false;
		OwnerCharacterMovementComponent->GravityScale = DefaultGravity;
		OwnerCharacterMovementComponent->AirControl = DefaultAirControl;
		bWallRunSupressed = true;
		GetWorld()->GetTimerManager().SetTimer(WallRunSupressTimerHandle,this,&UWallRunComponent::ResetWallrunSupress , ResetTime , false);
	}
}


void UWallRunComponent::WallRunLand()
{
	EndWallRun(0);
	bWallRunSupressed = false;
}

void UWallRunComponent::WallRunJump()
{
	if(bWallRunning)
	{
		if(bWallRunningLeft)
		{
			TargetZRotation = WallRunjumpRotation + OwnerCharacter->GetControlRotation().Yaw;
		}
		else
		{
			if(bWallRunningRight)
			{
				TargetZRotation = ((WallRunjumpRotation * -1) + OwnerCharacter->GetControlRotation().Yaw);
			}
		}
		

		GetWorld()->GetTimerManager().SetTimer(CharacterRotationHandle, this , &UWallRunComponent::TurnCharacter , 0.01 , true);
		EndWallRun(WallRunResetTime);
		OwnerCharacter->LaunchCharacter(FVector((WallRunJumpSideForce * WallRunNormal.X), (WallRunJumpSideForce * WallRunNormal.Y), WallRunJumpUpForce) , false , true);
	}
	
}

void UWallRunComponent::TurnCharacter()
{
	FRotator CurrentCharacterRotation = OwnerCharacter->GetController()->GetControlRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentCharacterRotation, FRotator(CurrentCharacterRotation.Pitch,TargetZRotation,CurrentCharacterRotation.Roll) , GetWorld()->GetDeltaSeconds(), 15.0);
	
	if (UKismetMathLibrary::EqualEqual_RotatorRotator(NewRotation , FRotator(CurrentCharacterRotation.Pitch , TargetZRotation,CurrentCharacterRotation.Roll), 5))
	{
		GetWorld()->GetTimerManager().ClearTimer(CharacterRotationHandle);
	}
	OwnerCharacter->GetController()->SetControlRotation(NewRotation);
}


TTuple<FVector, FVector> UWallRunComponent::GetEndVector()
{
	//returns pair of vector, used to trace for wallrun
	// use 0 to get leftvector, 1 to get rightvector
	FVector LeftEnd;
	LeftEnd = OwnerCharacter->GetActorLocation() + ((OwnerCharacter->GetActorRightVector() * -60));
	// + (OwnerCharacter->GetActorForwardVector() * -0));

	FVector RightEnd;
	RightEnd = OwnerCharacter->GetActorLocation() + ((OwnerCharacter->GetActorRightVector()* 60));
	// + (OwnerCharacter->GetActorForwardVector() * -0));
	
	return TTuple<FVector, FVector>(LeftEnd, RightEnd);
}

void UWallRunComponent::ResetWallrunSupress()
{
	GetWorld()->GetTimerManager().ClearTimer(WallRunSupressTimerHandle);
	bWallRunSupressed = false;
}

bool UWallRunComponent::IsValidWallRunVector()
{
	return UKismetMathLibrary::InRange_FloatFloat(HitResult.ImpactNormal.Z, -0.52, 0.52 , false , false);
}

