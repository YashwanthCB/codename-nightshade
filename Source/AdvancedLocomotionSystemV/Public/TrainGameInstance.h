

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TrainGameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnValueRemainsUnchanged, bool);


//TRICKS -> Blueprint logic in C++
#define DO_N_TIMES(N, X){ static unsigned int call_count = N; if(call_count > 0){ X; --call_count; } }

#define DO_ONCE(X) DO_N_TIMES(1, X)

#define DO_TWICE(X) DO_N_TIMES(2, X)
// END OF TRICKS


template<typename T>
class NSVariableMonitor
{
public:
	NSVariableMonitor(FTimerManager& InTimerManager, float InTimerDelay, T InCurrentValue, T InTriggerValue)
		: TimerManager(InTimerManager), TimerDelay(InTimerDelay), CurrentValue(InCurrentValue), TriggerValue(InTriggerValue)
	{
	}
	~NSVariableMonitor()
	{
		TimerManager.ClearTimer(TimerHandle);
	}
	FORCEINLINE void SetValue(T InNewVal);
	FORCEINLINE T GetValue() { return CurrentValue; }
	FORCEINLINE FOnValueRemainsUnchanged& GetOnValueRemainsUnchanged() { return OnValueRemainsUnchanged; }
	FORCEINLINE bool GetHasRemainedUnchanged();

private:
	FTimerManager& TimerManager;
	FTimerHandle TimerHandle;
	T TriggerValue;
	T CurrentValue;
	float TimerDelay;
	bool HasRemainedUnchanged;
	FOnValueRemainsUnchanged OnValueRemainsUnchanged;
};

typedef NSVariableMonitor<bool> NSBoolVariableMonitor;

UENUM(BlueprintType)
enum class NSGameBuildConfiguration : uint8
{
	Debug UMETA(DisplayName = "debug"),
	Development UMETA(DisplayName = "dev"),
	Shipping UMETA(DisplayName = ""),
	Test UMETA(DisplayName = "test")
};

DECLARE_MULTICAST_DELEGATE(FOnPlayerBeginPlayCalled);

UCLASS()
class ADVANCEDLOCOMOTIONSYSTEMV_API UTrainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable)
	float CalculateFalloff(float IntensityInDB, FVector SoundSourceLocation , FVector TargetActorLocation);

	void LoadConfigValues();

	FOnPlayerBeginPlayCalled OnPlayerBeginPlayCalled;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	FString ProjectVersion;
	
	UPROPERTY(BlueprintReadOnly)
	NSGameBuildConfiguration BuildConfiguration;
};

template <typename T>
void NSVariableMonitor<T>::SetValue(T InNewVal)
{
	CurrentValue = InNewVal;
	if (CurrentValue != TriggerValue && !TimerManager.IsTimerActive(TimerHandle))
	{
		//UE_LOG(LogTemp, Log, TEXT("NSVariableMonitor<T>::SetValue. The timer is set"));
		TimerManager.SetTimer(
			TimerHandle, [&] ()
			{
				HasRemainedUnchanged = true;
				OnValueRemainsUnchanged.Broadcast(true);
				UE_LOG(LogTemp, Log, TEXT("NSVariableMonitor<T>::SetValue. The value remains unchanged"));
			}
			, this->TimerDelay, false);
	}
	if (CurrentValue == TriggerValue)
	{
		if (TimerManager.IsTimerActive(TimerHandle))
		{
			HasRemainedUnchanged = false;
			OnValueRemainsUnchanged.Broadcast(false);
			UE_LOG(LogTemp, Error, TEXT("NSVariableMonitor<T>::SetValue. The value is changed.."));
			TimerManager.ClearTimer(TimerHandle);
		}
		else
		{
			//UE_LOG(LogTemp, Error, TEXT("NSVariableMonitor<T>::SetValue. The value is changed. But timer is not set"));			
		}
	}
}

template <typename T>
bool NSVariableMonitor<T>::GetHasRemainedUnchanged()
{
	return HasRemainedUnchanged;
}
