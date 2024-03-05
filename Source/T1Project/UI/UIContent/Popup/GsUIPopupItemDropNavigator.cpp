#include "GsUIPopupItemDropNavigator.h"

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/SlateWrapperTypes.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

#include "Currency/GsCurrencyHelper.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterElementData.h"
#include "Item/GsItem.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Popup/ItemDropNavigator/GsUIItemDropListContainer.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"

void UGsUIPopupItemDropNavigator::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_closeBtn->OnClicked.AddUniqueDynamic(this, &UGsUIPopupItemDropNavigator::OnClickedClose);

	if (nullptr == _targetIconSlotHelper)
	{
		_targetIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_targetIconSlotHelper->Initialize(_targetIconSelector);
		_targetIconSlotHelper->OnRefreshIcon.AddUniqueDynamic(this, &UGsUIPopupItemDropNavigator::OnRefreshIconItem);
	}
}

void UGsUIPopupItemDropNavigator::SetParameters(Parameters InParameters)
{
	_parameters = MoveTemp(InParameters);

	Invalidate();
}

void UGsUIPopupItemDropNavigator::SetParametersDropMapCreatures(TArray<TPair<MapId, CreatureId>> InDropMapCreatures)
{
	// 드랍 리스트 채우기
	FSimpleDelegate OnClickListEntryDelegate = FSimpleDelegate::CreateUObject(this, &UGsUIPopupItemDropNavigator::OnClickNavigateButton);
	_itemDropListContainer->SetParameters(MoveTemp(OnClickListEntryDelegate), MoveTemp(InDropMapCreatures));
}

void UGsUIPopupItemDropNavigator::SetParametersDropMapCreatures(TArray<FGsSchemaWorldMapMonsterElementDataRow> InDropMapCreatures)
{
	// 드랍 리스트 채우기
	FSimpleDelegate OnClickListEntryDelegate = FSimpleDelegate::CreateUObject(this, &UGsUIPopupItemDropNavigator::OnClickNavigateButton);
	_itemDropListContainer->SetParameters(MoveTemp(OnClickListEntryDelegate), MoveTemp(InDropMapCreatures));
}

void UGsUIPopupItemDropNavigator::SetParametersCraftIds(TArray<CraftId> InCraftIds)
{
	FSimpleDelegate OnClickListEntryDelegate = FSimpleDelegate::CreateUObject(this, &UGsUIPopupItemDropNavigator::OnClickNavigateButton);
	_itemDropListContainer->SetParameters(MoveTemp(OnClickListEntryDelegate), MoveTemp(InCraftIds));
}

void UGsUIPopupItemDropNavigator::Invalidate()
{
	FText TitleText;
	if (_parameters._titleSupply)
	{
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUI_WhereToGet"), TitleText);
	}
	else
	{
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUI_WhereToUse"), TitleText);
	}
	_titleTxt->SetText(TitleText);

	if (nullptr == _targetIconSlotHelper)
	{
		GSLOG(Error, TEXT("nullptr == _targetIconSlotHelper"));
		return;
	}

	if (CostType::ITEM == _parameters._costType)
	{
		const FGsSchemaItemCommon* ItemCommon = FGsItem::GetTableDataByID(_parameters._itemId);
		if (nullptr == ItemCommon)
		{
			GSLOG(Warning, TEXT("nullptr == ItemCommon, _parameters._itemId:%d"), _parameters._itemId);
			_targetNameTxt->SetText(FText::GetEmpty());
		}
		else
		{
			_targetNameTxt->SetText(ItemCommon->name);
			_targetNameTxt->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(ItemCommon->grade));
		}
	}
	else if (CostType::CURRENCY == _parameters._costType)
	{
		_targetNameTxt->SetText(FGsCurrencyHelper::GetCurrencyLocalizedText(_parameters._currencyType));
		_targetNameTxt->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(ItemGrade::NONE));
	}

	_targetIconSlotHelper->RefreshAll(1);
}

void UGsUIPopupItemDropNavigator::OnRefreshIconItem(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* Icon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == Icon)
	{
		GSLOG(Error, TEXT("nullptr == Icon, InIndex:%d, GetName():%s"), InIndex, *GetName());
		return;
	}

	if (CostType::ITEM == _parameters._costType)
	{
		Icon->SetNotOwnItem(_parameters._itemId, 0);
	}
	else
	{
		Icon->SetCurrency(_parameters._currencyType, 0);
	}

	Icon->SetItemNameVisibility(false);
}

void UGsUIPopupItemDropNavigator::OnClickedClose()
{
	Close();
}

void UGsUIPopupItemDropNavigator::OnClickNavigateButton()
{
	Close();
}

void UGsUIPopupItemDropNavigator::OnInputCancel()
{
	OnClickedClose();
}
