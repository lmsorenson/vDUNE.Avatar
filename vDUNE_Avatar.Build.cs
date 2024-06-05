// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class vDUNE_Avatar : ModuleRules
{
	public vDUNE_Avatar(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
