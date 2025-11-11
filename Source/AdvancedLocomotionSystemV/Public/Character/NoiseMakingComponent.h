

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "NoiseMakingComponent.generated.h"



UCLASS( ClassGroup=(Nightshade), meta=(BlueprintSpawnableComponent),Blueprintable)
class ADVANCEDLOCOMOTIONSYSTEMV_API UNoiseMakingComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNoiseMakingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
