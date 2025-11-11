

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterAbilityHelperInterface.generated.h"


DECLARE_DYNAMIC_DELEGATE(FCharacterLandDelegate);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterAbilityHelperInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ADVANCEDLOCOMOTIONSYSTEMV_API ICharacterAbilityHelperInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual FCharacterLandDelegate& GetCharacterLandDelegate() = 0;

	UFUNCTION(BlueprintNativeEvent , Category = "Interface")
	FVector GetHitGrappleSurfaceImpactNormal();
	UFUNCTION()
	virtual FVector GetGrappleOffset() = 0;


	//helper functions for grapple
	UFUNCTION(BlueprintNativeEvent , Category = "Interface")
	void InitializeGrappleHook();

	UFUNCTION(BlueprintNativeEvent, Category = "Interface")
	USkeletalMeshComponent* GetCharacterSkeletalMesh();

	UFUNCTION(BlueprintNativeEvent, Category = "Interface")
	UStaticMeshComponent* GetGrappleEndMesh();

	UFUNCTION(BlueprintNativeEvent, Category = "Interface")
	void UpdateCableLength(float length);

	UFUNCTION(BlueprintNativeEvent, Category = "Interface")
	float GetCurrentCableLength();

	UFUNCTION(BlueprintNativeEvent, Category = "Interface")
	void SetCableForce(FVector CableForce);

	UFUNCTION(BlueprintNativeEvent, Category = "Interface")
	FVector GetDestinationLocaton();

	UFUNCTION(BlueprintNativeEvent, Category = "Interface")
	FVector GetGrappleEndLocation();

	UFUNCTION(BlueprintNativeEvent, Category = "Interface")
	bool GetCanHoldPlayer();

	UFUNCTION(BlueprintNativeEvent, Category = "Interface")
	void UpdateMappingContext(UInputMappingContext *NewMappingContext);

	UFUNCTION(BlueprintNativeEvent, Category = "Interface")
	void AddImpulseToRope(FVector Impulse, int32 ParticleIndex , bool UseDistributedForce , int32 UnfoldingRange);

	UFUNCTION(BlueprintNativeEvent, Category = "Interface")
	FVector GetMantleLandLocation();

	UFUNCTION(BlueprintNativeEvent , BlueprintCallable, Category = "Interface")
	bool GetIsOutOfLedgeTolerenceRange();

	UFUNCTION(BlueprintNativeEvent , BlueprintCallable, Category = "Interface")
	FVector GetFinalLedgeVector();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
	float GetMaxClimbHeight();

	UFUNCTION(BlueprintNativeEvent , BlueprintCallable, Category = "Interface")
	float GetMaxLedgeGrabHeight();

	UFUNCTION(BlueprintNativeEvent, Category = "Interface")
	void SetActiveLedge(FVector LedgeLocation);

	UFUNCTION(BlueprintNativeEvent, Category = "Interface")
	FVector GetActiveLedge();

	UFUNCTION(BlueprintNativeEvent, Category = "Interface")
	FName GetGrappleParentSocketName();

	UFUNCTION(BlueprintNativeEvent, Category = "Interface")
	FVector GetGrappleRelativeInitialLocation();

	UFUNCTION(BlueprintNativeEvent, Category = "Interface")
	const UCameraComponent* GetCameraComponent();

	UFUNCTION(BlueprintNativeEvent, Category = "Interface")
	void SetCanUncrouch(bool CanUncrouch);

};
