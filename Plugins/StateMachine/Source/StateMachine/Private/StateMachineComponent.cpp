// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachineComponent.h"
#include "StateNet.h"

#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/ActorChannel.h"

#define GET_STRING_SERVER_OR_CLIENT ((GetWorld()&&GetWorld()->IsServer())?TEXT("Server"):TEXT("Client"))
#define GET_FSM_CLASS_NAME (GetClass()->GetFName().ToString().CStr())
#define GET_FSM_OWNER_NAME (IsValid(GetOwner())?GetOwner()->GetName().CStr():TEXT(""))

#define LOG_FSM(LogText, ...) UE_LOG(LogTemp, Display, TEXT("[%s] [%s] %s"##LogText), GET_STRING_SERVER_OR_CLIENT, GET_FSM_OWNER_NAME, GET_FSM_CLASS_NAME, __VA_ARGS__)


void UStateMachineComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStateMachineComponent, States);
	DOREPLIFETIME(UStateMachineComponent, TargetStateIndex);
	DOREPLIFETIME(UStateMachineComponent, CurrentStateIndex);
	DOREPLIFETIME(UStateMachineComponent, LastStateIndex);

}

UStateMachineComponent::UStateMachineComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(true);

	bAutoActivate = true;

	SetIsReplicatedByDefault(true);

	bWantsInitializeComponent = true;

}

UStateMachineComponent::~UStateMachineComponent()
{

	DestroyStates();
	//
}

void UStateMachineComponent::InitializeComponent()
{

	UWorld *World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	Super::InitializeComponent();

	if (NeedForceInitialize())
	{
		CurrentStateIndex = uint8(0);
		TargetStateIndex = uint8(0);
		LastStateIndex = uint8(0);
	}

	bool IsDedicatedServer = IsNetMode(NM_DedicatedServer);
	UNetDriver* NetDriver = GetWorld()->GetNetDriver();
	//
	OnInitStateObjectsEvent.Broadcast(!IsDedicatedServer || (NetDriver && NetDriver->IsServer()));
	//

}

bool UStateMachineComponent::ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags)
{

	bool Result = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (int i = 0; i != States.Num(); i++)
	{

		class UState* Current = States[i];
		if (!Current)
		{
			continue;
		}

		Result |= Channel->ReplicateSubobject(Current, *Bunch, *RepFlags);

	}

	return Result;

}

void UStateMachineComponent::BeginPlay()
{

	Super::BeginPlay();

	// ...
	
}

void UStateMachineComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);

}

bool UStateMachineComponent::IsProcessorFinished(uint8 StateFor) const
{

	UBaseStateType Identifier = (UBaseStateType)StateFor;
	switch (Identifier)
	{
	case UBaseStateType::MNoState:
	{

	}
	break;

	default:
		break;
	}


	return true;
}

void UStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TickState(DeltaTime);
	// ...

}


void UStateMachineComponent::TickState(float DeltaTime)
{

	bool IsDedicatedServer = IsNetMode(NM_DedicatedServer);
	UNetDriver* NetDriver = GetWorld()->GetNetDriver();
	//
	if (!IsDedicatedServer || (NetDriver && NetDriver->IsServer()))
	{

		UStateNet *CurrentPointer = GetCurrentStateInstance();
		if (IsValid(CurrentPointer))
		{
			CurrentPointer->Tick(DeltaTime);
		}

		ApplyTargetState();
	}

}

void UStateMachineComponent::CreateState(UClass* StateClass, FName Name, EObjectFlags Flags)
{

	States.Add(NewObject<UStateNet>(this, StateClass));

	if (!IsValid(States.Last()) && bLogDebugInfo)
	{
		//LOG_FSM("::CreateState() - NewState == NULL!");
	}

}

void UStateMachineComponent::CreateStates(const TCHAR* EStates, AActor *SMOwner, UClass* StateClass, bool SkipNotifiers)
{

	StatesEnum = FindObject<UEnum>(ANY_PACKAGE, EStates, true);
	if (!IsValid(StatesEnum))
	{
		if (bLogDebugInfo)
		{
			///LOG_FSM("::CreateStates() - EStates == NULL!");
		}

		return;
	}

	if (!IsValid(SMOwner))
	{
		if (bLogDebugInfo)
		{
			//LOG_FSM("::CreateStates() - SMOwner == NULL!");
		}

		return;
	}

	UWorld* World = SMOwner->GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	StateClassType = StateClass;

	int32 ESize = (StatesEnum->GetMaxEnumValue());

	States.Empty();

	for (int32 i = 0; i != ESize; i++)
	{
		int32 Value = StatesEnum->GetValueByIndex(i);

		CreateState(StateClass, StateClass->GetFName(), RF_NoFlags);

		EBaseStateBehavior SBehavior = EBaseStateBehavior::FreeState;

		if (!SkipNotifiers)
		{
			
			bool StoppedByNotify = GetEnumStateNameByIndex(i).Contains("_NOTIFIER");
			if (StoppedByNotify)
			{
				SBehavior = EBaseStateBehavior::StoppedByNotify;
			}

		}

		bool AIInfoLocker = GetEnumStateNameByIndex(i).Contains("_LOCKAIINFO");

		UFunction* Fn = FindFunction(FName("IsProcessorFinished"));

		States.Last()->Init(this, (uint8)Value, SBehavior, SMOwner, !AIInfoLocker, Fn);
	}

}

void UStateMachineComponent::DestroyStates()
{

	States.Empty();
	StateClassType = nullptr;
	StatesEnum = nullptr;

}

UStateNet *UStateMachineComponent::GetLastStateInstance()
{

	bool IsValidIndex = States.IsValidIndex(LastStateIndex);

	if (!IsValidIndex)
	{
		return nullptr;
	}

	return States[LastStateIndex];

}

void UStateMachineComponent::SetTargetState(uint8 Target)
{

	bool IsDedicatedServer = IsNetMode(NM_DedicatedServer);
	UNetDriver* NetDriver = GetWorld()->GetNetDriver();
	//
	
	if (!IsDedicatedServer || (NetDriver && NetDriver->IsServer()))
	{
		TargetStateIndex = Target;
		ApplyTargetState();
	}

	if (bLogDebugInfo)
	{
		//LOG_FSM("::SetTargetState( %s )", *GetEnumStateNameByIndex(Target));
	}

}

void UStateMachineComponent::OnRep_CurrentStateIndex()
{

	UE_LOG(LogTemp, Display, TEXT("[%s] UStateMachineComponent::OnRep_CurrentStateIndex/ CUrrent = [%d]; Last = [%d]"), *GetOwner()->GetName(), GetCurrentState(), GetLastState());

	UWorld* World = GetWorld();
	if (IsValid(World) && World->IsServer())
	{
		return;
	}

	if (GetLastState() == GetCurrentState())
	{
		return;
	}


	OnBeforeSwitchedFrom(LastStateIndex);

	SetLastState(CurrentStateIndex);

	OnAfterSwitchedTo(CurrentStateIndex);

}

FString UStateMachineComponent::GetEnumStateNameByIndex(int32 Index) const
{
	if (IsValid(StatesEnum))
	{
		FString EnumNameString = StatesEnum->GetNameStringByIndex(Index);

		if (!EnumNameString.IsEmpty())
		{
			return EnumNameString;
		}
	}

	return FString::FromInt(Index);
}

void UStateMachineComponent::OnRep_State()
{

	//UE_LOG(LogTemp, Display, TEXT("[%s] UStateMachineComponent::OnRep_State "), *GetOwner()->GetName());
	//OnStateMachineNotify(StatesFrom);
}

UStateNet *UStateMachineComponent::GetCurrentStateInstance()
{
	bool IsValidIndex = States.IsValidIndex(CurrentStateIndex);

	if (!IsValidIndex)
	{
		return nullptr;
	}

	return States[CurrentStateIndex];
}

const UStateNet *UStateMachineComponent::GetCurrentStateInstance() const
{
	bool IsValidIndex = States.IsValidIndex(CurrentStateIndex);

	if (!IsValidIndex)
	{
		return nullptr;
	}

	return States[CurrentStateIndex];
}

void UStateMachineComponent::SetCurrentState(uint8 Target)
{

	UWorld* World = GetWorld();

	if (IsValid(World) && World->IsServer())
	{
		UStateNet* OldCurrent = GetCurrentStateInstance();
		if (IsValid(OldCurrent))
		{
			OldCurrent->BeforeSwitchedFrom();
		}

		CurrentStateIndex = Target;

		UStateNet* NewCurrent = GetCurrentStateInstance();

		if (IsValid(NewCurrent))
		{
			NewCurrent->JustSwitchedTo();
		}
	}

	if (bLogDebugInfo)
	{
		//LOG_FSM("::SetCurrentState( %s )", *GetEnumStateNameByIndex(Target));
	}

}

void UStateMachineComponent::OnStateMachineNotify(const TArray<uint8> &StatesFrom, const uint8 NextItemState)
{


	bool IsDedicatedServer = IsNetMode(NM_DedicatedServer);
	UNetDriver* NetDriver = GetWorld()->GetNetDriver();
	//

	if (!IsDedicatedServer || (NetDriver && NetDriver->IsServer()))
	{

		UStateNet *CState = GetCurrentStateInstance();
		if (!IsValid(CState))
		{
			return;
		}

		bool CanStopCurrent = false;
		for (uint8 i = 0; i != StatesFrom.Num(); i++)
		{
			if (StatesFrom[i] != CState->GetIdentifierValue())
			{
				continue;
			}

			CanStopCurrent = true;
			break;
		}

		if (!CanStopCurrent)
		{
			return;
		}

		CState->OnQFSMSwitcherNotify();

		if (bLogDebugInfo)
		{
			//("::OnStateMachineNotify()");
		}
	}
	else
	{
		if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
		{
			Server_OnStateMachineNotify(StatesFrom, NextItemState);
		}

	}

}

void UStateMachineComponent::Server_OnStateMachineNotify_Implementation(const TArray<uint8> &StatesFrom, const uint8 NextItemState)
{
	Multicast_OnStateMachineNotify(StatesFrom, NextItemState);
}

void UStateMachineComponent::Multicast_OnStateMachineNotify_Implementation(const TArray<uint8> &StatesFrom, const uint8 NextItemState)
{
	OnStateMachineNotify(StatesFrom, NextItemState);
}

void UStateMachineComponent::OnBeforeSwitchedFrom(uint8 StateBeforSwitched)
{
	bool IsDedicatedServer = IsNetMode(NM_DedicatedServer);
	UNetDriver* NetDriver = GetWorld()->GetNetDriver();
	//

	if (!IsDedicatedServer || (NetDriver && NetDriver->IsServer()))
	{
		UState* CurrentPointer = GetCurrentStateInstance();
		if (IsValid(CurrentPointer))
		{
			CurrentPointer->BeforeSwitchedFrom();
		}


		OnBeforeSwitchedFromEvent.Broadcast(StateBeforSwitched);

	}

	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		Server_OnBeforeSwitchedFrom(StateBeforSwitched);
	}

}

void UStateMachineComponent::Server_OnBeforeSwitchedFrom_Implementation(uint8 StateBeforSwitched)
{

	OnBeforeSwitchedFrom(StateBeforSwitched);
	
	if (bLogDebugInfo)
	{
		//LOG_FSM("::OnBeforeSwitchedFrom( %s )", *GetEnumStateNameByIndex(StateBeforSwitched));
	}

}

void UStateMachineComponent::OnAfterSwitchedTo(uint8 StateJustSwitched)
{

	UWorld* World = GetWorld();

	bool IsDedicatedServer = IsNetMode(NM_DedicatedServer);
	UNetDriver* NetDriver = GetWorld()->GetNetDriver();
	//

	if (!IsDedicatedServer || (NetDriver && NetDriver->IsServer()))
	{
		UStateNet *CurrentPointer = GetCurrentStateInstance();
		if (IsValid(CurrentPointer))
		{
			CurrentPointer->JustSwitchedTo();
		}


		OnAfterSwitchedToEvent.Broadcast(StateJustSwitched);
	}
	else if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		Server_OnAfterSwitchedTo(StateJustSwitched);
	}

}

void UStateMachineComponent::Server_OnAfterSwitchedTo_Implementation(uint8 StateJustSwitched)
{

	OnAfterSwitchedTo(StateJustSwitched);


}

bool UStateMachineComponent::HasNoState() const
{
	return !!(GetCurrentState() == (uint8)-1);
}

uint8 UStateMachineComponent::GetNextState(uint8 StateIndex) const
{

	const UStateNet *CurrentPointer = GetCurrentStateInstance();
	if (IsValid(CurrentPointer))
	{
		uint8 ResultState = CurrentPointer->GetNextState(StateIndex);

		if (bLogDebugInfo)
		{
			//LOG_FSM("::GetNextState( %s ) Result[%s], Current[%s];", *GetEnumStateNameByIndex(StateIndex), *GetEnumStateNameByIndex(ResultState), *GetEnumStateNameByIndex(GetCurrentState()));
		}

		return ResultState;
	}
	else
	{
		return uint8(StateIndex);
	}
	
}

#define NOSTATE 0
void UStateMachineComponent::ApplyTargetState()
{

	bool IsDedicatedServer = IsNetMode(NM_DedicatedServer);
	UNetDriver* NetDriver = GetWorld()->GetNetDriver();
	//

	if (!IsDedicatedServer || (NetDriver && NetDriver->IsServer()))
	{

		if (GetTargetState() == GetCurrentState())
		{

			return;

		}

		if (!States.IsValidIndex(TargetStateIndex))
		{

			TargetStateIndex = CurrentStateIndex;
			return;

		}

		uint8 NextStateIdentifier = GetTargetState();
		UStateNet *CurrentPointer = GetCurrentStateInstance();


		if (!IsValid(CurrentPointer))
		{

			//UE_LOG(LogTemp, Display, TEXT("UStateMachineComponent:: Has No Current State!"));
		}
		else
		{

			bool IsFinished = IsProcessorFinished(CurrentPointer->GetIdentifierValue());
			if (!IsFinished)
			{
				return;
			}

			uint8 Target = GetTargetState();
			NextStateIdentifier = GetNextState(Target);
			if (NextStateIdentifier == uint8(-1))
			{
				NextStateIdentifier = Target;
			}

		}

		if (NextStateIdentifier != CurrentStateIndex)
		{
			bool Can = true;
			if (IsValid(CurrentPointer))
			{
				Can = CurrentPointer->CanBeSwitchedFrom();
			}

			if (Can)
			{

				OnBeforeSwitchedFrom(CurrentStateIndex);

				SetCurrentState(NextStateIdentifier);

				OnAfterSwitchedTo(NextStateIdentifier);

				SetLastState(CurrentStateIndex);

			}
			else
			{

			}

		}

	}

}
