// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/BMShop/GsUIPopupCouponList.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "GsUICouponListEntry.h"
#include "DataSchema/Item/GsSchemaItemEffectDiscountCouponData.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Item/GsItemManager.h"
#include "Item/GsItem.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"



void UGsUIPopupCouponList::BeginDestroy()
{
	if (nullptr != _couponListPanelSlotHelper)
	{
		_couponListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupCouponList::CouponListRefresh);
		_couponListPanelSlotHelper = nullptr;
	}
	
	Super::BeginDestroy();
}

void UGsUIPopupCouponList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _couponListPanelSlotHelper)
	{
		_couponListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_couponListPanelSlotHelper->Initialize(_couponListEntryWidgetClass, _couponListRootPanel);
		_couponListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupCouponList::CouponListRefresh);
	}

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupCouponList::OnOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupCouponList::OnCancel);
}

void UGsUIPopupCouponList::NativeConstruct()
{
	Super::NativeConstruct();

	_productID = 0;
	_selectedItemDBID = 0;
	_couponItemDBIDList.Empty();

	_btnOk->SetIsEnabled(true);

	_listSwitcher->SetActiveWidgetIndex(0);
}

void UGsUIPopupCouponList::NativeDestruct()
{

	Super::NativeDestruct();
}

void UGsUIPopupCouponList::SetCouponList(const BMProductId InProductID)
{
	_productID = InProductID;
	_couponItemDBIDList.Empty();

	FGsBMShopManager* shopMgr = GBMShop();
	TArray<FGsItem*> itemDataList;
	TArray<FGsItem*> expiredItemDataList;
	TMap<TWeakPtr<FGsItem>, const FGsSchemaItemEffectDiscountCouponData*> findCouponInfo;
	if (shopMgr->FindCouponDataByProductID(InProductID, findCouponInfo))
	{
		_selectedItemDBID = shopMgr->GetReserveCouponItemDBID();

		for (TPair<TWeakPtr<FGsItem>, const FGsSchemaItemEffectDiscountCouponData*> iter : findCouponInfo)
		{
			TWeakPtr<FGsItem> itemData = iter.Key;
			if(false == itemData.IsValid())
				continue;

			if (itemData.Pin()->IsPeriodItem() && itemData.Pin()->IsExpiredTime())
			{
				expiredItemDataList.Emplace(itemData.Pin().Get());
			}
			else
			{
				itemDataList.Emplace(itemData.Pin().Get());
			}
		}

		if (0 < itemDataList.Num())
		{
			itemDataList.Sort([](FGsItem& itemA, FGsItem& itemB)
				{
					// 기간 만료형 아이템 
					 bool diffFlag = itemA.IsPeriodItem();
					 if (diffFlag != itemB.IsPeriodItem())
					 {
						 return diffFlag ? true : false;
					 }
					 // 남은 기간
					 int32 diff = 0;
					 FTimespan spanA;
					 FTimespan spanB;
					if (itemA.GetExpiredRemainTime(spanA) && itemB.GetExpiredRemainTime(spanB))
					{
						// 만료시간 적은 순서부터
						diff = spanA.GetTotalSeconds() - spanB.GetTotalSeconds();
						if (0 != diff)
						{
							return spanA.GetTotalSeconds() < spanB.GetTotalSeconds();
						}
					}
					// 아이템 등급
					diff = (uint8)itemB.GetGradeType() - (uint8)itemA.GetGradeType();
					if (0 != diff)
					{
						return (uint8)itemB.GetGradeType() < (uint8)itemA.GetGradeType();
					}

					// 아이템 TID
					return itemB.GetTID() > itemA.GetTID();
				}
			);
		}

		SortCouponList(expiredItemDataList);
		for (int32 i = 0; i < expiredItemDataList.Num(); ++i)
		{
			itemDataList.Emplace(expiredItemDataList[i]);
		}
		for (FGsItem* iter : itemDataList)
		{
			_couponItemDBIDList.Emplace(iter->GetDBID());
		}
		/*
		// TestCode
		for (TPair<TWeakPtr<FGsItem>, const FGsSchemaItemEffectDiscountCouponData*> iter : findCouponInfo)
		{
			GSLOG(Error, TEXT("123123312312312321"));
		}
		*/
	}

	_listSwitcher->SetActiveWidgetIndex(0 < _couponItemDBIDList.Num() ? 0 : 1);
	_couponListPanelSlotHelper->RefreshAll(_couponItemDBIDList.Num());
}

void UGsUIPopupCouponList::SortCouponList(TArray<FGsItem*>& outSortList)
{
	if (0 < outSortList.Num())
	{
		outSortList.Sort([](FGsItem& itemA, FGsItem& itemB)
			{
				// 아이템 등급
				 int32 diff = (uint8)itemB.GetGradeType() - (uint8)itemA.GetGradeType();
				if (0 != diff)
				{
					return (uint8)itemB.GetGradeType() < (uint8)itemA.GetGradeType();
				}

				// 아이템 TID
				return itemB.GetTID() > itemA.GetTID();
			}
		);
	}
}

void UGsUIPopupCouponList::RemoveCouponItem(const uint64 InCouponItemDbid)
{
	FGsBMShopManager* shopMgr = GBMShop();
	for (const uint64 iter : _couponItemDBIDList)
	{
		if (InCouponItemDbid == iter)
		{
			shopMgr->SetReserveCouponItemDBID(0);
			SetCouponList(_productID);
			return;
		}
	}
}

void UGsUIPopupCouponList::CouponListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUICouponListEntry* entry = Cast<UGsUICouponListEntry>(InEntry);
	if (nullptr == entry)
		return;

	if (_couponItemDBIDList.IsValidIndex(InIndex))
	{
		uint64 curDbid = _couponItemDBIDList[InIndex];
		entry->SetCouponInfo(curDbid, curDbid == _selectedItemDBID);
		if (false == entry->OnClickCoupon.IsBoundToObject(this))
		{
			entry->OnClickCoupon.AddUObject(this, &UGsUIPopupCouponList::OnClickCouponEntry);
		}
		if (false == entry->OnExpiredRemainTime.IsBoundToObject(this))
		{
			entry->OnExpiredRemainTime.AddUObject(this, &UGsUIPopupCouponList::ExpiredRemainTime);
		}
	}
}

void UGsUIPopupCouponList::OnClickCouponEntry(uint64 InItemDbid, bool InIsSelect)
{
	for (int32 i = 0; i < _couponListPanelSlotHelper->GetChildrenCount(); ++i)
	{
		UWidget* widget = _couponListPanelSlotHelper->GetEntry(i);
		if(nullptr == widget)
			continue;

		UGsUICouponListEntry* entryWidget = Cast<UGsUICouponListEntry>(widget);
		if(nullptr == entryWidget)
			continue;

		entryWidget->SetSelectionImg(entryWidget->GetCouponItemDbid() == InItemDbid ? InIsSelect : false);
		if (entryWidget->GetCouponItemDbid() == InItemDbid)
		{
			if (false == entryWidget->IsPeriodCouponItem())
			{
				_btnOk->SetIsEnabled(true);
			}
			else
			{
				_btnOk->SetIsEnabled(entryWidget->IsRunningRemainTime());
			}
		}
	}
	_selectedItemDBID = InIsSelect ? InItemDbid : 0;
}

void UGsUIPopupCouponList::ExpiredRemainTime(uint64 InItemDBID)
{
	if (_selectedItemDBID == InItemDBID)
	{
		_btnOk->SetIsEnabled(false);
	}

	/*
	FGsBMShopManager* shopMgr = GBMShop();
	if (InItemDBID == shopMgr->GetReserveCouponItemDBID())
	{
		shopMgr->SetReserveCouponItemDBID(0);
		FGsBMShopCouponValidCheck param(_productID);
		GMessage()->GetBMShop().SendMessage(MessageContentStoreBM::CHECK_VALID_COUPON, &param);
	}
	*/
}

void UGsUIPopupCouponList::CheckOkBtnState()
{
	_btnOk->SetIsEnabled(0 >= _selectedItemDBID ? false : true);
}

void UGsUIPopupCouponList::OnOk()
{
	GBMShop()->SetReserveCouponItemDBID(_selectedItemDBID);

	MakeShared<FGsBMShopCheckValidCoupon>(_productID)->Send();
	Close();
}

void UGsUIPopupCouponList::OnCancel()
{
	Close();
}

void UGsUIPopupCouponList::OnInputOk()
{
	OnOk();
}

void UGsUIPopupCouponList::OnInputCancel()
{
	OnCancel();
}
