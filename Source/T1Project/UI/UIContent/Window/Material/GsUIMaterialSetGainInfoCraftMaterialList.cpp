#include "GsUIMaterialSetGainInfoCraftMaterialList.h"

#include "Runtime/UMG/Public/Components/ScrollBox.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "Currency/GsCostPackage.h"
#include "GsUIMaterialSetGainInfoCraftMaterialListEntry.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

void UGsUIMaterialSetGainInfoCraftMaterialList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GSLOG(Log, TEXT("UGsUIMaterialSetGainInfoCraftMaterialList::NativeOnInitialized()"));

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelRootList);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIMaterialSetGainInfoCraftMaterialList::OnRefreshEntry);
}

void UGsUIMaterialSetGainInfoCraftMaterialList::NativeConstruct()
{
	GSLOG(Log, TEXT("UGsUIMaterialSetGainInfoCraftMaterialList::NativeConstruct()"));

	Super::NativeConstruct();

	ResetAllDatas();
}

void UGsUIMaterialSetGainInfoCraftMaterialList::NativeDestruct()
{
	GSLOG(Log, TEXT("UGsUIMaterialSetGainInfoCraftMaterialList::NativeDestruct()"));

	ResetAllDatas();

	Super::NativeDestruct();
}

void UGsUIMaterialSetGainInfoCraftMaterialList::ResetAllDatas()
{
	_slotHelper->AllClearChildren();
}

void UGsUIMaterialSetGainInfoCraftMaterialList::SetParameters(Parameters InParameters)
{
	constexpr int32 LIST_INDEX_EMPTY = 0;
	constexpr int32 LIST_INDEX_NOT_EMPTY = 1;

	_parameters = MoveTemp(InParameters);

	// 스크롤 리셋
	_scroll->EndInertialScrolling();
	_scroll->ScrollToStart();

	if (_parameters._costPackage.IsValid())
	{
		_slotHelper->RefreshAll(_parameters._costPackage->GetCostElementsCount());
		_widgetSwitcher->SetActiveWidgetIndex(LIST_INDEX_NOT_EMPTY);
	}
	else
	{
		_widgetSwitcher->SetActiveWidgetIndex(LIST_INDEX_EMPTY);
		_slotHelper->AllClearChildren();
	}
}

void UGsUIMaterialSetGainInfoCraftMaterialList::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (nullptr == InEntry)
	{
		GSLOG(Error, TEXT("nullptr == InEntry"));
		return;
	}

	if (!_parameters._costPackage->IsValidCostElementsIndex(InIndex))
	{
		GSLOG(Error, TEXT("!_parameters._costPackage->IsValidCostElementsIndex(%d)"), InIndex);
		return;
	}

	const FGsCostElement* CostElement = _parameters._costPackage->GetCostElementAt(InIndex);
	if (nullptr == CostElement)
	{
		GSLOG(Error, TEXT("nullptr == CostElement, InIndex:%d"), InIndex);
		return;
	}

	UGsUIMaterialSetGainInfoCraftMaterialListEntry* ListEntry = Cast<UGsUIMaterialSetGainInfoCraftMaterialListEntry>(InEntry);
	if (nullptr == ListEntry)
	{
		GSLOG(Error, TEXT("nullptr == ListEntry"));
		return;
	}

	ListEntry->SetParameters({
		CostElement->_costType,
		CostElement->GetCurrencyType(),
		CostElement->GetItemId(),
		CostElement->GetItemLevel(),
		CostElement->GetCostName(),
		CostElement->GetCostAmount(),
		CostElement->PlayerHasAmountMax()
	});
}
