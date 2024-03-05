#include "GsUIMaterialSetGainInfoCraftMaterialListEntry.h"

#include "Runtime/UMG/Public/Components/TextBlock.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"

void UGsUIMaterialSetGainInfoCraftMaterialListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _slotHelper)
	{
		_slotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_slotHelper->Initialize(_iconSelector);
		_slotHelper->OnRefreshIcon.AddUniqueDynamic(this, &UGsUIMaterialSetGainInfoCraftMaterialListEntry::OnRefreshIcon);
	}
}

void UGsUIMaterialSetGainInfoCraftMaterialListEntry::NativeDestruct()
{
	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}

	Super::NativeDestruct();
}

void UGsUIMaterialSetGainInfoCraftMaterialListEntry::SetParameters(Parameters InParamerters)
{
	_parameters = MoveTemp(InParamerters);

	// 대상 아이콘
	_slotHelper->RefreshAll(1);

	if (CostType::ITEM == _parameters._costType)
	{
		const FText CollectionMaterialName = (1 < _parameters._itemLevel) ? FText::Format(NSLOCTEXT("GsCollection", "GsCollectionMaterialItemNameFormat", "+{0} {1}"), _parameters._itemLevel, _parameters._name) : _parameters._name;
		_txtTitle->SetText(CollectionMaterialName);
		_txtTitle->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGradeByItemId(_parameters._itemId));
	}
	else
	{
		_txtTitle->SetText(_parameters._name);
		_txtTitle->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(ItemGrade::NORMAL));
	}

	_txtDesc->SetText(FText::Format(NSLOCTEXT("GsCollection", "CraftMaterialAmountTotalFormat", "{0}/{1}"), _parameters._ownedCount, _parameters._requireCount));
	_txtDesc->SetColorAndOpacity(FGsUIColorHelper::GetColor((_parameters._ownedCount >= _parameters._requireCount) ? EGsUIColorType::DEFAULT_ENOUGH : EGsUIColorType::DEFAULT_LACK));
}

void UGsUIMaterialSetGainInfoCraftMaterialListEntry::OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* Icon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == Icon)
	{
		GSLOG(Error, TEXT("nullptr == Icon, InIndex:%d, GetName():%s"), InIndex, *GetName());
		return;
	}

	Icon->OnFinishedLongPressEvent.RemoveAll(this);
	if (CostType::CURRENCY == _parameters._costType)
	{
		Icon->SetCurrency(_parameters._currencyType, 0);

		if (!Icon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			Icon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIMaterialSetGainInfoCraftMaterialListEntry::OnFinishedLongPressEventItemIcon);
		}
	}
	else if (CostType::ITEM == _parameters._costType)
	{
		Icon->SetNotOwnItem(_parameters._itemId, 1, _parameters._itemLevel);

		if (!Icon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			Icon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIMaterialSetGainInfoCraftMaterialListEntry::OnFinishedLongPressEventItemIcon);
		}
	}

	Icon->SetDimmedImage(_parameters._ownedCount < _parameters._requireCount);
	Icon->SetItemNameVisibility(false);
	Icon->SetRedDot(false);
}

void UGsUIMaterialSetGainInfoCraftMaterialListEntry::OnFinishedLongPressEventItemIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}