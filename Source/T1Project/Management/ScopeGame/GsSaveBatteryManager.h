// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "Management/GsScopeHolder.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


/**
 *  절전 모드 데이터 관리 매니저
 */
class T1PROJECT_API FGsSaveBatteryManager final : public IGsManager
{
private:
	struct DisplayItem
	{
	public:
		int32 _itemTID;
		ItemAmount _itemAmount;

		DisplayItem() = default;
		explicit DisplayItem(int32 InItemTID, ItemAmount InItemAmount)
			:
			_itemTID(InItemTID), _itemAmount(InItemAmount)
		{
		}
	};

private:
	// 인벤토리 아이템 상태 ( 스택 가능 아이템만 )
	TMap<int32, ItemAmount> _itemAmountStateMap;
	TArray<ItemDBId> _nonStackableItemList;
	// 출력할 아이템들 TID 리스트
	TArray<DisplayItem> _displayItemList;
	// 출력되는 스택형 아이템의 개수 관리
	TMap<int32, ItemAmount> _displayItemAmountMap;
	// 자동 분해로 얻은 아이템 수량 정보
	TMap<int32, ItemAmount> _autoDecomposeItemAmountMap;

private:
	Exp _increasedExp = 0;
	Currency _increasedGold = 0;

	bool _isReStart = false;
	FDateTime _startTime;

	bool _isKeyboardPressed = 0;

	/************************************************************************/
	/* Get & Set                                                            */
	/************************************************************************/
public:
	void ResetData();

	Exp IncreaseExp(Exp InExp);
	Currency IncreaseGold(Currency InAmount);

	ItemAmount GetItemAmountByTID(int32 InItemTID);
	void SetItemAmount(int32 InItemTID, ItemAmount InItemAmount);

	bool IsSaveBatteryTurnedOn();

	void SetIsKeyboardPressed(bool InIsPressed) { _isKeyboardPressed = InIsPressed; }
	bool IsKeyboardPressed() { return _isKeyboardPressed; }

protected:
	void LoadInventoryState();

	/************************************************************************/
	/* 재시작 여부 및 시작 시간                                               */
	/************************************************************************/
public:
	bool IsReStart() { return _isReStart; }
	void SetReStart(bool InReStart) { _isReStart = InReStart; }

	FDateTime* GetStartTime() { return &_startTime; }

	/************************************************************************/
	/* 절전 모드 인벤토리 아이템                                              */
	/************************************************************************/
public:
	bool AddItem(const uint64 InItemDBID, OUT TArray<int32>& OutIndexList);
	bool FindLastItem(int32 InItemTID, OUT uint16& OutIndex);
	bool GetDisplayItemByIndex(int32 InIndex, OUT int32& OutTID, OUT ItemAmount& OutAmount);
	int32 GetDisplayItemCount();
	
	void AddAutoDecomposeItemAmount(ItemId InItemTID, ItemAmount InItemAmount);
	void ShowAndClearAutoDecomposeItemAmount();
	
};

#define GSSaveBattery() UGsScopeHolder::GetGameManagerFType<FGsSaveBatteryManager>(UGsScopeGame::EManagerType::SaveBattery)