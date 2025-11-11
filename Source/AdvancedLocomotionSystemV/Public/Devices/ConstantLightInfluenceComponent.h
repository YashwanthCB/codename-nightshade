

#pragma once

#include "CoreMinimal.h"
#include "Devices/LightInfluenceComponent.h"
#include "ConstantLightInfluenceComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Nightshade), meta=(BlueprintSpawnableComponent),Blueprintable)
class ADVANCEDLOCOMOTIONSYSTEMV_API UConstantLightInfluenceComponent : public ULightInfluenceComponent
{
	GENERATED_BODY()
public:
	virtual float GetLightExposure() override;

	virtual void RegisterLightSource(ALightBase* InLightBase) override;

	virtual void UnregisterLightSource(ALightBase* InLightBase) override;
};
