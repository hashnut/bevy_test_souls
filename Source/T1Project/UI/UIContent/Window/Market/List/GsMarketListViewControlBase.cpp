// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMarketListViewControlBase.h"
#include "UI/UIContent/Window/Market/GsUIMarketItemListEntryBase.h"


bool FGsMarketListViewControlBase::UpdateList(bool bInResetScroll, int32 InLastIndex)
{
	_lastIndex = InLastIndex;

	if (bInResetScroll)
	{
		_listView->ScrollToTop();
		_currIndex = 0;
	}
	else
	{
		// 현재 스크롤 위치까지 다 받지 않았음
		// _offsetIndexCount를 더하는 이유는 _currIndex가 스크롤 상단값이므로, 
		// 화면에 끝까지 보여지도록 체크하기 위해 임의로 넣은 것
		// 총 데이터 개수보다 클 수는 없으므로 Clamp 처리
		int32 targetIndex = FMath::Clamp(_currIndex + _offsetIndexCount, 0, GetDataCount() - 1);
		if (InLastIndex < targetIndex)
		{
			return false;
		}
	}	

	return true;
}

void FGsMarketListViewControlBase::ScrollToTop()
{
	_listView->ScrollToTop();
	_currIndex = 0;
}

bool FGsMarketListViewControlBase::IsEmptyList()
{
	return (0 == _listView->GetNumItems()) ? true : false;
}

bool FGsMarketListViewControlBase::OnScrolled(float InOffsetInItems)
{
	int32 prevIndex = _currIndex;
	_currIndex = FMath::FloorToInt(InOffsetInItems);
	if (prevIndex < _currIndex)
	{
		int offsetIndex = FMath::Clamp(_lastIndex - _offsetIndexCount, 0, GetDataCount());

		if (offsetIndex > prevIndex &&
			offsetIndex <= _currIndex)
		{
			return true;
		}
	}

	return false;
}

void FGsMarketListViewControlBase::RefreshAll()
{
	// UObject가 동일하면 NativeOnListItemObjectSet 가 불리지 않음.
	// 모든 아이템을 강제로 갱신해준다
	const TArray<UUserWidget*> entryList = _listView->GetDisplayedEntryWidgets();
	for (UUserWidget* widget : entryList)
	{
		if (UGsUIMarketItemListEntryBase* listEntry = Cast<UGsUIMarketItemListEntryBase>(widget))
		{
			listEntry->RefreshAll();
		}
	}
}
