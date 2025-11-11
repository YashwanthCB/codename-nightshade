


#include "Controllers/AI/PatrolGuardAIController.h"
#include "AI/SimpleDisturbanceIndicator.h"
#include "Character/VisionSenseComponent.h"


APatrolGuardAIController::APatrolGuardAIController() : VisibilityPercentageThreshold(35.0f), ReadyToConsume(true)
{
    PrimaryActorTick.bCanEverTick = true;
    InvestigationPlanComponent = CreateDefaultSubobject<UInvestigationPlanComponent>("InvestigationPlanComponent");
}

void APatrolGuardAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    UVisionSenseComponent* VisionSenseComponent = InPawn->GetComponentByClass<UVisionSenseComponent>();
    //checkf(!IsValid(VisionSenseComponent), TEXT("Can't find UVisionSenseComponent. Pawn: %s(%p). VisionSenseComponent: %s(%p)")
    //    , *InPawn->GetActorNameOrLabel(), &InPawn, *VisionSenseComponent->GetName(), &VisionSenseComponent);
    // ^^ sometimes becomes invalid. find why. it is becoming critical. find out why
    
    InvestigationPlanComponent->SetVisionSenseComponent(VisionSenseComponent);
    HearingProcessor = GetComponentByClass<class UNSAIHearingProcessor>();
}

class UNSAIHearingProcessor* APatrolGuardAIController::GetHearingProcessor() const
{
    return HearingProcessor;
}
