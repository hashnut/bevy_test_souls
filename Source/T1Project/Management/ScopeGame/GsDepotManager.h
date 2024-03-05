// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsManager.h"
#include "Item/GsItem.h"
#include "Item/GsItemBuffers.h"
#include "DataSchema/GsSchemaDepotConfig.h"

struct FGsSchemaDepotConfig;
struct ItemData;
struct ItemDataCreated;
struct ItemDataEquip;
/**
 * 창고 매니저
 */
class T1PROJECT_API FGsDepotManager final : public IGsManager
{
public:
	const int32 BASKET_MAX_SLOT_COUNT = 30;

public:
	enum DepotActionState : uint8
	{
		State_Deposit = 0,
		State_Take,

		State_Max,
	};


private:
	// 창고 아이템 buffers : 창고아이템은 기존 인벤토리 패킷에서 내려오지 않기에 별도 관리
	FGsItemBuffers* _depotItemBuffers = new FGsItemBuffers();

private:
	// 창고 물품 이동 리스트에 사용될 데이터 리스트
	TArray<TPair<uint64, ItemAmount>> _depotBasketItemInfoList;

private:
	const FGsSchemaDepotConfig* _depotConfigTableData = nullptr;

private:
	uint16 _depotMaxSlotCount = 0;
	uint32 _npcCreatureID = 0;
	uint8 _extendedSlotNumber = 0;
	TMap<uint32,bool> _autoSettingDataMap;
	TMap<int32, ItemGrade> _autoDepositGradeMap;
	DepotActionState _eDepotState = DepotActionState::State_Deposit;

public:
	//IGsManager
	virtual void Initialize() override;
	virtual void Finalize() override;

public:
	void ResetData();
	void ResetBasketItems();

public:
	void SendDepositItems();
	void AckDepotPacket();
	void SendTakeItems();
	void SendExtendDepotSlot(uint8 InExtendLevel);

private:
	void AddItem(const ItemData* In_addItem);
	void AddItem(const ItemDataEquip* In_addItem);
	void AddItemList(const TArray<ItemData>& InItemList);
	void AddItemList(const TArray<ItemDataEquip>& InItemList);
	void AddItemList(const TArray<ItemDataCreated>& InItemList);

public:
	void AddOrUpdateDepotItem(const ItemData* In_addItem);
	void AddOrUpdateDepotItem(const ItemDataEquip* In_addItem);
	void AddOrUpdateDepotItem(const uint64 InItemDBID, const ItemAmount InAmount);
	bool AddOrUpdateDepotBasketItem(const uint64 InItemDBID , const ItemAmount InAmount , const DepotActionState InStateType);
	void UpdateDepotBasketItemList(const TArray<ItemDBId>& InItemDBIdList);

public:
	void RemoveDepotItem(const uint64 InItemDBID);
	void RemoveDepotItemList(const TArray<ItemDBId>& InItemDBIdList);
	void RemoveDepotBasketItem(const uint64 InItemDBID);

public:
	// TPair<itemTID,flag> : 자동 보관 설정 data 저장
	void SaveAutoDepositSettingData(TMap<ItemId, bool>& InSettingDataMap, const TArray<TPair<int32, ItemGrade>>& InGroupGradeMap);

	ItemGrade GetAutoDepositGrade(int32 InGroupId);

public:
	void AutoDepositProc();
	void SortItemListByDBID(uint8 InSortTypeIndex, OUT TArray<uint64>& outDBIDList);
	void OnChangeAutoDepositSettingData(TArray<ItemId>& InItemIdList);
	void OnChangeAutoDepositGradeData(TArray<AutoDepotConfigPair>& InAutoDepotConfigList);

public:
	TWeakPtr<FGsItem> FindDepotItemByDBID(const uint64 In_ItemDBID, const ItemType In_ItemType = ItemType::MAX);

public:
	void GetItemDBIDByStorageType(EInventoryTab InTabType, uint8 InSortingTypeIndex, OUT TArray<uint64>& outDBIDList);
	TArray<TWeakPtr<FGsItem>> FindByTID(const ItemId In_ItemTID, const ItemType In_ItemType = ItemType::MAX);
	bool GetBasketItemInfoBySlotPageIndex(uint8 InStartIndexNum, uint8 InSlotIndex, OUT uint64& outDBID, OUT ItemAmount& outAmount);
	int32 GetBasketItemsPrice();
	int32 GetExpectWeight();
	int32 GetBasketTotalWeight();
	int32 GetItemTotalWeightInBasket(const ItemDBId InItemDBID);
	int32 GetDepotWithdrawCost() const;
	int32 GetBasketItemCount() const;
	uint16 GetEnoughDepotExtendLineCount() const;
	uint8 GetExtendedSlotNumber() const  { return _extendedSlotNumber; }
	bool GetItemIndexInBasketByDBID(const uint64 InItemDBID, OUT int32& outIndex);
	uint16 GetOwnedItemSlotCount() const;

public:
	uint16 GetDepotMaxSlotCount() { return _depotMaxSlotCount; }
	
	const FGsSchemaDepotConfig* GetDepotConfigTableData() { return _depotConfigTableData; }
	bool GetNextExtendSlotCurrenyData(OUT CurrencyPair& outPairData);
	void GetExtendSlotCurrenyData(OUT CurrencyPair& outPairData);
	Currency GetExtendSlotCurrency(int InSlotLevel);

public:
	void SetNpcCreatureID(uint32 InCreatureID) {_npcCreatureID = InCreatureID; }
	void SetDepotMaxSlotCount(uint16 InSlotCount) { _depotMaxSlotCount = InSlotCount; }
	void SetCurExtendedSlotNumber(uint8 InNumber) { _extendedSlotNumber = InNumber; }

public:
	const TMap<ItemType, TMap<uint64, TSharedPtr<FGsItem>>>& GetItemBuffers() const;

public:
	
	bool IsSelectedAutoSlot(const uint32 InItemTID);
	bool IsLowerOrEqualAutoDepositGrade(int32 InGroupId, ItemGrade InGrade);
	bool IsNeedCalculator(const uint64 InItemDBID, bool InIsFromInventory);
	bool IsFullBasketSlot();
	bool IsFullExpectWeight();
	bool IsFullDepotSlot();
	bool IsFullInvenSlot();
	bool IsEnoughExtendDepotSlotCost();
	bool CanExtendDepotSlot();
	bool IsExistItemInBasketByDBID(const uint64 InItemDBID);
	bool IsAllowedDepositItem(const uint64 InItemDBID);

private:
	FGsItem* GetGsItemLowPointer(TWeakPtr<FGsItem> InWeakPtr);

private:
	static ItemType GetItemTypeByTID(const int32 In_ItemTID);

private:
	// 요청한 창고 상태와 다르면 변경하고 정보를 초기화한다. (보관 <-> 찾기)
	void ChangeDepotState(DepotActionState InState);

};

#define GSDepot() UGsScopeHolder::GetGameManagerFType<FGsDepotManager>(UGsScopeGame::EManagerType::Depot)