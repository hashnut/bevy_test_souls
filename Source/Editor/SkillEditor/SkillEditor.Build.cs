// Fill out your copyright notice in the Description page of Project Settings.
using System.IO;
using UnrealBuildTool;

public class SkillEditor : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    public SkillEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        //PrivateIncludePaths.Add("SkillEditor/Private");

		//var MobilePatchingPrivatePath = Path.Combine(Path.GetFullPath(Target.RelativeEnginePath), "Plugins", "Runtime", "MobilePatchingUtils/Source/MobilePatchingUtils", "Private");
		var MobilePatchingPrivatePath = Path.Combine(Path.GetFullPath(Target.RelativeEnginePath), "Source/Runtime/Engine/Private");
		var PersonaPrivatePath = Path.Combine(Path.GetFullPath(Target.RelativeEnginePath), "Source/Editor/Persona/Private");

		PrivateIncludePaths.AddRange(new string[] {
                MobilePatchingPrivatePath,
				PersonaPrivatePath,
			});

		//PublicIncludePaths.AddRange(new string[] {
		//        Path.Combine(ModuleDirectory, "Public"),
		//        Path.Combine(ModuleDirectory, "Private")
		//    });

		//PrivateIncludePaths.AddRange(new string[] {
		//        Path.Combine(ModuleDirectory, "Public"),
		//        Path.Combine(ModuleDirectory, "Private")
		//    });

		//PublicIncludePathModuleNames.AddRange(new string[] { "Renderer", "PacketHandler", "AudioMixer", "AudioMixerCore", "AnimationCore" });

        //PrivateIncludePaths.AddRange(
        //    new string[] {
        //        "Developer/DerivedDataCache/Public",
        //        "Runtime/SynthBenchmark/Public",
        //        "Runtime/Engine/Private",
        //    }
        //);

        PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"ApplicationCore",
				"Json",
				"Slate",
				"SlateCore",
				"Engine",
				"InputCore",
				"UnrealEd", // for FAssetEditorManager
				"KismetWidgets",
				"Kismet",  // for FWorkflowCentricApplication
				"PropertyEditor",
				"RenderCore",				
				"AdvancedPreviewScene",
				"WorkspaceMenuStructure",
				"EditorStyle",
				"MeshPaint",
				"EditorWidgets",
				"Projects",
				"NavigationSystem",
				"Persona",
				"AnimationEditor",
				"AnimGraph",
				"SequenceRecorder",
				"SourceControl",
				"T1Project",
				"DataCenter",
                "RSQL",
                "RSQLEditor",
				"AnimationCore",
				//"Runtime",
			});        

        PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"Settings",
				"AssetTools",
				"LevelEditor",
				"ContentBrowser",
				"AnimationCore",
				//"Runtime",
			});

		DynamicallyLoadedModuleNames.AddRange(
			new string[] {
				"AssetTools",
				"ContentBrowser",
				//"Runtime",
			});

		PublicDefinitions.Add("GAMEVIL_ENGINE_SKILL_EDITOR_BSAM");
		//PublicDefinitions.Add("GAMEVIL_ENGINE_SKILL_EDITOR_CONVERTING_BSAM");
		//PublicDefinitions.Add("GAMEVIL_ENGINE_SKILL_EDITOR_MODIFY1_BSAM");
	}
}
