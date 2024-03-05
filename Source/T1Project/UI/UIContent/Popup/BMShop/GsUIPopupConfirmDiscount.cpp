// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/BMShop/GsUIPopupConfirmDiscount.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "GameObject/Define/GsGameObjectDefine.h"



void UGsUIPopupConfirmDiscount::BeginDestroy()
{
	if (nullptr != _iconSlotHelper)
	{
		_iconSlotHelper->Clear();
		_iconSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPopupConfirmDiscount::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _iconSlotHelper)
	{
		_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconSlotHelper->Initialize(_iconSelector);
		_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIPopupConfirmDiscount::OnRefreshItemIcon);
	}
	
	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupConfirmDiscount::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupConfirmDiscount::OnClickCancel);
}

void UGsUIPopupConfirmDiscount::NativeConstruct()
{
	Super::NativeConstruct();

	_btnOk->SetIsEnabled(true);
	_couponItemDbid = 0;
	_leftDays = 0;
	_isRunningRemainTime = false;

	_coolTimeMgr = GCoolTime();
}

void UGsUIPopupConfirmDiscount::NativeDestruct()
{
	if (OnClickOkCoupon.IsBound())
	{
		OnClickOkCoupon.Clear();
	}

	_reserveBulkState = false;
	_reservePairList.Empty();
	_coolTimeMgr = nullptr;

	Super::NativeDestruct();
}

void UGsUIPopupConfirmDiscount::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

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
			_isRunningRemainTime = false;
			_btnOk->SetIsEnabled(false);
		}
	}
}

void UGsUIPopupConfirmDiscount::SetCouponInfo(const uint64 InCouponItemDbid, TArray<BMProductIdAmountPair> InPairList, bool InIsBulkSale)
{
	if (0 >= InCouponItemDbid)
		return;

	_couponItemDbid = InCouponItemDbid;
	_reservePairList = InPairList;
	_reserveBulkState = InIsBulkSale;

	TWeakPtr<FGsItem> itemData = GItem()->FindItem(_couponItemDbid);
	if (false == itemData.IsValid())
		return;

	//Item Name
	_itemNameText->SetText(itemData.Pin()->GetName());

	//Remain Time
	SetRemainTime(itemData.Pin().Get());

	//Icon
	_iconSlotHelper->RefreshAll(1);

	//_btnOk->SetIsEnabled(!itemData.Pin()->IsExpiredTime());
}

void UGsUIPopupConfirmDiscount::SetRemainTime(const FGsItem* InCouponItem)
{
	_remainTimeText->SetVisibility(InCouponItem->IsPeriodItem() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	_remainTimeImage->SetVisibility(InCouponItem->IsPeriodItem() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (false == InCouponItem->IsPeriodItem())
		return;

	if (true == InCouponItem->IsExpiredTime()) // 기간 만료상태
	{
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Expired_End_03"), findText);
		_remainTimeText->SetText(findText);
		_isRunningRemainTime = false;
		_btnOk->SetIsEnabled(false);
	}
	else
	{
		FTimespan timeSpan;
		if (true == InCouponItem->GetExpiredRemainTime(timeSpan))
		{
			InCouponItem->GetConvertServerUTCExpiredTime(_remainDateTime);
			_isRunningRemainTime = true;
		}
		else
		{
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Expired_End_03"), findText);
			_remainTimeText->SetText(findText);
			_isRunningRemainTime = false;
			_btnOk->SetIsEnabled(false);
		}
	}
}

void UGsUIPopupConfirmDiscount::OnClickOk()
{	
	if (OnClickOkCoupon.IsBound())
	{
		OnClickOkCoupon.Broadcast(_reservePairList, _reserveBulkState, _couponItemDbid);
	}

	Close();
}

void UGsUIPopupConfirmDiscount::OnClickCancel()
{
	Close();
}

void UGsUIPopupConfirmDiscount::OnRefreshItemIcon(int32 InIndex, class UGsUIIconBase* InIcon)
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