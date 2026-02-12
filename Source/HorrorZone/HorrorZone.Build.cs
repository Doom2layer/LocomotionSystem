// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HorrorZone : ModuleRules
{
	public HorrorZone(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"UMG",
			"Slate",
			"RigVM",
			"ControlRig",
			"AnimGraphRuntime",
			"GameplayStateTreeModule",
			"AnimationLocomotionLibraryRuntime",
			"Niagara",
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"HorrorZone",
			"HorrorZone/Variant_Platforming",
			"HorrorZone/Variant_Platforming/Animation",
			"HorrorZone/Variant_Combat",
			"HorrorZone/Variant_Combat/AI",
			"HorrorZone/Variant_Combat/Animation",
			"HorrorZone/Variant_Combat/Gameplay",
			"HorrorZone/Variant_Combat/Interfaces",
			"HorrorZone/Variant_Combat/UI",
			"HorrorZone/Variant_SideScrolling",
			"HorrorZone/Variant_SideScrolling/AI",
			"HorrorZone/Variant_SideScrolling/Gameplay",
			"HorrorZone/Variant_SideScrolling/Interfaces",
			"HorrorZone/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
