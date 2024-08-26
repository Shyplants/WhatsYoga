// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SocketClient : ModuleRules
{
	public SocketClient(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { });

		PrivateIncludePaths.AddRange(new string[] {
			"SocketClient/Private"
		});


		PublicDependencyModuleNames.AddRange(new string[] { "Core", "Networking", "Sockets" });

		PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "Networking", "Sockets", "Slate", "SlateCore" });

		DynamicallyLoadedModuleNames.AddRange(new string[] { });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
