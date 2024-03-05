// Fill out your copyright notice in the Description page of Project Settings.


#include "GsScopeGame.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "ScopeGame/GsEventManager.h"
#include "ScopeGame/GsInventoryManager.h"
#include "ScopeGame/GsMinimapManager.h"
#include "ScopeGame/GsCameraModeManager.h"
#include "ScopeGame/GsAIManager.h"
#include "ScopeGame/GsClientSpawnManager.h"
#include "ScopeGame/GsInteractionManager.h"
#include "ScopeGame/GsDropObjectManager.h"
#include "ScopeGame/GsBookmarkManager.h"
#include "ScopeGame/GsPartyManager.h"
#include "ScopeGame/GsWorldMapManager.h"
#include "ScopeGame/GsCoolTimeManager.h"
#include "ScopeGame/GsQuickSlotManager.h"
#include "ScopeGame/GsChatManager.h"
#include "ScopeGame/GsNpcShopManager.h"
#include "ScopeGame/GsFairyManager.h"
#include "ScopeGame/GsBuddyManager.h"
#include "ScopeGame/GsCostumeManager.h"
#include "ScopeGame/GsItemCollectionManager.h"
#include "ScopeGame/GsMailManager.h"
#include "ScopeGame/GsSummonManager.h"
#include "ScopeGame/GsCraftManager.h"
#include "ScopeGame/GsDungeonManager.h"
#include "ScopeGame/GsDepotManager.h"
#include "ScopeGame/GsEffectTextManager.h"
#include "ScopeGame/GsMarketManager.h"
#include "ScopeGame/GsSpaceCrackManager.h"
#include "ScopeGame/GsMultiLevelRankManager.h"
#include "ScopeGame/GsValidatorChainManager.h"
#include "ScopeGame/GsCommunityEventManager.h"
#include "ScopeGame/GsEventProgressManager.h"
#include "ScopeGame/GsRankingManager.h"
#include "ScopeGame/GsAchievementManager.h"
#include "ScopeGame/GsSkillPreviewManager.h"
#include "ScopeGame/GsSaveBatteryManager.h"
#include "ScopeGame/GsCampManager.h"
#include "ScopeGame/GsSealManager.h"
#include "ScopeGame/GsTitleManager.h"
#include "ScopeGame/GsGameStatisticsManager.h"
#include "ScopeGame/GsUnlockManager.h"
#include "ScopeGame/GsObserverManager.h"
#include "ScopeGame/GsUserProfileManager.h"
#include "ScopeGame/GsInterServerManager.h"
#include "ScopeGame/GsTutorialManager.h"
#include "ScopeGame/GsBattlePassManager.h"
#include "ScopeGame/GsPKBookManager.h"
#include "ScopeGame/GsArenaManager.h"
#include "ScopeGame/GsContentsEventManager.h"
#include "ScopeGame/GsInvadeBattleLevelManager.h"
#include "ScopeGame/GsUtilizingContentsManager.h"
#include "ScopeGame/GsItemEnchantManager.h"
#include "ScopeGame/GsPresetManager.h"
#include "ScopeGame/GsMonsterCollectionManager.h"
#include "ScopeGame/GsPersonalTradingManager.h"
#include "ScopeGame/GsPassivityManager.h"
#include "ScopeGame/GsSiegeWarfareManager.h"
#include "ScopeGame/GsSpiritShotManager.h"
#include "ScopeGame/GsStatChangeNotifyManager.h"
#include "ScopeGame/GsSanctumManager.h"
#include "ScopeGame/GsCodexManager.h"
#include "ScopeGame/GsBattleArenaSeasonRankManager.h"
#include "ScopeGame/GsStarLegacyManager.h"


void UGsScopeGame::Create()
{
	UGsScopeBase::Create();

	// 인덱스로 사용하는 Enum과의 순서보장 위해 체크
	for (uint8 mgrType = 0; mgrType < EManagerType::Max; ++mgrType)
	{
		switch (mgrType) 
		{
		case EManagerType::Minimap:
			CreateUObjectManager<UGsMinimapManager>(this, StaticCast<int32>(EManagerType::Minimap));
			break;
		case EManagerType::GameObject:
			CreateUObjectManager<UGsGameObjectManager>(this, StaticCast<int32>(EManagerType::GameObject));
			break;
		case EManagerType::Supply:
			CreateUObjectManager<UGsSupplyManager>(this, StaticCast<int32>(EManagerType::Supply));
			break;
		case EManagerType::Event:
			CreateUObjectManager<UGsEventManager>(this, StaticCast<int32>(EManagerType::Event));
			break;
		case EManagerType::ContentsEvent:
			CreateUObjectManager<UGsContentsEventManager>(this, StaticCast<int32>(EManagerType::ContentsEvent));
			break;
		case  EManagerType::Inventory:
			CreateUObjectManager<UGsInventoryManager>(this, StaticCast<int32>(EManagerType::Inventory));
			break;				
		case EManagerType::QuickSlot:
			CreateUObjectManager<UGsQuickSlotManager>(this, StaticCast<int32>(EManagerType::QuickSlot));
			break;
		case EManagerType::NpcShop:
			CreateUObjectManager<UGsNpcShopManager>(this, StaticCast<int32>(EManagerType::NpcShop));
			break;		
		case EManagerType::CameraMode:
			CreateFObjectManager<FGsCameraModeManager>(StaticCast<int32>(EManagerType::CameraMode));
			break;
		case EManagerType::AI:
			CreateFObjectManager<FGsAIManager>(StaticCast<int32>(EManagerType::AI));
			break;
		case EManagerType::Interaction:
			CreateFObjectManager<FGsInteractionManager>(StaticCast<int32>(EManagerType::Interaction));
			break;
		case EManagerType::ClientSpawn:
			CreateFObjectManager<FGsClientSpawnManager>(StaticCast<int32>(EManagerType::ClientSpawn));
			break;
		case EManagerType::DropObject:
			CreateFObjectManager<FGsDropObjectManager>(StaticCast<int32>(EManagerType::DropObject));
			break;
		case EManagerType::Bookmark:
			CreateFObjectManager<FGsBookmarkManager>(StaticCast<int32>(EManagerType::Bookmark));
			break;
		case EManagerType::Party:
			CreateFObjectManager<FGsPartyManager>(StaticCast<int32>(EManagerType::Party));
			break;
		case EManagerType::WorldMap:
			CreateFObjectManager<FGsWorldMapManager>(StaticCast<int32>(EManagerType::WorldMap));
			break;
		case EManagerType::CoolTime:
			CreateFObjectManager<FGsCoolTimeManager>(StaticCast<int32>(EManagerType::CoolTime));
			break;
		case EManagerType::Chat:
			CreateUObjectManager<UGsChatManager>(this, StaticCast<int32>(EManagerType::Chat));
			break;
		case EManagerType::Fairy:
			CreateUObjectManager<UGsFairyManager>(this, StaticCast<int32>(EManagerType::Fairy));
			break;
		case EManagerType::Buddy:
			CreateFObjectManager<FGsBuddyManager>(StaticCast<int32>(EManagerType::Buddy));
			break;
		case EManagerType::Costume:
			CreateUObjectManager<UGsCostumeManager>(this, StaticCast<int32>(EManagerType::Costume));
			break;
		case EManagerType::ItemCollection:
			CreateFObjectManager<FGsItemCollectionManager>(StaticCast<int32>(EManagerType::ItemCollection));
			break;
		case EManagerType::MultiLevelRank:
			CreateFObjectManager<FGsMultiLevelRankManager>(StaticCast<int32>(EManagerType::MultiLevelRank));
			break;
		case EManagerType::Mail:
			CreateFObjectManager<FGsMailManager>(StaticCast<int32>(EManagerType::Mail));
			break;
		case EManagerType::Summon:
			CreateUObjectManager<UGsSummonManager>(this, StaticCast<int32>(EManagerType::Summon));
			break;
		case EManagerType::Craft:
			CreateUObjectManager<UGsCraftManager>(this, StaticCast<int32>(EManagerType::Craft));
			break;
		case EManagerType::SkillPreview:
			CreateUObjectManager<UGsSkillPreviewManager>(this, StaticCast<int32>(EManagerType::SkillPreview));
			break;
		case EManagerType::Dungeon:
			CreateFObjectManager<FGsDungeonManager>(StaticCast<int32>(EManagerType::Dungeon));
			break;
		case EManagerType::Depot:
			CreateFObjectManager<FGsDepotManager>(StaticCast<int32>(EManagerType::Depot));
			break;
		case EManagerType::EffectText:
			CreateFObjectManager<FGsEffectTextManager>(StaticCast<int32>(EManagerType::EffectText));
			break;
		case EManagerType::Market:
			CreateFObjectManager<FGsMarketManager>(StaticCast<int32>(EManagerType::Market));
			break;
		case EManagerType::SpaceCrack:
			CreateFObjectManager<FGsSpaceCrackManager>(StaticCast<int32>(EManagerType::SpaceCrack));
			break;
		case EManagerType::ValidatorChain:
			CreateFObjectManager<FGsValidatorChainManager>(StaticCast<int32>(EManagerType::ValidatorChain));
			break;
		case EManagerType::CommunityEvent:
			CreateFObjectManager<FGsCommunityEventManager>(StaticCast<int32>(EManagerType::CommunityEvent));
			break;
		case EManagerType::EventProgress:
			CreateFObjectManager<FGsEventProgressManager>(StaticCast<int32>(EManagerType::EventProgress));
			break;
		case EManagerType::Ranking:
			CreateFObjectManager<FGsRankingManager>(StaticCast<int32>(EManagerType::Ranking));
			break;
		case EManagerType::Achievement:
			CreateUObjectManager<UGsAchievementManager>(this, StaticCast<int32>(EManagerType::Achievement));
			break;
		case EManagerType::SaveBattery:
			CreateFObjectManager<FGsSaveBatteryManager>(StaticCast<int32>(EManagerType::SaveBattery));
			break;
		case EManagerType::MonsterCollection:
			CreateFObjectManager<FGsMonsterCollectionManager>(StaticCast<int32>(EManagerType::MonsterCollection));
			break;
		case EManagerType::Camp:
			CreateFObjectManager<FGsCampManager>(StaticCast<int32>(EManagerType::Camp));
			break;
		case EManagerType::Seal:
			CreateFObjectManager<FGsSealManager>(StaticCast<int32>(EManagerType::Seal));
			break;
		case EManagerType::Title:
			CreateFObjectManager<FGsTitleManager>(StaticCast<int32>(EManagerType::Title));
			break;
		case EManagerType::GameStatistics:
			CreateFObjectManager<FGsGameStatisticsManager>(StaticCast<int32>(EManagerType::GameStatistics));
			break;
		case EManagerType::Unlock:
			CreateFObjectManager<FGsUnlockManager>(StaticCast<int32>(EManagerType::Unlock));
			break;
		case EManagerType::Observer:
			CreateFObjectManager<FGsObserverManager>(StaticCast<int32>(EManagerType::Observer));
			break;
		case EManagerType::UserProfile:
			CreateFObjectManager<FGsUserProfileManager>(StaticCast<int32>(EManagerType::UserProfile));
			break;
		case EManagerType::InterServerInvasion:
			CreateUObjectManager<UGsInterServerManager>(this, StaticCast<int32>(EManagerType::InterServerInvasion));
			break;
		case EManagerType::Tutorial:
			CreateFObjectManager<FGsTutorialManager>(StaticCast<int32>(EManagerType::Tutorial));
			break;
		case EManagerType::BattlePass:
			CreateUObjectManager<UGsBattlePassManager>(this, StaticCast<int32>(EManagerType::BattlePass));
			break;
		case EManagerType::PKBook:
			CreateFObjectManager<FGsPKBookManager>(StaticCast<int32>(EManagerType::PKBook));
			break;
		case EManagerType::Arena:
			CreateFObjectManager<FGsArenaManager>(StaticCast<int32>(EManagerType::Arena));
			break;
		case EManagerType::InvadeBattleLevel:
			CreateFObjectManager<FGsInvadeBattleLevelManager>(StaticCast<int32>(EManagerType::InvadeBattleLevel));
			break;
		case EManagerType::UtilizingContents:
			CreateFObjectManager<FGsUtilizingContentsManager>(StaticCast<int32>(EManagerType::UtilizingContents));
			break;
		case EManagerType::ItemEnchant:
			CreateFObjectManager<FGsItemEnchantManager>(StaticCast<int32>(EManagerType::ItemEnchant));
			break;
		case EManagerType::Preset:
			CreateFObjectManager<FGsPresetManager>(StaticCast<int32>(EManagerType::Preset));
			break;
		case EManagerType::PersonalTrading:
			CreateFObjectManager<FGsPersonalTradingManager>(StaticCast<int32>(EManagerType::PersonalTrading));
			break;
		case EManagerType::Passivity:
			CreateFObjectManager<FGsPassivityManager>(StaticCast<int32>(EManagerType::Passivity));
			break;
		case EManagerType::SiegeWarfare:
			CreateFObjectManager<FGsSiegeWarfareManager>(StaticCast<int32>(EManagerType::SiegeWarfare));
			break;
		case EManagerType::SpiritShot:
			CreateFObjectManager<FGsSpiritShotManager>(StaticCast<int32>(EManagerType::SpiritShot));
			break;
		case EManagerType::StatChangeNotify:
			CreateFObjectManager<FGsStatChangeNotifyManager>(StaticCast<int32>(EManagerType::StatChangeNotify));
			break;
		case EManagerType::Sanctum:
			CreateFObjectManager<FGsSanctumManager>(StaticCast<int32>(EManagerType::Sanctum));
			break;
		case EManagerType::Codex:
			CreateFObjectManager<FGsCodexManager>(StaticCast<int32>(EManagerType::Codex));
			break;
		case EManagerType::BattleArenaRank:
			CreateFObjectManager<FGsBattleArenaSeasonRankManager>(StaticCast<int32>(EManagerType::BattleArenaRank));
			break;
		case EManagerType::StarLegacy:
			CreateFObjectManager<FGsStarLegacyManager>(StaticCast<int32>(EManagerType::StarLegacy));
			break;
		default:
			check(false);
			break;
		}
	}
}

void UGsScopeGame::Init()
{
	for (IGsManager* mgr : _managerList)
	{
		if (nullptr != mgr)
		{
#if !UE_BUILD_SHIPPING
			FDateTime StartTime = FDateTime::UtcNow();
#endif

			mgr->Initialize();

#if !UE_BUILD_SHIPPING
			FDateTime EndTime = FDateTime::UtcNow();
			int64 Milliseconds = (EndTime.ToUnixTimestamp() * 1000 + EndTime.GetMillisecond()) - (StartTime.ToUnixTimestamp() * 1000 + StartTime.GetMillisecond());
			int32 AddedIndex = _managerList.Find(mgr);
			if (AddedIndex != INDEX_NONE && _managerToType.Contains(AddedIndex))
				InitElapsedTimeMap.Emplace(static_cast<EManagerType>(_managerToType[AddedIndex]), Milliseconds);
#endif
		}
	}

#if !UE_BUILD_SHIPPING
	GSLOG(Log, TEXT("[InitializeTimeChecker] mgr->Initialize() Elapsed Time \n"));

	InitElapsedTimeMap.ValueSort([](const int64 l, const int64 r)
		{
			return l > r;
		});

	// 0.01 초 이상 걸리는 작업에 대해 로그 출력
	for (auto& p : InitElapsedTimeMap)
	{
		if (p.Value < 10)
			break;

		FString TimestampString = FString::Printf(TEXT("%lld milliseconds"), p.Value);
		GSLOG(Warning, TEXT("[InitializeTimeChecker] %s Manager : %s"), *(UGsScopeGame::GetManagerTypeString(p.Key)), *TimestampString);
	}

	InitElapsedTimeMap.Empty();

	GSLOG(Log, TEXT("\n"));
#endif

	// 주의: ScopeGame은 리소스 다운이 된 시점에 생성되므로, Init 후 바로 호출
	InitAfterResourceDownload();
}

void UGsScopeGame::InitAfterResourceDownload()
{
	for (IGsManager* mgr : _managerList)
	{
		if (nullptr != mgr)
		{
#if !UE_BUILD_SHIPPING
			FDateTime StartTime = FDateTime::UtcNow();
#endif

			mgr->InitializeAfterResourceDownload();

#if !UE_BUILD_SHIPPING
			FDateTime EndTime = FDateTime::UtcNow();
			int64 Milliseconds = (EndTime.ToUnixTimestamp() * 1000 + EndTime.GetMillisecond()) - (StartTime.ToUnixTimestamp() * 1000 + StartTime.GetMillisecond());
			int32 AddedIndex = _managerList.Find(mgr);
			if (AddedIndex != INDEX_NONE && _managerToType.Contains(AddedIndex))
				InitElapsedTimeMap.Emplace(static_cast<EManagerType>(_managerToType[AddedIndex]), Milliseconds);
#endif
		}
	}

#if !UE_BUILD_SHIPPING
	GSLOG(Log, TEXT("[InitializeTimeChecker] mgr->InitializeAfterResourceDownload() Elapsed Time \n"));

	InitElapsedTimeMap.ValueSort([](const int64 l, const int64 r)
		{
			return l > r;
		});

	// 0.01 초 이상 걸리는 작업에 대해 로그 출력
	for (auto& p : InitElapsedTimeMap)
	{
		if (p.Value < 10)
			break;

		FString TimestampString = FString::Printf(TEXT("%lld milliseconds"), p.Value);
		GSLOG(Warning, TEXT("[InitializeTimeChecker] %s Manager : %s"), *(UGsScopeGame::GetManagerTypeString(p.Key)), *TimestampString);
	}

	InitElapsedTimeMap.Empty();

	GSLOG(Log, TEXT("\n"));
#endif
}

FString UGsScopeGame::GetManagerTypeString(EManagerType InManagerType)
{
	switch (InManagerType)
	{
	case  EManagerType::Minimap:
	{
		return FString("Minimap");
	}
	break;
	case  EManagerType::GameObject:
	{
		return FString("GameObject");
	}
	break;
	case  EManagerType::Supply:
	{
		return FString("Supply");
	}
	break;
	case  EManagerType::Event:
	{
		return FString("Event");
	}
	break;
	case  EManagerType::ContentsEvent:
	{
		return FString("ContentsEvent");
	}
	break;
	case  EManagerType::Inventory:
	{
		return FString("Inventory");
	}
	break;
	case  EManagerType::QuickSlot:
	{
		return FString("QuickSlot");
	}
	break;
	case  EManagerType::Chat:
	{
		return FString("Chat");
	}
	break;
	case  EManagerType::NpcShop:
	{
		return FString("NpcShop");
	}
	break;
	case  EManagerType::Fairy:
	{
		return FString("Fairy");
	}
	break;
	case  EManagerType::Costume:
	{
		return FString("Costume");
	}
	break;
	case  EManagerType::Summon:
	{
		return FString("Summon");
	}
	break;
	case  EManagerType::Craft:
	{
		return FString("Craft");
	}
	break;
	case  EManagerType::Achievement:
	{
		return FString("Achievement");
	}
	break;
	case  EManagerType::SkillPreview:
	{
		return FString("SkillPreview");
	}
	break;
	case  EManagerType::BattlePass:
	{
		return FString("BattlePass");
	}
	break;
	case  EManagerType::CameraMode:
	{
		return FString("CameraMode");
	}
	break;
	case  EManagerType::ValidatorChain:
	{
		return FString("ValidatorChain");
	}
	break;
	case  EManagerType::AI:
	{
		return FString("AI");
	}
	break;
	case  EManagerType::Interaction:
	{
		return FString("Interaction");
	}
	break;
	case  EManagerType::ClientSpawn:
	{
		return FString("ClientSpawn");
	}
	break;
	case  EManagerType::DropObject:
	{
		return FString("DropObject");
	}
	break;
	case  EManagerType::Bookmark:
	{
		return FString("Bookmark");
	}
	break;
	case  EManagerType::Party:
	{
		return FString("Party");
	}
	break;
	case  EManagerType::WorldMap:
	{
		return FString("WorldMap");
	}
	break;
	case  EManagerType::CoolTime:
	{
		return FString("CoolTime");
	}
	break;
	case  EManagerType::Buddy:
	{
		return FString("Buddy");
	}
	break;
	case  EManagerType::Mail:
	{
		return FString("Mail");
	}
	break;
	case  EManagerType::Dungeon:
	{
		return FString("Dungeon");
	}
	break;
	case  EManagerType::Depot:
	{
		return FString("Depot");
	}
	break;
	case  EManagerType::EffectText:
	{
		return FString("EffectText");
	}
	break;
	case  EManagerType::Market:
	{
		return FString("Market");
	}
	break;
	case  EManagerType::SpaceCrack:
	{
		return FString("SpaceCrack");
	}
	break;
	case  EManagerType::ItemCollection:
	{
		return FString("ItemCollection");
	}
	break;
	case  EManagerType::MultiLevelRank:
	{
		return FString("MultiLevelRank");
	}
	break;
	case  EManagerType::CommunityEvent:
	{
		return FString("CommunityEvent");
	}
	break;
	case  EManagerType::EventProgress:
	{
		return FString("EventProgress");
	}
	break;
	case  EManagerType::Ranking:
	{
		return FString("Ranking");
	}
	break;
	case  EManagerType::SaveBattery:
	{
		return FString("SaveBattery");
	}
	break;
	case  EManagerType::MonsterCollection:
	{
		return FString("MonsterCollection");
	}
	break;
	case  EManagerType::Camp:
	{
		return FString("Camp");
	}
	break;
	case  EManagerType::Seal:
	{
		return FString("Seal");
	}
	break;
	case  EManagerType::Title:
	{
		return FString("Title");
	}
	break;
	case  EManagerType::GameStatistics:
	{
		return FString("GameStatistics");
	}
	break;
	case  EManagerType::Unlock:
	{
		return FString("Unlock");
	}
	break;
	case  EManagerType::Observer:
	{
		return FString("Observer");
	}
	break;
	case  EManagerType::UserProfile:
	{
		return FString("UserProfile");
	}
	break;
	case  EManagerType::InterServerInvasion:
	{
		return FString("InterServerInvasion");
	}
	break;
	case  EManagerType::Tutorial:
	{
		return FString("Tutorial");
	}
	break;
	case  EManagerType::PKBook:
	{
		return FString("PKBook");
	}
	break;
	case  EManagerType::Arena:
	{
		return FString("Arena");
	}
	break;
	case  EManagerType::InvadeBattleLevel:
	{
		return FString("InvadeBattleLevel");
	}
	break;
	case  EManagerType::UtilizingContents:
	{
		return FString("UtilizingContents");
	}
	break;
	case  EManagerType::ItemEnchant:
	{
		return FString("ItemEnchant");
	}
	break;
	case  EManagerType::Preset:
	{
		return FString("Preset");
	}
	break;
	case  EManagerType::PersonalTrading:
	{
		return FString("PersonalTrading");
	}
	break;
	case  EManagerType::Passivity:
	{
		return FString("Passivity");
	}
	break;
	case  EManagerType::SiegeWarfare:
	{
		return FString("SiegeWarfare");
	}
	break;
	case  EManagerType::SpiritShot:
	{
		return FString("SpiritShot");
	}
	break;
	case  EManagerType::StatChangeNotify:
	{
		return FString("StatChangeNotify");
	}
	break;
	case  EManagerType::Sanctum:
	{
		return FString("Sanctum");
	}
	break;
	case  EManagerType::Codex:
	{
		return FString("Codex");
	}
	break;
	case  EManagerType::BattleArenaRank:
	{
		return FString("BattleArenaRank");
	}
	break;
	case  EManagerType::StarLegacy:
	{
		return FString("StarLegacy");
	}
	break;
	case  EManagerType::FObjectMgrMax:
	{
		return FString("FObjectMgrMax");
	}
	break;
	}

	return FString();
}
