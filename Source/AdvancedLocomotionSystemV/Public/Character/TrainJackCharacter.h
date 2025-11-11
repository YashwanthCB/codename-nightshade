

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/CharacterAbilityHelperInterface.h"
#include "Interfaces/VisibleToAI.h"
#include "TrainGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TrainJackCharacter.generated.h"

ADVANCEDLOCOMOTIONSYSTEMV_API DECLARE_LOG_CATEGORY_EXTERN(LogTrainJackerCharacter, Log, All);


class UCameraComponent;

UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API ATrainJackCharacter : public ACharacter, public ICharacterAbilityHelperInterface, public IVisibleToAI
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FCharacterLandDelegate OnCharacterLand;

private:
	UPROPERTY()
	const UCameraComponent* CameraComponent;


public:
	// Sets default values for this character's properties
	ATrainJackCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Landed(const FHitResult& Hit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = NightShade)
	class ULightInfluenceComponent* GetLightInfluenceComponent();


public:
	FCharacterLandDelegate& GetCharacterLandDelegate() override;
	FVector GetGrappleOffset() override;


	// grapple variables
private:
	UPROPERTY(EditDefaultsOnly , Category = "Grapple" , meta = (AllowPrivateAccess = "true" , ToolTip = "forward distance of trace from the player"))
	float GrappleDetectionRange = 1000.0f;
	UPROPERTY(EditDefaultsOnly , Category = "Grapple" , meta = (AllowPrivateAccess = "true" , ToolTip = "max radius of grapple point detection algorithm"))
	float GrappleTraceRadius = 60.0f;
	UPROPERTY(EditDefaultsOnly , Category = "Grapple" , meta = (AllowPrivateAccess = "true" , ToolTip = "offset of final grapple point. this can be used to move grapple point in x,y,z position.if we want to display grapple bit up then we would add z"))
	FVector GrappleOffset;
	UPROPERTY(EditDefaultsOnly , Category = "Grapple" , meta = (AllowPrivateAccess = "true" , ToolTip = " max slope a detect point is allowed any greater value will be ignored"))
	float GrapplePointTolerationAngle;
	UPROPERTY(EditDefaultsOnly , Category = "Grapple" , meta = (AllowPrivateAccess = "true" , ToolTip = "refers to verticle sphere trace for detecting edges (previously it was line trace).this is used to define how up we want the starting of trace."))
	float VerticleSphereTraceZOffset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grapple" , meta = (AllowPrivateAccess = "true" ,ToolTip = "Wheather should player float for n second after reaching grapple point"))
	bool bCanHoldPlayer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grapple" , meta = (AllowPrivateAccess = "true" ,ToolTip = "Wheather should player float for n second after reaching grapple point"))
	float GroundCheckOffset;
	float GroundCheckDownTraceLength=150;


	UPROPERTY(BlueprintReadOnly , Category = "Grapple" , meta = (AllowPrivateAccess = "true"))
	FVector HitSurfaceNormal;
	UPROPERTY(BlueprintReadWrite , Category = "Grapple" , meta = (AllowPrivateAccess = "true"))
	FVector GrappleHitSurfaceNormal;
	UPROPERTY(BlueprintReadOnly , Category = "Grapple" , meta = (AllowPrivateAccess = "true"))
	FHitResult VerticleSphereHitResult;
	UPROPERTY(BlueprintReadWrite , meta = (AllowPrivateAccess = "true"))
	bool CanUnCrouch=true;



	UPROPERTY(BlueprintReadOnly , Category = "Grapple" , meta = (AllowPrivateAccess = "true"))
	FVector GrappleVector;
	// UPROPERTY(BlueprintReadOnly , Category = "Grapple" , meta = (AllowPrivateAccess = "true"))
	// FHitResult HorizontalSideCheckHitResult;
	// UPROPERTY(BlueprintReadOnly , Category = "Grapple" , meta = (AllowPrivateAccess = "true"))
	// FHitResult VerticleSphereHitResult;

private:
	UPROPERTY(EditDefaultsOnly , Category = "Debug" , meta = (AllowPrivateAccess = "true"))
	UDataTable *DebugDataTable;
	

	bool bShouldDrawDebug=false;
	TEnumAsByte<EDrawDebugTrace::Type> TraceDebug;

	UPROPERTY(BlueprintReadOnly , Category = "Grapple" , meta = (AllowPrivateAccess = "true"))
	FHitResult HorizontalSideCheckHitResult;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AGrapplePointHighlight> GrappleHighlightClass;
	UPROPERTY()
	class AGrapplePointHighlight *GrappleHighlightActor;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LightExposure;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInteractionComponent* InteractionComponent;


	// grapple functions
private:
	void CalculateGrappleVector(FHitResult SphereTraceHitResult);
	bool CheckAngle(FVector ImpactNormal);
	float GetTransitionPointHitResult(FHitResult& OutHitResult);
	FHitResult IsHitByWall(float InRadius);
	void SetGrappleHighlightVisibility(bool NewVisibility);

private:
	class ULightInfluenceComponent* LightInfluenceComponent;
};
