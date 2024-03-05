using UnrealBuildTool;
using System.IO;

public class AnimGraphEx : ModuleRules
{
	public AnimGraphEx(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		OverridePackageType = PackageOverrideType.EngineDeveloper;

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"Slate",
				"AnimGraphRuntime",
				"BlueprintGraph",				
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"InputCore",
				"SlateCore",
				"PropertyEditor",
				"EditorStyle",
				"ContentBrowser",
				"KismetWidgets",
				"ToolMenus",
				"KismetCompiler",
				"Kismet",
				"AnimGraph",
				"T1Project",
			}
		);


		PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"Persona",
				"SkeletonEditor",
				"AdvancedPreviewScene",
				"AnimationBlueprintEditor",
			}
		);
	}
}
