

#pragma once

#include "CoreMinimal.h"
#include "Character/Interfaces/Interactable.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Disturbable.h"
#include "SwitchBase.generated.h"

UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API ASwitchBase : public AActor, public IInteractable, public IDisturbable
{
	GENERATED_BODY()
	
public:	
	ASwitchBase();

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Fix(AAIController* InAIController) override;
	virtual FTransform GetDisturbedActorTransform() override;
	virtual FTransform GetIdealInteractionTransform() override;

	virtual bool CanInteract() const override;
	virtual FName GetDisplayText() const override;
	virtual FTransform GetInteractionTransform() const override;
	virtual void Interact(AActor* InInstigator) override;

	DECLARE_EVENT_OneParam(ASwitchBase, FOnEventSwitch, bool/**IsTurnedOn?*/);

	FOnEventSwitch& OnEventSwitch() {return EventSwitch;}
	
	UFUNCTION(BlueprintCallable)
	bool GetIsTurnedOn() { return IsTurnedOn; }

	FORCEINLINE bool GetIsInitiallyTurnedOn() { return IsInitiallyTurnedOn; }

protected:
	
	UFUNCTION(BlueprintImplementableEvent)
	void FixSwitch();

	UFUNCTION(BlueprintImplementableEvent, meta=(BlueprintProtected))
	void BPInteract(AActor* InInstigator);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nightshade")
	class USceneComponent* SceneComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nightshade")
	class UArrowComponent* InteractionTransformArrow;

	UPROPERTY(BlueprintReadWrite, Category = "Nightshade", meta = (BlueprintProtected))
	bool IsTurnedOn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Nightshade", meta = (BlueprintProtected))
	bool IsInitiallyTurnedOn;

	
private:
	FOnEventSwitch EventSwitch;
};
