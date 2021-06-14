// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/GameModeBase.h"
//#include "MasterItemInstrument.h"

#include "DefaultGameMode.generated.h"

UCLASS(ClassGroup = "NanahooGame/Default", DisplayName = "DefaultGameMode", BlueprintType, Blueprintable)
class THEGAME_API ADefaultGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	// Sets default values for this component's properties
	ADefaultGameMode(const FObjectInitializer& ObjectInitializer);
	~ADefaultGameMode();

	UFUNCTION(BlueprintNativeEvent)
	void DoStartScene_BP();

public:
	virtual void StartPlay() override;

};
