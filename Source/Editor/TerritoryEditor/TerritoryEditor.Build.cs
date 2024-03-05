// Fill out your copyright notice in the Description page of Project Settings.
using System.IO;
using UnrealBuildTool;

public class TerritoryEditor : ModuleRules
{
    public TerritoryEditor(ReadOnlyTargetRules Target) : base(Target)
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
                "AIModule"
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

        PublicDefinitions.Add("TERRITORY_EDITOR_DEBUG=0");
        PublicDefinitions.Add("TERRITORY_EDITRO_UNIQUEID_DEBUG=1");
        PublicDefinitions.Add("SPOT_ACTION_UTILITY=1");        
        PublicDefinitions.Add("DISABLE_XRAY_VISION=1");
        PublicDefinitions.Add("SAVE_RTT=0");
        PublicDefinitions.Add("REFACTORY_SAVE_ALL_MAP=1");
        PublicDefinitions.Add("TERRITORY_BLOCK_DEBUG=0");
    }
}