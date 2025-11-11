


#include "AI/BT/Services/BTService_VariableHandler.h"
#include "AI/Components/InvestigationPlanComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/VisionSenseComponent.h"
#include "Character/Components/NSAIHearingProcessor.h"
#include "Kismet/GameplayStatics.h"
#include "TrainGameInstance.h"
#include "Controllers/AI/PatrolGuardAIController.h"


void UBTService_VariableHandler::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	auto PatrolAIController = Cast<APatrolGuardAIController>(OwnerComp.GetAIOwner());
	auto CurrentState = static_cast<EAIAwarenessState>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(AIAwarenessState.SelectedKeyName));

	bool CurrentIsPlayerInFOV = PatrolAIController->GetVisionSenseComponent()->IsPlayerInFOV();
	if (CurrentIsPlayerInFOV != PreviousIsPlayerInFOV)
	{
		IsPlayerNotInFOV->SetValue(CurrentIsPlayerInFOV);
	}
	PreviousIsPlayerInFOV = PatrolAIController->GetVisionSenseComponent()->IsPlayerInFOV();
	
	if (CurrentState == EAIAwarenessState::Chase)
		return;
	
	if (PatrolAIController->GetVisionSenseComponent()->GetPlayerPawnVisibilityPercentage() > 15.0f)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AIAwarenessState.SelectedKeyName, static_cast<uint8>(EAIAwarenessState::Chase));
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(SuspiciousActor.SelectedKeyName, UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		return;
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(SuspiciousActor.SelectedKeyName, nullptr);

	AActor* LoudestActor;
	float Intensity;
	FName Tag;
	bool FoundSomethingLoud = PatrolAIController->GetHearingProcessor()->GetLoudestSoundingActor(LoudestActor, Intensity, Tag);
	if (FoundSomethingLoud && Intensity > 25.0f)
	{
		UE_LOG(LogTemp, Log, TEXT("UBTService_VariableHandler::TickNode. LoudestActor: %s, Location: %s, Intensity: %f")
			, *LoudestActor->GetActorNameOrLabel(), *LoudestActor->GetActorLocation().ToString(), Intensity);
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AIAwarenessState.SelectedKeyName, static_cast<uint8>(EAIAwarenessState::Suspicion));	
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(DestinationLocation.SelectedKeyName, LoudestActor->GetActorLocation());
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(SuspiciousActor.SelectedKeyName, LoudestActor);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(HeardSomething.SelectedKeyName, true);
		return;
	}

	if (PatrolAIController->GetInvestigationPlanComponent()->HasInvestigationActionPlan())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AIAwarenessState.SelectedKeyName, static_cast<uint8>(EAIAwarenessState::Suspicion));
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(HasInvestigationPlan.SelectedKeyName, true);
	}
}

void UBTService_VariableHandler::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	OwnerCompCache = &OwnerComp;
	UE_LOG(LogTemp, Log, TEXT("UBTService_VariableHandler::OnBecomeRelevant"));
	IsPlayerNotInFOV = MakeUnique<NSBoolVariableMonitor>(GetWorld()->GetTimerManager(), 4.0f, false, true);
	IsPlayerNotInFOV->GetOnValueRemainsUnchanged().AddLambda
	([&] (bool Value)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AIAwarenessState.SelectedKeyName, static_cast<uint8>(EAIAwarenessState::Suspicion));
	});
	PreviousIsPlayerInFOV = false;
}

void UBTService_VariableHandler::PlayerRemainsHidden(bool HasRemainedHidden) const
{
	if (HasRemainedHidden)
	{
		OwnerCompCache->GetBlackboardComponent()->SetValueAsEnum(AIAwarenessState.SelectedKeyName, static_cast<uint8>(EAIAwarenessState::Suspicion));
	}
}
