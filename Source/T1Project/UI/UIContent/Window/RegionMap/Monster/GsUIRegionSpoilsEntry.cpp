#include "GsUIRegionSpoilsEntry.h"
#include "Components/Image.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"

void UGsUIRegionSpoilsEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSpoils->OnClicked.AddDynamic(this, &UGsUIRegionSpoilsEntry::OnClickSelect);
}

void UGsUIRegionSpoilsEntry::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsSelected(false);
}

void UGsUIRegionSpoilsEntry::SetIsSelected(bool inSelect)
{
	_isSelect = inSelect;
	if (_isSelect)
	{
		_imgSelected->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_imgSelected->SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool UGsUIRegionSpoilsEntry::GetIsSelected() const
{
	return _isSelect;
}

void UGsUIRegionSpoilsEntry::OnClickSelect()
{
	if (_clickEvent)
	{
		_clickEvent(_indexInGroup);
	}
}
