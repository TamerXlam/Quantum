// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameMode.h"

ADefaultGameMode::ADefaultGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

ADefaultGameMode::~ADefaultGameMode()
{

}

void ADefaultGameMode::DoStartScene_BP_Implementation()
{

}

void ADefaultGameMode::StartPlay()
{
	Super::StartPlay();

	DoStartScene_BP();

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		const EActorIteratorFlags Flags = EActorIteratorFlags::SkipPendingKill;
//		for (auto It = TActorIterator<AMasterItemInstrument>(World); It; ++It)
//		{
//			AMasterItemInstrument* Actor = *It;
//			Actor->StartPlay();
//		}
	}
}