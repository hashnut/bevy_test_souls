// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "Management/GsScopeHolder.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsQuickSlotManager.generated.h"

class UGsUIHUDQuickSlot;

/**
 * HUD QuickSlot 包访等 Data 包府窍绰 Manager
 */
UCLASS()
class T1PROJECT_API UGsQuickSlotManager : 
	public UObject,
	public IGsManager
{
	GENERATED_BODY()

public:
	enum SlotRegisterItemStateType : uint8
	{
		Empty = 0,
		OnlyItem,
		OwnedItem,

		StateMax,
	};
	
private:
	TWeakObjectPtr<UGsUIHUDQuickSlot> _hudQuickSlotUI;
	TMap<QuickSlotId,QuickSlotInfo*> _quickSlotInfoMap;

private:
	uint8 _lastSlotPage = 0;
	ItemDBId _fromInvenSelectionDBID = 0;
	QuickSlotId _arrSelectionQuickSlotID[2];

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

private:
	bool isValidUI();

public:
	bool IsOwnedItemBySlotID(const QuickSlotId InSlotID);
	bool IsOwnedItemByItemDBID(const ItemDBId InItemDBID);
	bool IsExistSlotItemByItemTID(const int32 InItemTID);
	bool IsDimmedRegistItem(const QuickSlotId InSlotID);
	bool IsConsumableItem(const QuickSlotId InSlotID);
	bool IsIngredientItem(const QuickSlotId InSlotID);
	bool IsEquippedItem(const QuickSlotId InSlotID);
	bool IsAllowedAutoUseItem(const QuickSlotId InSlotID);

public:
	bool SetAutoState(const QuickSlotId InSlotID);

public:
	void FindAllSlotIdByItemTID(const int32 InItemTID , OUT TArray<uint8>& outSlotIDList);
	void FindAllSlotIdByItemDBID(const uint64 InItemDBID, OUT TArray<uint8>& outSlotIDList);
	void FindAllNotOwnedSlotIdByItemTID(const int32 InItemTID, OUT TArray<uint8>& outSlotIDList);

public:
	void SetPacketData(QuickSlotId InslotId, ItemDBId InItemDBID, ItemId InItemTID, bool isAutoState);
	void SetPacketData(QuickSlotId InslotId, ItemDBId InItemDBID, bool isAutoState);

public:
	void InvalidateAll();
	void InvalidateSlotList(TArray<QuickSlotId>& InInvalidateSlotIDList);
	void InvalidateAutoState(TArray<QuickSlotId>& InSlotIdList);
	void CheckSelectionInfo();
	void CheckSound(const bool InIsEquipped);

public:
	void ResetQuickSlotOwnedItemInfo(const uint64 InRemoveItemDBID, const int32 InItemTID ,OUT TArray<uint8>& outSlotIDList);
	void ResetSelectionInfo(const QuickSlotId InSlotId);
	void ResetAllSelectionInfo();
	void ResetQuickSlotSelectionInfo();

public:
	void CloseInven();
	void OnRemoveInvenItem(const ItemDBId InItemDbid);

public:
	void CompleteLoadUI();
	void SetLastSlotPage(const uint8 InPage) { _lastSlotPage = InPage; }
	void SetInvenSelectionItem(const uint64 InItemDBID);
	void SetQuickSlotSelectionItem(const QuickSlotId InSlotID, const bool InIsEmptySlot);

public:
	const QuickSlotInfo* GetQuickSlotInfoBySlotID(const QuickSlotId InSlotID);
	const SlotRegisterItemStateType GetSlotRegisterStateType(const QuickSlotId InSlotID);
	const uint8 GetLastQuickSlotPage() { return _lastSlotPage; }
	const bool IsEditMode();
	
};

#define GSQuickSlot() UGsScopeHolder::GetGameManager<UGsQuickSlotManager>(UGsScopeGame::EManagerType::QuickSlot)