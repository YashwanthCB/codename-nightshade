

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TracerActor.generated.h"

UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API ATracerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATracerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(EditDefaultsOnly)
	float Padding;

	UPROPERTY(EditDefaultsOnly)
	float Height;

	UPROPERTY(EditDefaultsOnly)
	float TraceLength;

	UPROPERTY(EditDefaultsOnly)
	float SphereRadius;

	UPROPERTY(EditDefaultsOnly)
	float HeightOffset;

	UPROPERTY(EditDefaultsOnly)
	float ForwardDirectionOffset;

	UPROPERTY()
	AActor *OwnerActor;


	
	TArray<FHitResult> HitResults;

	public:
	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> ClosestLedgeLocations;

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> TraceStartLocations;
	


private:
	int GetTraceCount();
	void FilterHitresults();
	// void SortArray();
	void SphereTrace();

	void DetectLedges(TArray<FHitResult> BlockingHits);

};
