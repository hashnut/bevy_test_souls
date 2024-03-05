using System.IO;
using UnrealBuildTool;

public class T1Project : ModuleRules
{
    public T1Project(ReadOnlyTargetRules Target) : base(Target)
    {
        //http://api.unrealengine.com/KOR/Programming/UnrealBuildSystem/IWYUReferenceGuide/index.html
        //IWYU 모드를 켜려면, 모듈이 IWYU 규칙으로 컴파일되는지 확인하고, 
        //모듈의 *.build.cs 파일을 열어 PCHUsage 를 PCHUsageMode.UseExplicitOrSharedPCHs 로 설정하면 됩니다.
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        if (Target.Platform == UnrealTargetPlatform.IOS)
        { 
            PrivateDependencyModuleNames.Add("IOSRuntimeSettings");
        }
        else
        {
            if (Target.Platform == UnrealTargetPlatform.Android)
            {
                PrivateDependencyModuleNames.Add("AndroidRuntimeSettings");
            }
        }

        var GeometryProcessingPublicPath = Path.Combine(Path.GetFullPath(Target.RelativeEnginePath), "Plugins", "Experimental", "GeometryProcessing/Source/GeometricObjects", "Public");
        var MobilePatchingPrivatePath = Path.Combine(Path.GetFullPath(Target.RelativeEnginePath), "Plugins", "Runtime", "MobilePatchingUtils/Source/MobilePatchingUtils", "Private");
        PrivateIncludePaths.AddRange(new string[] {
                MobilePatchingPrivatePath,
                GeometryProcessingPublicPath,
            });

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "UMG",
            "Sockets",
            "AIModule",
            "GameplayTasks",
            "PhysXVehicles",
            "ClientNetwork",
            "NavigationSystem",
            "LevelSequence",
            "MovieScene",
            "ActorSequence",
            "Landscape",
            "SignificanceManager",
            "Media",
            "MediaAssets",
            "EngineSettings",
            "RenderCore",
            "MobilePatchingUtils",
            "HerculesLib",
            "HIVESDK",
            "ApplicationCore",
            "AppFramework",
            "PhysicsCore",
            "GeometricObjects",
            //"WebBrowserWidget",
            "AnimGraphRuntime",
            "AnimationCore",
            "KawaiiPhysics", // KawaiiPhysics 플러그인은 코드 의존성이 없어 종종 빌드하지 않는 경우가 있으므로 추가.
            "Paper2D",
            "DataCenter",
            "CinematicCamera",
            "PlayGuard",
            "GsPlatformSpecific",
            "Sentry",
            "GameplayCameras",
            "CommonUI",
        });

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(new string[]{ 
                "UnrealEd",
                "WorldBrowser",
                "LevelEditor",
                "RSQLEditor",
                "InstanceTool",
                "ExportNavRuntime",
                "ServerNavVolume",
            });

            // 에디터에서 게임 플레이 시, 패치 과정이 생략되어 실제 패치가 된 상태인지 알 수 없음.
            // 에디터에서도 패치를 완료한 시점을 시뮬레이션 하기 위해, 특정 메세지 발생 시 패치가 완료된 것으로 간주하기 위해 메세지 등록 및 처리를 수행함.
            PublicDefinitions.Add("GS_PATCH_COMPLEATED_EDITOR_SIMULATION");
        }

        PrivateDependencyModuleNames.AddRange(new string[] {
            "OpenSSL",
            "JsonUtilities",
            "Json",
            "Slate",
            "SlateCore",
            "RSQL",
            "MOSUtil",
            "Http",
            "PakFile",
            "BuildPatchServices",
            "MoviePlayer",
        });

        PrivateIncludePathModuleNames.AddRange(new string[] { "MOSUtil" });

        //// BSAM - 해당 디파인을 키고 풀패키지로 패키징하면 Download 로직을 타지 않고 기존 처럼 테스트 할수 있다.
        //PublicDefinitions.Add("FULLPACKAGE_MODE_ON");
        //PublicDefinitions.Add("DEVELOPER_TOOLS_ON");
        PublicDefinitions.Add("WINDOWS_CROS_PLAY_PROCESS");
        // 4.27.2(plus) 엔진 변경에 대한 대응, HIVE 버전업에 대한 대응 임시 코드들임. 정식 적용하면 해당 디파인을 검색해서 제거
        PublicDefinitions.Add("BSAM_TEMP=1"); // 임시로 해놓은것 나중에 삭제 혹은 0으로 변경해야됨

        //DropObject 픽킹 설정 
        PublicDefinitions.Add("DROPOBJECT_PICKING_ACTIVE=0");

        //active
        PublicDefinitions.Add("CANT_LOOT_WHEN_DROP_OBJECT_SPAWN=0");
        PublicDefinitions.Add("QUEST_NPC_ACTIVE=0");
        PublicDefinitions.Add("ACTIVE_QUEST_BOARD_EVENT_PROGRESS=0");
        PublicDefinitions.Add("HIDE_INTERACTION_IN_CHAOS_TOWN=0");
        PublicDefinitions.Add("ACTIVE_COOPERATIVE_BOSS=0");        

        //Debug        
        PublicDefinitions.Add("AGGRO_DEBUG=0");        
        PublicDefinitions.Add("CLIENT_SPAWN_DEBUG=0");
        PublicDefinitions.Add("CONTRIBUTION_DEBUG=0");
        PublicDefinitions.Add("DROP_OBJECT_DEBUG=0");
        PublicDefinitions.Add("ENV_DEBUG=0");
        PublicDefinitions.Add("FIELD_BOSS_DEBUG=0");        
        PublicDefinitions.Add("FLOW_DUBUG_TEST=0");
        PublicDefinitions.Add("GUILD_DUNGEON_DEBUG=0");
        PublicDefinitions.Add("INTERACTION_DEBUG=0");
        PublicDefinitions.Add("INVADE_DEBUG=0");
        PublicDefinitions.Add("INVASION_TOWN_BATTLE_DEBUG=0");
        PublicDefinitions.Add("PK_BOOK_DEBUG=0");
        PublicDefinitions.Add("PROP_INTERACTION_DEBUG=0");
        PublicDefinitions.Add("QUEST_NPC_DEBUG=0");        
        PublicDefinitions.Add("QUEST_BOARD_DEBUG=0");
        PublicDefinitions.Add("REVIVE_RECONNECT_TEST=0");
        PublicDefinitions.Add("RECONNECT_DEBUG=0");
        PublicDefinitions.Add("RESURRECT_DEBUG=0");
        PublicDefinitions.Add("SPAWN_LOCATION_DEBUG=0");
        PublicDefinitions.Add("SPAWN_DEBUG=0");
        PublicDefinitions.Add("STAT_DEBUG=0");
        PublicDefinitions.Add("SOUND_DEBUG=0");
        PublicDefinitions.Add("STAT_DISTRIBUTION_DEBUG=0");
        PublicDefinitions.Add("STAT_CHANGE_NOTIFY_DEBUG=0");
        PublicDefinitions.Add("TERRITORY_DEBUG=0");       
        PublicDefinitions.Add("WORLD_BOSS_DEBUG=0");
        PublicDefinitions.Add("WORLD_MAP_MONSTER_DEBUG=0");
        PublicDefinitions.Add("WORLD_MAP_INVASION_DEBUG=0");
        PublicDefinitions.Add("WEAPON_DEBUG=0");
        PublicDefinitions.Add("BILLBOARD_DEBUG=0");
        PublicDefinitions.Add("PLAYER_SPAWN_DEBUG=0");
        PublicDefinitions.Add("PENALTY_DEBUG=0");
        PublicDefinitions.Add("WINDOW_QUEST_RENEW=0");
        PublicDefinitions.Add("USE_SKILL_ENCHANT=0");
        PublicDefinitions.Add("AUTO_MOVE_EVASIVE_MANEUVER=1");
    }
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }
}
