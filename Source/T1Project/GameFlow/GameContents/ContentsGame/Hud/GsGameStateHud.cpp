// Fill out your copyright notice in the Description page of Project Settings.

#include "GsGameStateHud.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Management/ScopeGame/GsInventoryManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
//#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Message/MessageParam/GsUIMessageParam.h"

#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/SaveBattery/GsSaveBatteryHandler.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Boss/GsFieldBossHandler.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Bookmark/GsBookmarkHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Party/GsPartyHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Scan/GsScanHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/LockOn/GsLockOnHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Observer/GsObserverHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/CounterAttack/GsCounterAttackHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Stat/GsPlayerStatHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/ItemContents/GsChangeNickNameHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Sanctum/GsSanctumHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Guild/GsAgitHandler.h"

#include "Buddy/GsBuddyHandler.h"
#include "PickOneBox/GsPickOneBoxHandler.h"
#include "Shortcut/GsShortcutHandler.h"
#include "Tutorial/GsTutorialHandler.h"
#include "PKBook/GsPKBookhandler.h"
#include "GameObject/Boss/GsBossHandler.h"
#include "Input/GsInputEventMsgBase.h"

FGsGameStateHud::FGsGameStateHud() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsHud)
{
	_itemDetailHandler = TUniquePtr<FGsItemDetailHandler>(new FGsItemDetailHandler());
	if (_itemDetailHandler.IsValid())
	{
		_itemDetailHandler->Init();
	}
	_invenHandler = TUniquePtr<FGsInvenHandler>(new FGsInvenHandler());
	if (_invenHandler.IsValid())
	{
		_invenHandler->Init();
	}
	_itemEnchantlHandler = TUniquePtr<FGsItemEnchantHandler>(new FGsItemEnchantHandler());
	if (_itemEnchantlHandler.IsValid())
	{
		_itemEnchantlHandler->Init();
	}
	_itemFusionHandler = TUniquePtr<FGsItemFusionHandler>(new FGsItemFusionHandler());
	if (_itemFusionHandler.IsValid())
	{
		_itemFusionHandler->Init();
	}
	_playerDetailInfoHandler = TUniquePtr<FGsPlayerDetailInfoHandler>(new FGsPlayerDetailInfoHandler());
	if (_playerDetailInfoHandler.IsValid())
	{
		_playerDetailInfoHandler->Init();
	}

	_playerStatHandler = TUniquePtr<FGsPlayerStatHandler>(new FGsPlayerStatHandler());
	if (_playerStatHandler.IsValid())
	{
		_playerStatHandler->Init();
	}

	_itemDecomposeHandler = TUniquePtr<FGsItemDecomposeHandler>(new FGsItemDecomposeHandler());
	if (_itemDecomposeHandler.IsValid())
	{
		_itemDecomposeHandler->Init();
	}
	_skillHandler = TUniquePtr<FGsSkillUIHandler>(new FGsSkillUIHandler());
	if (_skillHandler.IsValid())
	{
		_skillHandler->Init();
	}
	
	_quickSlotHandler = TUniquePtr<FGsQuickSlotHandler>(new FGsQuickSlotHandler());
	if (_quickSlotHandler.IsValid())
	{
		_quickSlotHandler->Init();
	}

	_bookmarkHandler = TUniquePtr<FGsBookmarkHandler>(new FGsBookmarkHandler());
	
	_partyHandler = TUniquePtr<FGsPartyHandler>(new FGsPartyHandler());
	if (_partyHandler.IsValid())
	{
		_partyHandler->Init();
	}

	_buddyHandler = TUniquePtr<FGsBuddyHandler>(new FGsBuddyHandler());
	if (_buddyHandler.IsValid())
	{
		_buddyHandler->Init();
	}
	
	_summonHandler = TUniquePtr<FGsSummonHandler>(new FGsSummonHandler());
	if (_summonHandler.IsValid())
	{
		_summonHandler->Init();
	}

	_bossHandler = TUniquePtr<FGsBossHandler>(new FGsBossHandler());
	if (_bossHandler.IsValid())
	{
		_bossHandler->Init();
	}
	
	_pickoneBoxHandler = TUniquePtr<FGsPickOneBoxHandler>(new FGsPickOneBoxHandler());
	if (_pickoneBoxHandler.IsValid())
	{
		_pickoneBoxHandler->Init();
	}

	_scanHandler = TUniquePtr<FGsScanHandler>(new FGsScanHandler());
	if (_scanHandler.IsValid())
	{
		_scanHandler->Init();
	}

	_lockOnHandler = TUniquePtr<FGsLockOnHandler>(new FGsLockOnHandler());
	if (_lockOnHandler.IsValid())
	{
		_lockOnHandler->Init();
	}

	_saveBatteryHandler = TUniquePtr<FGsSaveBatteryHandler>(new FGsSaveBatteryHandler());
	if (_saveBatteryHandler.IsValid())
	{
		_saveBatteryHandler->Init();
	}

	_shortcutHandler = TUniquePtr<FGsShortcutHandler>(new FGsShortcutHandler());
	if (_shortcutHandler.IsValid())
	{
		_shortcutHandler->Init();
	}

	_observerHandler = TUniquePtr<FGsObserverHandler>(new FGsObserverHandler());
	if (_observerHandler.IsValid())
	{
		_observerHandler->Init();
	}

	_tutorialHandler = TUniquePtr<FGsTutorialHandler>(new FGsTutorialHandler());
	if (_tutorialHandler.IsValid())
	{
		_tutorialHandler->Init();
	}

	_counterAttackHandler = TUniquePtr<FGsCounterAttackHandler>(new FGsCounterAttackHandler());
	if (_counterAttackHandler.IsValid())
	{
		_counterAttackHandler->Init();
	}

	_PKBookHandler = TUniquePtr<FGsPKBookHandler>(new FGsPKBookHandler());
	if (_PKBookHandler.IsValid())
	{
		_PKBookHandler->Init();
	}

	_changeNickNameHandler = TUniquePtr <FGsChangeNickNameHandler>(new FGsChangeNickNameHandler());
	if (_changeNickNameHandler.IsValid())
	{
		_changeNickNameHandler->Init();
	}

	_sanctumHandler = TUniquePtr <FGsSanctumHandler>(new FGsSanctumHandler());
	if (_sanctumHandler.IsValid())
	{
		_sanctumHandler->Init();
	}

	_battleArenaHandler = TUniquePtr <FGsBattleArenaGuildWarHandler>(new FGsBattleArenaGuildWarHandler());
	if (_battleArenaHandler.IsValid())
	{
		_battleArenaHandler->Init();
	}

	_agitHandler = TUniquePtr<FGsAgitHandler>(new FGsAgitHandler());
	if (_agitHandler.IsValid())
	{
		_agitHandler->Init();
	}

	_statFilterHandler = TUniquePtr<FGsStatFilterHandler>(new FGsStatFilterHandler());
	if (_statFilterHandler.IsValid())
	{
		_statFilterHandler->Init();
	}
}

void FGsGameStateHud::Close()
{
	if (_quickSlotHandler.IsValid())
	{
		_quickSlotHandler->Close();
	}
	if (_itemDetailHandler.IsValid())
	{
		_itemDetailHandler->Close();
	}
	if (_invenHandler.IsValid())
	{
		_invenHandler->Close();
	}
	if (_itemEnchantlHandler.IsValid())
	{
		_itemEnchantlHandler->Close();
	}
	if (_itemFusionHandler.IsValid())
	{
		_itemFusionHandler->Close();
	}
	if (_playerDetailInfoHandler.IsValid())
	{
		_playerDetailInfoHandler->Close();
	}
	if (_itemDecomposeHandler.IsValid())
	{
		_itemDecomposeHandler->Close();
	}
	if (_skillHandler.IsValid())
	{
		_skillHandler->Close();
	}
	if (_partyHandler.IsValid())
	{
		_partyHandler->Close();
	}
	if (_buddyHandler.IsValid())
	{
		_buddyHandler->Close();
	}
	if (_summonHandler.IsValid())
	{
		_summonHandler->Close();
	}

	if (_bossHandler.IsValid())
	{
		_bossHandler->Close();
	}

	if (_pickoneBoxHandler.IsValid())
	{
		_pickoneBoxHandler->Close();
	}

	if (_playerStatHandler.IsValid())
	{
		_playerStatHandler->Close();
	}

	if (_scanHandler.IsValid())
	{
		_scanHandler->Close();
	}
	if (_lockOnHandler.IsValid())
	{
		_lockOnHandler->Close();
	}

	if (_saveBatteryHandler.IsValid())
	{
		_saveBatteryHandler->Close();
	}

	if (_shortcutHandler.IsValid())
	{
		_shortcutHandler->Close();
	}

	if (_observerHandler.IsValid())
	{
		_observerHandler->Close();
	}

	if (_tutorialHandler.IsValid())
	{
		_tutorialHandler->Close();
	}
	if (_counterAttackHandler.IsValid())
	{
		_counterAttackHandler->Close();
	}
	if (_PKBookHandler.IsValid())
	{
		_PKBookHandler->Close();
	}

	if (_changeNickNameHandler.IsValid())
	{
		_changeNickNameHandler->Close();
	}

	if (_sanctumHandler.IsValid())
	{
		_sanctumHandler->Close();
	}

	if (_battleArenaHandler.IsValid())
	{
		_battleArenaHandler->Close();
	}

	if (_agitHandler.IsValid())
	{
		_agitHandler->Close();
	}

	if (_statFilterHandler.IsValid())
	{
		_statFilterHandler->Close();
	}

	FGsContentsGameBase::Close();
}

FGsGameStateHud::~FGsGameStateHud()
{
	_itemDetailHandler.Reset();
	_invenHandler.Reset();
	_itemEnchantlHandler.Reset();
	_itemFusionHandler.Reset();
	_playerDetailInfoHandler.Reset();	
	_playerStatHandler.Reset();
	_itemDecomposeHandler.Reset();
	_skillHandler.Reset();
	_quickSlotHandler.Reset();
	_bookmarkHandler.Reset();
	_partyHandler.Reset();
	_buddyHandler.Reset();
	_summonHandler.Reset();
	_bossHandler.Reset();
	_pickoneBoxHandler.Reset();
	_saveBatteryHandler.Reset();
	_shortcutHandler.Reset();
	_tutorialHandler.Reset();
	_PKBookHandler.Reset();
	_changeNickNameHandler.Reset();
	_sanctumHandler.Reset();
	_battleArenaHandler.Reset();
	_agitHandler.Reset();
	_statFilterHandler.Reset();
}

void FGsGameStateHud::Enter()
{
	FGsContentsGameBase::Enter();

	// Send Msg (로컬 플레이어 스폰 후 갱신하므로 이 타이밍에 갱신이 필요하지 않음)
	//GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_ALL);

	UGsUIManager* uiMgr = GUI();

	if (FGsMessageHolder* msg = GMessage())
	{
		// 네비게이션 바 비활성
		CloseNavigationBar();
		
		// HUD모드 원상복구
		FGsUIMsgParamInt param(static_cast<int32>(uiMgr->GetHUDMode()));
		msg->GetUI().SendMessage(MessageUI::CHANGE_HUD_MODE, &param);

		FGsUIMsgParamInputEnable inputParam(true);
		msg->GetUI().SendMessage(MessageUI::KEYBOARD_MAPPING_HUD_ENABLE, &inputParam);
	}
	
	uiMgr->RemoveUIScaleFlag(EGsUIScaleFlags::UI_SCALE_OTHER_STATE, false);
	uiMgr->AddUIScaleFlag(EGsUIScaleFlags::UI_SCALE_HUD_STATE);

	// [B1] | C2URWQ-4854 | HUD 가 복구되었을 때, 인풋이 막혀 있는 상황 해소
	// C2URWQ-2476 이슈 관련 수정으로 인한 Side Effect 인 것 같다.
	// 인풋 막기 풀기 -> C2URWQ-5361 관련, 중첩된 input lock 해제가 적용되어 필요없을 것 같다.
	// SETTING_WAIT_SERVER_RESPONSE 가 종료될 때의 메시지가 제대로 전송이 된다면...
	//FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::INPUT_NONE);
	//GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);

	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (FGsGuildAuctionData* auctionData = guildMgr->GetMyGuildAuctionData())
		{
			auctionData->ResetData();
		}
	}

#ifdef RESTORE_INVENTORY
	RestoreInventory();
#endif
}

void FGsGameStateHud::Exit()
{
	GUI()->RemoveUIScaleFlag(EGsUIScaleFlags::UI_SCALE_HUD_STATE);
	GUI()->AddUIScaleFlag(EGsUIScaleFlags::UI_SCALE_OTHER_STATE);

	if (FGsMessageHolder* msg = GMessage())
	{
		FGsUIMsgParamInputEnable param(false);
		msg->GetUI().SendMessage(MessageUI::KEYBOARD_MAPPING_HUD_ENABLE, &param);
	}

	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (FGsGuildAuctionData* auctionData = guildMgr->GetMyGuildAuctionData())
		{
			auctionData->ResetData();
		}
	}

	FGsContentsGameBase::Exit();
	//GSLOG(Log, TEXT("GsContentsGame_Hud : Exit"));
}

void FGsGameStateHud::Update(float In_deltaTime)
{
	if (_saveBatteryHandler.IsValid())
	{
		_saveBatteryHandler->Update(In_deltaTime);
	}

	if (_PKBookHandler.IsValid())
	{
		_PKBookHandler->Update(In_deltaTime);
	}

	if (_skillHandler.IsValid())
	{
		_skillHandler->Update(In_deltaTime);
	}

	if (_quickSlotHandler.IsValid())
	{
		_quickSlotHandler->Update(In_deltaTime);
	}

	if (_battleArenaHandler.IsValid())
	{
		_battleArenaHandler->Update(In_deltaTime);
	}
}

void FGsGameStateHud::OnReconectionEnd()
{
	FGsContentsGameBase::OnReconectionEnd();

	if (_quickSlotHandler.IsValid())
	{
		_quickSlotHandler->OnReconection();
	}

	if (_invenHandler.IsValid())
	{
		_invenHandler->OnReconection();
	}

	if (_itemEnchantlHandler.IsValid())
	{
		_itemEnchantlHandler->OnReconection();
	}

	if (_itemFusionHandler.IsValid())
	{
		_itemFusionHandler->OnReconection();
	}

	if (_playerDetailInfoHandler.IsValid())
	{
		_playerDetailInfoHandler->OnReconection();
	}

	if (_skillHandler.IsValid())
	{
		_skillHandler->OnReconnection();
	}

	if (_pickoneBoxHandler.IsValid())
	{
		_pickoneBoxHandler->OnReconnection();
	}

	if (_saveBatteryHandler.IsValid())
	{
		_saveBatteryHandler->OnReconectionEnd();
	}

	if (FGsBMShopManager* bmShopMgr = GBMShop())
	{
		bmShopMgr->OnReconectionEndBmShop();
	}

	if (_sanctumHandler.IsValid())
	{
		_sanctumHandler->OnReconnection();
	}

	if (_battleArenaHandler.IsValid())
	{
		_battleArenaHandler->OnReconection();
	}

	if (_agitHandler.IsValid())
	{
		_agitHandler->OnReconnection();
	}

	if (_statFilterHandler.IsValid())
	{
		_statFilterHandler->OnReconection();
	}
}

void FGsGameStateHud::RestoreInventory()
{
	// U2: 인벤토리, 상세창 복구
	// https://jira.com2us.com/jira/browse/CHR-21144
	if (UGsInventoryManager* inventoryMgr = GSInventory())
	{
		inventoryMgr->RestoreInventoryAndDetailPopup();
	}
}
