// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GsGameStateHud.h"
#include "../GsContentsGameBase.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Message/GsMessageContents.h"
#include "PlayerDetailInfo/Handler/GsPlayerDetailInfoHandler.h"
#include "Enchant/Handler/GsItemEnchantHandler.h"
#include "ItemDetail/GsItemDetailHandler.h"
#include "Inventory/Handler/GsInvenHandler.h"
#include "Fusion/Handler/GsItemFusionHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Skill/GsSkillUIHandler.h"
#include "QuickSlot/Handler/GsQuickSlotHandler.h"
#include "ItemDecompose/Handler/GsItemDecomposeHandler.h"
#include "../GameObject/Boss/GsFieldBossHandler.h"
#include "BattleArena/GsBattleArenaGuildWarHandler.h"
#include "../StatFilterSystem/GsStatFilterHandler.h"


//------------------------------------------------------------------------------
// InGame 에서 Hud 컨텐츠를 담당하는 Logic Class
//------------------------------------------------------------------------------
class FGsBookmarkHandler;
class FGsPartyHandler;
class FGsBuddyHandler;
class FGsSummonHandler;
class FGsPickOneBoxHandler;
class FGsScanHandler;
class FGsLockOnHandler;
class FGsSaveBatteryHandler;
class FGsShortcutHandler;
class FGsObserverHandler;
class FGsTutorialHandler;
class FGsCounterAttackHandler;
class FGsChangeNickNameHandler;
class FGsSanctumHandler;
class FGsAgitHandler;

class T1PROJECT_API FGsGameStateHud : public FGsContentsGameBase
{
private:
	TUniquePtr<FGsInvenHandler> _invenHandler;
	TUniquePtr<FGsItemDetailHandler> _itemDetailHandler;
	TUniquePtr<FGsItemEnchantHandler> _itemEnchantlHandler;
	TUniquePtr<FGsItemFusionHandler> _itemFusionHandler;
	TUniquePtr<FGsPlayerDetailInfoHandler> _playerDetailInfoHandler;
	TUniquePtr<class FGsPlayerStatHandler> _playerStatHandler;
	TUniquePtr<FGsItemDecomposeHandler> _itemDecomposeHandler;
	TUniquePtr<FGsSkillUIHandler> _skillHandler;
	TUniquePtr<FGsQuickSlotHandler> _quickSlotHandler;
	TUniquePtr<FGsBookmarkHandler> _bookmarkHandler;
	TUniquePtr<FGsPartyHandler> _partyHandler;
	TUniquePtr<FGsBuddyHandler> _buddyHandler;
	TUniquePtr<FGsSummonHandler> _summonHandler;
	TUniquePtr<class FGsBossHandler> _bossHandler;
	TUniquePtr<FGsPickOneBoxHandler> _pickoneBoxHandler;
	TUniquePtr<FGsScanHandler> _scanHandler;
	TUniquePtr<FGsLockOnHandler> _lockOnHandler;
	TUniquePtr<FGsSaveBatteryHandler> _saveBatteryHandler;
	TUniquePtr<FGsShortcutHandler> _shortcutHandler;
	TUniquePtr<FGsObserverHandler> _observerHandler;
	TUniquePtr<FGsTutorialHandler> _tutorialHandler;
	TUniquePtr<FGsCounterAttackHandler> _counterAttackHandler;
	TUniquePtr<class FGsPKBookHandler> _PKBookHandler;
	TUniquePtr<FGsChangeNickNameHandler> _changeNickNameHandler;
	TUniquePtr<FGsSanctumHandler> _sanctumHandler;
	TUniquePtr<FGsBattleArenaGuildWarHandler> _battleArenaHandler;
	TUniquePtr<FGsAgitHandler> _agitHandler;	
	TUniquePtr<FGsStatFilterHandler> _statFilterHandler;

public:
	FGsGameStateHud();
	virtual ~FGsGameStateHud();

public:
	virtual void Close() override;

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;
	virtual void OnReconectionEnd() override;	

protected:
	virtual bool IsChangeSoundMixer() const override { return false; }

	// get
public:
	class FGsPlayerStatHandler* GetPlayerStatHandler() { return _playerStatHandler.Get(); }
	FGsBookmarkHandler* GetBookmarkHandler() { return _bookmarkHandler.Get(); }
	FGsPartyHandler* GetPartyHandler() { return _partyHandler.Get(); }
	FGsScanHandler* GetScanHandler() { return _scanHandler.Get(); }
	FGsLockOnHandler* GetLockOnHandler() { return _lockOnHandler.Get(); }
	FGsBuddyHandler* GetBuddyHandler() { return _buddyHandler.Get(); }
	FGsSummonHandler* GetSummonHandler() { return _summonHandler.Get(); }
	class FGsBossHandler* GetBossHandler() { return _bossHandler.Get(); }
	FGsShortcutHandler* GetShortcutHandler() { return _shortcutHandler.Get(); }
	FGsObserverHandler* GetObserverHandler() { return _observerHandler.Get(); }
	FGsCounterAttackHandler* GetCounterAttackHandler() { return _counterAttackHandler.Get(); }
	class FGsPKBookHandler* GetPKBookHandler() { return _PKBookHandler.Get(); }
	FGsInvenHandler* GetInvenHandler() { return _invenHandler.Get(); }
	class FGsItemFusionHandler* GetItemFusionHandler() { return  _itemFusionHandler.Get(); }
	class FGsBattleArenaGuildWarHandler* GetBattleArenaGuildWarHandler() { return  _battleArenaHandler.Get(); }
	class FGsStatFilterHandler* GetStatFilterHandler() { return _statFilterHandler.Get(); }

private:
	void RestoreInventory();
};
