


#include "TrainGameInstance.h"

void UTrainGameInstance::Init()
{
	Super::Init();
	LoadConfigValues();
	UE_LOG(LogTemp, Log, TEXT("UTrainGameInstance::Init. ProjectVersion: %s"), *ProjectVersion);

#if UE_BUILD_DEBUG
	BuildConfiguration = NSGameBuildConfiguration::Debug;
#elif UE_BUILD_DEVELOPMENT
	BuildConfiguration = NSGameBuildConfiguration::Development;
#elif UE_BUILD_SHIPPING
	BuildConfiguration = NSGameBuildConfiguration::Shipping;
#elif UE_BUILD_TEST
	BuildConfiguration = NSGameBuildConfiguration::Test;
#endif
}

float UTrainGameInstance::CalculateFalloff(float IntensityInDB, FVector SoundSourceLocation , FVector TargetActorLocation)
{
	int Distance = (FVector::Dist(SoundSourceLocation , TargetActorLocation))/100;
	return IntensityInDB - (20*(log10(Distance)));
}

void UTrainGameInstance::LoadConfigValues()
{
	GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), ProjectVersion, GGameIni);
}
