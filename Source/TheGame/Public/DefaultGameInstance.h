// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DefaultGameInstance.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = "NanahooGame/Default", DisplayName = "DefaultGameInstance", BlueprintType, Blueprintable)
class THEGAME_API UDefaultGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UDefaultGameInstance(const FObjectInitializer& ObjectInitializer);
	~UDefaultGameInstance();

};
