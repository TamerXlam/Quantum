// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "DefaultGameSession.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = "NanahooGame/Default", DisplayName = "DefaultGameSession", BlueprintType, Blueprintable)
class THEGAME_API ADefaultGameSession : public AGameSession
{
	GENERATED_BODY()
	
public:
	ADefaultGameSession(const FObjectInitializer& ObjectInitializer);
	~ADefaultGameSession();
};
