// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TheGameEditor : ModuleRules
{
	public TheGameEditor( ReadOnlyTargetRules Target) : base(Target)
    {
        PrivatePCHHeaderFile = "TheGameEditor.h";

        PublicDependencyModuleNames.AddRange(new string[] {
        "Core",
        "CoreUObject",
        "InputCore",
        "Slate",
        "SlateCore",
        "EditorStyle",
      
      //  "Engine",
      // "UnrealEd",
        "AnimGraph",
        "BlueprintGraph",
        "AnimGraphRuntime",
        "LevelSequence",
         "TheGame"
        // "TheGameEditor"


        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
           //// "CoreUObject",
           // "DataTableEditor",
           // "UnrealED",
           //   "PropertyEditor",
           // "AnimGraph",
           // "AnimGraphRuntime",
           // "BlueprintGraph",
           //         "Kismet",

           // "TheGame"

        });
    }
}
