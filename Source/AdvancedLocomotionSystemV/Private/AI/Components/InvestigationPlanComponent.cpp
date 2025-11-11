


#include "AI/Components/InvestigationPlanComponent.h"
#include "AI/SimpleDisturbanceIndicator.h"
#include "Character/VisionSenseComponent.h"
#include "Devices/LightInfluenceComponent.h"

UInvestigationPlanComponent::UInvestigationPlanComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UInvestigationPlanComponent::InitializeComponent()
{
	Super::InitializeComponent();
	ULightInfluenceComponent::OnVisibleActorSpawnedOrDestroyed.AddUObject(this, &UInvestigationPlanComponent::OnVisibleActorSpawnedOrDestroyedRaw);
}

TArray<FInvestigationAction> UInvestigationPlanComponent::GetInvestigationActionPlans()
{
	return InvestigationActionPlans;
}

FInvestigationAction UInvestigationPlanComponent::ConsumeInvestigationPlan()
{
	if (!HasInvestigationActionPlan())
		return FInvestigationAction();

	FInvestigationAction FirstInvestigationAction = InvestigationActionPlans[0];
	InvestigationActionPlans.RemoveAt(0);
	return FirstInvestigationAction;
}

FInvestigationAction UInvestigationPlanComponent::PeekInvestigationPlan()
{
	if (!HasInvestigationActionPlan())
		return FInvestigationAction();
	return InvestigationActionPlans[0];
}

void UInvestigationPlanComponent::OnVisibleActorSpawnedOrDestroyedRaw(AActor* InActor, bool IsSpawned)
{
	auto DisturbanceIndicator = Cast<ASimpleDisturbanceIndicator>(InActor);
	if (!IsValid(DisturbanceIndicator)) // <- implicitly rejects Player Character
		return;
	
	UpdateDisturbanceActorInList(DisturbanceIndicator, IsSpawned);	
}

bool UInvestigationPlanComponent::UpdateDisturbanceActorInList(ASimpleDisturbanceIndicator* InDisturbanceIndicator,bool IsSpawned)
{
	if(!IsSpawned && DisturbanceIndicatorList.Contains(InDisturbanceIndicator))
	{
		DisturbanceIndicatorList.Remove(InDisturbanceIndicator);
		return true;
	}
	return false;
}

void UInvestigationPlanComponent::ConsolidateInvestigationActionPlan(TArray<FInvestigationAction> InUnprocessedInvestigationActions)
{
	InvestigationActionPlans.Append(InUnprocessedInvestigationActions);
}

void UInvestigationPlanComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInvestigationPlanComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// get the visibility map. iterate through all the actors. find indicators with > 30% visibility
	// check if it is not owned by another AI controller. -> if so, continue.
	// add them to the list indicatorlist and change the owner.
	// if destroyed -> delete that indicator.
	// get the action plans
	auto VisibilityMap = VisionSenseComponent->GetVisibilityMap();

	TArray<FInvestigationAction> InvestigationActions;

	bool NewActionsAvailable = false;
	
	for (auto VisibilityMapPair : VisibilityMap)
	{
		auto Indicator = Cast<ASimpleDisturbanceIndicator>(VisibilityMapPair.Key);
		if (!IsValid(Indicator)) // To avoid other types of actors from being detected as indicators
			continue;
		auto Visibility = VisibilityMapPair.Value;
		if (Visibility > 30.0f)
		{
			bool OwnedSuccessfully = TryOwning(Indicator); // If already owned by another AI, then current AI should not own it
			if (!OwnedSuccessfully)
				continue;
			NewActionsAvailable = true;
			DisturbanceIndicatorList.Add(Indicator);
			InvestigationActions.Append(Indicator->GetActionItem());
		}
	}
	if (NewActionsAvailable)
	{
		ConsolidateInvestigationActionPlan(InvestigationActions);
		for (auto InvestigationAction : InvestigationActionPlans)
		{
			UE_LOG(LogTemp, Log, TEXT("ACTIONPLAN_SEQUENCE. UInvestigationPlanComponent::TickComponent. %d"), InvestigationAction.InvestigationActionType);
		}
	}
}


bool UInvestigationPlanComponent::TryOwning(ASimpleDisturbanceIndicator*& InSimpleDisturbanceIndicator) const
{
	IDisturbable* OwningDisturbableActor = nullptr;
	if (!IsOwnedByOtherAIs(InSimpleDisturbanceIndicator) && IsOwnedByDisturbable(InSimpleDisturbanceIndicator))
	{
		OwningDisturbableActor = Cast<IDisturbable>(InSimpleDisturbanceIndicator->GetOwner());
		//checkf(!OwningDisturbableActor, TEXT("UInvestigationPlanComponent::TryChangeOwner DisturbanceIndicator. OwningDisturbableActor(%d) is null"), &OwningDisturbableActor);

		InSimpleDisturbanceIndicator->SetOwner(GetOwner());
		return true;
	}
	return false;
}
