// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WhatsYoga : ModuleRules
{
	public WhatsYoga(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "WhatsYoga" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Json", "JsonUtilities", "LevelSequence", "MovieScene", "UMG", "MovieSceneTracks" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "Sockets", "Networking" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
