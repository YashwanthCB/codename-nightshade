

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WallRunComponent.generated.h"


UCLASS( ClassGroup=(Nightshade), meta=(BlueprintSpawnableComponent),Blueprintable)
class ADVANCEDLOCOMOTIONSYSTEMV_API UWallRunComponent : public UActorComponent
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditDefaultsOnly,Category = "WallRun")
	bool bEnableGravity;
	UPROPERTY(EditDefaultsOnly,Category = "WallRun")
	float WallRunSpeed;
	UPROPERTY(EditDefaultsOnly,Category = "WallRun")
	float WallRunGravityScale;
	UPROPERTY(EditDefaultsOnly,Category = "WallRun")
	float WallRunJumpUpForce;
	UPROPERTY(EditDefaultsOnly,Category = "WallRun")
	float WallRunJumpSideForce;
	UPROPERTY(EditDefaultsOnly,Category = "WallRun")
	float WallRunjumpRotation;
	UPROPERTY(EditDefaultsOnly,Category = "WallRun")
	float CameraTiltAngle;
	UPROPERTY(EditDefaultsOnly,Category = "WallRun")
	float WallRunResetTime;
	UPROPERTY(BlueprintReadOnly)
	bool bWallRunning;


private:
	FVector WallRunNormal;
	float DefaultGravity;
	float DefaultAirControl;
	float TargetZRotation;
	bool bWallRunningLeft;
	bool bWallRunningRight;
	bool bWallRunSupressed;
	


private:
	UPROPERTY()
	ACharacter *OwnerCharacter;
	UPROPERTY()
	class UCharacterMovementComponent *OwnerCharacterMovementComponent;
	FTimerHandle WallRunUpdateTimerHandle;
	FTimerHandle WallRunSupressTimerHandle;
	FTimerHandle CharacterRotationHandle;
	FHitResult HitResult;

public:
	UFUNCTION(BlueprintCallable)
	void Initialize(ACharacter *Character);
	UFUNCTION(BlueprintCallable)
	void WallRunLand();
	UFUNCTION(BlueprintCallable)
	void WallRunJump();
	UFUNCTION(BlueprintCallable)
	void EndWallRun(float ResetTime);

private:
	void WallRunTick();
	bool WallRunMovement(FVector StartVector, FVector EndVector, float Direction);
	void WallRunUpdate();
	void CameraUpdate();
	void TiltCamera(float Roll);
	void ResetWallrunSupress();
	void TurnCharacter();

	TTuple<FVector,FVector> GetEndVector();
	bool IsValidWallRunVector();

};
