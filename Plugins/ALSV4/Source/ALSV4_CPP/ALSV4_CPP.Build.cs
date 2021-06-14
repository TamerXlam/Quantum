
using UnrealBuildTool;

public class ALSV4_CPP : ModuleRules
{
	public ALSV4_CPP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
			{"Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "GameplayTasks","PhysicsCore", "Niagara", "LevelScriptHelper"});

		PrivateDependencyModuleNames.AddRange(new[] {"Slate", "SlateCore"});
	}
}