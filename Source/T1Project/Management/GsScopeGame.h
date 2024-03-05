// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsScopeBase.h"
#include "GsScopeGame.generated.h"

/**
 * 인게임 매니저 모음. 로비로 돌아갈 때 전부해제.
 */
UCLASS()
class T1PROJECT_API UGsScopeGame final : public UGsScopeBase
{
	GENERATED_BODY()

public:
	enum EManagerType : uint8
	{
		Minimap = 0,		// finalize 할때 클리어 위해  gameobect class 참조해야 하므로 먼저 등록		
		GameObject,		
		Supply,
		Event,
		ContentsEvent,
		Inventory,
		QuickSlot,
		Chat,
		NpcShop,
		Fairy,
		Costume,
		Summon,		
		Craft,
		Achievement,
		SkillPreview,
		BattlePass,
		
		UObjectMgrMax,
		//-------------------------------------------------
		FObjectMgrStart = UObjectMgrMax,
		//-------------------------------------------------
		CameraMode = FObjectMgrStart,
		ValidatorChain,
		AI,
		Interaction,
		ClientSpawn,
		DropObject,
		Bookmark,
		Party,
		WorldMap,
		CoolTime,
		Buddy,		
		Mail,		
		Dungeon,
		Depot,
		EffectText,
		Market,
		SpaceCrack,
		ItemCollection,
		MultiLevelRank,
		CommunityEvent,
		EventProgress,
		Ranking,
		SaveBattery,
		MonsterCollection,
		Camp,
		Seal,
		Title,
		GameStatistics,
		Unlock,
		Observer,
		UserProfile,
		InterServerInvasion,
		Tutorial,
		PKBook,
		Arena,
		InvadeBattleLevel,
		UtilizingContents,
		ItemEnchant,
		Preset,
		PersonalTrading,
		Passivity,
		SiegeWarfare,
		SpiritShot,
		StatChangeNotify,
		Sanctum,
		Codex,
		BattleArenaRank,
		StarLegacy,
		//-------------------------------------------------
		FObjectMgrMax, 
		//-------------------------------------------------		
		Max = FObjectMgrMax
	};

private:
	/** (매니저 타입, Initialize 소요 시간 or InitializeAfterResourceDownload 소요 시간) 시간의 단위는 milisecond */
	TMap<EManagerType, int64> InitElapsedTimeMap;

public:
	virtual void Create() override;
	virtual void Init() override;
	virtual void InitAfterResourceDownload() override;

public:
	static FString GetManagerTypeString(EManagerType InManagerType);
};

