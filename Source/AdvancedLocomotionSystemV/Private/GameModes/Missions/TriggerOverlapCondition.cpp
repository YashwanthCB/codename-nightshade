


#include "GameModes/Missions/TriggerOverlapCondition.h"

#include "Components/ShapeComponent.h"
#include "Engine/TriggerBox.h"
#include "Kismet/GameplayStatics.h"

UTriggerOverlapCondition::UTriggerOverlapCondition() : bConditionReached(false)
{
}

void UTriggerOverlapCondition::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox = Cast<ATriggerBox>(GetOwner());
	TriggerBox->GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &UTriggerOverlapCondition::OnComponentBeginOverlapCallback);
}

void UTriggerOverlapCondition::OnComponentBeginOverlapCallback(UPrimitiveComponent* OverlappedComp, AActor* Other,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& OverlapInfo)
{
	UE_LOG(LogTemp, Display, TEXT("UTriggerOverlapCondition::OnComponentBeginOverlapCallback. Called"));
	
	if (Other == Cast<AActor>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		UE_LOG(LogTemp, Display, TEXT("UTriggerOverlapCondition::OnComponentBeginOverlapCallback. Player Crossed overlap"));
		NotifyObjectiveConditionReached();
	}
}
