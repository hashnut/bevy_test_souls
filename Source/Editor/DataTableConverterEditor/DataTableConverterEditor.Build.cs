using System.IO;

namespace UnrealBuildTool.Rules
{
    public class DataTableConverterEditor : ModuleRules
    {
        public DataTableConverterEditor(ReadOnlyTargetRules Target) : base(Target)
        {
            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
            bUseUnity = false;
            
            PublicIncludePathModuleNames.Add("DataCenter");

            PublicDependencyModuleNames.AddRange(new string[] {
                "Core",
                "WorldBrowser",
                "TerritoryEditor"
            });

            PrivateDependencyModuleNames.AddRange(new string[] {
                "CoreUObject",
                "Engine",
                "UnrealEd",
                "AssetRegistry",

                "RSQL",
                "RSQLEditor",
                "HTTP",

                "T1Project",
                "DataCenter",

                "SlateCore", 
                "Slate",
                "UnrealEd",
                "EditorStyle",

                "ExportNavEditor",

                "BuildPatchServices",
            });

            PrivateIncludePaths.AddRange(new string[] {
                Path.Combine(ModuleDirectory, "Public"),
            });
        }
    }
}