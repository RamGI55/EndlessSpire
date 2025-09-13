// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EndlessSpire : ModuleRules
{
	public EndlessSpire(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayTasks", "UMG", "AIModule", "LevelSequence", "Paper2D", "GameplayCameras" ,"NavigationSystem", "Niagara", "CinematicCamera"});

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "Renderer" });
	}
}
