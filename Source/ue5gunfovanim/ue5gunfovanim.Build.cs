// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ue5gunfovanim : ModuleRules
{
	public ue5gunfovanim(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
