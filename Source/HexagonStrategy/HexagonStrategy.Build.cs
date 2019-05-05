// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class HexagonStrategy : ModuleRules
{
	public HexagonStrategy(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","OnlineSubsystem","OnlineSubsystemUtils","GameplayTasks","AIModule" });

		PrivateDependencyModuleNames.AddRange(new string[] { "SlateCore","Slate","GameplayAbilities","GameplayTags","GameplayTasks"});

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				"OnlineSubsystemNull",
				"NetworkReplayStreaming",
				"NullNetworkReplayStreaming",
				"HttpNetworkReplayStreaming",
				"LocalFileNetworkReplayStreaming"
            }   
        );

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
