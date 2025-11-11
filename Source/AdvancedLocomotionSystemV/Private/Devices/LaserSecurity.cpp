


#include "Devices/LaserSecurity.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AISense_Hearing.h"

ALaserSecurity::ALaserSecurity()
{
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	NiagaraLaserComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Laser"));
	LaserProducerComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserLightMesh"));
	LaserReceiverComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserReceiver"));
	AlarmAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AlarmAudio"));
	RootComponent = SceneRoot;
	
	NiagaraLaserComponent->SetupAttachment(RootComponent);
	LaserProducerComponent->SetupAttachment(RootComponent);
	LaserReceiverComponent->SetupAttachment(RootComponent);
}

void ALaserSecurity::BeginPlay()
{
	Super::BeginPlay();
	LaserProducerLocation = LaserProducerComponent->GetComponentLocation();
	LaserReceiverLocation = LaserReceiverComponent->GetComponentLocation();
	NiagaraLaserComponent->SetWorldLocation(LaserProducerLocation);
	NiagaraLaserComponent->SetVariableVec3("LaserEnd", LaserReceiverLocation);
	LaserHitSomethingEvent.AddUObject(this, &ALaserSecurity::InternalLaserSenseSomethingCallback);
	AlarmAudioComponent->Stop();
}

void ALaserSecurity::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	AlarmAudioComponent->Stop();
}

void ALaserSecurity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FHitResult HitResult;
	DidHitAnything = GetWorld()->LineTraceSingleByChannel(HitResult, LaserProducerLocation, LaserReceiverLocation, ECC_Visibility);
	if (DidHitAnything)
	{
		SetActorTickEnabled(false);
		NiagaraLaserComponent->SetHiddenInGame(true);
		LaserHitSomethingEvent.Broadcast();
	}
}

void ALaserSecurity::ResetLaser()
{
	if (!IsSwitchedOn)
		return;
	SetActorTickEnabled(true);
	DidHitAnything = false;
	NiagaraLaserComponent->SetHiddenInGame(false);
	AlarmAudioComponent->Stop();
}

void ALaserSecurity::InternalLaserSenseSomethingCallback()
{
	AlarmAudioComponent->Play();
	
	UE_LOG(LogTemp, Display, TEXT("ALaserSecurity::OnLaserSenseSomething. Laser sense something"));
	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.CallbackTarget = this;
	LatentActionInfo.ExecutionFunction = FName("ResetLaser");
	LatentActionInfo.UUID = 1223;
	LatentActionInfo.Linkage = 0;

	UAISense_Hearing::ReportNoiseEvent(this, GetActorLocation(), 40.0f, this);
	
	UKismetSystemLibrary::Delay(this, 3.0f, LatentActionInfo);
}
