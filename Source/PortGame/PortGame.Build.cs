// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PortGame : ModuleRules
{
	public PortGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "PortGame" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" , "MotionWarping",
			"AnimGraphRuntime","UMG","NavigationSystem","AIModule","GameplayTasks" ,"Niagara","MovieScene" , "LevelSequence"});

       
    }

}
