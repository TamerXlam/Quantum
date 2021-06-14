
#pragma once

#include "CoreMinimal.h"



#include "State.h"
#include "GameFramework\GameStateBase.h"

#include "StateNet.generated.h"

class UStateMachineComponent;

UCLASS(ClassGroup = "TheGame|StateMachine", BlueprintType, Blueprintable)
class STATEMACHINE_API UStateNet : public UState
{
	GENERATED_BODY()

	/* private: UPROPERTY LIST */

	UPROPERTY(Replicated)
	uint8 Identifier;

	UPROPERTY(Replicated)
	UStateMachineComponent *StateMachine;

	UPROPERTY(ReplicatedUsing = OnRep_Behavior)
	EBaseStateBehavior Behavior;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentBehavior)
	EBaseStateBehavior CurrentBehavior;

	UPROPERTY(Replicated)
	bool ProcessorResult;

	UPROPERTY(Replicated)
	float StartTime;

	UPROPERTY(Replicated)
	AActor *StateOwner = nullptr;

public:
	bool operator == (const UStateNet& rhs_) const { return Identifier == rhs_.Identifier; }

public:
	virtual void BeginPlay();

	UFUNCTION(BlueprintPure)
	AActor* GetOwner() const { return Cast<AActor>(GetOuter()); };

	virtual uint8 GetIdentifierValue() const override { return Identifier; }

	/* public: UPROPERTY LIST */
	UFUNCTION()
	void OnRep_Behavior();

	UFUNCTION()
	void OnRep_CurrentBehavior();

	UFUNCTION()
	AActor *GetStateOwner() const { return StateOwner; }

	virtual void PostInitProperties() override;

public:
	void Tick(float DeltaSeconds) override;
	//virtual bool IsTickable() const override;
	//virtual TStatId GetStatId() const override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "TheGame|StateMachine")
	void EventTick(float DeltaTime);

protected:
	const UFunction *GetProcessor() const { return Processor; }



private:
	UFunction *Processor;
	/* private: FUNCTION LIST */

public:

	/* public: FUNCTION LIST */

	UStateNet(const FObjectInitializer& ObjectInitializer);

	~UStateNet();

	FORCEINLINE const UStateMachineComponent* GetStateMachine() const { return StateMachine; }

	virtual UWorld* GetWorld() const override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const override;

	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual bool CallRemoteFunction(UFunction *Function, void *Parameters, FOutParmRec *OutParms, FFrame *Stack) override;

	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;

	virtual void Init(UStateMachineComponent *SMOwner, uint8 IS, EBaseStateBehavior SB, AActor *SOwner, bool TickableAIInfo, UFunction *SProcessor) override;

	virtual bool CanBeSwitchedFrom() const;

	virtual void BeforeSwitchedFrom() override;

	virtual void JustSwitchedTo() override;

	virtual EBaseStateBehavior GetBehavior() const override { return Behavior; }
	virtual EBaseStateBehavior GetCurrentBehavior() const override { return CurrentBehavior; }

	virtual float GetTimeInState(const UObject* WorldContextObject) const override { return  GetServerWorldTimeSeconds(WorldContextObject) - StartTime; }

	static float GetServerWorldTimeSeconds(const UObject* WorldContextObject);

	virtual void Reset() override
	{

		Super::Reset();

		ProcessorResult = true;
		CurrentBehavior = Behavior;

	}

	virtual void UpdateCurrentState(float DeltaTime) override { Super::UpdateCurrentState(DeltaTime); }

	//virtual bool GetAnimDataCache(FBaseStateAnimData &Outer) const override { Outer = ADCache; return true; }

	virtual void OnQFSMSwitcherNotify() override;

};
