// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class TheGameTarget : TargetRules
{
	public TheGameTarget(TargetInfo Target) : base(Target)
	{
		DefaultBuildSettings = BuildSettingsVersion.V2;
		bLegacyPublicIncludePaths = true;
		Type = TargetType.Game;

		ExtraModuleNames.AddRange(new string[] { "TheGame" });
	}
}
