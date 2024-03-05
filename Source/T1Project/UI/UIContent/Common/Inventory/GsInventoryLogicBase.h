// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GameObject/Define/GsGameObjectDefine.h"


class UGsUIIconBase;
class UGsUIIconItem;
class FGsItem;

/**
 * 인벤토리 로직(UGsUIInventoryCommon) 에서 사용
 */
class T1PROJECT_API FGsInventoryLogicBase
{
protected:
	EInventoryTab _currTabType = EInventoryTab::AllTab;
	
	ItemDBId _selectedItemDBId = INVALID_ITEM_DB_ID;
	
//	ItemDBId _focusIndex = INDEX_NONE;

public:
	FGsInventoryLogicBase() = default;
	virtual ~FGsInventoryLogicBase() = default;

public:
	virtual void Clear();

public:
	// 2021/08/09 PKT - Update Tick
	virtual void Tick(int32 InDeltaSeconds) {/*None*/}

	// 창 닫힐 때(데이터 비우기에 활용)
	virtual void OnNativeDestruct();
	// 리스트 갱신
	virtual void OnInvalidateList() {}
	// 아이콘 생성
	virtual void OnCreateItemIcon(UGsUIIconBase* InIcon) {}
	// 아이콘 갱신
	virtual void OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon) {}
	// 아이콘 클릭
	virtual void OnClickItemIcon(UGsUIIconItem& InIcon) {}
	// 아이콘 선택
	virtual void OnSelectItemIcon(UGsUIIconItem* InPrevIcon, UGsUIIconItem* InCurrIcon) {}
	// 탭 선택
	virtual void OnSelectTab(EInventoryTab InType);
	// 슬롯 개수 변화
	virtual void OnChangeInvenSlotCount(uint32 InMaxSlotCount, uint32 InTotalExtendCount) {}
	// 슬롯 확장 버튼 클릭
	virtual void OnClickSlotLock() {}

	// 데이터 갯수
	virtual int32 GetDataCount() const { return 0; }
	// 해당 ItemDBId의 인덱스 얻기
	virtual int32 GetDataIndex(ItemDBId InItemDBId) const { return -1; }

public:
	EInventoryTab GetCurrTabType() const { return _currTabType; }
	void SetCurrTabType(EInventoryTab InType) { _currTabType = InType; }

	ItemDBId GetSelectedItemDBId() const { return _selectedItemDBId; }
	void SetSelectedItemDBId(ItemDBId InItemDBId) { _selectedItemDBId = InItemDBId; }

//	virtual void SetFocus(int32 InIndex, UGsUIIconItem* InIcon, bool InbFocus);
//	int32 GetFocusIndex() const;

	virtual ItemAmount GetInventorySlotCount() const;
	virtual ItemAmount GetOwnedItemCount() const;
	virtual int32 GetEnoughExtendSlotCount() const { return 0; }

	virtual bool CheckRedDotByInvenTab(EInventoryTab InTab) { return false; }
};
