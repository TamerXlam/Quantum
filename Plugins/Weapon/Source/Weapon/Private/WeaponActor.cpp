// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

AWeaponActor::AWeaponActor(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{

}

AWeaponActor::~AWeaponActor()
{

}

void AWeaponActor::BeginPlay()
{

	Super::BeginPlay();



	// ...

}

void AWeaponActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);

	// ...
}


void AWeaponActor::Fire_BP_Implementation()
{

	//
}

