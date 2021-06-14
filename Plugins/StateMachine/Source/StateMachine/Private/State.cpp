
#include "State.h"
#include "StateMachineComponent.h"
#include "GameFramework\GameStateBase.h"

UState::UState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

#if WITH_EDITOR
	//SetIsTemporarilyHiddenInEditor(true);
#endif

	MimimalTimeInState = 0.f;

	NeedForTick = false;

}

UState::~UState()
{

}


void UState::Init(UStateMachineComponent* Owner, uint8 IS, EBaseStateBehavior SB, AActor *SOwner, bool TickableAIInfo, UFunction* SProcessor)
{
	if (!IsValid(Owner))
	{

	}

	TargetData.StateStartedOwnerForwardVector = GetOwnerForwardVector();
	TargetData.StateStartedOwnerUpVector = GetOwnerUpVector();
	TargetData.StateStartedOwnerRightVector = GetOwnerRightVector();
	TargetData.StateStartedOwnerToTargetVector = GetOwnerToTargetVector();

	NeedForTargetInfoTick = TickableAIInfo;

	//
}

void UState::Tick(float DeltaSeconds)
{

	if (GetNeedForTargetInfoTick())
	{

		TargetData.StateStartedOwnerForwardVector = GetOwnerForwardVector();
		TargetData.StateStartedOwnerUpVector = GetOwnerUpVector();
		TargetData.StateStartedOwnerRightVector = GetOwnerRightVector();
		TargetData.StateStartedOwnerToTargetVector = GetOwnerToTargetVector();

	}


}


void UState::OnQFSMSwitcherNotify()
{

}

void UState::BeforeSwitchedFrom()
{
	//
	NeedForTick = false;
}

void UState::JustSwitchedTo()
{
	NeedForTick = true;
	//


}

FVector UState::GetOwnerForwardVector() const
{
	return GetStateStartedOwnerForwardVector();
}

FVector UState::GetOwnerUpVector() const
{
	return GetStateStartedOwnerUpVector();
}

FVector UState::GetOwnerRightVector() const
{
	return GetStateStartedOwnerRightVector();
}

FVector UState::GetOwnerToTargetVector() const
{
	return GetStateStartedOwnerToTargetVector();
}