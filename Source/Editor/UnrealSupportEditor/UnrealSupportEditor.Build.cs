// Fill out your copyright notice in the Description page of Project Settings.
using System.IO;
using UnrealBuildTool;

public class UnrealSupportEditor : ModuleRules
{
    public UnrealSupportEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(
            new string[]{"Core",
                "CoreUObject",
                "SlateCore",
                "Slate",
                "UnrealEd",
                "LevelEditor",
                "T1Project",
                "InputCore",
                "Engine",
                "AdvancedPreviewScene",
                "EditorStyle",
                "PropertyEditor",
                "Json",
                "JsonUtilities",
                "AppFramework",                
                "UMG",
                "ComponentVisualizers",
                "WorldBrowser",                            
                "Landscape",
                "RSQL",
                "RSQLEditor",
                "Settings",
                "MainFrame",
                "Projects",
                "EngineSettings",
                "GameplayTasks",
                "AssetRegistry",
                "Kismet",                
                "BlueprintGraph",
                "Persona",
                "NavigationSystem",
                "DataTableEditor",
                "SceneOutliner",
                "AssetRegistry",
                "AssetTools",
                "ContentBrowser",
                "EditorWidgets",
                "DataCenter",
                "NavigationSystem",
                "AIModule",
                "EditorScriptingUtilities",
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
              "ProceduralMeshComponent",
              "WorldBrowser",
              "Blutility",
            });

        string enginePath = Path.GetFullPath(Target.RelativeEnginePath);
        PublicIncludePaths.Add(enginePath + "Source/Editor/Blutility/Private");        
    }
}