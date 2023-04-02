// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectFA : ModuleRules
{
	public ProjectFA(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AIModule", "NavigationSystem", "EnhancedInput",
			"GameplayTasks", "UMG"
		});
		
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
	}
}
