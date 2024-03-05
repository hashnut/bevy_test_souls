// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/BMShop/GsUIBMShopStorageEntry.h"
#include "T1Project.h"
#include "Item/GsItem.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopStorageItemData.h"


void UGsUIBMShopStorageEntry::BeginDestroy()
{
	if (nullptr != _itemiconSelector)
	{
		_itemiconSelector = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIBMShopStorageEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnAccept->OnClicked.AddDynamic(this, &UGsUIBMShopStorageEntry::OnClickAccept);

	if (nullptr == _iconSlotHelper)
	{
		_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconSlotHelper->Initialize(_itemiconSelector);
		_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIBMShopStorageEntry::OnRefreshItemIcon);
	}
}

void UGsUIBMShopStorageEntry::OnShow()
{
	_currentDay = 999;
	_isRunningTime = false;
}

void UGsUIBMShopStorageEntry::OnHide()
{
	_storageItemData = nullptr;
	_iconSlotHelper->Clear();
}

void UGsUIBMShopStorageEntry::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (nullptr != _storageItemData && true == _isRunningTime)
	{
		FDateTime clientDataTime = FGsTimeSyncUtil::GetClientNowDateTime();
		FTimespan span = clientDataTime - _storageItemData->GetStorageItemPurchaseDateTime();
		if (7 <= span.GetDays())
		{
			// Finished
			_currentDay = 999;
			_isRunningTime = false;

			FText findText;
			FText::FindText(TEXT("BMShopText"), TEXT("storageTermExpired"), findText);
			_withdrawDurationText->SetText(findText);

			return;
		}

		if (_currentDay == span.GetDays())
		{
			return;
		}

		FString result;
		FText findText;
		FText::FindText(TEXT("BMShopText"), TEXT("timeDay"), findText);
		result = (FString::FromInt(7 - span.GetDays()) + findText.ToString());
		_withdrawDurationText->SetText(FText::FromString(result));
		_currentDay = span.GetDays();
	}

	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIBMShopStorageEntry::SetData(const uint64 InStorageItemDBID)
{
	_storageItemData = GBMShop()->FindStorageItems(InStorageItemDBID);
	if (nullptr == _storageItemData)
	{
		GSLOG(Warning, TEXT("[UGsUIBMShopStorageEntry::SetData] InStorageItemDBID Error : %llu"), InStorageItemDBID);
		return;
	}

	FDateTime clientDataTime = FGsTimeSyncUtil::GetClientNowDateTime();
	FDateTime dateTime = _storageItemData->GetStorageItemPurchaseDateTime();
	FTimespan span = clientDataTime - dateTime;
	if (7 > span.GetDays())  // 청약철회 가능 기간은 7일 이라고 함
	{
		_isRunningTime = true;
	}
	else
	{
		_isRunningTime = false;
		_currentDay = 999;

		FText findText;
		FText::FindText(TEXT("BMShopText"), TEXT("storageTermExpired"), findText);
		_withdrawDurationText->SetText(findText);
	}

	const FGsSchemaItemCommon* commonData = FGsItem::GetTableDataByID(_storageItemData->GetStorageItemTID());
	if (nullptr == commonData)
		return;

	_itemNameText->SetText(commonData->name);
	//_itemNameText->SetText(true == _productData->IsHiveProduct() ? FText::FromString(_productData->GetConvertHiveTitle()) :
		//_productData->GetName());

	_iconSlotHelper->RefreshAll(1);

	/*
	GSLOG(Warning, TEXT("[UGsUIBMShopStorageEntry] clientDataTime GetMonth %d"), clientDataTime.GetMonth());
	GSLOG(Warning, TEXT("[UGsUIBMShopStorageEntry] clientDataTime GetDay %d"), clientDataTime.GetDay());
	GSLOG(Warning, TEXT("[UGsUIBMShopStorageEntry] clientDataTime GetHour %d"), clientDataTime.GetHour());
	GSLOG(Warning, TEXT("[UGsUIBMShopStorageEntry] clientDataTime GetMinute %d"), clientDataTime.GetMinute());
	GSLOG(Warning, TEXT("[UGsUIBMShopStorageEntry] clientDataTime GetSecond %d"), clientDataTime.GetSecond());

	GSLOG(Warning, TEXT("[UGsUIBMShopStorageEntry] dateTime GetMonth %d"), dateTime.GetMonth());
	GSLOG(Warning, TEXT("[UGsUIBMShopStorageEntry] dateTime GetDay %d"), dateTime.GetDay());
	GSLOG(Warning, TEXT("[UGsUIBMShopStorageEntry] dateTime GetHour %d"), dateTime.GetHour());
	GSLOG(Warning, TEXT("[UGsUIBMShopStorageEntry] dateTime GetMinute %d"), dateTime.GetMinute());
	GSLOG(Warning, TEXT("[UGsUIBMShopStorageEntry] dateTime GetSecond %d"), dateTime.GetSecond());

	GSLOG(Warning, TEXT("[UGsUIBMShopStorageEntry] span GetDays %d"), span.GetDays());
	GSLOG(Warning, TEXT("[UGsUIBMShopStorageEntry] span GetHours %d"), span.GetHours());
	GSLOG(Warning, TEXT("[UGsUIBMShopStorageEntry] span GetMinutes %d"), span.GetMinutes());
	GSLOG(Warning, TEXT("[UGsUIBMShopStorageEntry] span GetSeconds %d"), span.GetSeconds());
	GSLOG(Warning, TEXT("[UGsUIBMShopStorageEntry] span GetTotalSeconds %d"), span.GetTotalSeconds());
	*/
}

void UGsUIBMShopStorageEntry::OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	if (nullptr == _storageItemData || 0 >= _storageItemData->GetStorageItemTID())
	{
		itemIcon->SetNotOwnItem(0, 0);
		itemIcon->SetItemNameToolTipFlag(false);
		return;
	}

	if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIBMShopStorageEntry::OnLongPressIcon);
	}

	itemIcon->SetNotOwnItem(_storageItemData->GetStorageItemTID(), 1);
	itemIcon->SetItemNameToolTipFlag(true);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameVisibility(false);
}

void UGsUIBMShopStorageEntry::OnLongPressIcon(class UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

void UGsUIBMShopStorageEntry::OnClickAccept()
{
	//GSLOG(Warning, TEXT("[UGsUIBMShopStorageEntry::OnClickAccept]"));
	if (nullptr == _storageItemData)
	{
		return;
	}

	MakeShared<FGsBMShopAcceptStorage>(_storageItemData->GetStorageItemDBID())->Send();
}