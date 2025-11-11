


#include "Tutorial/TTNotepad.h"

#include "GameModes/NSFPSGameMode.h"
#include "Kismet/GameplayStatics.h"

ATTNotepad::ATTNotepad()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ATTNotepad::BeginPlay()
{
	Super::BeginPlay();
	auto FPSGameMode = Cast<ANSFPSGameMode>(UGameplayStatics::GetGameMode(this));
	FPSGameMode->NotifyNotepadPlacedInLevel();
}

void ATTNotepad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATTNotepad::CanInteract() const
{
	return true;
}

FName ATTNotepad::GetDisplayText() const
{
	return FName("Take the clue");
}

FTransform ATTNotepad::GetInteractionTransform() const
{
	return GetActorTransform();
}

void ATTNotepad::Interact(AActor* InInstigator)
{
	auto FPSGameMode = Cast<ANSFPSGameMode>(UGameplayStatics::GetGameMode(this));
	FPSGameMode->NotepadPickedup();
	Destroy();
}

