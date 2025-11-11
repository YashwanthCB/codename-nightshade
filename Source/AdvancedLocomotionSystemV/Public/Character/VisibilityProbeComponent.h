

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "VisibilityProbeComponent.generated.h"


UCLASS( ClassGroup=(Nightshade), meta=(BlueprintSpawnableComponent),Blueprintable)
class ADVANCEDLOCOMOTIONSYSTEMV_API UVisibilityProbeComponent : public UArrowComponent
{
	GENERATED_BODY()

public:	
	UVisibilityProbeComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
