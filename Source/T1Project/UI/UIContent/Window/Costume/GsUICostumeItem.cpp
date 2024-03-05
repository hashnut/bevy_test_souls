
#include "GsUICostumeItem.h"
#include "Costume/GsCostumeData.h"

#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGame/GsPresetManager.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Preset/GsPresetHelper.h"

#include "Engine/AssetManager.h"

#include "PaperSprite.h"
#include "Image.h"
#include "TextBlock.h"
#include "CanvasPanel.h"
#include "WidgetSwitcher.h"

void UGsUICostumeItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSelect->OnClicked.AddDynamic(this, &UGsUICostumeItem::OnSelected);
	_btnSelectCollection->OnClicked.AddDynamic(this, &UGsUICostumeItem::OnSelected);
}

void UGsUICostumeItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (_isBlocked)
	{
		_blockTime -= InDeltaTime;
		if (_blockTime <= 0.f)
		{
			SetBlockSelected(false, 0.f);
		}
	}
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUICostumeItem::RefreshUI()
{
	Super::RefreshUI();

	if (_data.IsValid())
	{
		SetVisibility(ESlateVisibility::Visible);
		CommonRefreshUI();

		switch (WidgetType)
		{
		case UGsUICostumeItem::UIItemType::NONE:
		case UGsUICostumeItem::UIItemType::ITEM:
			CostumeDataRefreshUI();
			break;
		case UGsUICostumeItem::UIItemType::COMPOSITION:
			CostumeDataComposeUI();
			break;
		case UGsUICostumeItem::UIItemType::COLLECTION:
			CostumeDataCollectionUI();
			break;
		case UGsUICostumeItem::UIItemType::RESOURCE:
			CostumeDataResourceUI();
			break;
		case UGsUICostumeItem::UIItemType::SUMMONWAIT:
			CostumeDataSummonWaitUI();
			break;
		case UGsUICostumeItem::UIItemType::POPUP_COLLECTION:
			CostumeDataPopupCollectionUI();
			break;
		case UGsUICostumeItem::UIItemType::APPEARANCE:
			CostumeDataAppearanceUI();
			break;
		case UGsUICostumeItem::UIItemType::EXCHANGE:
			CostumeDataExchangeUI();
			break;
		default:
			break;
		}
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}	

void UGsUICostumeItem::CommonRefreshUI()
{
	if (auto costumeData = _data.Pin())
	{
		_btnSelectCollection->SetVisibility(ESlateVisibility::Collapsed);
		_btnSelectCollection->SetIsEnabled(false);
		_btnSelect->SetIsEnabled(false);
		_redDot->SetVisibility(ESlateVisibility::Hidden);
		_checkIcon->SetVisibility(ESlateVisibility::Hidden);
		_completeMark->SetVisibility(ESlateVisibility::Hidden);
		_summoned->SetVisibility(ESlateVisibility::Hidden);
		_imagePreset->SetVisibility(ESlateVisibility::Collapsed);
		_textAmount->SetVisibility(ESlateVisibility::Hidden);

		_bgSelected->SetVisibility(_isSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		_bgNotCollected->SetVisibility(costumeData->isCollectedCostume() ?
			ESlateVisibility::Hidden : ESlateVisibility::Visible);

		_textAmount->SetText(FText::AsNumber(costumeData->GetAmount()));
	}
}

void UGsUICostumeItem::CostumeDataRefreshUI()
{
	if (auto costumeData = _data.Pin())
	{
		auto wearOn = GSCostume()->GetWearOnCostumeData();
		bool isEquipped = (wearOn.IsValid() && wearOn.Pin()->_id == costumeData->_id);
		bool isPreset = !isEquipped && FGsPresetHelper::IsPreset(costumeData->GetPresetValue()) && GSPreset()->IsActivePreset();

		_btnSelect->SetIsEnabled(true);

		_summoned->SetVisibility(isEquipped ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		_imagePreset->SetVisibility(isPreset ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		_redDot->SetVisibility(costumeData->_isNew ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		_textAmount->SetVisibility(costumeData->GetAmount() > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UGsUICostumeItem::CostumeDataComposeUI()
{
	if (auto costumeData = _data.Pin())
	{
		_btnSelect->SetIsEnabled(costumeData->GetAmount() - _useCount > 0);
		_bgSelected->SetVisibility(ESlateVisibility::Hidden);

		_textAmount->SetVisibility(_useCount >= 0 && costumeData->GetAmount() - _useCount > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		_textAmount->SetText(FText::AsNumber(costumeData->GetAmount() - _useCount));

		_bgNotCollected->SetVisibility(_useCount > 0 && costumeData->GetAmount() - _useCount == 0 ?
			ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UGsUICostumeItem::CostumeDataCollectionUI()
{
	_btnSelectCollection->SetVisibility(ESlateVisibility::Visible);
	_btnSelectCollection->SetIsEnabled(true);
	_bgSelected->SetVisibility(ESlateVisibility::Hidden);
	_redDot->SetVisibility(_isNewComplete ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	_completeMark->SetVisibility(!_complete ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	_checkIcon->SetVisibility(_complete ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	_bgNotCollected->SetVisibility(ESlateVisibility::Hidden);
}

void UGsUICostumeItem::CostumeDataResourceUI()
{
	if (auto costumeData = _data.Pin())
	{
		_btnSelect->SetIsEnabled(costumeData->GetAmount() - _useCount > 0);
		_bgSelected->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsUICostumeItem::CostumeDataSummonWaitUI()
{
	_btnSelectCollection->SetVisibility(ESlateVisibility::Visible);
	_btnSelectCollection->SetIsEnabled(true);
	_redDot->SetVisibility(ESlateVisibility::Hidden);
	_bgNotCollected->SetVisibility(ESlateVisibility::Hidden);
}

void UGsUICostumeItem::CostumeDataPopupCollectionUI()
{
	_bgSelected->SetVisibility(ESlateVisibility::Hidden);
	_redDot->SetVisibility(_isNewComplete ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	_completeMark->SetVisibility(!_complete ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	_checkIcon->SetVisibility(_complete ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	_bgNotCollected->SetVisibility(ESlateVisibility::Hidden);
}

void UGsUICostumeItem::CostumeDataAppearanceUI()
{
	if (auto costumeData = _data.Pin())
	{
		auto wearOn = GSCostume()->GetWearOnCostumeData();
		ESlateVisibility eqiupVisible = (wearOn.IsValid() && wearOn.Pin()->_id == costumeData->_id) ?
			ESlateVisibility::Visible : ESlateVisibility::Hidden;

		_btnSelect->SetIsEnabled(true);
		_summoned->SetVisibility(eqiupVisible);
	}
}

void UGsUICostumeItem::CostumeDataExchangeUI()
{
	if (auto costumeData = _data.Pin())
	{
		auto wearOn = GSCostume()->GetWearOnCostumeData();
		bool isEquipped = (wearOn.IsValid() && wearOn.Pin()->_id == costumeData->_id);

		_btnSelect->SetIsEnabled(true);

		_summoned->SetVisibility(isEquipped ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		_imagePreset->SetVisibility(ESlateVisibility::Hidden);
		_redDot->SetVisibility(costumeData->_isNew ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		_textAmount->SetVisibility(ESlateVisibility::Hidden);
	}
}

CostumeId UGsUICostumeItem::GetCostumeID()
{
	if (auto costumeData = _data.Pin())
	{
		return costumeData->_id;
	}
	return 0;
}

void UGsUICostumeItem::SetSelected(bool isSelected, bool isNotiSelect)
{ 
	_isSelected = isSelected;
	SetBlockSelected(false, 0.f);
	if (_isSelected && isNotiSelect)
	{
		if (auto costumeData = _data.Pin())
		{
			OnItemSelected.ExecuteIfBound(costumeData->_id);
		}
	}
}

void UGsUICostumeItem::SetBlockSelected(bool selectBlock /*= true*/, float blockTime /*= 0.5f*/)
{
	_isBlocked = selectBlock;
	_blockTime = blockTime;

	_btnSelect->SetIsEnabled(!selectBlock);
}

void UGsUICostumeItem::OnSelected()
{
	if (_isBlocked) return;

	if (auto costumeData = _data.Pin())
	{
		OnItemSelected.ExecuteIfBound(costumeData->_id);
	}
}
