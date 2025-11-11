


#include "Devices/SecurityCamera.h"
#include "Components/AudioComponent.h"
#include "Character/VisionSenseComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"

ASecurityCamera::ASecurityCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	CameraMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraMesh"));
	AlarmMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AlarmMesh"));
	AlarmAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AlarmAudioComponent"));
	VisionSenseComponent = CreateDefaultSubobject<UVisionSenseComponent>(TEXT("VisionSenseComponent"));
	
	RootComponent = Root;
	CameraMesh->SetupAttachment(Root);
	AlarmMesh->SetupAttachment(Root);
	AlarmAudioComponent->SetupAttachment(Root);
	VisionSenseComponent->SetupAttachment(CameraMesh);

}

void ASecurityCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASecurityCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float PlayerVisibility = VisionSenseComponent->GetVisibilityPercentage(UGameplayStatics::GetPlayerPawn(this, 0));

	if ((PlayerVisibility > 40.0f) != AlarmAudioComponent->IsPlaying())
	{
		if (PlayerVisibility > 40.0f)
		{
			AlarmAudioComponent->Play();
			UAISense_Hearing::ReportNoiseEvent(this, UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation(), 40.0f, this);
			UE_LOG(LogTemp, Log, TEXT("ASecurityCamera::Tick. Started Playing"));
		}
		else
		{
			AlarmAudioComponent->Stop();
			UE_LOG(LogTemp, Log, TEXT("ASecurityCamera::Tick. Stopped Playing"));
		}
	}
	if (PlayerVisibility > 40.0f)
	{
	}

	UE_LOG(LogTemp, Log, TEXT("ASecurityCamera::Tick. PlayerVisibility: %f"), PlayerVisibility);
}