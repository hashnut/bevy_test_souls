#include "GsUIPresetCopy.h"

#include "Components/Image.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Management/ScopeGame/GsPresetManager.h"

#include "Net/GsNetSendServiceWorld.h"


void UGsUIPresetCopy::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnCopyFirst->OnClicked.AddDynamic(this, &UGsUIPresetCopy::OnClickCopyFirst);
	_btnCopySecond->OnClicked.AddDynamic(this, &UGsUIPresetCopy::OnClickCopySecond);
	_btnCopyThird->OnClicked.AddDynamic(this, &UGsUIPresetCopy::OnClickCopyThird);
	_btnCopyCancel->OnClicked.AddDynamic(this, &UGsUIPresetCopy::Hide);
	_btnCopyOK->OnClicked.AddDynamic(this, &UGsUIPresetCopy::OnClickCopyOK);
}

void UGsUIPresetCopy::NativeConstruct()
{
	Super::NativeConstruct();

	Hide();
}

void UGsUIPresetCopy::Show()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_copySelectedPresetId = INVALID_PRESET_ID;
	_isOpened = true;

	FText findText;
	FText::FindText(TEXT("PresetText"), TEXT("Preset_Copy_Guide"), findText);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);

	InvalidateCopy();
	InvalidatePresetName();
}

void UGsUIPresetCopy::Hide()
{
	SetVisibility(ESlateVisibility::Collapsed);
	_copySelectedPresetId = INVALID_PRESET_ID;
	_isOpened = false;
}

void UGsUIPresetCopy::OnClickPresetCopy(PresetId InPresetId)
{
	if (_copySelectedPresetId == InPresetId)
	{
		return;
	}

	if (GSPreset()->GetCurrentPresetId() == InPresetId)
	{
		return;
	}

	_copySelectedPresetId = InPresetId;

	InvalidateCopy();
}

void UGsUIPresetCopy::OnClickCopyFirst()
{
	OnClickPresetCopy(EGsPresetId::FIRST);
}

void UGsUIPresetCopy::OnClickCopySecond()
{
	OnClickPresetCopy(EGsPresetId::SECOND);
}

void UGsUIPresetCopy::OnClickCopyThird()
{
	OnClickPresetCopy(EGsPresetId::THIRD);
}

void UGsUIPresetCopy::OnClickCopyOK()
{
	PresetId currentPresetId = GSPreset()->GetCurrentPresetId();
	if (currentPresetId != _copySelectedPresetId)
	{
		FGsNetSendServiceWorld::SendReqUserPresetCopyTo(_copySelectedPresetId);
	}
	else
	{
		// 동일한 프리셋
		InvalidateCopy();
		InvalidatePresetName();
	}

	Hide();
}

void UGsUIPresetCopy::InvalidateCopy()
{
	int32 selectedIndex = _btnCopyFirst->_slotIndexSelected;
	int32 unselectedIndex = _btnCopyFirst->_slotIndexUnselected;
	PresetId currentPresetId = GSPreset()->GetCurrentPresetId();
	if (currentPresetId == _copySelectedPresetId)
	{
		_copySelectedPresetId = INVALID_PRESET_ID;
	}
	PresetId selectedPresetId = _copySelectedPresetId;

	_btnCopyFirst->SetSwitcherIndex((currentPresetId != EGsPresetId::FIRST) ? selectedIndex : unselectedIndex);
	_btnCopySecond->SetSwitcherIndex((currentPresetId != EGsPresetId::SECOND) ? selectedIndex : unselectedIndex);
	_btnCopyThird->SetSwitcherIndex((currentPresetId != EGsPresetId::THIRD) ? selectedIndex : unselectedIndex);

	// 버튼 딤드 및 선택 여부 확인
	auto GetSwitcherIndex = [currentPresetId, selectedPresetId](EGsPresetId InPresetId) -> int32
	{
		return (currentPresetId != InPresetId) ? ((selectedPresetId == InPresetId) ? PresetButtonIndex::SELECTED : PresetButtonIndex::NORMAL) : PresetButtonIndex::DIMMED;
	};

	_btnCopyFirst->SetSwitcherIndex(GetSwitcherIndex(EGsPresetId::FIRST));
	_btnCopySecond->SetSwitcherIndex(GetSwitcherIndex(EGsPresetId::SECOND));
	_btnCopyThird->SetSwitcherIndex(GetSwitcherIndex(EGsPresetId::THIRD));

	// 선택된 슬롯 유무에 따라 확인 버튼 활성화 결정
	_btnCopyOK->SetIsEnabled(_copySelectedPresetId != INVALID_PRESET_ID);
}

void UGsUIPresetCopy::InvalidatePresetName()
{
	FGsPresetManager* presetManager = GSPreset();
	if (nullptr == presetManager)
	{
		return;
	}

	_textFirst = FText::FromString(presetManager->GetPresetName(EGsPresetId::FIRST));
	_textSecond = FText::FromString(presetManager->GetPresetName(EGsPresetId::SECOND));
	_textThird = FText::FromString(presetManager->GetPresetName(EGsPresetId::THIRD));
}
