#include "GsUIIconRegionSelectButton.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

void UGsUIIconRegionSelectButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnIcon->OnClicked.AddDynamic(this, &UGsUIIconRegionSelectButton::OnClickButton);
}

void UGsUIIconRegionSelectButton::OnClickButton()
{
	if (_callbackClick != nullptr)
	{
		_callbackClick();
	}
}