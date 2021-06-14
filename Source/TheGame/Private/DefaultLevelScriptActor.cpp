// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultLevelScriptActor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

ADefaultLevelScriptActor::ADefaultLevelScriptActor(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{

}

ADefaultLevelScriptActor::~ADefaultLevelScriptActor()
{

}

void ADefaultLevelScriptActor::BeginPlay()
{

	Super::BeginPlay();


	StartGameNet();

	// ...

}

void ADefaultLevelScriptActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);

	// ...
}

void ADefaultLevelScriptActor::StartGameNet()
{

	UWorld *World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	if (World->IsServer())
	{
		//UGameplayStatics::OpenLevel(World, "127.0.0.1", true);
	}
	else/* if (World->IsClient())*/
	{
		//UGameplayStatics::OpenLevel(this, "127.0.0.1", true);
	}

	//
	StartGameNet_BP();
}
