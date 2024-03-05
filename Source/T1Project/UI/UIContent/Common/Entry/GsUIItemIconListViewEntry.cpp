#include "GsUIItemIconListViewEntry.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Interface/GsIconSelectorInterface.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "../../Helper/GsUIHelper.h"
#include "T1Project/UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "../Currency/GsCurrencyHelper.h"


void UGsUIItemIconListViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _iconSlotHelper)
	{
		_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconSlotHelper->Initialize(Cast<IGsIconSelectorInterface>(_iconSelector));
		_iconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUIItemIconListViewEntry::OnCreateIcon);
		_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIItemIconListViewEntry::OnRefreshIcon);
	}
}

void UGsUIItemIconListViewEntry::NativeDestruct()
{
	_iconSlotHelper->Clear();
	_iconSelector->RemoveAllChildren();	

	Super::NativeDestruct();
}

void UGsUIItemIconListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	UGsUIItemIconListViewEntryData* data = Cast<UGsUIItemIconListViewEntryData>(inListItemObject);
	if (data)
	{
		_itemDataArray = data->_itemDataArray;		
		_iconSlotHelper->RefreshAll(_itemDataArray.Num());
	}
}

void UGsUIItemIconListViewEntry::OnCreateIcon(UGsUIIconBase* InIcon)
{
	UGsUIIconItem* itemIcon = Cast<UGsUIIconItem>(InIcon);
	if (itemIcon)
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIItemIconListViewEntry::OnLongPressIcon);
	}
}

void UGsUIItemIconListViewEntry::OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon)
{	
	if (_itemDataArray.IsValidIndex(InIndex))
	{
		UGsUIIconItem* itemIcon = Cast<UGsUIIconItem>(InIcon);
		if (itemIcon)
		{
			/*itemIcon->SetEnableDetailPopup(true);
			itemIcon->SetCenterPopup(true);*/
			if (CurrencyType::NONE == _itemDataArray[InIndex]._currencyType)
			{
				itemIcon->SetNotOwnItem(_itemDataArray[InIndex]._itemId, _itemDataArray[InIndex]._itemAmount);
			}
			else
			{
				itemIcon->SetCurrency(_itemDataArray[InIndex]._currencyType, _itemDataArray[InIndex]._itemAmount);
			}
		}
	}
}

// 아이콘 상세보기 창
void UGsUIItemIconListViewEntry::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}
