

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SecurityCamera.generated.h"

UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API ASecurityCamera : public AActor
{
	GENERATED_BODY()

public:
	ASecurityCamera();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Nightshade)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Nightshade)
	UStaticMeshComponent* CameraMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Nightshade)
	UStaticMeshComponent* AlarmMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Nightshade)
	UAudioComponent* AlarmAudioComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Nightshade)
	class UVisionSenseComponent* VisionSenseComponent;
};
