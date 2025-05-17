// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NeuroStrike : ModuleRules
{
	public NeuroStrike(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
