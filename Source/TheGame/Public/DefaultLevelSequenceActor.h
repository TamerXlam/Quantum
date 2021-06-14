// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequenceActor.h"

#include "DefaultLevelSequenceActor.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = "TheGame|Default", DisplayName = "DefaultLevelSequenceActor", BlueprintType, Blueprintable)
class THEGAME_API ADefaultLevelSequenceActor : public ALevelSequenceActor
{
	GENERATED_BODY()
	
public:
	ADefaultLevelSequenceActor(const FObjectInitializer& ObjectInitializer);
	~ADefaultLevelSequenceActor();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	virtual bool IsSupportedForNetworking() const override { return true; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
};
