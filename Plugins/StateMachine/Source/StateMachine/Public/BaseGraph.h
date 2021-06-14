// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
//#include "State.h"
//
//#include "MSMGraph.generated.h"
//
//USTRUCT(BlueprintType)
//struct FMSMGraph
//{
//	GENERATED_USTRUCT_BODY()
//
//	virtual EState GetWrapped(EState Target, EState Current) const
//	{
//
//		EState  Result = Target;
//		switch (Target)
//		{
//		case EState::IIActive:
//		{
//			Result = MActiveGraph(Current);
//		}
//		break;
//
//		case EState::IIHidden:
//		{
//			Result = MHiddenGraph(Current);
//		}
//		break;
//
//		}
//
//
//		return Result;
//	}
//
//
//private:
//	virtual EState MActiveGraph(EState Current) const
//	{
//
//		EState Result = Current;
//		switch (Current)
//		{
//		case EState::IIHidden:
//			Result = EState::IIShowing;
//			break;
//		case EState::IIShowing:
//			Result = EState::IIActive;
//			break;
//
//		default:
//			Result = EState::IINoState;
//			break;
//
//		}
//
//		return Result;
//
//	}
//
//	virtual EState MHiddenGraph(EState Current) const
//	{
//
//		EState Result = Current;
//		switch (Current)
//		{
//		case EState::IIActive:
//			Result = EState::IIHidding;
//			break;
//		case EState::IIHidding:
//			Result = EState::IIHidden;
//			break;
//
//		default:
//			Result = EState::IINoState;
//			break;
//
//		}
//
//		return Result;
//
//	}
//};
//
