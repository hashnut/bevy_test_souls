#include "GsUIMonsterInfoComboboxItem.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Core/Public/Delegates/Delegate.h"

#include "Components/Image.h"

void UGsUIMonsterInfoComboboxItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnItem->OnClicked.AddDynamic(this, &UGsUIMonsterInfoComboboxItem::OnClickItem);
}
// 아이템 클릭
void UGsUIMonsterInfoComboboxItem::OnClickItem()
{
	// ToggleGroup에 콜백 전달. 켜기 위해 true 전달.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}
void UGsUIMonsterInfoComboboxItem::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;
}

// 텍스트 세팅
void UGsUIMonsterInfoComboboxItem::SetText(const FText& In_name)
{
	_textName = In_name;
}

// 현재 레벨 item 표시
void UGsUIMonsterInfoComboboxItem::SetCurrentMonsgerLevelItem(bool In_isCurrentLevel)
{
	if (_imgSelected == nullptr)
	{
		return;
	}

	_imgSelected->SetVisibility((In_isCurrentLevel) ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}