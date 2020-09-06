// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PR_Resistance : ModuleRules
{
	public PR_Resistance(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
