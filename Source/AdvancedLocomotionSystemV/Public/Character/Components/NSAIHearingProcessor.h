

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "NSAIHearingProcessor.generated.h"

// TODO: migrate the derived class to C++
UCLASS( ClassGroup=(Nightshade), meta=(BlueprintSpawnableComponent),Blueprintable)
class ADVANCEDLOCOMOTIONSYSTEMV_API UNSAIHearingProcessor : public UActorComponent
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void InitializeProcessing(UAIPerceptionComponent* InAIPerceptionHearingComponent);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "Initialize Processing Copmponent"))
	void ReceiveInitializeProcessing();
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Nightshade | AI")
	bool GetLoudestSoundingActor(AActor*& OutActor, float& Sound, FName& OutTag);

protected:
	UPROPERTY(BlueprintReadOnly)
	UAIPerceptionComponent* AIPerceptionHearingComponent;

	UPROPERTY(BlueprintReadOnly)
	float HearingRange;
};
