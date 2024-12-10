// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Course01 : ModuleRules
{
	public Course01(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "slua_unreal", "UMG", "Slate", "SlateCore"
		});
	}
}