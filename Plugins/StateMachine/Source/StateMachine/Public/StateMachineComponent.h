
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateNet.h"

#include "StateMachineComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInitStateObjectsEvent, bool, IsServer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeforeSwitchedFromEvent, uint8, StateBeforSwitched);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAfterSwitchedToEvent, uint8, StateJustSwitched);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemTakenEvent);

class UStateNet;

UCLASS(ClassGroup = "ThwGame|StateMachine", meta=(DisplayName = "State Machine Component", BlueprintSpawnableComponent) )
class STATEMACHINE_API UStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

	/* private: UPROPERTY LIST */

	//	UPROPERTY(Category = "TheGame/StateMachine", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//	TSubclassOf<UEnum> States_CM_Sub;

	UPROPERTY()
	UEnum* StatesEnum = nullptr;

	UPROPERTY(Category = "TheGame|StateMachine", BlueprintReadOnly, ReplicatedUsing = OnRep_State/*Replicated*/, VisibleAnywhere, Replicated, meta = (AllowPrivateAccess = "true"))
	TArray<class UStateNet*> States;

	UPROPERTY(Category = "TheGame|StateMachine", BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UClass* StateClassType = nullptr;

	UPROPERTY(Category = "TheGame|StateMachine", BlueprintReadOnly, VisibleAnywhere, Replicated, meta = (AllowPrivateAccess = "true"))
	uint8 LastStateIndex = uint8(-1);

	UPROPERTY(Category = "TheGame|StateMachine", BlueprintReadOnly, VisibleAnywhere, Replicated, meta = (AllowPrivateAccess = "true"))
	uint8 TargetStateIndex = uint8(-1);

	UPROPERTY(Category = "TheGame|StateMachine", BlueprintReadOnly, VisibleAnywhere, ReplicatedUsing = OnRep_CurrentStateIndex, meta = (AllowPrivateAccess = "true"))
	uint8 CurrentStateIndex = uint8(-1);

	UPROPERTY(Category = "TheGame|StateMachine", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bLogDebugInfo = false;

public:
	TArray<class UStateNet*> &GetStates() { return States; };

	UPROPERTY()
	FOnBeforeSwitchedFromEvent OnBeforeSwitchedFromEvent;

	UPROPERTY()
	FOnAfterSwitchedToEvent OnAfterSwitchedToEvent;

	UPROPERTY()
	FInitStateObjectsEvent OnInitStateObjectsEvent;

	UPROPERTY()
	FOnItemTakenEvent OnItemTakenEvent;


public:
	UFUNCTION()
	virtual void OnBeforeSwitchedFrom(uint8 StateBeforSwitched);
	UFUNCTION(Reliable, Server)
	void Server_OnBeforeSwitchedFrom(uint8 StateBeforSwitched);

	UFUNCTION()
	virtual void OnAfterSwitchedTo(uint8 StateJustSwitched);
	UFUNCTION(Reliable, Server)
	void Server_OnAfterSwitchedTo(uint8 StateJustSwitched);

protected:
	virtual bool NeedForceInitialize() const { return false; }

	UFUNCTION()
	virtual void SetCurrentState(uint8 Current);

	UFUNCTION()
	virtual void TickState(float DeltaTime);

	UFUNCTION()
	virtual uint8 GetNextState(uint8 TargetState) const;

private:

	UFUNCTION()
	void ApplyTargetState();

	UFUNCTION()
	void SetLastState(uint8 Last) { LastStateIndex = Last; }

public:

	UFUNCTION()
	void OnRep_State();

	UFUNCTION()
	uint8 GetTargetState() const { return TargetStateIndex; }

	UFUNCTION()
	uint8 GetCurrentState() const { return CurrentStateIndex; }

	UFUNCTION()
	uint8 GetLastState() const { return LastStateIndex; }

	UFUNCTION()
	void DestroyStates();

	UFUNCTION()
	UStateNet *GetCurrentStateInstance();

	UFUNCTION()
	UStateNet *GetLastStateInstance();

	UFUNCTION(Category = "TheGame|StateMachine", BlueprintNativeEvent)
	void SetTargetState_BP(uint8 StateBeforSwitched);

	UFUNCTION()
	virtual void SetTargetState(uint8 Target);

	UFUNCTION()
	UStateNet* GetTargetStateInstance() { return States[TargetStateIndex]; }

	UFUNCTION()
	void OnRep_CurrentStateIndex();

	UFUNCTION()
	virtual bool IsProcessorFinished(uint8 StateFor) const;


public:
	virtual void StartInteractive() {}
	virtual void StopInteractive() {}

private:
	/* private: VARIABLE LIST */

	const TCHAR* StateClassInfo;

	UClass *StateFlagsInfo;

public:
	/* public: VARIABLE LIST */

private:

	/* private: FUNCTION LIST */

	FString GetEnumStateNameByIndex(int32 Index) const;

public:

	/* public: FUNCTION LIST */

	UStateMachineComponent(const FObjectInitializer& ObjectInitializer);

	~UStateMachineComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void SetStatesData(const TCHAR* EStates, UClass* StateClass) { StateClassInfo = EStates;  StateFlagsInfo = StateClass; }

	void CreateState(UClass* StateClass, FName Name, EObjectFlags Flags);

	void CreateStates(const TCHAR* EStates, AActor* SMOwner, UClass* StateClass, bool SkipNotifiers = false);

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual void InitializeComponent() override;

	UFUNCTION()
	virtual bool IsInteracted() const { return false; }

	UFUNCTION()
	virtual bool HasNoState() const;

	UFUNCTION()
	virtual bool IsDied() const { return false; }

	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

	virtual void SetTargetState_BP_Implementation(uint8 StateJustSwitched) {}

	UFUNCTION()
	virtual void OnStateMachineNotify(const TArray<uint8> &StatesFrom, const uint8 NextItemState);

	UFUNCTION(Reliable, Server)
	void Server_OnStateMachineNotify(const TArray<uint8> &StatesFrom, const uint8 NextItemState);

	UFUNCTION(Reliable, NetMulticast)
	void Multicast_OnStateMachineNotify(const TArray<uint8> &StatesFrom, const uint8 NextItemState);

	const UStateNet *GetCurrentStateInstance() const;

	const UStateNet* GetTargetStateInstance() const { return States[TargetStateIndex]; }

	FORCEINLINE bool GetInitialized() const { return !!(States.Num() > 0); }

	UFUNCTION()
	void SetTargetStateIndex(uint8 Target) { TargetStateIndex = Target; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//virtual void InitStateObjects();

};
