// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "State.h"
#include "BaseKismetMathLibrary.generated.h"

UCLASS(meta = (BlueprintThreadSafe, ScriptName = "MathLibrary"))
class STATEMACHINE_API UBaseKismetMathLibrary : public UKismetMathLibrary
{
	GENERATED_BODY()

public:
	UBaseKismetMathLibrary(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{

	}


};