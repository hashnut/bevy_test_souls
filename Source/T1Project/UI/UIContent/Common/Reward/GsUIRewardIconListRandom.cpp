// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRewardIconListRandom.h"
#include "Components/ListView.h"
#include "UI/UIContent/Common/Reward/GsRewardIconListRandomListItem.h"
#include "UI/UIContent/Common/Reward/GsUIRewardIconListRandomListEntry.h"
#include "Classes/Curves/CurveFloat.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Sound/GsSoundPlayer.h"
#include "Reward/GsRewardHelper.h"

const int32 REWARD_RANDOM_SLOT_COUNT = 5;


void UGsUIRewardIconListRandom::BeginDestroy()
{
	if (_poolItem)
	{
		_poolItem->RemovePool();
		_poolItem = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIRewardIconListRandom::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_poolItem = NewObject<UGsPoolUObject>(this);

	_listView->OnListViewScrolled().AddUObject(this, &UGsUIRewardIconListRandom::OnScrollListView);
}

void UGsUIRewardIconListRandom::NativeDestruct()
{
	_selectedIndex = INVALID_ITEM_BAG_INDEX;
	_dataList.Empty();

	for (UGsRewardIconListRandomListItem* item : _itemList)
	{
		item->Clear();
	}
	_itemList.Empty();

	if (_poolItem)
	{
		_poolItem->ReleaseAll();
	}

	_bIsPlayingAnimation = false;
	_currTime = 0.f;
	_totalDist = 0.f;
	_maxTime = 0.f;
	
	if (0 < _soundId)
	{
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			if (soundPlayer->IsPlaying(_soundId))
			{
				soundPlayer->StopSound(_soundId);
			}
		}
		_soundId = 0;
	}

	Super::NativeDestruct();
}

void UGsUIRewardIconListRandom::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (false == _bIsPlayingAnimation)
	{
		return;
	}

	_currTime += InDeltaTime;
	if (_currTime >= _maxTime)
	{
		_bIsPlayingAnimation = false;

		_listView->SetScrollOffset(_totalDist);

		EndScrollAnimation();
	}
	else
	{
		float currValue = _curve->GetFloatValue(_currTime);
		float offset = currValue * _totalDist;

		_listView->SetScrollOffset(offset);
	}
}

void UGsUIRewardIconListRandom::SetData(const FGsRewardUnboxingData* InData)
{
	Super::SetData(InData);

	_rewardData = InData;

	_itemList.Empty();
	_poolItem->ReleaseAll();
	_dataList.Empty();
	FGsRewardHelper::GetIconDataList(_rewardData, _dataList);
	
	// 데이터 넣기
	SetItemList();

	// 리스트 갱신
	_listView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_listView->SetListItems(_itemList);

	InvalidateList();

	// 애니메이션 막기
	_bIsPlayingAnimation = false;
	_currTime = 0.f;
	_totalDist = 0.f;
	_maxTime = 0.f;

	_panelEffect->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIRewardIconListRandom::SetReceivedIndex(ItemBagIndex InIndex)
{
	_selectedIndex = InIndex;

	if (INVALID_ITEM_BAG_INDEX == InIndex)
	{
		return;
	}

	_panelEffect->SetVisibility(ESlateVisibility::HitTestInvisible);

	_itemList.Empty();
	_poolItem->ReleaseAll();
	_dataList.Empty();
	FGsRewardHelper::GetIconDataList(_rewardData, _dataList);

	// 룰렛 연출을 위해 ListView에 _repeatCount만큼 생성
	for (int i = 0; i < _repeatCount; ++i)
	{
		SetItemList();
	}

	// 리스트 갱신
	_listView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_listView->SetListItems(_itemList);

	InvalidateList();

	// 애니메이션
	_bIsPlayingAnimation = true;
	_currTime = 0.f;

	// 마지막에서 2번째 세트에서 멈춘다. 마지막 슬롯 우측에도 슬롯이 보이게 하기 위함
	// Index에서 -2를 하여 중앙으로 가게 만든다
	_totalDist = (_repeatCount - 2) * REWARD_RANDOM_SLOT_COUNT;
	_totalDist += InIndex - 2;
	_totalDist += _offsetX;

	float minTime = 0.f;
	_curve->GetTimeRange(minTime, _maxTime);

	StartScrollAnimation();
}

void UGsUIRewardIconListRandom::SetItemList()
{
	for (int i = 0; i < REWARD_RANDOM_SLOT_COUNT; ++i)
	{
		UGsRewardIconListRandomListItem* listItem = _poolItem->Claim<UGsRewardIconListRandomListItem>();

		if (_dataList.IsValidIndex(i))
		{
			listItem->SetData(&_dataList[i], _bCenterPopup, _bEnableDetailPopup);
		}
		else
		{
			listItem->SetData(nullptr, _bCenterPopup, _bEnableDetailPopup);
		}

		_itemList.Emplace(listItem);
	}
}

void UGsUIRewardIconListRandom::InvalidateList()
{
	const TArray<UUserWidget*> entryList = _listView->GetDisplayedEntryWidgets();
	for (UUserWidget* widget : entryList)
	{
		if (UGsUIRewardIconListRandomListEntry* listEntry = Cast<UGsUIRewardIconListRandomListEntry>(widget))
		{
			listEntry->RefreshAll();
		}
	}
}
void UGsUIRewardIconListRandom::StartScrollAnimation()
{
	// 사운드재생
	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		_soundId = soundPlayer->PlaySound2D(TEXT("UI_Slot_Start"));
	}

	OnStartScrollAnimation();
}

void UGsUIRewardIconListRandom::EndScrollAnimation()
{
	// 사운드재생
	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		if (soundPlayer->IsPlaying(_soundId))
		{
			soundPlayer->StopSound(_soundId);
		}

		_soundId = soundPlayer->PlaySound2D(TEXT("UI_Slot_Win"));
	}

	OnEndScrollAnimation();
}

void UGsUIRewardIconListRandom::OnScrollListView(float InOffsetInItems, float InDistanceRemaining)
{
	// 애니메이션 중이 아닐 땐 스크롤을 막음
	if (false == _bIsPlayingAnimation)
	{
		_listView->SetScrollOffset(_totalDist);
	}
}
