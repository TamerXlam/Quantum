// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultLevelSequenceActor.h"

void ADefaultLevelSequenceActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

ADefaultLevelSequenceActor::ADefaultLevelSequenceActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{


}

ADefaultLevelSequenceActor::~ADefaultLevelSequenceActor()
{

}

void ADefaultLevelSequenceActor::BeginPlay()
{

	Super::BeginPlay();


	// ...

}

void ADefaultLevelSequenceActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);

	// ...
}