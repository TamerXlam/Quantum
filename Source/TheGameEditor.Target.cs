// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class TheGameEditorTarget : TargetRules
{
	public TheGameEditorTarget(TargetInfo Target) : base(Target)
	{
		DefaultBuildSettings = BuildSettingsVersion.V2;
		bLegacyPublicIncludePaths = true;
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "TheGame", "TheGameEditor"  } );
	}
}
