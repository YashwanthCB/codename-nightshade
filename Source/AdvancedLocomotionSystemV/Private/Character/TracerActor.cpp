


#include "Character/TracerActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ATracerActor::ATracerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATracerActor::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = GetParentActor();
	TraceStartLocations.Init(FVector(0,0,0),GetTraceCount()+1);
	HitResults.SetNum(GetTraceCount()+1);
	
	
	// UE_LOG(LogTemp, Warning, TEXT("%d") , TraceStartLocations.Num());
}

// Called every frame
void ATracerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TraceStartLocations.IsEmpty())
	{
		return;
	}
	SphereTrace();
}


void ATracerActor::SphereTrace()
{
	FVector XY = ((OwnerActor->GetActorForwardVector() * ForwardDirectionOffset) + OwnerActor->GetActorLocation());
	for (int index = 0; index < TraceStartLocations.Num() ; index++) 
	{
		// UE_LOG(LogTemp, Warning, TEXT("%d") , index);
		TraceStartLocations[index] = (FVector(XY.X , XY.Y , (((SphereRadius + Padding)*index+1) + OwnerActor->GetActorLocation().Z + HeightOffset)));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("\n\n"));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerActor);
	FHitResult HitResult;
	for (int index = 0; index < TraceStartLocations.Num() ; index++) 
	{
		UKismetSystemLibrary::SphereTraceSingle(GetWorld() , TraceStartLocations[index]  , (OwnerActor->GetActorForwardVector()*TraceLength) + TraceStartLocations[index] , SphereRadius , UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel5) , false , ActorsToIgnore , EDrawDebugTrace::None , HitResult , true , FColor(25,50,0,0));
		HitResults[index] = HitResult;
	}

	FilterHitresults();
}



//getting only hitresults that hits, only getting upper hit index among common actor hits
void ATracerActor::FilterHitresults()
{
	TArray<FHitResult> LocalHitResults;
	LocalHitResults.SetNum(HitResults.Num());
	int filteredindex = 0;

	//getting upper result of hit
	for (int i = 0; i < HitResults.Num(); i++)
	{
		if (HitResults[i].bBlockingHit)
		{
			LocalHitResults[filteredindex] = HitResults[i];
		}
		else
		{
			filteredindex++;
		}
	}
	filteredindex = 0;

	for (int32 index = LocalHitResults.Num() - 1; index >= 0; --index)
	{
   	 	if (!LocalHitResults[index].bBlockingHit)
  		{
  	      LocalHitResults.RemoveAtSwap(index);
  		}
	}
	
	LocalHitResults.Shrink();
	// UE_LOG(LogTemp, Warning, TEXT(" detected ledges = %d") , LocalHitResults.Num());
	// BlockingHitResults = LocalHitResults;
	DetectLedges(LocalHitResults);
	LocalHitResults.Empty();
}

//verticle line trace to hit impact location
void ATracerActor::DetectLedges(TArray<FHitResult> BlockingHits)
{
	ClosestLedgeLocations.Init(FVector(0,0,0),BlockingHits.Num());
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerActor);
	FHitResult HitResult;
	for (int index = 0; index < BlockingHits.Num() ; index++) 
	{
		FVector Start ( BlockingHits[index].ImpactPoint.X ,  BlockingHits[index].ImpactPoint.Y , ( BlockingHits[index].ImpactPoint.Z + 50));
		FVector End ((BlockingHits[index].ImpactPoint.X) ,BlockingHits[index].ImpactPoint.Y , BlockingHits[index].ImpactPoint.Z -1);
		End = End + (OwnerActor->GetActorForwardVector()*0.1);
		UKismetSystemLibrary::LineTraceSingle(GetWorld() , Start, End  , UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel5) , false , ActorsToIgnore , EDrawDebugTrace::None , HitResult , true);
		ClosestLedgeLocations[index] = HitResult.ImpactPoint;
	}
	// SortArray();
}

//sorting ClosestLedgeLocations based on distance to player character 
//unnecessary
// void ATracerActor::SortArray()
// {
// 	if (ClosestLedgeLocations.IsEmpty())
// 	{
// 		return;
// 	}

// 	FVector PL = OwnerActor->GetActorLocation();
// 	ClosestLedgeLocations.Sort([PL](const FVector& A, const FVector& B) {
//     return FVector::DistXY(A, PL) < FVector::DistXY(B, PL);
// });

	// float MinDist = UKismetMathLibrary::FFloor(FVector::DistXY(ClosestLedgeLocations[0] , PL));
	// for (int index = ClosestLedgeLocations.Num()-1; index >=0 ; --index)
	// {
	// 	float temp = UKismetMathLibrary::FFloor(FVector::DistXY(ClosestLedgeLocations[index] , PL));
	// 	// UE_LOG(LogTemp, Warning, TEXT(" mindist = %f and temp = %f index = %d \n\n") , MinDist , temp , index);
		
	// 	if (!UKismetMathLibrary::NearlyEqual_FloatFloat(MinDist , temp ,1))
	// 	{
	// 		ClosestLedgeLocations.RemoveAtSwap(index , 1 , true);
	// 		// index = ClosestLedgeLocations.Num()-1;
	// 	}
	// }
// }


int ATracerActor::GetTraceCount()
{
	int count=0;

	int LastIndex = trunc((Height/(Padding + SphereRadius)))-1;
	for (int index = 0; index < LastIndex ; index++)
	{
		count++;
	}
	return count;
}

