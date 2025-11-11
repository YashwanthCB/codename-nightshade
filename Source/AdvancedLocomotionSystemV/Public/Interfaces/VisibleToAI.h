

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VisibleToAI.generated.h"

class ULightInfluenceComponent;

UINTERFACE(MinimalAPI, Blueprintable)
class UVisibleToAI : public UInterface
{
	GENERATED_BODY()
};

class ADVANCEDLOCOMOTIONSYSTEMV_API IVisibleToAI
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = NightShade)
	ULightInfluenceComponent* GetLightInfluenceComponent();
};
