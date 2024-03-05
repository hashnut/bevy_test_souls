// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUITrayOfflinePlayResults.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Message/GsMessageContents.h"
#include "Management/GsMessageHolder.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Core/Public/Containers/UnrealString.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Reward/GsRewardHelper.h"


void UGsUITrayOfflinePlayResults::BeginDestroy()
{
	// 2021/06/21 PKT - release
	if (_slotHelper)
	{
		_slotHelper->OnRefreshIcon.RemoveDynamic(this, &UGsUITrayOfflinePlayResults::OnRefreshIcon);
		_slotHelper = nullptr;
	}
	
	if (_btnOk)
	{
		_btnOk->OnClicked.RemoveDynamic(this, &UGsUITrayOfflinePlayResults::OnCloseTray);
		_btnOk = nullptr;
	}	

	_refreshItemList.Empty();

	Super::BeginDestroy();
}

void UGsUITrayOfflinePlayResults::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 2021/06/21 PKT - create & bind
	_slotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_slotHelper->Initialize(_iconSelector);
	_slotHelper->OnRefreshIcon.AddDynamic(this, &UGsUITrayOfflinePlayResults::OnRefreshIcon);

	// 2021/06/18 PKT - bind
	_btnOk->OnClicked.AddDynamic(this, &UGsUITrayOfflinePlayResults::OnCloseTray);
}

void UGsUITrayOfflinePlayResults::NativeConstruct()
{
	Super::NativeConstruct();

	_closeDelegate = nullptr;
}

void UGsUITrayOfflinePlayResults::NativeDestruct()
{	
	 // 2021/06/22 PKT - reset
	_refreshItemList.Empty();
	_iconSelector->RemoveAllChildren();
	_slotHelper->Clear();

	Super::NativeDestruct();
}

void UGsUITrayOfflinePlayResults::OnCloseTray()
{
	if (_closeDelegate)
	{	// 2022/11/02 PKT - Close Event
		_closeDelegate();
	}

	Close();
}

void UGsUITrayOfflinePlayResults::OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	if (false == _refreshItemList.IsValidIndex(InIndex))
	{
#if WITH_EDITOR
		GSLOG(Error, TEXT("UGsUITrayOfflinePlayResults::OnRefreshIcon : invaild index [%d]"), InIndex);
#endif
		return;
	}

	TWeakObjectPtr<UGsUIIconItemInventory> itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (true == itemIcon.IsValid())
	{
		const ItemDataCreated& data = _refreshItemList[InIndex];
		itemIcon->SetNotOwnItem(data.mTemplateId, data.mItemAmount);
		itemIcon->SetItemNameVisibility(false);
		if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUITrayOfflinePlayResults::OnLongPressIcon);
		}
	}
}

void UGsUITrayOfflinePlayResults::SetPlayTime(time_t InPlayTime)
{
	FDateTime dateTime = FGsTimeSyncUtil::ConvertToDateTime(InPlayTime);
	FString textRrunningTime = FString::Printf(TEXT("%02d:%02d:%02d"), dateTime.GetHour(), dateTime.GetMinute(), dateTime.GetSecond());
	// 2021/06/23 PKT - set..
	_textRuningTime->SetText(FText::FromString(textRrunningTime));
}

void UGsUITrayOfflinePlayResults::SetIncreasedGold( Currency InAmount)
{
	// 2021/06/23 PKT - set..
	_pickupCurrency->SetData(CurrencyType::GOLD, InAmount);
}

void UGsUITrayOfflinePlayResults::SetIncreasedExp(OffLineExp InIncreasedEXP)
{
	/**
	 * 1. 현재 경험치를 얻는다.
	 * 2. Offline Play 이전 경험치를 얻는다.
	 * 3. 이전 경험치 / 현재 경험치를 화면에 출력
	 */

	uint64 previousExp = 0;
	LevelExpData currentLevelExp;

	// 2021/06/21 PKT - User Data ( 현재 경험치를 얻기 위한... )
	const FGsNetUserData* userData = (nullptr != GGameData()) ? GGameData()->GetUserData() : nullptr;
	if (nullptr == userData)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("UGsUITrayOfflinePlayResults::SetData : invaild userData == nullptr"));
#endif
	}
	else
	{
		// 현재 값
		currentLevelExp.Key = userData->mLevel;
		currentLevelExp.Value = GGameData()->GetCurrentLevelExpRate();

		// 2021/06/21 PKT - 이전 경험치 : 현재 경험치 - 오프라인 플레이 중 획득 경험치		
		previousExp = FMath::Max<uint64>(0, userData->mExp - InIncreasedEXP);
	}	

	Level level = FGsRewardHelper::GetLevelByExp(previousExp);
	LevelExpData previousLevelExp;
	previousLevelExp.Key = level;
	previousLevelExp.Value = FGsRewardHelper::GetExpRate(level, previousExp, true);
	
	// 2021/06/21 PKT - 중복 코드를 막기 위해 람다 사용
	auto SetUI = [](const LevelExpData& InLevelExp, UTextBlock* InLevel, UTextBlock* InPercent)
	{
		FString buffer = (InLevelExp.Key == -1) ?
			FString("") : FString::Printf(TEXT("Lv.%d"), InLevelExp.Key);
		// 2021/06/21 PKT - Level
		InLevel->SetText(FText::FromString(buffer));

		buffer = FString::Printf(TEXT("%.5f%%"), InLevelExp.Value * 100.0f);
		InPercent->SetText(FText::FromString(buffer));
	};

	// 2021/06/23 PKT - Set UI...
	SetUI(previousLevelExp, _textBeforeLevel, _textBeforeExpPercent);
	SetUI(currentLevelExp, _textAfterLevel, _textAfterExpPercent);
}

void UGsUITrayOfflinePlayResults::SetIncreasedItem(const TArray<ItemDataCreated>& InItemList)
{
	// 2021/06/23 PKT - 기존 아이템 정보 삭제...
	_refreshItemList.Empty();

	for (const ItemDataCreated& item : InItemList)
	{
		// 2021/06/21 PKT - add...
		_refreshItemList.Emplace(item);
	}

	int32 maxCount = _refreshItemList.Num();
	// 2021/06/23 PKT - icon create / set
	_slotHelper->RefreshAll(maxCount);
}

void UGsUITrayOfflinePlayResults::OnLongPressIcon(class UGsUIIconItem& InIcon)
{
	FGsItemDetailOpenMessageParam param(InIcon.GetItemIconData(), true, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
}

void UGsUITrayOfflinePlayResults::SetData(const FGsOfflinePlayResultData& InData, TFunction<void(void)> InCloseEvent)
{
	_closeDelegate = InCloseEvent;
	 /**
	  * 2021/06/21 PKT - 오프라인 플레이 이용 시간 
	  */
	SetPlayTime(InData._runningTime);

	/**
	 * 2021/06/21 PKT - 오프라인 플레이 획득 재화(골드) 정보
	 *					골드 외 재화가 사냥으로 벌어 들이는 경우가 있을까??? 꼭 사냥이 아니더라도...
	 */
	SetIncreasedGold(InData._CcurrencyTable.mGold);

	/**
	 * 2021/06/21 PKT - 오프라인 플레이 획득 경험치
	 */
	SetIncreasedExp(InData._Exp);

	/**
	 * 2021/06/21 PKT - 그 외 모든 아이템 정보 출력
	 */
	SetIncreasedItem(InData._itemList);
}
