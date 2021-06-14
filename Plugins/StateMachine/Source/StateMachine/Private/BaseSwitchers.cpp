// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseSwitchers.h"
#include "StateMachineComponent.h"


void UOnBaseStateIsFinishedNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!IsValid(MeshComp))
	{
		return;
	}

	AActor* MOwner = MeshComp->GetOwner();
	if (!IsValid(MOwner))
	{
		return;
	}

	UStateMachineComponent* MSM = MOwner->FindComponentByClass<UStateMachineComponent>();
	if (!IsValid(MSM))
	{
		return;
	}
	
	uint8 Identifier = MSM->GetCurrentState();

	TArray<uint8> States = GetStates();
	bool CanBeStopped = false;
	for (int i = 0; i != States.Num(); i++)
	{
		if (States[i] != Identifier)
		{
			continue;
		}

		CanBeStopped = true;
		break;
	}


	if (!CanBeStopped)
	{
		return;
	}

	UStateNet *SInstance = MSM->GetCurrentStateInstance();
	if (IsValid(SInstance))
	{
		SInstance->OnQFSMSwitcherNotify();
	}
	
	//
}