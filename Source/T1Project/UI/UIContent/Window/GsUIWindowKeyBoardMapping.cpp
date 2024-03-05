// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowKeyBoardMapping.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageUI.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/GsScopeHolder.h"

#include "UMG/Public/Components/InputKeySelector.h"
#include "UMG/Public/Components/Image.h"

#include "Option/GsOptionEnumType.h"
#include "Option/GsInputSettings.h"

#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "UMG/Public/Components/TextBlock.h"
#include "KeyBoardMapping/GsUIKeyBoardMappingCircle.h"
#include "KeyBoardMapping/GsUIKeyBoardMappingRect.h"
#include "KeyBoardMapping/GsUIKeyBoardMappingEntry.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "Runtime/Slate/Public/Framework/Commands/InputChord.h"
#include "Option/GsGameUserSettings.h"
#include "Runtime/DataCenter/Public/DataSchema/Option/GsSchemaOptionConfig.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "UI/UIContent/Helper/GsUIHelper.h"


void UGsUIWindowKeyBoardMapping::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIWindowKeyBoardMapping::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_helpContentsKey = EGsHelpContents::KeyMapping;

	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowKeyBoardMapping::OnClickCloseAllStack);
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowKeyBoardMapping::OnClickBack);
	_btnRefresh->OnClicked.AddDynamic(this, &UGsUIWindowKeyBoardMapping::OnClickRefresh);

	_btnSkillLeftArrow->OnClicked.AddDynamic(this, &UGsUIWindowKeyBoardMapping::OnClickSkillLeftArrow);
	_btnSkillRightArrow->OnClicked.AddDynamic(this, &UGsUIWindowKeyBoardMapping::OnClickSkillRightArrow);

	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_MOVE_UP, _moveUp);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_MOVE_DOWN, _moveDown);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_MOVE_LEFT, _moveLeft);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_MOVE_RIGHT, _moveRight);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_NORMALATTACK, _normalAttack);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_PICKUP, _pickUp);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_SCAN, _scan);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_SWITCH_SPIRITSHOT, _switchSpiritshot);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_SWITCH_AUTO, _switchAuto);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_SWITCH_AUTOPOTION, _switchAutopotion);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_SKILLSLOT_1, _skillslot_1);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_SKILLSLOT_2, _skillslot_2);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_SKILLSLOT_3, _skillslot_3);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_SKILLSLOT_4, _skillslot_4);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_SKILLSLOT_5, _skillslot_5);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_SKILLSLOT_6, _skillslot_6);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_SKILLSLOT_7, _skillslot_7);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_SKILLSLOT_8, _skillslot_8);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_SKILLSLOT_9, _skillslot_9);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_SKILLSLOT_10, _skillslot_10);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_SKILLSLOT_11, _skillslot_11);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_SKILLSLOT_12, _skillslot_12);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_COSTUMESKILL_1, _costumeskillslot_1);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_1, _quickslot_1);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_2, _quickslot_2);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_3, _quickslot_3);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_4, _quickslot_4);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_5, _quickslot_5);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_6, _quickslot_6);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_7, _quickslot_7);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_8, _quickslot_8);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_9, _quickslot_9);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_10, _quickslot_10);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_LEFT, _quickslotLeft);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_RIGHT, _quickslotRight);

	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_CAM_CHANGE, _camChange);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_MINIMAP, _minimap);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_BOOKMARK, _bookmark);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_HUD_TAP_PREV, _hudTapPrev);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_HUD_TAP_NEXT, _hudTapNext);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_CHAT, _chat);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_CHARACTER, _character);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_BMSHOP, _bmshop);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_INVENTORY, _inventory);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_SKILL, _skill);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_EVENT, _event);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_MENU, _menu);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_OK, _ok);
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_NO, _no);	
	_keySelectors.Emplace(EGsKeyMappingType::KEY_MAPPING_TARGET_CHANGE, _targetChange);

	for (auto& iter : _keySelectors)
	{
		UGsUIKeyBoardMappingEntry* inputKeyEntry = iter.Value;
		if (nullptr != inputKeyEntry)
		{
			inputKeyEntry->OnSelected.BindUObject(this, &UGsUIWindowKeyBoardMapping::OnKeySelected);
			inputKeyEntry->OnSelectingKeyChanged.BindUObject(this, &UGsUIWindowKeyBoardMapping::SetSelectKeyName);
		}
	}
}

void UGsUIWindowKeyBoardMapping::NativeConstruct()
{
	Super::NativeConstruct();

	// 이벤트 등록
	_msgHandle = GMessage()->GetUI().AddUObject(MessageUI::KEYBOARD_MAPPING_UI_UPDATE, this, &UGsUIWindowKeyBoardMapping::OnUpdate);

	// 사용 중인 키맵핑값 얻어 와서 적용
	_inputActionBaseKeyMappings.Empty();
	_inputActionKeyMappings.Empty();
	_inputAxisKeyMappings.Empty();
	GInputSettings()->GetKeyMappingDatas(_inputActionBaseKeyMappings, _inputActionKeyMappings, _inputAxisKeyMappings);

	auto Func = [&](TMap<EGsKeyMappingType, FInputActionKeyMapping>& InMap)
	{
		for (auto& iter : InMap)
		{
			if (_keySelectors.Contains(iter.Key))
			{
				UGsUIKeyBoardMappingEntry* entry = _keySelectors.FindRef(iter.Key);
				if (entry)
				{
					FInputActionKeyMapping& keyMapping = iter.Value;
					entry->SetData(iter.Key, 
						FInputChord(keyMapping.Key, keyMapping.bShift, keyMapping.bCtrl, keyMapping.bAlt, keyMapping.bCmd));
				}
			}
		}
	};

	Func(_inputActionBaseKeyMappings);
	Func(_inputActionKeyMappings);

	for (auto& iter : _inputAxisKeyMappings)
	{
		if (_keySelectors.Contains(iter.Key))
		{
			UGsUIKeyBoardMappingEntry* entry = _keySelectors.FindRef(iter.Key);
			if (entry)
			{
				FInputAxisKeyMapping& keyMapping = iter.Value;
				entry->SetData(iter.Key,
					FInputChord(keyMapping.Key));
			}
		}
	}

	_scrollSkill->ScrollToStart();
	_scrollSkillIndex = 0;
}

void UGsUIWindowKeyBoardMapping::NativeDestruct()
{
	if (_msgHandle.Value.IsValid())
	{
		GMessage()->GetUI().Remove(_msgHandle);
	}

	Super::NativeDestruct();
}


void UGsUIWindowKeyBoardMapping::CloseInternal()
{
	Super::CloseInternal();
}

bool UGsUIWindowKeyBoardMapping::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsKeyBoardMapping))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_KEYBOARD_MAPPING);
		return true;
	}

	return false;
}

void UGsUIWindowKeyBoardMapping::OnClickRefresh()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupYesNo"));
	if (!widget.IsValid())
	{
		GSLOG(Error, TEXT("!widget.IsValid()"));
		return;
	}

	if (UGsUIPopupYesNo* popup = Cast<UGsUIPopupYesNo>(widget.Get()))
	{
		FText textDesc;
		FText::FindText(TEXT("OptionText"), TEXT("Keyboard_Popup_Reset_Desc"), textDesc);

		TFunction<void(bool)> InCallback = [this](bool InIsOk)
		{
			if (true == InIsOk)
			{
				// 키맵핑 초기값을 얻어와서 적용
				_inputActionBaseKeyMappings.Empty();
				_inputActionKeyMappings.Empty();
				_inputAxisKeyMappings.Empty();
				GInputSettings()->GetDefaultKeyMappingDatas(
					_inputActionBaseKeyMappings, 
					_inputActionKeyMappings, 
					_inputAxisKeyMappings);
				// 갱신
				OnUIUpdate();

				return;
			}
		};
		popup->SetData(textDesc, InCallback);
	}
}

void UGsUIWindowKeyBoardMapping::OnUIUpdate()
{
	auto Func = [&](TMap<EGsKeyMappingType, FInputActionKeyMapping>& InMap)
	{
		for (auto& iter : InMap)
		{
			if (_keySelectors.Contains(iter.Key))
			{
				class UGsUIKeyBoardMappingEntry* entry = _keySelectors.FindRef(iter.Key);
				if (entry)
				{
					FInputActionKeyMapping& keyMapping = iter.Value;
					entry->SetData(iter.Key, FInputChord(keyMapping.Key, keyMapping.bShift, keyMapping.bCtrl, keyMapping.bAlt, keyMapping.bCmd));					
				}
			}
		}
	};
	Func(_inputActionBaseKeyMappings);
	Func(_inputActionKeyMappings);

	for (auto& iter : _inputAxisKeyMappings)
	{
		if (_keySelectors.Contains(iter.Key))
		{
			class UGsUIKeyBoardMappingEntry* entry = _keySelectors.FindRef(iter.Key);
			if (entry)
			{
				FInputAxisKeyMapping& keyMapping = iter.Value;
				entry->SetData(iter.Key, FInputChord(keyMapping.Key));				
			}
		}
	}
}

void UGsUIWindowKeyBoardMapping::OnClickBack()
{
	for (auto& iter : _keySelectors)
	{
		UGsUIKeyBoardMappingEntry* entry = iter.Value;
		if (nullptr != entry)
		{
			GInputSettings()->SetKeyboardMappingData(iter.Key, entry->GetSelectedKey());
		}
	}
	
	GInputSettings()->SaveAll();

	;
	if (UGsOptionManager* optionMgr = GOption())
	{
		if (const FGsSchemaOptionConfig* config = GGameUserSettings()->GetOptionConfig())
		{
			optionMgr->SetOptionWindowMenuIndex(config->_keyMappingParentTabIndex);
			optionMgr->SetOptionWindowDivisionIndex(config->_keyMappingChildTabIndex);
			optionMgr->SetScrollOffset(config->_keyMappingScrollIndex);
		}
	}

	Super::OnClickBack();
}

void UGsUIWindowKeyBoardMapping::OnClickCloseAllStack()
{
	for (auto& iter : _keySelectors)
	{
		UGsUIKeyBoardMappingEntry* entry = iter.Value;
		if (nullptr != entry)
		{
			GInputSettings()->SetKeyboardMappingData(iter.Key, entry->GetSelectedKey());
		}
	}

	GInputSettings()->SaveAll();

	Super::OnClickCloseAllStack();
}

void UGsUIWindowKeyBoardMapping::OpenConsentPopup(EGsKeyMappingType In_Type, EGsKeyMappingType In_SameKeyType, 
	FInputChord In_InputChordNew, FInputChord In_InputChordOld)
{
	FText keyNameText;
	GInputSettings()->GetKeyNameTextEx(In_SameKeyType, keyNameText);

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupYesNo"));
	if (!widget.IsValid())
	{
		GSLOG(Error, TEXT("!widget.IsValid()"));
		return;
	}

	if (UGsUIPopupYesNo* popup = Cast<UGsUIPopupYesNo>(widget.Get()))
	{
		FText textDesc;
		FText::FindText(TEXT("OptionText"), TEXT("Keyboard_Popup_Alert_Change_Desc"), textDesc);
		textDesc = FText::Format(textDesc, keyNameText);

		TFunction<void(bool)> InCallback = [this, In_Type, In_SameKeyType, In_InputChordNew, In_InputChordOld](bool InIsOk)
		{
			if (false == InIsOk)
			{
				class UGsUIKeyBoardMappingEntry* entry = _keySelectors.FindRef(In_Type);
				if (entry)
				{
					entry->SetData(In_Type, In_InputChordOld);
				}
				return;
			}
			else
			{
				GInputSettings()->SetKeyboardMappingData(In_Type, In_InputChordNew);				
				GInputSettings()->SetKeyboardMappingData(In_SameKeyType, FInputChord());

				class UGsUIKeyBoardMappingEntry* entry = _keySelectors.FindRef(In_Type);
				if (entry)
				{
					entry->SetData(In_Type, In_InputChordNew);
				}
				entry = _keySelectors.FindRef(In_SameKeyType);
				if (entry)
				{
					entry->SetDataEmpty();
				}
			}
		};
		popup->SetData(textDesc, InCallback);
	}
}

void UGsUIWindowKeyBoardMapping::SetData(EGsKeyMappingType In_Type, FInputChord In_InputChord)
{
	FInputChord InputChordOld;
	GInputSettings()->GetKeyMappingData(In_Type, InputChordOld);

	// 기존 단축키가 대체되어 None 으로 들어갈 상황이라면 공백 처리 (OpenConsentPopup 에서 처리해 주었을 것이다)
	if (!In_InputChord.Key.IsValid())
	{
		class UGsUIKeyBoardMappingEntry* entry = _keySelectors.FindRef(In_Type);
		if (entry)
		{
			entry->SetData(In_Type, In_InputChord);
		}
		return;
	}

	// 새로운 단축키 입력에 특수키(Ctrl, Alt, Shift 등이 들어 있는 것 체크)가 있거나, 마우스 버튼일 경우
	if (In_InputChord.Key.IsValid() &&
		(GInputSettings()->HasSpecialKey(In_InputChord) || In_InputChord.Key.IsMouseButton()))
	{
		FText findText;
		FText::FindText(TEXT("OptionText"), TEXT("Keyboard_Notice_Change_Cannot"), findText);
		FGsUIHelper::TrayMessageTicker(findText);

		class UGsUIKeyBoardMappingEntry* entry = _keySelectors.FindRef(In_Type);
		if (entry)
		{
			entry->SetData(In_Type, InputChordOld);
		}

		return;
	}

	EGsKeyMappingType outSameKeyType;
	bool isValid = GInputSettings()->IsValidInputKey(In_Type, In_InputChord, outSameKeyType);	
	// 기존 키 맵핑에서 겹치는 게 있을 경우
	if (false == isValid)
	{
		OpenConsentPopup(In_Type, outSameKeyType, In_InputChord, InputChordOld);
	}
	// 겹치는 것이 없을 경우
	else
	{
		GInputSettings()->SetKeyboardMappingData(In_Type, In_InputChord);
	}
}

void UGsUIWindowKeyBoardMapping::OnKeySelected(EGsKeyMappingType In_Type, FInputChord inInputChord)
{
	SetData(In_Type, inInputChord);
}

void UGsUIWindowKeyBoardMapping::OnUpdate(const IGsMessageParam*)
{
	OnUIUpdate();
}
void UGsUIWindowKeyBoardMapping::SetSelectKeyName(bool In_isSelect, EGsKeyMappingType In_Type)
{
	FText keyNameText = FText::GetEmpty();
	FText textDesc = FText::GetEmpty();

	if (In_isSelect)
	{
		GInputSettings()->GetKeyNameTextEx(In_Type, keyNameText);
		
		FText::FindText(TEXT("OptionText"), TEXT("Keyboard_Notice_Change_After"), textDesc);
		textDesc = FText::Format(textDesc, keyNameText);
	}
	else
	{
		FText::FindText(TEXT("OptionText"), TEXT("Keyboard_Notice_Change_Before"), textDesc);
	}

	_selectKeyName->SetText(textDesc);
}


void UGsUIWindowKeyBoardMapping::OnClickSkillLeftArrow()
{
	if (0 >= _scrollSkillIndex)
	{
		return;
	}

	UWidget* widget = _scrollSkill->GetChildAt(--_scrollSkillIndex);
	if (nullptr != widget)
	{
		_scrollSkill->ScrollWidgetIntoView(widget);
	}
}

void UGsUIWindowKeyBoardMapping::OnClickSkillRightArrow()
{
	if (1 <= _scrollSkillIndex)
	{
		return;
	}

	UWidget* widget = _scrollSkill->GetChildAt(++_scrollSkillIndex);
	if (nullptr != widget)
	{
		_scrollSkill->ScrollWidgetIntoView(widget);
	}
}