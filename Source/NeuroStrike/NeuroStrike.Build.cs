// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NeuroStrike : ModuleRules
{
	public NeuroStrike(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
