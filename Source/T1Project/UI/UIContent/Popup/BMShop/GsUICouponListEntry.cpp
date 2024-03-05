// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/BMShop/GsUICouponListEntry.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "GameObject/Define/GsGameObjectDefine.h"



void UGsUICouponListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _iconSlotHelper)
	{
		_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconSlotHelper->Initialize(_iconSelector);
		_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUICouponListEntry::OnRefreshItemIcon);
	}

	_btnSelect->OnClicked.AddDynamic(this, &UGsUICouponListEntry::OnClickCouponList);
}

void UGsUICouponListEntry::BeginDestroy()
{
	if (nullptr != _iconSlotHelper)
	{
		_iconSlotHelper->Clear();
		_iconSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUICouponListEntry::OnShow()
{
	_isRunningRemainTime = false;
	_isSelected = false;
	_couponItemDbid = 0;
	_leftDays = 0;

}

void UGsUICouponListEntry::OnHide()
{
	_isRunningRemainTime = false;
	_isSelected = false;
	_couponItemDbid = 0;
	_leftDays = 0;

	if (OnClickCoupon.IsBound())
	{
		OnClickCoupon.Clear();
	}
	if (OnExpiredRemainTime.IsBound())
	{
		OnExpiredRemainTime.Clear();
	}

	_iconSelector->RemoveAllChildren();

	_coolTimeMgr = nullptr;
}

void UGsUICouponListEntry::SetCouponInfo(const uint64 InCouponItemDbid, const bool InIsSelected)
{
	if (0 >= InCouponItemDbid)
		return;
	
	_coolTimeMgr = GCoolTime();

	_isSelected = InIsSelected;
	_couponItemDbid = InCouponItemDbid;

	TWeakPtr<FGsItem> itemData = GItem()->FindItem(_couponItemDbid);
	if (false == itemData.IsValid())
		return;

	//Item Name
	_itemNameText->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(itemData.Pin()->GetGradeType()));
	_itemNameText->SetText(itemData.Pin()->GetName());

	//Remain Time
	SetRemainTime(itemData.Pin().Get());

	//Icon
	_iconSlotHelper->RefreshAll(1);

	_selectionImage->SetVisibility(InIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUICouponListEntry::SetSelectionImg(const bool InFlag)
{
	_isSelected = InFlag;
	_selectionImage->SetVisibility(InFlag ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUICouponListEntry::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);

	if (_isRunningRemainTime)
	{
		if (nullptr == _coolTimeMgr)
		{
			_coolTimeMgr = GCoolTime();
		}

		//FTimespan span = _remainDateTime - FGsTimeSyncUtil::GetServerNowDateTimeUTC();
		double coolTime = (double)_coolTimeMgr->GetCoolTime(EGsCoolTime::ItemPeriod, _couponItemDbid, 0);
		FTimespan span = FTimespan::FromSeconds(coolTime);
		if (1 <= span.GetTotalSeconds())
		{
			FText leftText;
			FText::FindText(TEXT("BMShopText"), TEXT("leftSaleTime"), leftText); // 남음
			if (1 <= span.GetDays())
			{
				if (span.GetDays() == _leftDays)
					return;

				// *일 만 표기
				FText findText;
				FText::FindText(TEXT("BMShopText"), TEXT("timeDay"), findText); // 일
				FString result = FString::FromInt(span.GetDays()) + findText.ToString();
				FText resultText = leftText.Format(leftText, FText::FromString(result));
				_remainTimeText->SetText(resultText);

				_leftDays = span.GetDays();
			}
			else
			{
				FString timeStr;
				FGsTimeStringHelper::GetTimeStringHMS(span, timeStr, false);

				FText resultText = leftText.Format(leftText, FText::FromString(timeStr));
				_remainTimeText->SetText(resultText);
			}
		}
		else
		{
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Expired_End_03"), findText);
			_remainTimeText->SetText(findText);
			if (OnExpiredRemainTime.IsBound())
			{
				OnExpiredRemainTime.Broadcast(_couponItemDbid);
			}
			_isRunningRemainTime = false;
		}
	}
}

void UGsUICouponListEntry::SetRemainTime(const FGsItem* InCouponItem)
{
	_isPeriodCoupon = InCouponItem->IsPeriodItem();
	_remainTimeText->SetVisibility(_isPeriodCoupon ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	_remainTimeImage->SetVisibility(_isPeriodCoupon ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (false == _isPeriodCoupon)
		return;

	if (true == InCouponItem->IsExpiredTime()) // 기간 만료상태
	{
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Expired_End_03"), findText);
		_remainTimeText->SetText(findText);
		if (OnExpiredRemainTime.IsBound())
		{
			OnExpiredRemainTime.Broadcast(_couponItemDbid);
		}
		_isRunningRemainTime = false;
	}
	else
	{
		FTimespan timeSpan;
		if (true == InCouponItem->GetExpiredRemainTime(timeSpan))
		{
			InCouponItem->GetConvertServerUTCExpiredTime(_remainDateTime);
			_isRunningRemainTime = true;

			/*
			FText leftText;
			FText::FindText(TEXT("BMShopText"), TEXT("leftSaleTime"), leftText); // 남음
			// 만료시간이 24시간 이내인 경우 남은 시간 표기
			// 만료시간이 24시간 이상인 경우 *일 만 표기
			int64 hour = timeSpan.GetTicks() / ETimespan::TicksPerHour;
			if (24 < hour)
			{
				// *일 만 표기
				FText findText;
				FText::FindText(TEXT("BMShopText"), TEXT("timeDay"), findText); // 일
				FString result = FString::FromInt(timeSpan.GetDays()) + findText.ToString();
				FText resultText = leftText.Format(leftText, FText::FromString(result));
				_remainTimeText->SetText(resultText);
			}
			else
			{
				// 시 분 초 표기
				//FString timeStr;
				//FGsTimeStringHelper::GetTimeStringHMS(timeSpan, timeStr, false);
				//_remainTimeText->SetText(FText::FromString(timeStr));
			}

			InCouponItem->GetConvertServerUTCExpiredTime(_remainDateTime);
			_isRunningRemainTime = true;
			*/
		}
		else
		{
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Expired_End_03"), findText);
			_remainTimeText->SetText(findText);
			if (OnExpiredRemainTime.IsBound())
			{
				OnExpiredRemainTime.Broadcast(_couponItemDbid);
			}
			_isRunningRemainTime = false;
		}
	}
}

void UGsUICouponListEntry::OnClickCouponList()
{
	if (OnClickCoupon.IsBound())
	{
		OnClickCoupon.Broadcast(_couponItemDbid, !_isSelected);
	}
}

void UGsUICouponListEntry::OnRefreshItemIcon(int32 InIndex, class UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->SetOwnedItem(_couponItemDbid);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetItemNameToolTipFlag(true);
	itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);
}


