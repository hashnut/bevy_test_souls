#include "GsUISealSlot.h"

#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "UI/UIContent/Window/GsUIWindowSeal.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UILib/Base/GsUIVFX.h"

void UGsUISealSlot::SetParameters(Parameters InParamerters)
{
	_parameters = MoveTemp(InParamerters);

	_locked->SetVisibility(_parameters._isOpened ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

	_colorSwitcher->SetVisibility(_parameters._isOpened ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	int32 ColorIndex = 0;
	switch (_parameters._sealColor)
	{
	case SealSlotColor::RARE:
		ColorIndex = 1;
		break;
	case SealSlotColor::EPIC:
		ColorIndex = 2;
		break;
	case SealSlotColor::UNIQUE:
		ColorIndex = 3;
		break;
	}
	_colorSwitcher->SetActiveWidgetIndex(ColorIndex);
}

void UGsUISealSlot::OnUIEvent(const FGsUIWindowSealEventParameters& InEventParameters)
{
	switch (InEventParameters._event)
	{
	case EGsUISealWindowEvent::Reset:
		if (nullptr != _casting)
		{
			_casting->HideVFX();
		}
		break;

	case EGsUISealWindowEvent::SealSlot0:
	case EGsUISealWindowEvent::SealSlot1:
	case EGsUISealWindowEvent::SealSlot2:
		if (nullptr != _casting && (InEventParameters._nodeId == _parameters._nodeId) && (InEventParameters._slotIndex == _parameters._slotIndex))
		{
			// 하나라도 열린 경우에 연출 출력. 모두 잠금인 경우, 연출 불필요.
			if (_parameters._isStatUnlocked || _parameters._isColorUnlocked)
			{
				_casting->ShowVFX();
			}
		}
		break;
	}
}
