


#include "Character/VisionSenseComponent.h"
#include "Character/VisibilityProbeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/VisibleToAI.h"
#include "Devices/LightInfluenceComponent.h"

VisibilityChecker::VisibilityChecker(USceneComponent* InCallingSceneComponent, AActor* InOwnerActor, AActor* PointOfInterest, float InHalfFOV)
{
	CallingSceneComponent = InCallingSceneComponent;
	OwnerActor = InOwnerActor;
	ActorToTrackVisibility = PointOfInterest;
	HalfFOV = InHalfFOV;
	PointOfInterestVisibilityPercentage = 0.0f;
	IsPointOfInterestInFOV = false;
	LineTraceParams.AddIgnoredActor(InOwnerActor);
	CacheVisibilityProbes();
}

void VisibilityChecker::Process()
{
	DetectPointOfInterest();
}

void VisibilityChecker::CacheVisibilityProbes()
{
	if(!IsValid(ActorToTrackVisibility))
		return;

	for (UActorComponent* Component : ActorToTrackVisibility->GetComponents())
	{
		if (Component && Component->IsA(UVisibilityProbeComponent::StaticClass()))
		{
			VisibilityProbes.Add(Cast<UVisibilityProbeComponent>(Component));
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%d") , VisibilityProbes.Num());
}


#define IF_PROXIMITY_SENSE(__DIST__, __FOV__) if(Distance < __DIST__ && IsVectorInFOV(ActorToTrackVisibility->GetActorLocation(), __FOV__)){ProbeHitRatio = 1.3f;}

void VisibilityChecker::DetectPointOfInterest()
{
	float Distance = FVector::Dist(OwnerActor->GetActorLocation() , ActorToTrackVisibility->GetActorLocation());
	UE_LOG(LogTemp, Warning, TEXT("VisibilityChecker::DetectPointOfInterest called"));
	if(Distance < 500.0f || VisibilityProbes.Num() == 0) // No multi line check needed if the character is very near or no probes are present in the actor
	{
		UE_LOG(LogTemp, Warning, TEXT("VisibilityChecker::DetectPointOfInterest. Distance < 500.0f"));
		float ProbeHitRatio = 0.0f;
		float LightExposure = 100.0f;
		
		//if(VisibilityProbes.Num() == 0)
		//{
			LightExposure = GetLightExposureFromActor(ActorToTrackVisibility);
		//}
		if(DoesSingleLineTraceHitActorAtVector(ActorToTrackVisibility->GetActorLocation()))
		{
			//low Dist(for high FOV) to high Dist(for low FOV)
			
			IF_PROXIMITY_SENSE(125.0, 180)
			else IF_PROXIMITY_SENSE(250, 135)
			else IF_PROXIMITY_SENSE(300, 90)
			else IF_PROXIMITY_SENSE(500, 50)
		}
		PointOfInterestVisibilityPercentage = ProbeHitRatio * LightExposure;
		IsPointOfInterestInFOV = ProbeHitRatio > 0.2f;
		UE_LOG(LogTemp, Warning, TEXT("VisibilityChecker::DetectPointOfInterest. PointOfInterestVisibilityPercentage: %f, %f, %f"), PointOfInterestVisibilityPercentage, ProbeHitRatio, LightExposure);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("VisibilityChecker::DetectPointOfInterest. Distance >= 500.0f"));
		ShootLineTraces();
	}
}

void VisibilityChecker::ShootLineTraces()
{
	UE_LOG(LogTemp, Warning, TEXT("VisibilityChecker::ShootLineTraces called"));
	//checkf(VisibilityProbes.Num(), TEXT("VisibilityProbes.Num() is 0. Please add probes to the actor. Can not divide by zero"));
	float AIVisibility = (float)GetProbeHitCount() / VisibilityProbes.Num();
	float LightExposure = GetLightExposureFromActor(ActorToTrackVisibility);
	PointOfInterestVisibilityPercentage = AIVisibility * LightExposure;
	IsPointOfInterestInFOV = AIVisibility > 0.2f;
	UE_LOG(LogTemp, Warning, TEXT("VisibilityChecker::ShootLineTraces. PointOfInterestVisibilityPercentage %f = %f * %f"), PointOfInterestVisibilityPercentage, AIVisibility, LightExposure);
}

bool VisibilityChecker::DoesSingleLineTraceHitActorAtVector(FVector InVector)
{
	FHitResult HitResult;
	// from AI.VisionSenseComponent to Player.Probe
	//bool bHitDetected = CallingSceneComponent->GetWorld()->LineTraceSingleByChannel(HitResult, CallingSceneComponent->GetComponentLocation(), InVector, ECollisionChannel::ECC_Visibility, LineTraceParams);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerActor);
	
	bool bHitDetected = UKismetSystemLibrary::LineTraceSingle(
		CallingSceneComponent->GetWorld(), CallingSceneComponent->GetComponentLocation(), InVector
		, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true);
	
	return HitResult.GetActor() == ActorToTrackVisibility;
}

bool VisibilityChecker::IsVectorInFOV(FVector InVector, float InHalfFOV)
{
	FVector ProbeConnectionVector = InVector - CallingSceneComponent->GetComponentLocation();
	ProbeConnectionVector.Normalize();
	float Angle = (180.0)/UE_DOUBLE_PI * FMath::Acos(FVector::DotProduct(ProbeConnectionVector, CallingSceneComponent->GetForwardVector())); // angle in degrees
	UE_LOG(LogTemp, Warning, TEXT("UVisionSenseComponent::IsProbeInFOV. Angle: %f, InHalfFOV: %f, %s"), Angle, InHalfFOV, Angle <= InHalfFOV ? TEXT("Visible") : TEXT("Invisible") );
	return Angle <= InHalfFOV;
}

float VisibilityChecker::GetLightExposureFromActor(AActor* InActor) 
{
	//checkf(ActorToTrackVisibility->Implements<UVisibleToAI>(),
	//	TEXT("VisibilityChecker::ShootLineTraces- %s does not implement interface IVisibleToAI"), *ActorToTrackVisibility->GetActorNameOrLabel());
	float Exposure = IVisibleToAI::Execute_GetLightInfluenceComponent(ActorToTrackVisibility)->GetLightExposure(); 
	return Exposure;
}

int VisibilityChecker::GetProbeHitCount()
{
	int ProbeHitCount = 0;
	for(auto& Probe : VisibilityProbes)
	{
		bool bTraceHitAtVector = DoesSingleLineTraceHitActorAtVector(Probe->GetComponentLocation());

		UE_LOG(LogTemp, Warning, TEXT("VisibilityChecker::GetProbeHitCount. bTraceHitAtVector: %s %s"), *Probe->GetName(), bTraceHitAtVector ? TEXT("True") : TEXT("False"));
		
		if(bTraceHitAtVector && IsVectorInFOV(Probe->GetComponentLocation(), HalfFOV))
		{
			++ProbeHitCount;
		}
	}
	return ProbeHitCount;
}

UVisionSenseComponent::UVisionSenseComponent() : HalfFOV(80.0f)
{
	PrimaryComponentTick.bCanEverTick = true;
	//bRegistered = true;
	bWantsInitializeComponent = true;
}

void UVisionSenseComponent::InitializeComponent()
{
	Super::InitializeComponent();
	ULightInfluenceComponent::OnVisibleActorSpawnedOrDestroyed.AddUObject(this, &UVisionSenseComponent::OnVisibleActorSpawnedOrDestroyedRaw);
}

void UVisionSenseComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UVisionSenseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UE_LOG(LogTemp, Warning, TEXT("UVisionSenseComponent::TickComponent. Called MeasureVisibility, %f"), DeltaTime);
	MeasureVisibility();
	PrintVisibility();
}

float UVisionSenseComponent::GetVisibilityPercentage(AActor* InActor)
{
	if(VisibilityMap.Contains(InActor))
		return VisibilityMap[InActor];
	return 0.0f;
}

bool UVisionSenseComponent::IsActorInFOV(AActor* InActor)
{
	if (FOVCheckMap.Contains(InActor))
		return FOVCheckMap[InActor];
	return false;
}

float UVisionSenseComponent::GetPlayerPawnVisibilityPercentage()
{
	return GetVisibilityPercentage(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

bool UVisionSenseComponent::IsPlayerInFOV()
{
	return IsActorInFOV(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void UVisionSenseComponent::MeasureVisibility(AActor* InActor)
{
	VisibilityChecker VisibilityCheckerObject(this, GetOwner(), InActor, HalfFOV);
	VisibilityCheckerObject.Process();
	VisibilityMap[InActor] = VisibilityCheckerObject.GetVisibility();
	bool InFOV = VisibilityCheckerObject.IsInFOV();
	FOVCheckMap[InActor] = InFOV;
}

void UVisionSenseComponent::MeasureVisibility()
{
	for(auto Pair : VisibilityMap)
	{
		MeasureVisibility(Pair.Key);
	}
}

void UVisionSenseComponent::PrintVisibility()
{
	for(auto Pair : VisibilityMap)
	{
		UE_LOG(LogTemp, Warning, TEXT("UVisionSenseComponent::PrintVisibility. VisibilityMap: Key: %s, Value: %f") , *Pair.Key->GetActorNameOrLabel(), Pair.Value);
	}
	for(auto Pair : FOVCheckMap)
	{
		UE_LOG(LogTemp, Warning, TEXT("UVisionSenseComponent::PrintVisibility. FOVCheckMap: Key: %s, Value: %d") , *Pair.Key->GetActorNameOrLabel(), Pair.Value);
	}
}

void UVisionSenseComponent::OnVisibleActorSpawnedOrDestroyedRaw(AActor* InActor, bool IsSpawned)
{
	//checkf(InActor->Implements<UVisibleToAI>(),
	//	TEXT("UVisionSenseComponent::OnVisibleActorSpawnedRawCallback- %s does not implement interface IVisibleToAI"), *InActor->GetActorNameOrLabel());
	
	if(IsSpawned)
	{
		VisibilityMap.Add(InActor, 0.0f);
		FOVCheckMap.Add(InActor, false);
	}
	else
	{
		VisibilityMap.Remove(InActor);
		FOVCheckMap.Remove(InActor);
	}
}