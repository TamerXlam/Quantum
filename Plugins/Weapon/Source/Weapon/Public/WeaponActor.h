// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WeaponActor.generated.h"


/**
 *
 */
UCLASS(ClassGroup = "TheGame|Weapon", DisplayName = "WeaponActor", BlueprintType, Blueprintable)
class WEAPON_API AWeaponActor : public AActor
{
	GENERATED_BODY()

public:
	AWeaponActor(const FObjectInitializer& ObjectInitializer);
	~AWeaponActor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(Category = "TheGame|Weapon", BlueprintNativeEvent, BlueprintCallable)
	void Fire_BP();
	virtual void Fire_BP_Implementation();


};
