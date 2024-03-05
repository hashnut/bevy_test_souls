using System.IO;
using UnrealBuildTool;

public class Zenonia : ModuleRules
{
    public Zenonia(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "T1Project",
        });
    }
}
