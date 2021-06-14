// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseLevelScriptActor.h"
#include "DefaultLevelScriptActor.generated.h"

/**
 *
 */
UCLASS(ClassGroup = "TheGame|Default", DisplayName = "DefaultLevelScriptActor", BlueprintType, Blueprintable)
class THEGAME_API ADefaultLevelScriptActor : public ABaseLevelScriptActor
{
	GENERATED_BODY()

public:
	ADefaultLevelScriptActor(const FObjectInitializer& ObjectInitializer);
	~ADefaultLevelScriptActor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void StartGameNet();

public:
	UFUNCTION(Category = "TheGame|LevelScriptHelper", BlueprintNativeEvent)
	void StartGameNet_BP();
	virtual void StartGameNet_BP_Implementation() { }

};
