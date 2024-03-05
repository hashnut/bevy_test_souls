// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRewardIconListDefault.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"
#include "Reward/GsRewardHelper.h"


void UGsUIRewardIconListDefault::BeginDestroy()
{
	_listHelper = nullptr;
	_toggleGroup.Clear();
	_dataList.Empty();

	Super::BeginDestroy();
}

void UGsUIRewardIconListDefault::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 보상 슬롯 관리
	_listHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_listHelper->Initialize(_iconSelector);
	_listHelper->OnCreateIcon.AddDynamic(this, &UGsUIRewardIconListDefault::OnCreateIcon);
	_listHelper->OnRefreshIcon.AddDynamic(this, &UGsUIRewardIconListDefault::OnRefreshIcon);

	// 보상 슬롯 선택 시 토글 관리
	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIRewardIconListDefault::OnClickRewardIcon);
}

void UGsUIRewardIconListDefault::NativeDestruct()
{
	_listHelper->Clear();
	_toggleGroup.Clear();
	_iconSelector->RemoveAllChildren();
	_dataList.Empty();

	Super::NativeDestruct();
}

void UGsUIRewardIconListDefault::SetData(const FGsRewardUnboxingData* InData)
{
	Super::SetData(InData);

	_dataList.Empty();
	FGsRewardHelper::GetIconDataList(_rewardData, _dataList);
	
	InvalidateIconList();
}

ItemBagIndex UGsUIRewardIconListDefault::GetSelectedIndex() const
{
	if (GetCanSelectIcon())
	{
		return _toggleGroup.GetSelectedIndex();
	}

	return INVALID_ITEM_BAG_INDEX;
}

void UGsUIRewardIconListDefault::InvalidateIconList()
{
	if (nullptr == _rewardData)
	{
		return;
	}

	// 보상 아이템 아이콘 리스트 표시
	_listHelper->RefreshAll(_dataList.Num());

	// 아이템 슬롯 토글 세팅, 처음엔 선택슬롯 없도록 -1 세팅	
	if (GetCanSelectIcon())
	{
		_toggleGroup.Clear();
		_toggleGroup.AddToggleWidgetByParentPanel(_iconSelector);
		_toggleGroup.SetSelectedIndex(-1);
	}
}

void UGsUIRewardIconListDefault::OnCreateIcon(UGsUIIconBase* InIcon)
{
	UGsUIIconItemReward* itemIcon = Cast<UGsUIIconItemReward>(InIcon);
	if (itemIcon)
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIRewardIconListDefault::OnLongPressIcon);
		// UGsUIIconItem::NativeDestruct 에서 OnFinishedLongPressEvent.Clear 수행하므로 따로 지워줄 필요는 없다
	}
}

void UGsUIRewardIconListDefault::OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	if (false == _dataList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUIIconItemReward* itemIcon = Cast<UGsUIIconItemReward>(InIcon))
	{
		itemIcon->SetEnableDetailPopup(_bEnableDetailPopup);
		itemIcon->SetCenterPopup(_bCenterPopup);

		// 아이콘에 데이터 세팅
		FGsRewardIconDataWrapper::SetUIIconItem(&_dataList[InIndex], itemIcon);
	}
}

void UGsUIRewardIconListDefault::OnClickRewardIcon(int32 InIndex)
{
	// 토글기능을 풀려있을 때만 클릭한 인덱스 선택
	if (GetCanSelectIcon())
	{
		_toggleGroup.SetSelectedIndex(InIndex);
	}
}

bool UGsUIRewardIconListDefault::GetCanSelectIcon() const
{
	if (_bEnableToggleGroup)
	{
		if (_rewardData)
		{
			if (RewardType::PICK_ONE == _rewardData->_rewardType ||
				RewardType::SUB_PICK_ONE == _rewardData->_rewardType)
			{
				return true;
			}
		}
	}

	return false;
}
