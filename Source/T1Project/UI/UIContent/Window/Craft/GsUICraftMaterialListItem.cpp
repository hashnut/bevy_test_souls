#include "GsUICraftMaterialListItem.h"

#include "Runtime/Core/Public/Math/Color.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Runtime/UMG/Public/Components/SlateWrapperTypes.h"
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
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsIconSelectorInterface.h"

void UGsUICraftMaterialListItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _slotHelperIcon)
	{
		_slotHelperIcon = NewObject<UGsDynamicIconSlotHelper>(this);
		_slotHelperIcon->Initialize(_iconSelector);
		_slotHelperIcon->OnRefreshIcon.AddUniqueDynamic(this, &UGsUICraftMaterialListItem::OnRefreshIconItem);
	}

	_tglMain->OnToggleButtonSelected.AddUniqueDynamic(this, &UGsUICraftMaterialListItem::OnToggleButtonSelectedMain);
	_btnOverayCraft->OnClicked.AddUniqueDynamic(this, &UGsUICraftMaterialListItem::OnClickedCraft);
	_btnOverayCandidate->OnClicked.AddUniqueDynamic(this, &UGsUICraftMaterialListItem::OnClickedShowCandidate);
}

void UGsUICraftMaterialListItem::NativeDestruct()
{
	OnHide();

	_iconSelector->RemoveAllChildren();

	Super::NativeDestruct();
}

void UGsUICraftMaterialListItem::SetParameters(Parameters InParameters)
{
	_parameters = MoveTemp(InParameters);

	_slotHelperIcon->RefreshAll(1);

	if (CostType::CURRENCY == _parameters._costType)
	{
		const FText CostName = FGsCurrencyHelper::GetCurrencyLocalizedText(_parameters._currencyType);

		_txtTitle->SetText(CostName);
		_txtTitle->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(ItemGrade::NONE));
	}
	else if (CostType::ITEM == _parameters._costType)
	{
		const UGsTableItemCommon* TableItemCommon = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
		if (nullptr == TableItemCommon)
		{
			GSLOG(Error, TEXT("nullptr == TableItemCommon"));
			return;
		}

		const FGsSchemaItemCommon* ItemCommon = nullptr;
		if (!TableItemCommon->FindRowById(_parameters._itemId, ItemCommon))
		{
			GSLOG(Error, TEXT("TableItemCommon->FindRowById(InItemId, Row), _parameters._itemId:%d"), _parameters._itemId);
			return;
		}

		if (nullptr == ItemCommon)
		{
			GSLOG(Error, TEXT("nullptr == ItemCommon, _parameters._itemId:%d"), _parameters._itemId);
		}
		else
		{
			_txtTitle->SetText(ItemCommon->name);
			_txtTitle->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGradeByItemId(_parameters._itemId));
		}
	}

	{
		const FText RequireAmount = FText::AsNumber(_parameters._requireCount);
		const FText OwnedAmount = FText::AsNumber(_parameters._ownedCount);
		const FText Amount = FText::Format(NSLOCTEXT("Craft", "CraftMaterialAmount", "{0}/{1}"), OwnedAmount, RequireAmount);
		_txtSubTitle->SetText(Amount);

		const EGsUIColorType TitleColorType = _parameters._isEnoughCostElement ? EGsUIColorType::DEFAULT_ENOUGH : EGsUIColorType::DEFAULT_LACK;
		_txtSubTitle->SetColorAndOpacity(FGsUIColorHelper::GetColor(TitleColorType));

		const bool CanCraft = _parameters._hasNavigateToCraft;
		_wgtCraftIcon->SetVisibility(CanCraft ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		_btnOverayCraft->SetVisibility(CanCraft ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		const bool HasCandidate = _parameters._hasCandidate;
		_wgtCandidateIcon->SetVisibility(HasCandidate ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		_btnOverayCandidate->SetVisibility(HasCandidate ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		// 첫번째 대체아이템으로 모두 지불 가능한 경우 기본 색상이지만, 첫번재 대체아이템이 아닌 경우 알림을 위해 색상 변경 출력
		const FLinearColor& CandidateIconColor = (_parameters._isEnoughCostElement && !_parameters._isEnoughCostElementOnlyFirstCandidate) ? FGsUIColorHelper::GetColor(EGsUIColorType::CONGESTION_HIGH) : FLinearColor::White;
		_wgtCandidateIcon->SetColorAndOpacity(CandidateIconColor);

		_wgtDimmed->SetVisibility(_parameters._isEnoughCostElement ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}
}

IGsToggleGroupEntry* UGsUICraftMaterialListItem::GetToggleGroupEntry() const
{
	return _tglMain;
}

void UGsUICraftMaterialListItem::OnRefreshIconItem(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* Icon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == Icon)
	{
		GSLOG(Error, TEXT("nullptr == Icon, InIndex:%d, GetName():%s"), InIndex, *GetName());
		return;
	}

	if (CostType::CURRENCY == _parameters._costType)
	{
		Icon->SetCurrency(_parameters._currencyType, _parameters._requireCount);

		if (!Icon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			Icon->OnFinishedLongPressEvent.AddUObject(this, &UGsUICraftMaterialListItem::OnFinishedLongPressEventItemIcon);
		}

		// ejrrb10 | 재료가 재화 타입일 경우, 유효기간에 대한 정보를 Clear 해 준다 (현우 차석님 요청)
		Icon->SetPeriodTime(nullptr);
	}
	else if (CostType::ITEM == _parameters._costType)
	{
		// [U2] | ejrrb10 | 재료 목록 및 롱프레스 노출 아이콘에서, 장비형 아이템(스택 불가)일 경우 포트레이트에 아이템 갯수 표기 X (CHR-22015)
		UGsItemManager* itemMgr = GItem();
		if (nullptr == itemMgr)
		{
			GSLOG(Error, TEXT("nullptr == itemMgr"));
			return;
		}

		if (GItem()->IsItemEquipmentByTId(_parameters._itemId))
		{
			Icon->SetNotOwnItem(_parameters._itemId, 1, _parameters._itemLevel);
		}
		else
		{
			Icon->SetNotOwnItem(_parameters._itemId, _parameters._requireCount, _parameters._itemLevel);
		}

		if (!Icon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			Icon->OnFinishedLongPressEvent.AddUObject(this, &UGsUICraftMaterialListItem::OnFinishedLongPressEventItemIcon);
		}
	}
	else
	{
		Icon->OnFinishedLongPressEvent.RemoveAll(this);
	}
	
	Icon->SetItemNameVisibility(false);
	Icon->SetRedDot(false);
}

void UGsUICraftMaterialListItem::OnFinishedLongPressEventItemIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

void UGsUICraftMaterialListItem::OnToggleButtonSelectedMain(bool InSelected)
{
	const bool IsShowOveray = InSelected && (_parameters._hasNavigateToCraft || _parameters._hasCandidate);
	_selectedOveray->SetVisibility(IsShowOveray ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUICraftMaterialListItem::OnClickedCraft()
{
	_parameters._onClickedCraft.ExecuteIfBound(_parameters._index);
}

void UGsUICraftMaterialListItem::OnClickedShowCandidate()
{
	_parameters._onClickedCandidate.ExecuteIfBound(_parameters._index);
}
