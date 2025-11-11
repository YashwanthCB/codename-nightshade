


#include "Character/Components/NSAIHearingProcessor.h"
#include "Perception/AISenseConfig_Hearing.h"

void UNSAIHearingProcessor::InitializeProcessing(UAIPerceptionComponent* InAIPerceptionHearingComponent)
{
	AIPerceptionHearingComponent = InAIPerceptionHearingComponent;
	auto HearingSenseConfiguration = Cast<UAISenseConfig_Hearing>(*InAIPerceptionHearingComponent->GetSensesConfigIterator());
	HearingRange = HearingSenseConfiguration->HearingRange;
	ReceiveInitializeProcessing();
}