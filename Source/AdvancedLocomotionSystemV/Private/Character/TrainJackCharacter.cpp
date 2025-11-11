


#include "Character/TrainJackCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DataTable.h"
#include "Camera/CameraComponent.h"
#include "Character/Components/InteractionComponent.h"
#include "Dev/DebugDrawTable.h"
#include "Devices/LightBase.h"
#include "WidgetActors/GrapplePointHighlight.h"
#include "Kismet/GameplayStatics.h"

ADVANCEDLOCOMOTIONSYSTEMV_API DEFINE_LOG_CATEGORY(LogTrainJackerCharacter);

ATrainJackCharacter::ATrainJackCharacter(): MaxHP(100), CurrentHP(100), LightInfluenceComponent(nullptr)
{
	UE_LOG(LogTrainJackerCharacter, Log, TEXT(" constructor"));
	PrimaryActorTick.bCanEverTick = true;

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
}

// Called when the game starts or when spawned
void ATrainJackCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (DebugDataTable)
	{
		if (FDebugDrawStruct *DebugStruct = DebugDataTable->FindRow<FDebugDrawStruct>(FName(UGameplayStatics::GetCurrentLevelName(GetWorld())) , ""))
		{
			// UGameplayStatics::GetCurrentLevelName(GetWorld()))
			UE_LOG(LogTemp, Warning, TEXT("row found"));
			bShouldDrawDebug = DebugStruct->bShouldDrawDebug;
			TraceDebug = DebugStruct->TraceDebug;
			// bShouldDrawDebug = RowData->bShouldDrawDebug;
			// TraceDebug = RowData->TraceDebug;
		}
		else
		{
			bShouldDrawDebug = true;
			TraceDebug = EDrawDebugTrace::ForOneFrame;
		}
	}

	CameraComponent = ICharacterAbilityHelperInterface::Execute_GetCameraComponent(this);
	if (!CameraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("CameraComponent nullptr"));
		SetActorTickEnabled(false);

	}

	if (GrappleHighlightClass && IsPawnControlled())
	{
		GrappleHighlightActor = GetWorld()->SpawnActor<AGrapplePointHighlight>(GrappleHighlightClass, FVector(0,0,0), FRotator(0,0,0), FActorSpawnParameters());
		GrappleHighlightActor->SetHidden(true);
	}
	CurrentHP = MaxHP;

	auto TrainGameInstance = Cast<UTrainGameInstance>(GetWorld()->GetGameInstance());
	
	if (TrainGameInstance)
	{
		TrainGameInstance->OnPlayerBeginPlayCalled.Broadcast();
	}
}

// Called every frame
void ATrainJackCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult InHitResult;	
	float EstimatedRadius = GetTransitionPointHitResult(InHitResult);
	HitSurfaceNormal = InHitResult.ImpactNormal;
	CalculateGrappleVector(InHitResult);
	


	FHitResult temp;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	UKismetSystemLibrary::SphereTraceSingle(GetWorld() , InHitResult.TraceStart , InHitResult.TraceEnd , EstimatedRadius  , UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4) , false ,ActorsToIgnore  , TraceDebug , temp , true);
	

	if(!bShouldDrawDebug)
	{
		UKismetSystemLibrary::FlushPersistentDebugLines(GetWorld());
	}


	// UE_LOG(LogTemp, Warning, TEXT("%.2f") , EstimatedRadius);
	// UE_LOG(LogTemp, Warning, TEXT("%s") , *InHitResult.ImpactPoint.ToString());
	// UKismetSystemLibrary::DrawDebugSphere(GetWorld() , InHitResult.ImpactPoint , 5 , 12 , FLinearColor::Red , 0.1 , 0.1);

	

	/// print EstimatedRadius and draw the point you get from InHitResult in viewport 
}

// Called to bind functionality to input
void ATrainJackCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FCharacterLandDelegate& ATrainJackCharacter::GetCharacterLandDelegate()
{
	return OnCharacterLand;
}

void ATrainJackCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	if (OnCharacterLand.IsBound())
	OnCharacterLand.ExecuteIfBound();
}

void ATrainJackCharacter::CalculateGrappleVector(FHitResult SphereTraceHitResult)
{

	// UE_LOG(LogTemp, Warning, TEXT("%f") , SphereTraceHitResult.ImpactNormal.Z);
	
	if (SphereTraceHitResult.ImpactPoint == FVector(0,0,0))
	{
		GrappleVector = FVector::ZeroVector;
		return;
	}
	
	// if (SphereTraceHitResult.ImpactPoint == FVector(0,0,0) || UKismetMathLibrary::InRange_FloatFloat(SphereTraceHitResult.ImpactNormal.Z, -2 , -0.001))
	// {
	// 	GrappleVector = FVector::ZeroVector;
	// 	SetGrappleHighlightVisibility(false);
	// 	return;
	// }

	
	FVector Start = FVector( SphereTraceHitResult.ImpactPoint.X ,  SphereTraceHitResult.ImpactPoint.Y ,  SphereTraceHitResult.ImpactPoint.Z + VerticleSphereTraceZOffset);
	FVector End = FVector (SphereTraceHitResult.ImpactPoint.X ,  SphereTraceHitResult.ImpactPoint.Y ,  SphereTraceHitResult.ImpactPoint.Z - 10);
	// FHitResult VerticleSphereHitResult;
	

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	//vertical sphere trace
	bool bSphereTraceHitSomething = UKismetSystemLibrary::SphereTraceSingle(GetWorld() , Start , End , 1 , UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility) , false , ActorsToIgnore , TraceDebug , VerticleSphereHitResult , true);

	if (bSphereTraceHitSomething && IsValid(VerticleSphereHitResult.GetActor()) && !VerticleSphereHitResult.GetActor()->ActorHasTag("GRAPPLE"))
		return;
	
	

	//horizontal line trace
	UKismetSystemLibrary::LineTraceSingle(GetWorld() , FVector(VerticleSphereHitResult.ImpactPoint.X + (SphereTraceHitResult.ImpactNormal * 5).X, VerticleSphereHitResult.ImpactPoint.Y + (SphereTraceHitResult.ImpactNormal * 5).Y , VerticleSphereHitResult.ImpactPoint.Z+0.7) , FVector(VerticleSphereHitResult.ImpactPoint.X , VerticleSphereHitResult.ImpactPoint.Y , VerticleSphereHitResult.ImpactPoint.Z + 0.2) , UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4) , false , ActorsToIgnore , TraceDebug , HorizontalSideCheckHitResult , true);
	

	FHitResult GroundCheckHitResult;
	// UKismetSystemLibrary::LineTraceSingle(GetWorld() , VerticleSphereHitResult.ImpactPoint.X + () , VerticleSphereHitResult.ImpactPoint.Y + () , , UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4) , false , ActorsToIgnore , TraceDebug ,  , true);
	FVector NegetiveCharacterForwardVector = GetActorForwardVector() * -1;

	UKismetSystemLibrary::LineTraceSingle(GetWorld() , FVector(VerticleSphereHitResult.ImpactPoint.X + (NegetiveCharacterForwardVector.X * GroundCheckOffset), VerticleSphereHitResult.ImpactPoint.Y + (NegetiveCharacterForwardVector.Y * GroundCheckOffset) , VerticleSphereHitResult.ImpactPoint.Z + GroundCheckDownTraceLength) , FVector(VerticleSphereHitResult.ImpactPoint.X + (NegetiveCharacterForwardVector.X * GroundCheckOffset), VerticleSphereHitResult.ImpactPoint.Y + (NegetiveCharacterForwardVector.Y * GroundCheckOffset) , VerticleSphereHitResult.ImpactPoint.Z - 10) , UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4) , false , ActorsToIgnore , TraceDebug , GroundCheckHitResult , true);

	if(CheckAngle(VerticleSphereHitResult.ImpactNormal) && !HorizontalSideCheckHitResult.bBlockingHit && !GroundCheckHitResult.bBlockingHit)
	{
		GrappleVector = FVector(VerticleSphereHitResult.ImpactPoint.X + (SphereTraceHitResult.ImpactNormal.X * GrappleOffset.X) , VerticleSphereHitResult.ImpactPoint.Y + (SphereTraceHitResult.ImpactNormal.Y * GrappleOffset.Y) , VerticleSphereHitResult.ImpactPoint.Z + GrappleOffset.Z);
		SetGrappleHighlightVisibility(true);
		return;
	}
	GrappleVector = FVector::ZeroVector;
	SetGrappleHighlightVisibility(false);
	
}

float ATrainJackCharacter::GetTransitionPointHitResult(FHitResult& OutHitResult)
{
    float low = 0;
    float high = GrappleTraceRadius;
    
    while(low <= high)
    {
    	float mid = low + (high - low)/2; // rounding up by 1 digits 2 3 4
		
		if(IsHitByWall(mid).bBlockingHit != IsHitByWall(mid+1).bBlockingHit)
		{
			OutHitResult = IsHitByWall(mid+1); //did +1 because assiging only mid causes impact point blank when looking above ledge
			return mid+1;
		}
		
		if (IsHitByWall(mid).bBlockingHit == IsHitByWall(low).bBlockingHit)
		{
			low = mid+1;
		}
		else
		{
			high = mid;
		}
    }
	OutHitResult = IsHitByWall(high);
    return high;

}


FHitResult ATrainJackCharacter::IsHitByWall(float InRadius)
{
	FVector Start = CameraComponent->GetComponentLocation();
	FVector End = Start + (CameraComponent->GetComponentRotation().Vector() * GrappleDetectionRange);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult SphereHitResult;

	UKismetSystemLibrary::SphereTraceSingle(GetWorld() , Start , End , InRadius  , UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4) , false , ActorsToIgnore , EDrawDebugTrace::None , SphereHitResult , true);

	return SphereHitResult;
	

	// if (InRadius <= 20)
	// 	return false;
	// return true;
}

bool ATrainJackCharacter::CheckAngle(FVector ImpactNormal)
{
	float temp = UKismetMathLibrary::DegAcos(UKismetMathLibrary::Dot_VectorVector(GetActorUpVector() , ImpactNormal));
	
	// UE_LOG(LogTemp, Warning, TEXT("%f") , temp);
	if (temp > GrapplePointTolerationAngle)
	{
		return false;
	}
	return true;
}	

FVector ATrainJackCharacter::GetGrappleOffset()
{
	return GrappleOffset;
}


void ATrainJackCharacter::SetGrappleHighlightVisibility(bool NewVisibility)
{
	if (GrappleHighlightActor)
	{
		if(NewVisibility)
		{
			GrappleHighlightActor->SetHidden(!NewVisibility);
			GrappleHighlightActor->SetActorLocation(GrappleVector);
			return;
		}
		GrappleHighlightActor->SetHidden(!NewVisibility);

	}
	
}

ULightInfluenceComponent* ATrainJackCharacter::GetLightInfluenceComponent_Implementation()
{
	if(!LightInfluenceComponent)
	{
		LightInfluenceComponent = GetComponentByClass<ULightInfluenceComponent>();
	}
	return LightInfluenceComponent;
}
