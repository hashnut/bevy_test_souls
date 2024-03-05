#include "GsUISealNode.h"

#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Window/GsUIWindowSeal.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UILib/Base/GsUIVFX.h"

void UGsUISealNode::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_colorActiveVfxs.Empty();
	//_colorActiveVfxs.Emplace(SealSlotColor::NONE, MakeWeakObjectPtr<UGsUIVFX>(_colorActiveVfx0));
	_colorActiveVfxs.Emplace(SealSlotColor::RARE, MakeWeakObjectPtr<UGsUIVFX>(_colorActiveVfx1));
	_colorActiveVfxs.Emplace(SealSlotColor::EPIC, MakeWeakObjectPtr<UGsUIVFX>(_colorActiveVfx2));
	_colorActiveVfxs.Emplace(SealSlotColor::UNIQUE, MakeWeakObjectPtr<UGsUIVFX>(_colorActiveVfx3));

	if (nullptr != _button)
	{
		_button->OnToggleButtonSelected.AddUniqueDynamic(this, &UGsUISealNode::OnToggleButtonSelected);
	}
}

void UGsUISealNode::SetParameters(Parameters InParamerters)
{
	_parameters = MoveTemp(InParamerters);

	if (_parameters._showSlotColor)
	{
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

		_colorSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_colorSwitcher->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUISealNode::AddWidgetsOnSelected(UWidget* InWidget)
{
	if (nullptr == InWidget)
	{
		return;
	}

	for (TWeakObjectPtr<UWidget>& WidgetOnSelectedPtr : _widgetsOnSelected)
	{
		if (!WidgetOnSelectedPtr.IsValid())
		{
			continue;
		}

		if (WidgetOnSelectedPtr.Get() == InWidget)
		{
			return;
		}
	}

	_widgetsOnSelected.Emplace(MakeWeakObjectPtr(InWidget));
}

UGsToggleButton* UGsUISealNode::GetToggleButton() const
{
	return _button;
}

void UGsUISealNode::OnToggleButtonSelected(bool InSelected)
{
	_selectedSwitcher->SetActiveWidgetIndex(InSelected ? 1 : 0);
	for (TWeakObjectPtr<UWidget>& WidgetOnSelectedPtr : _widgetsOnSelected)
	{
		if (WidgetOnSelectedPtr.IsValid())
		{
			WidgetOnSelectedPtr->SetVisibility(InSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}
	}
}

void UGsUISealNode::OnUIEvent(const FGsUIWindowSealEventParameters& InEventParameters)
{
	switch (InEventParameters._event)
	{
	case EGsUISealWindowEvent::SealNodeValue:
	{
		if ((InEventParameters._nodeId == _parameters._sealNodeId) && _parameters._showSlotColor)
		{
			for (const TPair<SealSlotColor, TWeakObjectPtr<UGsUIVFX>>& ColorActiveVfxPair : _colorActiveVfxs)
			{
				if (ColorActiveVfxPair.Value.IsValid())
				{
					if (ColorActiveVfxPair.Key == _parameters._sealColor)
					{
						ColorActiveVfxPair.Value.Get()->ShowVFX();
					}
					else
					{
						ColorActiveVfxPair.Value.Get()->HideVFX();
					}
				}
			}
		}
		break;
	}
	}
}
