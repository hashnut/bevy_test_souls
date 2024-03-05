// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICustomizePreset.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Window/Customize/GsUICustomizePresetEntry.h"
#include "UI/UIContent/Window/Customize/GsCustomizeUIData.h"
#include "DataSchema/Customize/GsSchemaCustomizeMenu.h"
#include "GameObject/Customize/GsCustomizeHandler.h"
#include "UI/UIContent/Helper/GsUIHelper.h"


void UGsUICustomizePreset::BeginDestroy()
{
	if (_slotHelperPreset)
	{
		_slotHelperPreset->OnRefreshEntry.RemoveDynamic(this, &UGsUICustomizePreset::OnRefreshEntryPreset);
	}
	_slotHelperPreset = nullptr;

	Super::BeginDestroy();
}

void UGsUICustomizePreset::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelperPreset = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperPreset->Initialize(_presetEntryWidgetClass, _panelPreset);
	_slotHelperPreset->OnRefreshEntry.AddDynamic(this, &UGsUICustomizePreset::OnRefreshEntryPreset);

	_toggleGroupPreset.OnSlotClicked.BindUObject(this, &UGsUICustomizePreset::OnClickPreset);

	_btnRandom->OnClicked.AddDynamic(this, &UGsUICustomizePreset::OnClickRandomButton);
}

void UGsUICustomizePreset::Show()
{
	if (nullptr == _selectedData)
	{
		return;
	}

	// 프리셋 슬롯 세팅
	if (const FGsSchemaCustomizeMenu* menu = _selectedData->GetMenu())
	{
		_slotHelperPreset->RefreshAll(menu->presetList.Num());
	}

	_toggleGroupPreset.Clear();
	_toggleGroupPreset.AddToggleWidgetByParentPanel(_panelPreset);

	int32 presetIndex = _selectedData->GetValue(EGsUICustomizeType::PRESET_INDEX);
	_toggleGroupPreset.SetSelectedIndex(presetIndex);
	
	SetPreset(presetIndex);
}

void UGsUICustomizePreset::OnRefreshEntryPreset(int32 InIndex, UWidget* InEntry)
{
	UGsUICustomizePresetEntry* entry = Cast<UGsUICustomizePresetEntry>(InEntry);
	if (entry)
	{
		int32 tempId = InIndex + 1;
		entry->SetData(tempId);
	}
}

void UGsUICustomizePreset::OnClickPreset(int32 InIndex, bool bIsSelected)
{
	if (false == bIsSelected)
	{
		return;
	}

	// 프리셋이 바뀌지 않았는데 들어오면 무시
	if (InIndex == _selectedData->GetValue(EGsUICustomizeType::PRESET_INDEX))
	{
		return;
	}

	// 변경된 값이 없을 경우 바로적용
	if (false == _selectedData->IsChanged())
	{
		SetPreset(InIndex);
		return;
	}

	// 변경된 값이 있을 경우 팝업을 띄워 의사 체크

	// TEXT: 편집 내용이 변경됩니다. 변경하시겠습니까?
	FText textMsg;
	FText::FindText(TEXT("UICommonText"), TEXT("CustomizePresetChange"), textMsg);
	FGsUIHelper::PopupYesNo(textMsg, [this, InIndex](bool bYes)
		{
			if (bYes)
			{
				SetPreset(InIndex);
			}
			else
			{
				// 슬롯을 다시 되돌려놓는다.
				_toggleGroupPreset.SetSelectedIndex(_selectedData->GetValue(EGsUICustomizeType::PRESET_INDEX));
			}
		});
}

void UGsUICustomizePreset::OnClickRandomButton()
{
	OnClickRandom.ExecuteIfBound();
}

void UGsUICustomizePreset::SetPreset(int32 InIndex)
{
	OnSetPreset.ExecuteIfBound(InIndex);
}
