// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ListView.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Classes/GsPoolUObject.h"
//#include "UTIL/GsIntervalChecker.h"

/**
 * 거래소 리스트 뷰 동작 도움 
 */
class T1PROJECT_API FGsMarketListViewControlBase
{
protected:
	UListView* _listView;
	int32 _offsetIndexCount; // 자연스러운 스크롤을 위해 마지막으로 받은 인덱스에서 뺄 값 
	int32 _lastIndex; // 서버로부터 받은 리스트의 마지막 인덱스
	int32 _currIndex;

	TWeakObjectPtr<UGsPoolUObject> _listItemPool;

public:
	explicit FGsMarketListViewControlBase(UListView* InListView, int32 InOffsetIndexCount, UGsPoolUObject* InListItemPool)
		: _listView(InListView), _offsetIndexCount(InOffsetIndexCount), _lastIndex(0), _currIndex(0)
	{
		_listItemPool = InListItemPool;

		// 타임 락 설정(페이지 경계에서 왔다갔다 할 경우를 대비)
		//_checkerLock.SetIntervalSecond(2.f);
	}
	virtual ~FGsMarketListViewControlBase() = default;

public:
	virtual void ClearData() = 0;

	// T: 해당 스크롤 위치까지 받았음 / F: 못받았음 
	virtual bool UpdateList(bool bInResetScroll, int32 InLastIndex = 0);	

	virtual void SetData(class UGsMarketItemGroupListItem* InData) {}
	virtual void SetData(class UGsMarketItemListItem* InData) {}
	virtual void SetData(class UGsMarketLogListItem* InData) {}
	
	virtual void SortByPrice(bool isDescending) {}
	virtual void SortByPricePerUnit(bool isDescending) {}
	virtual void SortByEnchantLevel() {}
	
	virtual int32 FindDataIndex(ItemId InItemId) { return -1; }
	virtual void AddData(class UGsMarketItemGroupListItem* InData) {}
	virtual void RemoveData(ItemId InItemId) {}
	virtual void ChangeData(int32 InIndex, class UGsMarketItemGroupListItem* InData) {}	
	virtual int32 GetDataCount() const { return 0; }

	// 상위애서 Count개 만큼 선택ㄴ
	virtual void GetTopNTransactionIdList(int32 InCount, OUT TArray<MarketTransactionId>& OutList) {}

public:
	void ScrollToTop();
	bool IsEmptyList();
	// True 리턴 시 다음 페이지 요청이 필요한 것
	bool OnScrolled(float InOffsetInItems);
	void RefreshAll();
	int32 GetCurrScrollIndex() const { return _currIndex; }
};
