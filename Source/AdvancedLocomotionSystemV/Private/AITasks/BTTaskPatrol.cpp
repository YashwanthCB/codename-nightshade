


#include "AITasks/BTTaskPatrol.h"
#include "DrawDebugHelpers.h"
#include "Interfaces/AIInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Controllers/AI/PatrolGuardAIController.h"


UBTTaskPatrol::UBTTaskPatrol()
{
	NodeName = TEXT("Patrol");
}

EBTNodeResult::Type UBTTaskPatrol::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);


	OwnerComponent = &OwnerComp;
	UE_LOG(LogTemp, Warning, TEXT("UBTTaskPatrol::OnInstanceCreated display name %s"), *UKismetSystemLibrary::GetDisplayName(OwnerComponent));
	OwnerComp.GetAIOwner()->ReceiveMoveCompleted.AddDynamic(this, &UBTTaskPatrol::OnDestinationReached);

	AAIController *AIController = OwnerComp.GetAIOwner();
	APawn *ControlledPawn = AIController->GetPawn();
	IAIInterface *AIInterface = Cast<IAIInterface>(AIController);

	if(!AIController || !AIInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("failed"));
		EBTNodeResult::Failed;
	}

	DestinationLocation = AIInterface->GetDestinationLocation();

	if(PatrolDirection == EPatrolDirection::FORWARD)
	{
		AIController->MoveToLocation(DestinationLocation);
	}
	else
	{
		AIController->MoveToLocation(AIInterface->GetAIInitialLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("UBTTaskPatrol::ExecuteTask Going to destination %s %s"), *DestinationLocation.ToString(), *UKismetSystemLibrary::GetDisplayName(ControlledPawn));
	PatrolDirection = PatrolDirection == EPatrolDirection::FORWARD ? EPatrolDirection::BACKWARD : EPatrolDirection::FORWARD;
	return EBTNodeResult::InProgress;
}

void UBTTaskPatrol::OnDestinationReached(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	// UE_LOG(LogTemp, Warning, TEXT("finished moving"));
	UE_LOG(LogTemp, Warning, TEXT("UBTTaskPatrol::OnDestinationReached Destination Reached %s"), *UKismetSystemLibrary::GetDisplayName(OwnerComponent->GetAIOwner()->GetPawn()));
	//FinishLatentTask(*this->OwnerComponent, EBTNodeResult::Succeeded);
}
