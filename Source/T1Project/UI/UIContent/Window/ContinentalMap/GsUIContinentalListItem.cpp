#include "GsUIContinentalListItem.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsAnimatedImage.h"

#include "UI/UIContent/Window/ContinentalMap/GsUIAnimatedWidget.h"

#include "UMG/Public/Components/WidgetSwitcher.h"

#include "Core/Public/Delegates/Delegate.h"



void UGsUIContinentalListItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	_btnItem->OnClicked.AddDynamic(this, &UGsUIContinentalListItem::OnClickButton);


}

void UGsUIContinentalListItem::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	int tabType = (bInIsSelected == true) ? 0 : 1;
	_switcherType->SetActiveWidgetIndex(tabType);
}
bool UGsUIContinentalListItem::GetIsSelected() const
{
	return _bIsSelected;
}

// 클릭 버튼
void UGsUIContinentalListItem::OnClickButton()
{
	// ToggleGroup에 콜백 전달. 켜기 위해 true 전달.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}

// 텍스트 세팅
void UGsUIContinentalListItem::SetText(const FText& In_name, const FText& In_timeZone)
{
	_textNameSelected = In_name;
	_textNameUnSelected = In_name;

	_textTimeZoneSelected = In_timeZone;
	_textTimeZoneUnSelected = In_timeZone;
}

// 대륙맵안인가
void UGsUIContinentalListItem::SetIsInContinentalMap(bool In_inMap)
{
	int switchIndex = (In_inMap == true) ? 0 : 1;
	_switchLocal->SetActiveWidgetIndex(switchIndex);
}