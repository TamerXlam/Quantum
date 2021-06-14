
#include "StateNet.h"
#include "StateMachineComponent.h"

#include "Engine/NetDriver.h"

void UStateNet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStateNet, Identifier);
	DOREPLIFETIME(UStateNet, StateMachine);
	DOREPLIFETIME(UStateNet, ProcessorResult);
	DOREPLIFETIME(UStateNet, Behavior);
	DOREPLIFETIME(UStateNet, CurrentBehavior);
	DOREPLIFETIME(UStateNet, StartTime);
	DOREPLIFETIME(UStateNet, StateOwner);

	UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass());
	if (BPClass) BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);

	//
}

UWorld* UStateNet::GetWorld() const
{
	if (GIsEditor && !GIsPlayInEditorWorld) return nullptr;
	else if (GetOuter()) return GetOuter()->GetWorld();
	else return nullptr;
}

bool UStateNet::CallRemoteFunction(UFunction* Function, void* Parameters, FOutParmRec* OutParms, FFrame* Stack)
{
	if (!GetOuter()) return false;
	AActor *OuterActor = Cast<AActor>(GetOuter());

	if (!IsValid(OuterActor))
	{
		return false;
	}

	UNetDriver *NetDriver = OuterActor->GetNetDriver();
	if (!NetDriver)
	{
		return false;
	}

	NetDriver->ProcessRemoteFunction(OuterActor, Function, Parameters, OutParms, Stack, this);

	return true;
}

int32 UStateNet::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	return (GetOuter() ? GetOuter()->GetFunctionCallspace(Function, Stack) : FunctionCallspace::Local);
}

UStateNet::UStateNet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	

}

UStateNet::~UStateNet()
{

}

void UStateNet::PostInitProperties()
{
	Super::PostInitProperties();

	if (GetOuter() && GetOuter()->GetWorld())
		BeginPlay();

}

void UStateNet::Init(UStateMachineComponent *SMOwner, uint8 IS, EBaseStateBehavior SB, AActor *SOwner, bool TickableAIInfo, UFunction* SProcessor)
{

	Super::Init(SMOwner, IS, SB, SOwner, TickableAIInfo, SProcessor);

	Identifier = IS;

	StateOwner = SOwner;

	StateMachine = SMOwner;

	Processor = SProcessor;

	Behavior = SB;
	CurrentBehavior = SB;

	StartTime = GetServerWorldTimeSeconds(SOwner);

}

void UStateNet::BeginPlay()
{

	//
}

void UStateNet::OnRep_Behavior()
{

	CurrentBehavior = Behavior;
}

void UStateNet::OnRep_CurrentBehavior()
{

	if (GetCurrentBehavior() != EBaseStateBehavior::MFinished)
	{
		return;
	}

	if (!IsValid(StateMachine))
	{
		return;
	}

	//TArray<uint8> StatesForBreaking;
	//StatesForBreaking.Add(GetIdentifierValue());
	//StateMachine->OnStateMachineNotify(StatesForBreaking);

}

void UStateNet::Tick(float DeltaSeconds)
{
	EventTick(DeltaSeconds);

	Super::Tick(DeltaSeconds);

	if (GetProcessor())
	{

		const UStateMachineComponent *CurrentOwner = GetStateMachine();
		if (IsValid(CurrentOwner))
		{
			ProcessorResult = CurrentOwner->IsProcessorFinished(GetIdentifierValue());
		}


	}
}

void UStateNet::BeforeSwitchedFrom()
{
	Super::BeforeSwitchedFrom();

}

void UStateNet::JustSwitchedTo()
{
	Super::JustSwitchedTo();


	CurrentBehavior = Behavior;

	StartTime = GetServerWorldTimeSeconds(GetStateOwner());


}

void UStateNet::OnQFSMSwitcherNotify()
{

	Super::OnQFSMSwitcherNotify();


	if (CurrentBehavior != EBaseStateBehavior::StoppedByNotify)
	{
		return;
	}

	CurrentBehavior = EBaseStateBehavior::MFinished;

}

bool UStateNet::CanBeSwitchedFrom() const
{
	bool Result = (CurrentBehavior == EBaseStateBehavior::MFinished || CurrentBehavior == EBaseStateBehavior::FreeState);//!!(/*ADCache.TimeInState >= MimimalTimeInState &&*/ ADCache.CanBeSwitchedFrom());
	//bool ResultBP = CanBeSwitchedFromBP_Implementation();

	return !!(Result && ProcessorResult);
}

float UStateNet::GetServerWorldTimeSeconds(const UObject* WorldContextObject)
{
	if (IsValid(WorldContextObject))
	{
		UWorld* World = WorldContextObject->GetWorld();
		if (IsValid(World))
		{
			const auto GameState = World->GetGameState();
			if (IsValid(GameState))
			{
				return GameState->GetServerWorldTimeSeconds();
			}
			return World->GetTimeSeconds();
		}
	}
	return 0.0f;
}

