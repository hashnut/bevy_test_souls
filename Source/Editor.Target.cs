// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class EditorTarget : TargetRules
{
	public EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "Zenonia", "SkillEditor", "TerritoryEditor", "DataTableConverterEditor", "AnimGraphEx", "UnrealSupportEditor" } );
	}
}
