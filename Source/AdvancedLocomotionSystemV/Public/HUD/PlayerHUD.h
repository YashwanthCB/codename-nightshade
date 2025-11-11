

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	APlayerHUD();
	static APlayerHUD* GetInstance() { return Instance; }
	FORCEINLINE void SetNightVisionHeatNormalized(float value) { NightVisionHeatNormalized = value; }
	FORCEINLINE void SetNightVisionOverheated(bool value) { bIsNightVisionOverheated = value; }
	FORCEINLINE void SetNightVisionActive(bool value) { bIsNightVisionActive = value; }

	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void BeginPlay() override;

private:
	void InteractionAvailableCallback(class IInteractable*, bool);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	float NightVisionHeatNormalized;

	UPROPERTY(BlueprintReadOnly)
	bool bIsNightVisionActive;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsNightVisionOverheated;
	
	UPROPERTY(BlueprintReadOnly)
	FText InteractionDisplayText;

	UPROPERTY(BlueprintReadOnly)
	FTransform InteractionPointTransform;
	
private:
	static APlayerHUD* Instance;
	class UInteractionComponent* InteractionComponent;
	IInteractable* InteractableActor;
	bool CanInteract;
};
