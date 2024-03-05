#include "GsUIRegionComboboxItem.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Core/Public/Delegates/Delegate.h"

#include "Components/Image.h"

void UGsUIRegionComboboxItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnItem->OnClicked.AddDynamic(this, &UGsUIRegionComboboxItem::OnClickItem);
}
// 아이템 클릭
void UGsUIRegionComboboxItem::OnClickItem()
{
	// ToggleGroup에 콜백 전달. 켜기 위해 true 전달.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);		
	}
}
void UGsUIRegionComboboxItem::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;
}

// 텍스트 세팅
void UGsUIRegionComboboxItem::SetText(const FText& In_name)
{
	_textName = In_name;
}

// 현재 맵 item 표시
void UGsUIRegionComboboxItem::SetCurrentMapItem(bool In_isCurrentMap)
{
	if (_imgSelected == nullptr)
	{
		return;
	}

	_imgSelected->SetVisibility((In_isCurrentMap) ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIRegionComboboxItem::SetIsInTown(bool In_isInTown)
{
	_imgInTown->SetVisibility((In_isInTown == true)?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}