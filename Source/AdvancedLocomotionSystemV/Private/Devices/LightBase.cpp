


#include "Devices/LightBase.h"

#include "TrainGameInstance.h"
#include "AI/Components/DisturbanceIndicatorSpawner.h"
#include "Components/LightComponent.h"
#include "Devices/LightInfluenceComponent.h"
#include "Interfaces/VisibleToAI.h"
#include "Devices/SwitchBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


ALightBase::ALightBase() : LightIntensityFactor(1.0f)
{
    PrimaryActorTick.bCanEverTick = true;
    
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    LightIndicatorSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LightIndicatorSpawnPoint"));
    DisturbanceIndicatorSpawner = CreateDefaultSubobject<UDisturbanceIndicatorSpawner>(TEXT("DisturbanceIndicatorSpawner"));

    SetRootComponent(Root);
    LightIndicatorSpawnPoint->SetupAttachment(Root);
}

float ALightBase::GetLightExposureOnVisibleActor_Implementation(AActor* InActor)
{
    return 0.0f;
}

float ALightBase::GetLightExposureOn(AActor* InActor)
{
    // Local variables from Blueprint
    FVector LineSegmentEndVector = FVector::ZeroVector; // Default value
    const double MinLengthToLight = 150.0; // Default value from Blueprint

    // Early return if light is not turned on
    if (!IsTurnedOn)
    {
        return 0.0f; // Default return value when light is off
    }

    // Get locations of this light and the input actor
    FVector LightLocation = GetActorLocation();
    FVector ActorLocation = InActor ? InActor->GetActorLocation() : FVector::ZeroVector;

    // Calculate distance between light and actor
    double DistanceToActor = UKismetMathLibrary::Vector_Distance(LightLocation, ActorLocation);

    // Check if distance is within MinLengthToLight
    if (DistanceToActor <= MinLengthToLight)
    {
        // Calculate LineSegmentEndVector based on light's forward vector
        FVector ForwardVector = GetPrimitiveLightForwardVector(); // Custom function
        LineSegmentEndVector = LightLocation + (ForwardVector * MinLengthToLight);
    }
    else
    {
        LineSegmentEndVector = ActorLocation; // Use actor's location directly
    }

    // Perform line trace from light to LineSegmentEndVector
    FHitResult HitResult;
    bool bHit = UKismetSystemLibrary::LineTraceSingle(
        this,                           // World context object
        LightLocation,                  // Start
        LineSegmentEndVector,           // End
        ETraceTypeQuery::TraceTypeQuery1, // Trace channel
        false,                          // bTraceComplex
        TArray<AActor*>(),              // Actors to ignore (empty)
        EDrawDebugTrace::ForOneFrame,   // Debug trace type
        HitResult,                      // Out hit result
        true,                           // bIgnoreSelf
        FLinearColor(0.100626f, 0.0f, 1.0f, 1.0f), // Trace color
        FLinearColor(0.626202f, 1.0f, 0.332033f, 1.0f), // Hit color
        5.0f                            // Draw time
    );

    // If no hit or hit something else, return 0.0f
    if (!bHit)
    {
        return 0.0f;
    }

    // Cast the input actor to VisibleToAI interface
    if (IVisibleToAI* VisibleActor = Cast<IVisibleToAI>(InActor))
    {
        // Get the LightInfluenceComponent from the interface
        ULightInfluenceComponent* LightInfluenceComp = IVisibleToAI::Execute_GetLightInfluenceComponent(InActor);// VisibleActor->GetLightInfluenceComponent();
        if (LightInfluenceComp)
        {
            // Get the component to check against the hit result
            USceneComponent* ComponentToCheck = LightInfluenceComp->GetComponentToCheckHit();

            // Break the hit result to get the hit component
            UPrimitiveComponent* HitComponent = HitResult.GetComponent();

            // Compare the hit component with the component to check
            if (ComponentToCheck && ComponentToCheck == HitComponent)
            {
                // Calculate light intensity at the hit point
                double Intensity = GetLightIntensityAt(HitResult.ImpactPoint); // Custom function
                return static_cast<float>(Intensity); // Return as float
            }
        }
    }

    // Default return if conditions fail
    return 0.0f;
}

void ALightBase::BeginPlay()
{
    Super::BeginPlay();
    
    auto TrainGameInstance = Cast<UTrainGameInstance>(GetWorld()->GetGameInstance());
	
    if (TrainGameInstance)
    {
        TrainGameInstance->OnPlayerBeginPlayCalled.AddUObject(this, &ALightBase::OnPlayerBeginPlayCalled);
    }
}

void ALightBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (GetWorld()->IsPreviewWorld())
        return;
    
    DisturbanceIndicatorSpawner->SetSpawnPoint(LightIndicatorSpawnPoint->GetComponentTransform());

    if (IsValid(SwitchReference))
    {
        IsInitiallyTurnedOn = IsTurnedOn = SwitchReference->GetIsInitiallyTurnedOn();
        SwitchReference->OnEventSwitch().AddUObject(this, &ALightBase::OnEventSwitchCallback);
        SwitchLight(IsTurnedOn);
    }
    else
    {
        IsInitiallyTurnedOn = IsTurnedOn;
        SwitchLight(IsTurnedOn);
    }
}


void ALightBase::Fix(AAIController* InAIController)
{
    SwitchLight(IsInitiallyTurnedOn);
    FixLight();
}

FTransform ALightBase::GetDisturbedActorTransform()
{
    return GetActorTransform();
}

FTransform ALightBase::GetIdealInteractionTransform()
{
    return GetActorTransform();
}

void ALightBase::SwitchLight(bool InIsSwitchedOn)
{
    IsTurnedOn = InIsSwitchedOn;
    if (IsTurnedOn == IsInitiallyTurnedOn)
    {
        DisturbanceIndicatorSpawner->DestroyDisturbanceIndicator();
    }
    else
    {
        DisturbanceIndicatorSpawner->SpawnDisturbanceIndicator(GetActorLocation(), SwitchReference);
    }
    SwitchLightBP(InIsSwitchedOn);
}

void ALightBase::OnEventSwitchCallback(bool InIsSwitchedOn)
{
    SwitchLight(InIsSwitchedOn);
}

FVector ALightBase::GetPrimitiveLightForwardVector()
{
    return GetLightComponent()->GetForwardVector();
}

float ALightBase::GetLightIntensityAt(FVector InPosition)
{
    // Get the base intensity from the SpotLight component
    float BaseIntensity = GetLightComponent() ? GetLightComponent()->Intensity * LightIntensityFactor : 0.0f;

    // Calculate angular and linear attenuation
    double AngularAttenuation = GetAngularAttenuationAt(InPosition);
    double LinearAttenuation = GetLinearAttenuationAt(InPosition);

    // Multiply the attenuations with the base intensity
    double AttenuatedIntensity = AngularAttenuation * LinearAttenuation * BaseIntensity;

    // Map the attenuated intensity from range [0, 600] to [0, 1]
    double MappedIntensity = UKismetMathLibrary::MapRangeClamped(
        AttenuatedIntensity,  // Value
        0.0,                  // InRangeA
        600.0,                // InRangeB
        0.0,                  // OutRangeA
        1.0                   // OutRangeB
    );

    //FMath::Ease

    // Apply easing function (CircularOut) from [0, 1] to [0, 100]
    double EasedIntensity = UKismetMathLibrary::Ease(
        0.0,                  // A (start value)
        100.0,                // B (end value)
        MappedIntensity,// Alpha
        EEasingFunc::CircularOut // Easing function
    );

    EasedIntensity = FMath::InterpEaseOut(0.0, 100.0, MappedIntensity, 2.0);
    return EasedIntensity;
}

float ALightBase::GetAngularAttenuationAt(FVector InPosition)
{
    double Angle = 0.0;

    if (!GetLightComponent())
    {
        return 0.0; // Early return if SpotLight1 is invalid
    }

    // Get the light's location and forward vector
    FVector LightLocation = GetLightComponent()->K2_GetComponentLocation();
    FVector LightForwardVector = GetLightComponent()->GetForwardVector();

    // Calculate the rotation from light to target and get its forward vector
    FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(LightLocation, InPosition);
    FVector DirectionToTarget = UKismetMathLibrary::GetForwardVector(LookAtRotation);

    // Calculate the dot product between the light's forward vector and the direction to the target
    double DotProduct = UKismetMathLibrary::Dot_VectorVector(LightForwardVector, DirectionToTarget);

    // Calculate the angle in degrees using arccosine
    Angle = UKismetMathLibrary::DegAcos(DotProduct);

    // Get the spotlight's inner and outer cone angles
    // TODO: Consider the light subclass to extract these. Can we move this to derived classes?
    float InnerConeAngle = 0.0f;//GetLightComponent()->InnerConeAngle;
    float OuterConeAngle = 44.0f;//GetLightComponent()->OuterConeAngle;

    // Check if the angle is within the inner cone
    if (Angle <= InnerConeAngle)
    {
        return 1.0; // Full intensity within inner cone
    }
    else
    {
        // Map the angle from [InnerConeAngle, OuterConeAngle] to [1.0, 0.0]
        double Attenuation = UKismetMathLibrary::MapRangeUnclamped(
            Angle,           // Value
            InnerConeAngle,  // InRangeA
            OuterConeAngle,  // InRangeB
            1.0,             // OutRangeA (full intensity)
            0.0              // OutRangeB (no intensity)
        );
        return Attenuation;
    }
}

float ALightBase::GetLinearAttenuationAt(FVector InPosition)
{
    if (!GetLightComponent())
    {
        return 0.0; // Early return if SpotLight1 is invalid
    }

    // Get the light's location
    FVector LightLocation = GetLightComponent()->K2_GetComponentLocation();

    // Calculate the distance from the light to the point
    double Distance = UKismetMathLibrary::Vector_Distance(InPosition, LightLocation);

    // Get the attenuation radius from the spotlight
    // TODO: Consider the light subclass to extract these. Can we move this to derived classes?
    float AttenuationRadius = 1000.0f;//GetLightComponent()->AttenuationRadius;

    // Check if the distance exceeds the attenuation radius
    if (Distance > AttenuationRadius)
    {
        return 0.0; // No intensity beyond the attenuation radius
    }
    else
    {
        // Map the distance from [0, AttenuationRadius] to [0, 1] (clamped)
        double MappedDistance = UKismetMathLibrary::MapRangeClamped(
            Distance,         // Value
            0.0,              // InRangeA
            AttenuationRadius,// InRangeB
            0.0,              // OutRangeA
            1.0               // OutRangeB
        );

        // Apply ease-in-out interpolation from 1.0 (full intensity) to 0.0 (no intensity)
        double AttenuatedIntensity = UKismetMathLibrary::FInterpEaseInOut(
            1.0,             // A (start value)
            0.0,             // B (end value)
            MappedDistance,   // Alpha
            2.0               // Exponent
        );

        return AttenuatedIntensity;
    }
}