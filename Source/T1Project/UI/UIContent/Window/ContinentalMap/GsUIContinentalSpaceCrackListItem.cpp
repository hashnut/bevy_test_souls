#include "GsUIContinentalSpaceCrackListItem.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/Image.h"

#include "Core/Public/Delegates/Delegate.h"

void UGsUIContinentalSpaceCrackListItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	_btnItem->OnClicked.AddDynamic(this, &UGsUIContinentalSpaceCrackListItem::OnClickButton);
}

// set data 
void UGsUIContinentalSpaceCrackListItem::SetContinentalSpaceCrackListItemData(bool In_isDeco, const FText& In_name, bool In_isSpawned)
{
	int decoSwitchType = (In_isDeco == false) ? 0 : 1;
	_switcherDecoCheckType->SetActiveWidgetIndex(decoSwitchType);

	if (In_isDeco == false)
	{	
		_textMapName = In_name;

		int switchType = (In_isSpawned == false) ? 0 : 1;
		_switcherActiveType->SetActiveWidgetIndex(switchType);
	}
}

void UGsUIContinentalSpaceCrackListItem::OnClickButton()
{
	// ToggleGroup에 콜백 전달. 켜기 위해 true 전달.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}

void UGsUIContinentalSpaceCrackListItem::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	ESlateVisibility newVisibility = (bInIsSelected == 1) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	_imgSelect->SetVisibility(newVisibility);
	
}
bool UGsUIContinentalSpaceCrackListItem::GetIsSelected() const
{
	return _bIsSelected;
}