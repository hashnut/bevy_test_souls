#pragma once

#include "Engine/EngineTypes.h"

//UE4
#include "Engine/DeveloperSettings.h"

//Territory editor
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "Actor/Spawn/SpawnActor/GsTerritoryNpc.h"
#include "Actor/Spot/GsTerritorySpot.h"
#include "Actor/Spot/GsTerritoryGameSpot.h"
#include "Actor/Warp/GsTerritoryWarp.h"
#include "Actor/Spawn/SpawnActor/GsTerritoryProp.h"
#include "Actor/Unique/GsTerrEdRTTActor.h"
#include "Actor/Spot/GsTerritorySpaceCrackPortalSpot.h"
#include "Actor/Line/GsTerritoryLine.h"
#include "Actor/Fence/GsTerritoryFenceTown.h"
#include "Actor/Fence/GsTerritoryFenceSafeZone.h"
#include "Actor/Fence/GsTerritoryFenceEnv.h"
#include "Actor/Fence/GsTerritoryFenceSound.h"
#include "Actor/Fence/GsTerritoryFenceSpaceCrack.h"
#include "Actor/Fence/GsTerritoryFenceArena.h"
#include "Actor/Fence/GsTerritoryFenceArenaReady.h"
#include "Actor/Fence/GsTerritoryFenceArrival.h"
#include "Actor/Fence/GsTerritoryFenceTownChaos.h"
#include "Actor/Fence/GsTerritoryFenceNoCampZone.h"
#include "Actor/Fence/GsTerritoryFenceNonPK.h"

#include "GsTerritoryEditorSetting.generated.h"

/**
 * For terrainEditor setting  in project setting.
 */

const FString MISSING_CIRCLE_ZONE = TEXT("Territory editor에 circle zone class가 설정되어 있지 않습니다");
const FString MISSING_POLYGON_ZONE = TEXT("Territory editor에 polygon zone class가 설정되어 있지 않습니다");
const FString MISSING_LINE_ZONE = TEXT("Territory editor에 line zone class가 설정되어 있지 않습니다");

USTRUCT()
struct FGsTerritoryLevelSetting
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, category = "GsTerritoryEditorSetting", EditDefaultsOnly, meta = (AllowedClasses = "UWorld"))
		FSoftObjectPath _map;
	UPROPERTY(Config, category = "GsTerritoryEditorSetting", EditDefaultsOnly, meta = (AllowedClasses = "UDataTable"))
		FSoftObjectPath _uniqueTable;
};

class UGsDataTable;

UCLASS(ClassGroup = TerritoryEditor, Config = TerritoryEditor, category = "TerritoryEditor", meta = (DisplayName = "Territory Editor Settings"))
class TERRITORYEDITOR_API UGsTerritoryEditorSetting : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(config, EditAnywhere, category = "TerritoryEditor")
		bool _isActive = true;

	//zone
public:
	UPROPERTY(config, EditAnywhere, category = "SpawnData")
		TSubclassOf<AGsTerritoryZone> _circleZoneClass;
	UPROPERTY(config, EditAnywhere, category = "SpawnData")
		TSubclassOf<AGsTerritoryZone> _polygonZoneClass;
	UPROPERTY(config, EditAnywhere, category = "SpawnData")
		TSubclassOf<AGsTerritoryZone> _lineZoneClass;

	//actor
public:
	UPROPERTY(config, EditAnywhere, category = "SpawnData")
		TSubclassOf<AGsTerritoryNpc> _npcClass;
	UPROPERTY(config, EditAnywhere, category = "SpawnData")
		TSubclassOf<AGsTerritoryProp> _propClass;

	//spot
public:
	UPROPERTY(config, EditAnywhere, category = "Spot")
		TSubclassOf<AGsTerritoryNormalSpot> _normalSpotClass;
	UPROPERTY(config, EditAnywhere, category = "Spot")
		TSubclassOf<AGsTerritorySpawnSpot> _spawnSpotClass;
	UPROPERTY(config, EditAnywhere, category = "Spot")
		TSubclassOf<AGsTerritoryResurrectionSpot> _ressurectionSpotClass;
	UPROPERTY(config, EditAnywhere, category = "Spot")
		TSubclassOf<AGsTerritoryTeleportSpot> _teleportSpotClass;
	UPROPERTY(config, EditAnywhere, category = "Spot")
		TSubclassOf<AGsTerritoryWarpSpot> _warpSpotClass;
	UPROPERTY(config, EditAnywhere, category = "Warp")
		TSubclassOf<AGsTerritoryWarp> _warpClass;
	UPROPERTY(config, EditAnywhere, category = "Spot")
		TSubclassOf<AGsTerritorySpaceCrackPortalSpot> _spaceCrackPortalClass;

	//fence
public:
	UPROPERTY(config, EditAnywhere, category = "Fence")
		TSubclassOf<AGsTerritoryFenceTown> _fenceTownClass;
	UPROPERTY(config, EditAnywhere, category = "Fence")
		TSubclassOf<AGsTerritoryFenceSafeZone> _fenceSafeZoneClass;
	UPROPERTY(config, EditAnywhere, category = "Fence")
		TSubclassOf<AGsTerritoryFenceTownChaos> _fenceTownChaosClass;
	UPROPERTY(config, EditAnywhere, category = "Fence")
		TSubclassOf<AGsTerritoryFenceArena> _fenceArenaClass;
	UPROPERTY(config, EditAnywhere, category = "Fence")
		TSubclassOf<AGsTerritoryFenceArenaReady> _fenceArenaReadyClass;
	UPROPERTY(config, EditAnywhere, category = "Fence")
		TSubclassOf<AGsTerritoryFenceArrival> _fenceArrivalClass;
	UPROPERTY(config, EditAnywhere, category = "Fence")
		TSubclassOf<AGsTerritoryFenceEnv> _fenceEnvClass;
	UPROPERTY(config, EditAnywhere, category = "Fence")
		TSubclassOf<AGsTerritoryFenceSound> _fenceSoundClass;
	UPROPERTY(config, EditAnywhere, category = "Fence")
		TSubclassOf<AGsTerritoryFenceSpaceCrack> _fenceSpaceCrackClass;
	UPROPERTY(config, EditAnywhere, category = "Fence")
		TSubclassOf<AGsTerritoryFenceNoCampZone> _fenceNoCampZoneClass;
	UPROPERTY(config, EditAnywhere, category = "Fence")
		TSubclassOf<AGsTerritoryFenceNonPK> _fenceNonPKClass;

	//line
public:
	UPROPERTY(config, EditAnywhere, category = "Line")
		TSubclassOf<AGsTerritoryLine> _patrolLineClass;	

public:
	UPROPERTY(config, EditAnywhere, category = "Unique data")
		TSubclassOf<AGsTerrEdRTTActor> _rttClass;

public:
	UPROPERTY(config, EditAnywhere, category = "TerritoryEditor setting", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		float _territoryActorViewDistance = 3000;

public:
	UPROPERTY(config, EditAnywhere, category = "TerritoryEditor setting", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		FDirectoryPath _raonJsonSavePath;

public:
	UPROPERTY(config, EditAnywhere, category = "Level editor", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		float _InitialWidgetViewDistance = 5000;
	UPROPERTY(config, EditAnywhere, category = "Level editor", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		float _InitialActorViewDistance = 5000;
	UPROPERTY(config, EditAnywhere, category = "Level editor", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		float _InitialSightRadiusViewDistance = 5000;

public:
	UPROPERTY(config, EditAnywhere, category = "Territory Actor color", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		FColor _zoneColor = FColor::Blue;
	UPROPERTY(config, EditAnywhere, category = "Territory Actor color", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		FColor _zoneFenceColor = FColor::Blue;
	UPROPERTY(config, EditAnywhere, category = "Territory Actor color", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		FColor _zoneFloorColor = FColor::Green;
	UPROPERTY(config, EditAnywhere, category = "Territory Actor color", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		FColor _zonePillarColor = FColor::Red;

public:
	UPROPERTY(config, EditAnywhere, category = "Territory Actor color", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		FColor _npcColor = FColor::Red;
	UPROPERTY(config, EditAnywhere, category = "Territory Actor color", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		FColor _propColor = FColor::Yellow;

public:
	UPROPERTY(config, EditAnywhere, category = "Territory Actor color", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		FColor _normalSpotColor = FColor::Green;
	UPROPERTY(config, EditAnywhere, category = "Territory Actor color", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		FColor _spawnSpotColor = FColor::Blue;
	UPROPERTY(config, EditAnywhere, category = "Territory Actor color", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		FColor _resurrectionSpotColor = FColor::Red;
	UPROPERTY(config, EditAnywhere, category = "Territory Actor color", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		FColor _teleportSpotColor = FColor::Yellow;
	UPROPERTY(config, EditAnywhere, category = "Territory Actor color", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		FColor _warpSpotColor = FColor::Orange;
	UPROPERTY(config, EditAnywhere, category = "Territory Actor color", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		FColor _warpColor = FColor::Orange;
	UPROPERTY(config, EditAnywhere, category = "Territory Actor color", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		FColor _spaceCrackSpotColor = FColor::Purple;

	UPROPERTY(config, EditAnywhere, category = "Territory Actor color", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		FColor _territoryColor = FColor::Green;
	UPROPERTY(config, EditAnywhere, category = "Territory Actor color", meta = (Tooltip = "Territory RSQL json folder in project", RelativeToGameProjectDir))
		FColor _fenceColor = FColor::Green;

public:
	UPROPERTY(config, EditAnywhere, category = "Territory Zone")
		bool _drawZonePillar = false;
	UPROPERTY(config, EditAnywhere, category = "Territory Zone")
		TSoftObjectPtr<UMaterial> _opaqueMaterial;
	UPROPERTY(config, EditAnywhere, category = "Territory Zone")
		TSoftObjectPtr<UMaterial> _transparentMaterial;
	UPROPERTY(config, EditAnywhere, category = "Territory Zone")
		float _initTransparentFloorAlpha = 0.2f;

public:
	UGsTerritoryEditorSetting(const FObjectInitializer& ObjectInitializer);
	virtual ~UGsTerritoryEditorSetting();

public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

};