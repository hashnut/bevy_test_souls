using UnrealBuildTool;
using System;
using System.Collections.Generic;
using Tools.DotNETCommon;

public class ZenoniaTarget : TargetRules
{

    [Tools.DotNETCommon.CommandLine("-RAON_PACKAGE_SHIPPING_LOG")]
    [System.ComponentModel.Description("Force print log on Shipping")]
    bool RaonPackageShippingLog = false;

    [Tools.DotNETCommon.CommandLine("-RAON_PACKAGE_GUARD=")]
    [System.ComponentModel.Description("Apply Guard")]
    string RaonPackageGuard = null;

    public ZenoniaTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;

        //Use these config options in shipping to enable logs, and to enable debugger.
        //if (Configuration == UnrealTargetConfiguration.Shipping)
        //{
        //    BuildEnvironment = TargetBuildEnvironment.Unique;
        //    bUseChecksInShipping = true;
        //    bUseLoggingInShipping = true;
        //    bLoggingToMemoryEnabled = true;
        //}

        //// bsam - xocde 13.3 이상에서 엄격한 컴파일러 옵션때문에 에러 발생부분을 수정하기 위한 커밋(언리얼 포럼)
        //if (UnrealTargetPlatform.IOS == Platform)
        //{
        //    bOverrideBuildEnvironment = true;
        //    AdditionalCompilerArguments = "-Wno-unused-but-set-variable";
        //}

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            // bUseMallocProfiler = true;
            //GlobalDefinitions.Add("MALLOC_LEAKDETECTION=1");
            //GlobalDefinitions.Add("MEMPRO_ENABLED=1");
        }
        
        bUseLoggingInShipping = RaonPackageShippingLog && (Target.Configuration == UnrealTargetConfiguration.Shipping);
        Log.WriteLine(LogEventType.Log, string.Format("bUseLoggingInShipping: {0}", bUseLoggingInShipping));

        ExtraModuleNames.AddRange(new string[] { "Zenonia" });

        if (null != RaonPackageGuard)
        {
            ProjectDefinitions.Add("RAON_PACKAGE_GUARD=" + RaonPackageGuard.Trim());
        }         
    }
}
