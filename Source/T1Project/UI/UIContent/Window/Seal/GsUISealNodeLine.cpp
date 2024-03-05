#include "GsUISealNodeLine.h"

#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "UI/UIContent/Window/GsUIWindowSeal.h"
#include "UI/UILib/Base/GsUIVFX.h"

void UGsUISealNodeLine::SetParameters(Parameters InParamerters)
{
	_parameters = MoveTemp(InParamerters);

	int32 ColorIndex = 0;
	switch (_parameters._sealColor)
	{
	case SealSlotColor::NONE:
		ColorIndex = 0;
		break;
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
	_colorSwitcher->SetVisibility(_parameters._isVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUISealNodeLine::OnUIEvent(const FGsUIWindowSealEventParameters& InEventParameters)
{
	switch (InEventParameters._event)
	{
	case EGsUISealWindowEvent::Reset:
	{
		//_colorSwitcher->SetVisibility(ESlateVisibility::HitTestInvisible);
		if (nullptr != _casting)
		{
			_casting->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	break;

	case EGsUISealWindowEvent::SealLines:
	{
		if (InEventParameters._nodeId == _parameters._sealNodeId)
		{
			//_colorSwitcher->SetVisibility(ESlateVisibility::Collapsed);
			if (nullptr != _casting)
			{
				_casting->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
	}
	break;

	case EGsUISealWindowEvent::SealLinesValue:
	{
		//_colorSwitcher->SetVisibility(ESlateVisibility::HitTestInvisible);
		if (nullptr != _casting)
		{
			_casting->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	break;
	}
}
