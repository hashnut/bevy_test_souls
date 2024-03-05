// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"

#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGame.h"

#include "Message/GsMessageItem.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"

#include "DataSchema/GsSchemaInventoryConfig.h"

#include "RedDot/GsRedDotCheck.h"
#include "GameObject/Define/GsGameObjectDefine.h"

#include "GsInventoryManager.generated.h"

#define RESTORE_INVENTORY

class FGsItem;
class UGsItemManager;
class UGsUIPopupInventory;
/**
 *  Inventory Manager
 */
UCLASS()
class T1PROJECT_API UGsInventoryManager : 
	public UObject,
	public IGsManager,
	public IGsRedDotCheck
{
	GENERATED_BODY()

public:
	// 인벤토리 관련 아이콘 생성할때, 분할로드 기준이 되는 설정값
	// 초당 30프레임을 기준으로 함
	const float FRAME_RATE = 0.031f;
	const uint8 CREATE_MAX_COUNT_BY_TICK = 60; // 틱당 생성하는 itemicon 최대갯수
	const uint8 CREATE_MIN_COUNT_BY_TICK = 5;  // 틱당 생성하는 itemicon 최소 갯수
	const uint8 BALANCE_COUNT = 5; // 추가되거나 차감될때 기준이되는 카운트
	const uint8 CREATE_EVERY_TICK_COUNT_DEFAULT = 30; // 매 틱마다 생성할 아이콘 슬롯 갯수(Default)

private:
	// 인벤토리 탭에 따른 출력할 아이템들의 DBID 정보
	TMap<EInventoryTab, TArray<TPair<ItemType,ItemDBId>>> _invenItemDbidMap;

	TWeakObjectPtr<UGsUIPopupInventory> _invenUIPtr;
	TWeakObjectPtr<UGsItemManager> _itemMgr;

private:
	TArray<TPair<MessageContentInven, FDelegateHandle>>	_listInvenDelegate;
	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemDelegate;
	MsgGameObjHandleArray _listGameObjectDelegate;
	TPair<MessageContents, FDelegateHandle> _contentsDelegate;
	//MsgHudHandle _hudDelegate;


private:
	const FGsSchemaInventoryConfig* _invenConfigData = nullptr;

private:
	uint32 _inventorySlotCount = 0;
	uint32 _totalExtendCount = 0;

	// HUD에서 창 복구 시 사용하는 ItemDBId
	ItemDBId _restoreItemDBId = INVALID_ITEM_DB_ID;

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

public:
	void CreateItemList();
	void ResetItemList();

public:
	void UpdateInvenWeightRate();
	void UpdateItemWeightRate();
	void CheckItemSlotCount();
	void WarmUpInvenIconSlot();

public:
	void OpenInventory(EInventoryTab InTabType);
	void CloseInventoryPopup(bool InIsCallUI, const bool InIsNeedRecord);
	void OnChangeInvenTab(EInventoryTab InTabType);
	void OnUnSelect();

public:
	void SendPenaltyInfoFromPacketData(WeightPenaltyGrade InPenaltyGradeType);	
	void OnChangeInvenSlotCount(uint32 InMaxSlotCount, uint32 InTotalExtendCount);
	void SendExtendSlot(uint8 InExtendLevel);

private:
	void LoadConfigTable();
	void RegisterMsg();
	void RemoveMsg();

private:
	EInventoryTab ConvertTabType(const ItemType InItemType);

private:
	void AddItemList(struct FGsItemMessageParamBase& InParam);
	void UpdateItemList(struct FGsItemMessageParamBase& InParam);
	void RemoveItemList(struct FGsItemMessageParamBase& InParam);
	void OnPlayerStatUpdate(const struct IGsMessageParam* inParam);
	void OpenInventoryTab(const struct IGsMessageParam* inParam);
	void OnChangeIconDimmed(struct FGsItemMessageParamBase& InParam);
	void OnChangeInvenIndex(const struct IGsMessageParam* inParam);
	void OpenInventoryByHUD();
	void OnUserPresetEquipUpdate(const struct IGsMessageParam* inParam);
	
	void InvalidatePlayerStat();

private:
	bool IsOpenInventoryPopup() const;
	bool FindInvenUIptr();

public:
	bool IsInventoryOpen() const;
	bool CheckIsHUDState() const;
	void RestoreInventoryAndDetailPopup();
	void SetRestoreItemDBId(ItemDBId InItemDBId) { _restoreItemDBId = InItemDBId; }
private:
	bool RestoreInventory();

public:
	uint8 GetEnoughExtendSlotCount();
	uint16 GetMaxinumSlotLineCount();
	uint16 GetTotalMaxinumSlotLineCount();
	uint16 GetTotalMaxinumSlotCount();
	CurrencyType GetExtendSlotCurrencyType();
	int32 GetExtendSlotCost();
	int32 GetOwnedItemPenaltyValueByType(const WeightPenaltyGrade InPenaltyGrade);
	TArray<ItemDBId> GetDisplayItemList(const EInventoryTab InTabType);
	ItemDBId GetSelectionItemDbid();
	bool IsEnoughExtendSlotCost();
	bool IsUseLongPress();
	bool IsReactionLockTypeItem(const ItemCategorySub InSubType);

public:
	uint32 GetInvenSlotMaxCountWithTemp();
	uint32 GetInvenSlotMaxCount() { return _inventorySlotCount; }
	void SetInvenSlotMaxCount(uint32 InSlotCount) { _inventorySlotCount = InSlotCount; }

	uint32 GetInvenTotalExtendCount() { return _totalExtendCount; }
	void SetInvenTotalExtendCount(uint32 InCount) { _totalExtendCount = InCount; }

	/************************************************************************/
	/* IGsRedDotCheck implementation                                        */
	/************************************************************************/

public:
	bool CheckRedDot(bool bInUpdate = true) final;

};

#define GSInventory() UGsScopeHolder::GetGameManager<UGsInventoryManager>(UGsScopeGame::EManagerType::Inventory)