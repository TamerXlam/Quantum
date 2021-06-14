// Fill out your copyright notice in the Description page of Project Settings.
using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class TheGameServerTarget : TargetRules
{
	public TheGameServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;

		ExtraModuleNames.Add("TheGame"); // This would be your project module if not BP only
	}
	
}