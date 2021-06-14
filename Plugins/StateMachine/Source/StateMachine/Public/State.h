
#pragma once

#include "CoreMinimal.h"

#include "UObject/Object.h"

#include "BaseGraph.h"

#include "State.generated.h"

class UStateMachineComponent;

UENUM(BlueprintType)
enum class UBaseStateType : uint8
{
	/*0*/	MNoState	UMETA(DisplayName = "No State"),

	MLast				UMETA(DisplayName = "Last"),
};

UENUM(BlueprintType, Blueprintable)
enum class EBaseStateBehavior : uint8
{
	MNone				UMETA(Hidden),

	MFinished			UMETA(Hidden),

	FreeState			UMETA(DisplayName = "FreeState"),
	StoppedByNotify		UMETA(DisplayName = "StoppedByNotify"),
};

USTRUCT(BlueprintType, Blueprintable)
struct FStateOwnerTargetInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FVector StateStartedOwnerForwardVector = FVector::ZeroVector;

	UPROPERTY()
	FVector StateStartedOwnerUpVector = FVector::ZeroVector;

	UPROPERTY()
	FVector StateStartedOwnerRightVector = FVector::ZeroVector;

	UPROPERTY()
	FVector StateStartedOwnerToTargetVector = FVector::ZeroVector;

};

UCLASS(ClassGroup = "TheGame|StateMachine", BlueprintType, Blueprintable)
class STATEMACHINE_API UState : public UObject
{
	GENERATED_BODY()

	/* private: UPROPERTY LIST */

	UPROPERTY()
	float MimimalTimeInState;

	UPROPERTY()
	FStateOwnerTargetInfo TargetData;

	UPROPERTY()
	bool NeedForTick;


private:

	UPROPERTY()
	bool NeedForTargetInfoTick = false;

public:

	/* public: UFUNCTION LIST */

	UFUNCTION()
	virtual uint8 GetIdentifierValue() const { return uint8(-1); }

	UFUNCTION()
	virtual bool CanBeSwitchedFrom() const { return true; }

	UFUNCTION()
	virtual void OnQFSMSwitcherNotify();

	UFUNCTION()
	virtual uint8 GetNextState(uint8 TargetState) const { return TargetState; };

	UFUNCTION()
	virtual void BeforeSwitchedFrom();

	UFUNCTION()
	virtual void JustSwitchedTo();

public:
	/* public: FUNCTION LIST */

	UState(const FObjectInitializer& ObjectInitializer);

	~UState();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override { Super::GetLifetimeReplicatedProps(OutLifetimeProps); }

	virtual bool IsSupportedForNetworking() const override { return true; }

	UFUNCTION()
	virtual void Init(UStateMachineComponent *Owner, uint8 IS, EBaseStateBehavior SB, AActor *SOwner, bool TickableAIInfo, UFunction *SProcessor);

	UFUNCTION()
	virtual EBaseStateBehavior GetBehavior() const { return EBaseStateBehavior::FreeState; }

	UFUNCTION()
	virtual EBaseStateBehavior GetCurrentBehavior() const { return EBaseStateBehavior::FreeState; }

	UFUNCTION()
	virtual float GetTimeInState(const UObject* WorldContextObject) const { return 0.f; }

	virtual void Reset() { NeedForTick = false; }
	
	virtual void Tick(float DeltaSeconds);

	virtual void UpdateCurrentState(float DeltaTime) { }


	FORCEINLINE FVector GetStateStartedOwnerForwardVector() const { return TargetData.StateStartedOwnerForwardVector; }

	FORCEINLINE FVector GetStateStartedOwnerUpVector() const { return TargetData.StateStartedOwnerUpVector; }

	FORCEINLINE FVector GetStateStartedOwnerRightVector() const { return TargetData.StateStartedOwnerRightVector; }

	FORCEINLINE FVector GetStateStartedOwnerToTargetVector() const { return TargetData.StateStartedOwnerToTargetVector; }

	FORCEINLINE bool GetNeedForTargetInfoTick() const { return NeedForTargetInfoTick; }

protected:
	virtual FVector GetOwnerForwardVector() const;

	virtual FVector GetOwnerUpVector() const;

	virtual FVector GetOwnerRightVector() const;

	virtual FVector GetOwnerToTargetVector() const;
	
};
