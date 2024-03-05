
#pragma once

#include "Classes/GsManager.h"
#include "Management/GsScopeHolder.h"
#include "Message/GsMessageItem.h"
#include "Message/GsMessageGameObject.h"
#include "IDelegateInstance.h"
#include "RedDot/GsRedDotCheck.h"
#include "Currency/GsCostPackage.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


/**
* file		GsInvadeBattleRankManager.h
* @brief	침공전 관련 배틀 랭크 타이틀 관련 메니져. 기존 칭호 메니져에 포함 시킬까 하다 추후 따로 분리 될수 있는 여지가 있어 별도 메니져로 관리.
* @author	PKT
* @date		2022/08/22
**/

class T1PROJECT_API FGsInvadeBattleLevelManager final : public IGsManager, public IGsRedDotCheck
{
private:
	TArray<const struct FGsSchemaInvadeBattleLevel*> _battleLevelHolder;

	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemActionDelegates;

	MsgGameObjHandleArray _listGameObjectDelegate;

	ItemId _attackCurrencyItemId = INVALID_ITEM_ID;
	ItemId _defenceCurrencyItemId = INVALID_ITEM_ID;

	/**
	 * Init / Release
	 */
public:
	virtual void Initialize() override;
	virtual void Finalize() override;

protected:
	bool CheckRedDot(bool bInUpdate = true) override { return false; }

protected:
	void RegisterMessage();
	void UnRegisterMessage();

	/**
	 * Init Setting
	 */
	void LoadBattleLevel();

	/**
	 * Recv Message
	 */
private:
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam* InParam);

	void OnAddItem(struct FGsItemMessageParamBase& InParam);
	void OnUpdateItem(struct FGsItemMessageParamBase& InParam);
	void OnRemoveItem(struct FGsItemMessageParamBase& InParam);

	void SendMessageIfValidCostItem(const ItemId InItemId);

	/**
	 * Send Meesage
	 */
protected:
	void SendMessage();

	/**
	 * UI Display Info
	 */
public:
	const TArray<const struct FGsSchemaInvadeBattleLevel*>& ViewList() const;

	CurrencyDelta GetAttackCurrencyItemCount() const;
	CurrencyDelta GetDefenceCurrencyItemCount() const;

	// 2022/08/25 PKT - 배틀 등급 정보
	const struct FGsSchemaInvadeBattleLevel* FindBattleLevelInfo(InvadeBattleLevel InLevel) const;
	static const struct FGsSchemaInvadeConfig* GetConfigData();
	static const struct FGsSchemaInvadeWorldConfigDataEx* GetInvadeWorldConfigDataEx();
};

#define GsInvadeBattleLevelManager() UGsScopeHolder::GetGameManagerFType<FGsInvadeBattleLevelManager>(UGsScopeGame::EManagerType::InvadeBattleLevel)