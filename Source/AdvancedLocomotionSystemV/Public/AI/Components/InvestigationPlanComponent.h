

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Controllers/AI/PatrolGuardAIController.h"
#include "Interfaces/Disturbable.h"
#include "InvestigationPlanComponent.generated.h"

UENUM(BlueprintType)
enum class EInvestigationActionType : uint8
{
	None UMETA(DisplayName = "None"),
	GotoLocation UMETA(DisplayName = "Goto Location"),
	Interact UMETA(DisplayName = "Interact"),
	Chase UMETA(DisplayName = "Chase")
};

USTRUCT(BlueprintType)
struct FInvestigationAction // check if we can use linked list for this
{
	GENERATED_BODY()
	FInvestigationAction() : InvestigationActionType(EInvestigationActionType::None), Location(FVector::ZeroVector), DisturbableActor(nullptr) { }

	FInvestigationAction(EInvestigationActionType InInvestigationActionType, FVector InLocation = FVector::Zero(), TScriptInterface<class IDisturbable> InDisturbableActor = nullptr)
		: InvestigationActionType(InInvestigationActionType), Location(InLocation), DisturbableActor(InDisturbableActor)
		{	}
	
	UPROPERTY(BlueprintReadWrite, Category = "AI | Investigation")
	EInvestigationActionType InvestigationActionType;
	
	UPROPERTY(BlueprintReadWrite, Category = "AI | Investigation")
	FVector Location;

	UPROPERTY(BlueprintReadWrite, Category = "AI | Investigation")
	TScriptInterface<class IDisturbable> DisturbableActor;
};

UCLASS(BlueprintType)
class ADVANCEDLOCOMOTIONSYSTEMV_API UInvestigationActionContainer : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI | Investigation")
	FInvestigationAction InvestigationAction;

	UInvestigationActionContainer() : Super()
	{
		UE_LOG(LogTemp, Log, TEXT("UInvestigationActionContainer. constructor called"));
	}
	
	virtual ~UInvestigationActionContainer() override
	{
		UE_LOG(LogTemp, Log, TEXT("~UInvestigationActionContainer. destructor called"));
	}
};

UCLASS( ClassGroup=(Nightshade), meta=(BlueprintSpawnableComponent) )
class ADVANCEDLOCOMOTIONSYSTEMV_API UInvestigationPlanComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInvestigationPlanComponent();
	
protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:	
	TArray<FInvestigationAction> GetInvestigationActionPlans();

public:
	UFUNCTION(BlueprintCallable)
	virtual FInvestigationAction ConsumeInvestigationPlan();

	UFUNCTION(BlueprintCallable)
	virtual FInvestigationAction PeekInvestigationPlan();
	
	
	UFUNCTION(BlueprintCallable)
	virtual FORCEINLINE bool HasInvestigationActionPlan() const { UE_LOG(LogTemp, Log, TEXT("InvestigationActionPlans.Num %d"), InvestigationActionPlans.Num()); return InvestigationActionPlans.Num() > 0; }
	FORCEINLINE void SetVisionSenseComponent(class UVisionSenseComponent* InVisionSenseComponent) { this->VisionSenseComponent = InVisionSenseComponent; }
	
private:
	FORCEINLINE void  OnVisibleActorSpawnedOrDestroyedRaw(AActor* InActor, bool IsSpawned);
	FORCEINLINE bool UpdateDisturbanceActorInList(class ASimpleDisturbanceIndicator* InDisturbanceIndicator,
	                                              bool IsSpawned);
	
protected:
	virtual void ConsolidateInvestigationActionPlan(TArray<FInvestigationAction> InUnprocessedInvestigationActions);
	
private:
	FORCEINLINE static bool IsOwnedByDisturbable(const AActor* InActor)
	{
		if (InActor->GetOwner()->Implements<class UDisturbable>())
			return true;
		return false;
	}
	FORCEINLINE bool IsOwnedByOtherAIs(const AActor* InActor) const
	{
		if (InActor->GetOwner()->IsA<APatrolGuardAIController>() && InActor->GetOwner() != GetOwner())
			return true;
		return false;
	}
	FORCEINLINE bool TryOwning(class ASimpleDisturbanceIndicator*& InSimpleDisturbanceIndicator) const;

private:
	UPROPERTY()
	class UVisionSenseComponent* VisionSenseComponent; // <- risk of stale pointer
	TArray<ASimpleDisturbanceIndicator*> DisturbanceIndicatorList;
	TArray<FInvestigationAction> InvestigationActionPlans;
};
