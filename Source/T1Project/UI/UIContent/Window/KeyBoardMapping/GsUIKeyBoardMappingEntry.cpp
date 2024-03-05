// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIKeyBoardMappingEntry.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Components/TextBlock.h"
#include "UserWidget.h"
#include "Option/GsOptionEnumType.h"
#include "UMG/Public/Components/InputKeySelector.h"
#include "UMG/Public/Components/Image.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"


void UGsUIKeyBoardMappingEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_keySelector->OnKeySelected.AddDynamic(this, &UGsUIKeyBoardMappingEntry::OnKeySelect);
	_keySelector->OnIsSelectingKeyChanged.AddDynamic(this, &UGsUIKeyBoardMappingEntry::OnIsSelectingKeyChanged);
}

void UGsUIKeyBoardMappingEntry::SetData(EGsKeyMappingType InType, const FInputChord& InInputChord)
{
	_keyMapptingType = InType;

	// 특수키의 경우, IsValidChord 함수가 false 를 return 하므로, Key 값이 None 인지를 체크하도록 변경(C2URWQ-3495)
	if (InInputChord.Key.IsValid())
	{
		//_selectKeyName->SetText(InInputChord.GetInputText());
		_selectKeyName->SetText(InInputChord.Key.GetDisplayName(false));
	}
	else
	{
		_selectKeyName->SetText(FText::GetEmpty());
	}

	// 직접 셋팅
	_keySelector->SetSelectedKey(InInputChord);	
}

void UGsUIKeyBoardMappingEntry::SetDataEmpty()
{
	_selectKeyName->SetText(FText::GetEmpty());
	_keySelector->SetSelectedKey(FInputChord());
}

void UGsUIKeyBoardMappingEntry::OnKeySelect(FInputChord inInputChord)
{
	// GetDisplayName(false) 를 통해 '하이픈' 등의 기호를 실제 기호로 표시
	_selectKeyName->SetText(inInputChord.Key.GetDisplayName(false));
	
	OnSelected.ExecuteIfBound(_keyMapptingType, inInputChord);
}

void UGsUIKeyBoardMappingEntry::OnIsSelectingKeyChanged()
{
	bool isVisible = _selectImg->IsVisible();
	_selectImg->SetVisibility(isVisible ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

	if (nullptr != _touchEffect)
	{
		_touchEffect->SetVisibility((isVisible) ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible );

		if (!isVisible)
		{
			_touchEffect->ShowVFX();
		}
		else
		{
			_touchEffect->HideVFX();
		}
	}

	OnSelectingKeyChanged.ExecuteIfBound(!isVisible, _keyMapptingType);
}

void UGsUIKeyBoardMappingEntry::SetTextKeyName(const FText& InKeyName)
{
	_selectKeyName->SetText(InKeyName);
}

const FInputChord& UGsUIKeyBoardMappingEntry::GetSelectedKey()
{
	return _keySelector->SelectedKey;
}