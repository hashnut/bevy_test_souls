#include "GsUICraftExpectedListItem.h"

#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "Currency/GsCurrencyHelper.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Interface/GsIconSelectorInterface.h"
#include "../../Common/Icon/GsUIIconItem.h"

void UGsUICraftExpectedListItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _slotHelperIcon)
	{
		_slotHelperIcon = NewObject<UGsDynamicIconSlotHelper>(this);
		_slotHelperIcon->Initialize(_iconSelector);
		_slotHelperIcon->OnRefreshIcon.AddDynamic(this, &UGsUICraftExpectedListItem::OnRefreshIconItem);
	}
}

void UGsUICraftExpectedListItem::NativeDestruct()
{
	_iconSelector->RemoveAllChildren();

	Super::NativeDestruct();
}

void UGsUICraftExpectedListItem::SetParameters(Parameters InParameters)
{
	_parameters = MoveTemp(InParameters);

	// 스위처 선택
	const int32* SwitcherIndex = _switcherResultTypeMap.Find(_parameters._craftResult);
	if (nullptr == SwitcherIndex)
	{
		GSLOG(Error, TEXT("nullptr == SwitcherIndex"));
	}
	else
	{
		_switcherResultType->SetActiveWidgetIndex(*SwitcherIndex);
	}

	// 아이콘 출력
	_slotHelperIcon->RefreshAll(1);

	// 이름 출력
	_txtTitle->SetText(_parameters._craftTargetObject.GetCommonName());
	_txtTitle->SetColorAndOpacity(_parameters._craftTargetObject.GetCommonColor());
}

void UGsUICraftExpectedListItem::OnRefreshIconItem(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* Icon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == Icon)
	{
		GSLOG(Error, TEXT("nullptr == Icon, InIndex:%d, GetName():%s"), InIndex, *GetName());
		return;
	}

	const CostType CurrentCostType = _parameters._craftTargetObject.GetCostType();
	if (CostType::CURRENCY == CurrentCostType)
	{
		Icon->SetCurrency(_parameters._craftTargetObject.GetCurrencyType(), _parameters._count);
		
		if (!Icon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			Icon->OnFinishedLongPressEvent.AddUObject(this, &UGsUICraftExpectedListItem::OnFinishedLongPressEventItemIcon);
		}
	}
	else if (CostType::ITEM == CurrentCostType)
	{
		Icon->SetNotOwnItem(_parameters._craftTargetObject.GetItemId(), _parameters._count, _parameters._enchant);

		if (!Icon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			Icon->OnFinishedLongPressEvent.AddUObject(this, &UGsUICraftExpectedListItem::OnFinishedLongPressEventItemIcon);
		}
	}
	else
	{
		Icon->OnFinishedLongPressEvent.RemoveAll(this);
	}

	Icon->SetItemNameVisibility(false);
	Icon->SetRedDot(false);
}

void UGsUICraftExpectedListItem::OnFinishedLongPressEventItemIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}