#include "GsUIPopupCeiling.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemIngredient.h"
#include "Sound/GsSoundPlayer.h"

#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/RichTextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIContent/Window/Fairy/GsUIFairyComposeRes.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"

void UGsUIPopupCeiling::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIPopupCeiling::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_toggleGroupGrade.OnSelectChanged.BindUObject(this, &UGsUIPopupCeiling::OnTabGradeChanged);

	_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_iconSlotHelper->Initialize(_resourceIconSelector);
	_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIPopupCeiling::OnRefreshMaterialIcon);

	_btnPrev->OnClicked.AddDynamic(this, &UGsUIPopupCeiling::OnPrevLayer);
	_btnNext->OnClicked.AddDynamic(this, &UGsUIPopupCeiling::OnNextLayer);
	_btnSummon->OnClicked.AddDynamic(this, &UGsUIPopupCeiling::OnClickSummon);
}

void UGsUIPopupCeiling::NativeDestruct()
{
	OnCelingStart.Unbind();
	OnCelingCanceled.Unbind();

	Super::NativeDestruct();
}

void UGsUIPopupCeiling::NativeConstruct()
{
	Super::NativeConstruct();

	_runCeiling = false;
	_iconSlotHelper->RefreshAll(0);
	_toggleGroupGrade.SetSelectedIndex(0, true);

	_selectLayer = 0;
	SetSelectLayer(0);
}

void UGsUIPopupCeiling::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

bool UGsUIPopupCeiling::OnBack()
{
	OnClickClose();

	return true;
}

void UGsUIPopupCeiling::OnInputCancel()
{
	OnClickClose();
}

void UGsUIPopupCeiling::OnTabGradeChanged(int32 index)
{
	OnSelectedTab(index);
}

void UGsUIPopupCeiling::OnSelectedTab(int32 index)
{
	_selectIndex = index;
}

void UGsUIPopupCeiling::OnClickSummon()
{
	OnStartSummonCeiling();
}

void UGsUIPopupCeiling::OnClickClose()
{
	for (auto widget : _childwidgets)
	{
		if (widget.IsValid()) widget->Close();
	}
	_childwidgets.Reset();
	OnCelingCanceled.ExecuteIfBound();
	Close();
}

void UGsUIPopupCeiling::OnRefreshMaterialIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	if (_ceilingMaterial)
	{
		itemIcon->SetNotOwnItem(_ceilingMaterial->id, 1);
		itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);
	}

	if(!itemIcon->OnFinishedLongPressEvent.IsBound())
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIPopupCeiling::OnLongPressIcon);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetItemEnchantLevel(0);
	itemIcon->SetDimmedImage(false);
}

void UGsUIPopupCeiling::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

void UGsUIPopupCeiling::OnPrevLayer()
{
	SetSelectLayer(_selectLayer - 1);
}

void UGsUIPopupCeiling::OnNextLayer()
{
	SetSelectLayer(_selectLayer + 1);
}

void UGsUIPopupCeiling::SetSelectLayer(uint8 layer)
{
	_selectLayer = layer;
	_textLayer = FText::FromString(FString::FormatAsNumber(layer + 1));

	_btnPrev->SetIsEnabled(layer > 0);
	_btnNext->SetIsEnabled(layer < _needCelingItemAmount.Num() - 1);

	_textItemCount = FText::AsNumber(_ownItemCount);
	_textItemRequireCount = FText::AsNumber(_needCelingItemAmount[layer]);

	_colorItemAmount = FGsUIColorHelper::GetColor((_ownItemCount >= _needCelingItemAmount[layer]) ?
		EGsUIColorType::ENCHANT_ENOUGH_COST : EGsUIColorType::ENCHANT_NEED_COST);

	float probability = _needCelingItemAmount[layer] / float(_maxCeilingAmount);
	_textceilingRate = FText::FromString(FString::Printf(TEXT("%.02f%%"), probability * 100.f));
}

void UGsUIPopupCeiling::OnCeilingResult(bool success)
{
	_runCeiling = false;
	_iconSlotHelper->RefreshAll(0);
	_toggleGroupGrade.SetSelectedIndex(_selectIndex, true);
}
