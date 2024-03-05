#include "GsUICandidateGoodsListItem.h"

#include "Runtime/UMG/Public/Components/SlateWrapperTypes.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"

#include "Item/GsItemManager.h"

void UGsUICandidateGoodsListItem::NativeConstruct()
{
	Super::NativeConstruct();

	_btnSort->SetToggleCallback([this](int32, bool) -> void { OnClickedSort(); });
}

void UGsUICandidateGoodsListItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _slotHelper)
	{
		_slotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_slotHelper->Initialize(_iconSelector);
		_slotHelper->OnRefreshIcon.AddUniqueDynamic(this, &UGsUICandidateGoodsListItem::OnRefreshIconItem);
	}

	_btnCraft->OnClicked.AddUniqueDynamic(this, &UGsUICandidateGoodsListItem::OnClickedCraft);
}

void UGsUICandidateGoodsListItem::NativeDestruct()
{
	_iconSelector->RemoveAllChildren();

	Super::NativeDestruct();
}

void UGsUICandidateGoodsListItem::SetParameters(TWeakPtr<Parameters> InParameters)
{
	_parameters = InParameters;

	Invalidate();
}

void UGsUICandidateGoodsListItem::Invalidate() const
{
	// 아이콘
	_slotHelper->RefreshAll(1);

	// 이름
	const UGsTableItemCommon* TableItemCommon = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	if (nullptr == TableItemCommon)
	{
		GSLOG(Error, TEXT("nullptr == TableItemCommon"));
		return;
	}

	if (!_parameters.IsValid())
	{
		GSLOG(Error, TEXT("!_parameters.IsValid()"));
		return;
	}

	TSharedPtr<Parameters> CurrentParameters = _parameters.Pin();
	const FGsSchemaItemCommon* ItemCommon = nullptr;
	if (!TableItemCommon->FindRowById(CurrentParameters->_itemId, ItemCommon))
	{
		GSLOG(Error, TEXT("TableItemCommon->FindRowById(InItemId, Row), CurrentParameters->_itemId:%d"), CurrentParameters->_itemId);
		return;
	}

	if (nullptr == ItemCommon)
	{
		GSLOG(Error, TEXT("nullptr == ItemCommon, CurrentParameters->_itemId:%d"), CurrentParameters->_itemId);
	}
	else
	{
		_txtTitle->SetText(ItemCommon->name);
		_txtTitle->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGradeByItemId(CurrentParameters->_itemId));
	}

	// 제작 버튼
	_btnCraft->SetVisibility(CurrentParameters->_canCraft ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	// 번호
	const bool HasSortOrder = CurrentParameters->IsValidSortOrder();
	_btnSort->SetIsSelected(HasSortOrder);
	if (HasSortOrder)
	{
		// 0부터 시작하므로, +1 출력
		_txtOnSort->SetText(FText::AsNumber(CurrentParameters->_sortOrder + 1));
	}
	else
	{
		_txtOnSort->SetText(FText::GetEmpty());
	}
}

void UGsUICandidateGoodsListItem::OnRefreshIconItem(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* Icon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == Icon)
	{
		GSLOG(Error, TEXT("nullptr == Icon, InIndex:%d, GetName():%s"), InIndex, *GetName());
		return;
	}

	if (!_parameters.IsValid())
	{
		GSLOG(Error, TEXT("!_parameters.IsValid()"));
		return;
	}

	TSharedPtr<Parameters> CurrentParameters = _parameters.Pin();
	Icon->SetNotOwnItem(CurrentParameters->_itemId, CurrentParameters->_amount, CurrentParameters->_itemLevel);
	Icon->SetDimmedImage(0 >= CurrentParameters->_amount);
	Icon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::NotUseDetailWndType);
	Icon->SetItemNameVisibility(false);
	Icon->SetRedDot(false);

	// [C2URWQ-7329] 장비 아이템인 경우, 수량 표시를 하지 않는다
	UGsItemManager* itemManager = GItem();
	if (itemManager == nullptr)
		return;

	if (itemManager->IsItemEquipmentByTId(CurrentParameters->_itemId))
		Icon->SetItemCountTextVisibility(ESlateVisibility::Collapsed);

	// [CHR-23564] 아이템을 갖고 있지 않을 경우, 대체 선택을 할 수 없도록 만든다
	_btnSort->SetIsEnabled(CurrentParameters->_amount > 0);
}

void UGsUICandidateGoodsListItem::OnClickedCraft()
{
	if (!_parameters.IsValid())
	{
		GSLOG(Error, TEXT("!_parameters.IsValid()"));
		return;
	}

	TSharedPtr<Parameters> CurrentParameters = _parameters.Pin();
	CurrentParameters->_onClickCraft.ExecuteIfBound(CurrentParameters->_index);
}

void UGsUICandidateGoodsListItem::OnClickedSort()
{
	if (!_parameters.IsValid())
	{
		GSLOG(Error, TEXT("!_parameters.IsValid()"));
		return;
	}

	TSharedPtr<Parameters> CurrentParameters = _parameters.Pin();
	CurrentParameters->_onClickSort.ExecuteIfBound(CurrentParameters->_index);
	Invalidate();
}