// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/UIContent/Window/SpiritShot/GsUISpiritShotMaterialListItem.h"

#include "Runtime/Core/Public/Math/Color.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

#include "Classes/GsMessageHandlerOneParam.h"
#include "Currency/GsCostPackage.h"
#include "Currency/GsCurrencyHelper.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "T1Project.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Interface/GsIconSelectorInterface.h"

void UGsUISpiritShotMaterialListItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _slotHelperIcon)
	{
		_slotHelperIcon = NewObject<UGsDynamicIconSlotHelper>(this);
		_slotHelperIcon->Initialize(_iconSelector);
		_slotHelperIcon->OnRefreshIcon.AddUniqueDynamic(this, &UGsUISpiritShotMaterialListItem::OnRefreshIconItem);
	}
}

void UGsUISpiritShotMaterialListItem::NativeDestruct()
{
	OnHide();

	_iconSelector->RemoveAllChildren();

	Super::NativeDestruct();
}

void UGsUISpiritShotMaterialListItem::SetParameters(Parameters InParameters)
{
	_parameters = MoveTemp(InParameters);

	// 아이콘 업데이트
	_slotHelperIcon->RefreshAll(1);

	// 아이템 이름 세팅
	if (CostType::CURRENCY == _parameters._costType)
	{
		const FText costName = FGsCurrencyHelper::GetCurrencyLocalizedText(_parameters._currencyType);

		_itemName->SetText(costName);
		_itemName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(ItemGrade::NONE));
	}
	else if (CostType::ITEM == _parameters._costType)
	{
		const UGsTableItemCommon* tableItemCommon = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
		if (nullptr == tableItemCommon)
		{
			GSLOG(Error, TEXT("nullptr == tableItemCommon"));
			return;
		}

		const FGsSchemaItemCommon* itemCommon = nullptr;
		if (!tableItemCommon->FindRowById(_parameters._itemId, itemCommon))
		{
			GSLOG(Error, TEXT("tableItemCommon->FindRowById(InItemId, Row), _parameters._itemId:%d"), _parameters._itemId);
			return;
		}

		if (nullptr == itemCommon)
		{
			GSLOG(Error, TEXT("nullptr == ItemCommon, _parameters._itemId:%d"), _parameters._itemId);
		}
		else
		{
			_itemName->SetText(itemCommon->name);
			_itemName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGradeByItemId(_parameters._itemId));
		}
	}

	// 수량 세팅 (축약하여 표시 -> 전체 갯수 표기하도록 재수정 요청)
	//FText ownedAmountText;
	//FGsUIStringHelper::GetItemAmountShortText(_parameters._ownedAmount, ownedAmountText);
	//FText requireAmountText;
	//FGsUIStringHelper::GetItemAmountShortText(_parameters._requireAmount, requireAmountText);

	FText amountFormat = FText::Format(FTextFormat::FromString("{0}/{1}"), FText::AsNumber(_parameters._ownedAmount), FText::AsNumber(_parameters._requireAmount));
	_amountText->SetText(amountFormat);

	// 부족 여부 컬러링
	const EGsUIColorType amountColorType = _parameters._isEnoughCostElement ? EGsUIColorType::DEFAULT_ENOUGH : EGsUIColorType::DEFAULT_LACK;
	_amountText->SetColorAndOpacity(FGsUIColorHelper::GetColor(amountColorType));
}

void UGsUISpiritShotMaterialListItem::OnRefreshIconItem(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* icon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == icon)
	{
		GSLOG(Error, TEXT("nullptr == icon, InIndex:%d, GetName():%s"), InIndex, *GetName());
		return;
	}

	// 아이콘 롱프레스 시, 필요 갯수가 출력되도록 설정할 것이다 (제작 시스템과 통일성을 맞추기 위함)
	if (CostType::CURRENCY == _parameters._costType)
	{
		icon->SetCurrency(_parameters._currencyType, _parameters._requireAmount);

		if (!icon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			icon->OnFinishedLongPressEvent.AddUObject(this, &UGsUISpiritShotMaterialListItem::OnFinishedLongPressEventItemIcon);
		}
	}
	else if (CostType::ITEM == _parameters._costType)
	{
		icon->SetNotOwnItem(_parameters._itemId, _parameters._requireAmount, _parameters._itemLevel);

		if (!icon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			icon->OnFinishedLongPressEvent.AddUObject(this, &UGsUISpiritShotMaterialListItem::OnFinishedLongPressEventItemIcon);
		}
	}
	else
	{
		icon->OnFinishedLongPressEvent.RemoveAll(this);
	}

	icon->SetItemNameVisibility(false);
	icon->SetRedDot(false);

	// 아이콘 크기가 커지면서, 아이콘 포트레이트에도 갯수를 표기하도록 재수정 요청을 받음
	// 아이템 갯수 및 강화 수치 텍스트 숨기기 (재료 목록 리스트의 아이콘에는 수량이 표시되면 어색하다)
	//icon->SetItemCountTextVisibility(ESlateVisibility::Collapsed);
	//icon->SetEnchantNumTextVisibility(ESlateVisibility::Collapsed);

	// 아이콘 배경 색 없애기(기획 덕규님 요청으로 해제 함.)
	//icon->SetGrade(ItemGrade::NONE);
}

void UGsUISpiritShotMaterialListItem::OnFinishedLongPressEventItemIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}
