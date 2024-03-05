// Fill out your copyright notice in the Description page of Project Settings.


#include "Option/GsInputSettings.h"

#include "GameFramework/InputSettings.h"
#include "Option/GsOptionEnumType.h"

#include "DataSchema/KeyMapping/GsSchemaKeyMapping.h"
#include "UTIL/GsTableUtil.h"

#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"

#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"

#include "Net/GsNetSendService.h"
#include "Message/GsMessageUI.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/GsMessageHolder.h"
#include "Text.h"
#include "Runtime/Slate/Public/Framework/Commands/InputChord.h"
#include "GameFramework/PlayerInput.h"
#include "Runtime/Engine/Classes/Engine/EngineBaseTypes.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

void FGsInputSettings::Initialize()
{
	if (nullptr == _inputSettings)
	{
		_inputSettings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	}

	InitTblKeyMappingLoad();	
}

void FGsInputSettings::Finalize()
{
	_inputSettings = nullptr;
}

void FGsInputSettings::FillActionNamesAndMappingByName()
{	
	if (_inputSettings)
	{
		_inputSettings->GetActionNames(_actionNames);

		_actionBaseMappings.Reset();
		_actionMappings.Reset();
		for (auto actionName : _actionNames)
		{
			EGsKeyMappingType keyType;
			GetKeyMappingType(actionName, keyType);

			if (keyType == EGsKeyMappingType::KEY_MAPPING_OK ||
				keyType == EGsKeyMappingType::KEY_MAPPING_NO)
			{
				_inputSettings->GetActionMappingKeyboardByName(actionName, _actionBaseMappings);
			}
			else
			{
				_inputSettings->GetActionMappingKeyboardByName(actionName, _actionMappings);
			}
		}
	}

	auto Func = [&](TArray<FInputActionKeyMapping> InArray, TMap<EGsKeyMappingType, FInputActionKeyMapping>& InMap)
	{
		for (auto iter : InArray)
		{
			FName keyName = iter.ActionName;
			for (int i = 0; i < _schemaKeyMappings.Num(); ++i)
			{
				if (const FGsSchemaKeyMapping* schemaKeyMapping = _schemaKeyMappings[i])
				{
					if (schemaKeyMapping->keyName == keyName)
					{
						if (EGsOptionKeyBindType::Action == schemaKeyMapping->keyBindType)
						{
							InMap.Emplace(schemaKeyMapping->keyMappingType, iter);
						}
						break;
					}
				}
			}
		}
	};

	_actionBaseKeyMapptings.Empty();
	Func(_actionBaseMappings, _actionBaseKeyMapptings);

	_actionKeyMapptings.Empty();
	Func(_actionMappings, _actionKeyMapptings);
}

void FGsInputSettings::FillAxisNamesAndMappingByName()
{	
	if (_inputSettings)
	{
		_inputSettings->GetAxisNames(_axisNames);

		_axisMappings.Reset();
		for (auto axisName : _axisNames)
			_inputSettings->GetAxisMappingKeyboardByName(axisName, _axisMappings);
	}

	_axisKeyMapptings.Empty();
	for (auto inputaxisKeyMappingction : _axisMappings)
	{
		FName keyName = inputaxisKeyMappingction.AxisName;
		for (int i = 0; i < _schemaKeyMappings.Num(); ++i)
		{
			if (const FGsSchemaKeyMapping* schemaKeyMapping = _schemaKeyMappings[i])
			{
				if (schemaKeyMapping->keyName == keyName)
				{
					if (EGsOptionKeyBindType::Axis == schemaKeyMapping->keyBindType)
					{
						_axisKeyMapptings.Emplace(schemaKeyMapping->keyMappingType, inputaxisKeyMappingction);
					}

					break;
				}
			}
		}
	}
}

void FGsInputSettings::InitTblKeyMappingLoad()
{
	_schemaKeyMappings.Empty();
	if (const UGsTable* table = FGsSchemaKeyMapping::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaKeyMapping>(_schemaKeyMappings);
	}

	for (const FGsSchemaKeyMapping* schemaKeyMapping : _schemaKeyMappings)
	{
		if (nullptr == schemaKeyMapping)
			continue;

		AddInitTblKey(schemaKeyMapping->keyMappingType, schemaKeyMapping);
	}

	if (_inputSettings)
	{
		_inputSettings->ForceRebuildKeymaps();
		_inputSettings->SaveKeyMappings();
	}

	FillActionNamesAndMappingByName();
	FillAxisNamesAndMappingByName();
}

void FGsInputSettings::AddInitTblKey(EGsKeyMappingType In_KeyMapptingType, const FGsSchemaKeyMapping* In_SchemaKeyMapping)
{
	if (In_SchemaKeyMapping)
	{
		if (EGsOptionKeyBindType::Action == In_SchemaKeyMapping->keyBindType)
		{	
			TArray<FInputActionKeyMapping> outActionKeyMapping;
			_inputSettings->GetActionMappingKeyboardByName(In_SchemaKeyMapping->keyName, outActionKeyMapping);

			if (0 < outActionKeyMapping.Num())
			{
				for (int i = 0; i < outActionKeyMapping.Num(); ++i)
				{
					_inputSettings->RemoveActionMapping(outActionKeyMapping[i], false);
				}
			}

			FInputActionKeyMapping newInputActionKeyMapping = FInputActionKeyMapping(In_SchemaKeyMapping->keyName,
				In_SchemaKeyMapping->keyInputChord.Key, In_SchemaKeyMapping->keyInputChord.bShift,
				In_SchemaKeyMapping->keyInputChord.bCtrl, In_SchemaKeyMapping->keyInputChord.bAlt, In_SchemaKeyMapping->keyInputChord.bCmd);

			_inputSettings->AddActionMapping(newInputActionKeyMapping);
		}
		else if (EGsOptionKeyBindType::Axis == In_SchemaKeyMapping->keyBindType)
		{
			TArray<FInputAxisKeyMapping> outAxisKeyMapping;
			_inputSettings->GetAxisMappingKeyboardByName(In_SchemaKeyMapping->keyName, outAxisKeyMapping);

			if (0 < outAxisKeyMapping.Num())
			{
				for (int i = 0; i < outAxisKeyMapping.Num(); ++i)
				{
					_inputSettings->RemoveAxisMapping(outAxisKeyMapping[i], false);
				}
			}

			FInputAxisKeyMapping newInputAxisKeyMapping = FInputAxisKeyMapping(In_SchemaKeyMapping->keyName,
				In_SchemaKeyMapping->keyInputChord.Key, In_SchemaKeyMapping->keyScale);
			_inputSettings->AddAxisMapping(newInputAxisKeyMapping);

			//_inputSettings->SaveKeyMappings();
		}
	}
}

void FGsInputSettings::SaveAll()
{
	auto Func = [&](TMap<EGsKeyMappingType, FInputActionKeyMapping>& InMap)
	{
		for (auto& iter : InMap)
		{
			FInputActionKeyMapping actionKeyMapping = iter.Value;

			TArray<FInputActionKeyMapping> outActionKeyMapping;
			_inputSettings->GetActionMappingKeyboardByName(actionKeyMapping.ActionName, outActionKeyMapping);

			if (0 < outActionKeyMapping.Num())
			{
				for (int i = 0; i < outActionKeyMapping.Num(); ++i)
				{
					_inputSettings->RemoveActionMapping(outActionKeyMapping[i], false);
				}
			}

			FInputActionKeyMapping newInputActionKeyMapping = FInputActionKeyMapping(actionKeyMapping.ActionName,
				actionKeyMapping.Key, actionKeyMapping.bShift,
				actionKeyMapping.bCtrl, actionKeyMapping.bAlt, actionKeyMapping.bCmd);

			_inputSettings->AddActionMapping(newInputActionKeyMapping);
		}
	};
	Func(_actionBaseKeyMapptings);
	Func(_actionKeyMapptings);

	for (auto& iter : _axisKeyMapptings)
	{
		FInputAxisKeyMapping axisKeyMapping = iter.Value;

		TArray<FInputAxisKeyMapping> outAxisKeyMapping;
		_inputSettings->GetAxisMappingKeyboardByName(axisKeyMapping.AxisName, outAxisKeyMapping);

		if (0 < outAxisKeyMapping.Num())
		{
			for (int i = 0; i < outAxisKeyMapping.Num(); ++i)
			{
				_inputSettings->RemoveAxisMapping(outAxisKeyMapping[i], false);
			}
		}

		FInputAxisKeyMapping newInputAxisKeyMapping = FInputAxisKeyMapping(axisKeyMapping.AxisName,
			axisKeyMapping.Key, axisKeyMapping.Scale);

		_inputSettings->AddAxisMapping(newInputAxisKeyMapping);

	}
	if (_inputSettings) // 저장은 해둠
	{
		_inputSettings->ForceRebuildKeymaps();
		_inputSettings->SaveKeyMappings();
	}
	// 서버 저장
	SaveServerKeyboardMapping();
}

void FGsInputSettings::GetActionBaseKeyMappingDatas(OUT TMap<EGsKeyMappingType, FInputActionKeyMapping>& Out_InputActionBaseKeyMappings)
{
	Out_InputActionBaseKeyMappings = _actionBaseKeyMapptings;
}

void FGsInputSettings::GetActionKeyMappingDatas(OUT TMap<EGsKeyMappingType, FInputActionKeyMapping>& Out_InputActionKeyMappings)
{
	Out_InputActionKeyMappings = _actionKeyMapptings;
}

void FGsInputSettings::GetAxisKeyMappingDatas(OUT TMap<EGsKeyMappingType, FInputAxisKeyMapping>& Out_InputAxisKeyMappings)
{
	Out_InputAxisKeyMappings = _axisKeyMapptings;
}

void FGsInputSettings::GetDefaultKeyMappingDatas(OUT TMap<EGsKeyMappingType, FInputActionKeyMapping>& Out_InputActionBaseKeyMappings,
	OUT TMap<EGsKeyMappingType, FInputActionKeyMapping>& Out_InputActionKeyMappings,
	OUT TMap<EGsKeyMappingType, FInputAxisKeyMapping>& Out_InputAxisKeyMappings)
{
	InitTblKeyMappingLoad();

	Out_InputActionBaseKeyMappings = _actionBaseKeyMapptings;
	Out_InputActionKeyMappings = _actionKeyMapptings;
	Out_InputAxisKeyMappings = _axisKeyMapptings;
}

void FGsInputSettings::GetKeyMappingDatas(OUT TMap<EGsKeyMappingType, FInputActionKeyMapping>& Out_InputActionBaseKeyMappings, 
	OUT TMap<EGsKeyMappingType, FInputActionKeyMapping>& Out_InputActionKeyMappings,
	OUT TMap<EGsKeyMappingType, FInputAxisKeyMapping>& Out_InputAxisKeyMappings)
{
	Out_InputActionBaseKeyMappings = _actionBaseKeyMapptings;
	Out_InputActionKeyMappings = _actionKeyMapptings;
	Out_InputAxisKeyMappings = _axisKeyMapptings;
}

void FGsInputSettings::GetKeyMappingData(EGsKeyMappingType In_Type, FInputChord& Out_InputChord)
{
	if (_actionBaseKeyMapptings.Contains(In_Type))
	{
		FInputActionKeyMapping actionBaseKeyMapping = _actionBaseKeyMapptings.FindRef(In_Type);
		Out_InputChord.Set(FInputChord(actionBaseKeyMapping.Key, actionBaseKeyMapping.bShift, 
			actionBaseKeyMapping.bCtrl, actionBaseKeyMapping.bAlt, actionBaseKeyMapping.bCmd));

		return;
	}

	if (_actionKeyMapptings.Contains(In_Type))
	{
		FInputActionKeyMapping actionKeyMapping = _actionKeyMapptings.FindRef(In_Type);
		Out_InputChord.Set(FInputChord(actionKeyMapping.Key, actionKeyMapping.bShift, 
			actionKeyMapping.bCtrl, actionKeyMapping.bAlt, actionKeyMapping.bCmd));
			
		return;
	}

	if (_axisKeyMapptings.Contains(In_Type))
	{
		FInputAxisKeyMapping axisKeyMapping = _axisKeyMapptings.FindRef(In_Type);
		Out_InputChord.Set(FInputChord(axisKeyMapping.Key));

		return;
	}
}

bool FGsInputSettings::IsValidInputKey(EGsKeyMappingType In_Type, FInputChord In_InputChord, OUT EGsKeyMappingType& Out_SameTypeKey)
{
	auto Func = [&](TMap<EGsKeyMappingType, FInputActionKeyMapping>& InMap)
	{		
		FInputActionKeyMapping actionKeyMapping = InMap.FindRef(In_Type);
		if (actionKeyMapping.Key == In_InputChord.Key &&
			actionKeyMapping.bShift == In_InputChord.bShift &&
			actionKeyMapping.bCtrl == In_InputChord.bCtrl &&
			actionKeyMapping.bAlt == In_InputChord.bAlt &&
			actionKeyMapping.bCmd == In_InputChord.bCmd)
		{
			return true;
		}

		for (auto& iter : InMap)
		{
			FInputActionKeyMapping saveActionKeyMapping = iter.Value;
			if (saveActionKeyMapping.Key == In_InputChord.Key &&
				saveActionKeyMapping.bShift == In_InputChord.bShift &&
				saveActionKeyMapping.bCtrl == In_InputChord.bCtrl &&
				saveActionKeyMapping.bAlt == In_InputChord.bAlt &&
				saveActionKeyMapping.bCmd == In_InputChord.bCmd)
			{
				Out_SameTypeKey = iter.Key;
				return false;
			}
		}

		return true;
	};

	if (_actionBaseKeyMapptings.Contains(In_Type))
	{
		return Func(_actionBaseKeyMapptings);
	}

	if (_actionKeyMapptings.Contains(In_Type))
	{
		FInputActionKeyMapping actionKeyMapping = _actionKeyMapptings.FindRef(In_Type);

		bool isSame = Func(_actionKeyMapptings);
		if (false == isSame)
			return isSame;

		for (auto& iter : _axisKeyMapptings)
		{
			FInputAxisKeyMapping saveAxisKeyMapping = iter.Value;
			if (saveAxisKeyMapping.Key == In_InputChord.Key)
			{
				if (false == In_InputChord.bShift &&
					false == In_InputChord.bCtrl &&
					false == In_InputChord.bAlt &&
					false == In_InputChord.bCmd)
				{
					Out_SameTypeKey = iter.Key;
					return false;
				}				
			}
		}
	}

	if (_axisKeyMapptings.Contains(In_Type))
	{
		FInputAxisKeyMapping axisKeyMapping = _axisKeyMapptings.FindRef(In_Type);
		if (axisKeyMapping.Key == In_InputChord.Key)
		{
			return true;
		}

		for (auto& iter : _axisKeyMapptings)
		{
			FInputAxisKeyMapping saveAxisKeyMapping = iter.Value;
			if (saveAxisKeyMapping.Key == In_InputChord.Key)
			{				
				Out_SameTypeKey = iter.Key;
				return false;				
			}
		}

		for (auto & iter : _actionKeyMapptings)
		{
			FInputActionKeyMapping saveActionKeyMapping = iter.Value;
			if (saveActionKeyMapping.Key == In_InputChord.Key &&
				saveActionKeyMapping.bShift == In_InputChord.bShift &&
				saveActionKeyMapping.bCtrl == In_InputChord.bCtrl &&
				saveActionKeyMapping.bAlt == In_InputChord.bAlt &&
				saveActionKeyMapping.bCmd == In_InputChord.bCmd)
			{
				Out_SameTypeKey = iter.Key;
				return false;
			}
		}
	}

	return true;
}

bool FGsInputSettings::HasSpecialKey(FInputChord In_InputChord) const
{
	if (In_InputChord.bShift || In_InputChord.bCtrl ||
		In_InputChord.bAlt || In_InputChord.bCmd)
	{
		return true;
	}
	
	return false;
}

void FGsInputSettings::GetKeyNameTextEx(EGsKeyMappingType In_Type, OUT FText& Out_KeyNameText)
{
	for (const FGsSchemaKeyMapping* schemaKeyMapping : _schemaKeyMappings)
	{
		if (nullptr == schemaKeyMapping)
			continue;

		if (schemaKeyMapping->keyMappingType == In_Type)
		{
			Out_KeyNameText = schemaKeyMapping->text;
			return;
		}
	}
}

void FGsInputSettings::GetKeyName(EGsKeyMappingType In_Type, OUT FName& Out_KeyName)
{
	for (const FGsSchemaKeyMapping* schemaKeyMapping : _schemaKeyMappings)
	{
		if (nullptr == schemaKeyMapping)
			continue;

		if (schemaKeyMapping->keyMappingType == In_Type)
		{
			Out_KeyName = schemaKeyMapping->keyName;
			return;
		}
	}
}

void FGsInputSettings::GetKeyMappingType(const FName& In_KeyName, OUT EGsKeyMappingType& Out_Type)
{
	for (const FGsSchemaKeyMapping* schemaKeyMapping : _schemaKeyMappings)
	{
		if (nullptr == schemaKeyMapping)
			continue;

		if (schemaKeyMapping->keyName == In_KeyName)
		{
			Out_Type = schemaKeyMapping->keyMappingType;
			return;
		}
	}
}

void FGsInputSettings::OpenConsentPopup(FText& In_KeyText, EGsKeyMappingType In_Type, FInputChord In_InputChord)
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
		FText::FindText(TEXT("UICommonText"), TEXT("Number_PointLarge1"), textDesc);
		textDesc = FText::Format(textDesc, In_KeyText);

		TFunction<void(bool)> InCallback = [this, In_Type, In_InputChord](bool InIsOk)
		{
			if (false == InIsOk)
			{
				// ui를 다시 돌려주는 메세지를 보낸다.
				GMessage()->GetUI().SendMessage(MessageUI::KEYBOARD_MAPPING_UI_UPDATE, nullptr);
				return;
			}

			SaveKeyboardMappingData(In_Type, In_InputChord);
		};
		popup->SetData(textDesc, InCallback);
	}
}

void FGsInputSettings::SaveKeyboardMappingData(EGsKeyMappingType In_Type, FInputChord In_InputChord)
{
	FName keyName;
	GetKeyName(In_Type, keyName);

	auto Func = [&](TMap<EGsKeyMappingType, FInputActionKeyMapping>& InMap)
	{
		if (InMap.Contains(In_Type))
		{
			FInputActionKeyMapping actionKeyMapping = InMap.FindRef(In_Type);
			if (actionKeyMapping.Key != In_InputChord.Key ||
				actionKeyMapping.bShift != In_InputChord.bShift ||
				actionKeyMapping.bCtrl != In_InputChord.bCtrl ||
				actionKeyMapping.bAlt != In_InputChord.bAlt ||
				actionKeyMapping.bCmd != In_InputChord.bCmd)
			{
				// 유저의 입력으로 값이 달라졌다.
				FInputActionKeyMapping newInputActionKeyMapping = FInputActionKeyMapping(keyName, In_InputChord.Key,
					In_InputChord.bShift, In_InputChord.bCtrl, In_InputChord.bAlt, In_InputChord.bCmd);

				// 삭제 후
				InMap.Remove(In_Type);
				// 재 삽입
				InMap.Emplace(In_Type, newInputActionKeyMapping);
			}
		}
	};

	Func(_actionBaseKeyMapptings);
	Func(_actionKeyMapptings);	

	if (_axisKeyMapptings.Contains(In_Type))
	{
		FInputAxisKeyMapping axisKeyMapping = _axisKeyMapptings.FindRef(In_Type);
		if (axisKeyMapping.Key != In_InputChord.Key)
		{
			float Scale = axisKeyMapping.Scale;
			// 유저의 입력으로 값이 달라졌다.
			// 삭제 후
			_axisKeyMapptings.Remove(In_Type);
			// 재 삽입
			FInputAxisKeyMapping newInputAxisKeyMapping = FInputAxisKeyMapping(keyName,
				In_InputChord.Key, Scale);
			_axisKeyMapptings.Emplace(In_Type, newInputAxisKeyMapping);
		}
	}
}

void FGsInputSettings::SetKeyboardMappingData(EGsKeyMappingType In_Type, FInputChord In_InputChord)
{
	SaveKeyboardMappingData(In_Type, In_InputChord);
}

void FGsInputSettings::LoadServerKeyboardMappingData(PD::GC::PKT_GC_ACK_KEYBOARD_MAPPING_LOAD_READ& Packet)
{
	using KeyListIterator = PD::GC::PKT_GC_ACK_KEYBOARD_MAPPING_LOAD_READ::KeyListIterator;
	for (KeyListIterator it = Packet.GetFirstKeyListIterator();
		it != Packet.GetLastKeyListIterator(); ++it)
	{
		EGsKeyMappingType type = static_cast<EGsKeyMappingType>(it->Multi());

		uint32 keyCode = it->Key0();
		uint32 charCode = it->Key1();
		FKey key = FInputKeyManager::Get().GetKeyFromCodes(keyCode, charCode);
		EGsOptionKeyActionFlags actionFlag = static_cast<EGsOptionKeyActionFlags>(it->Action());
		float scale = it->Scaling();

		if (scale == 0.f)
		{
			if (type == EGsKeyMappingType::KEY_MAPPING_MOVE_DOWN ||
				type == EGsKeyMappingType::KEY_MAPPING_MOVE_LEFT) scale = -1.f;
			else if (type == EGsKeyMappingType::KEY_MAPPING_MOVE_UP ||
				type == EGsKeyMappingType::KEY_MAPPING_MOVE_RIGHT) scale = 1.f;
		}
		
		bool bShift = false;
		bool bCtrl = false;
		bool bAlt = false;
		bool bCmd = false;
		if (actionFlag == EGsOptionKeyActionFlags::SHIFT)
		{
			bShift = true;
		}
		else if (actionFlag == EGsOptionKeyActionFlags::CTRL)
		{
			bCtrl = true;
		}
		else if (actionFlag == EGsOptionKeyActionFlags::ALT)
		{
			bAlt = true;
		}
		else if (actionFlag == EGsOptionKeyActionFlags::CMD)
		{
			bCmd = true;
		}

		AddServerKey(type, FInputChord(key, bShift, bCtrl, bAlt, bCmd), scale);
	}

	if (_inputSettings)
	{
		_inputSettings->ForceRebuildKeymaps();
		_inputSettings->SaveKeyMappings();
	}

	FillActionNamesAndMappingByName();
	FillAxisNamesAndMappingByName();

	_keyMappingLoaded = true;
}

void FGsInputSettings::RequestKeyboardMapping()
{
	if (_isHaveServerKeyMappings && !_keyMappingLoaded)
	{
		FGsNetSendService::SendKeyboardMappingLoad();
	}
}

void FGsInputSettings::SaveServerKeyboardMapping()
{
	FGsNetSendService::SendKeyboardMappingSave();
}

void FGsInputSettings::AddServerKey(EGsKeyMappingType In_KeyMapptingType, FInputChord In_InputChord, float In_KeyScale)
{
	FName keyName;
	GetKeyName(In_KeyMapptingType, keyName);

	auto Func = [&](TMap<EGsKeyMappingType, FInputActionKeyMapping>& InMap)
	{
		if (InMap.Contains(In_KeyMapptingType))
		{
			TArray<FInputActionKeyMapping> outActionKeyMapping;
			_inputSettings->GetActionMappingKeyboardByName(keyName, outActionKeyMapping);

			if (0 < outActionKeyMapping.Num())
			{
				for (int i = 0; i < outActionKeyMapping.Num(); ++i)
				{
					_inputSettings->RemoveActionMapping(outActionKeyMapping[i], false);
				}
			}

			FInputActionKeyMapping newInputActionKeyMapping = FInputActionKeyMapping(keyName,
				In_InputChord.Key, In_InputChord.bShift,
				In_InputChord.bCtrl, In_InputChord.bAlt, In_InputChord.bCmd);

			_inputSettings->AddActionMapping(newInputActionKeyMapping);
		}
	};

	Func(_actionBaseKeyMapptings);
	Func(_actionKeyMapptings);

	if (_axisKeyMapptings.Contains(In_KeyMapptingType))
	{
		TArray<FInputAxisKeyMapping> outAxisKeyMapping;
		_inputSettings->GetAxisMappingKeyboardByName(keyName, outAxisKeyMapping);

		if (0 < outAxisKeyMapping.Num())
		{
			for (int i = 0; i < outAxisKeyMapping.Num(); ++i)
			{
				_inputSettings->RemoveAxisMapping(outAxisKeyMapping[i], false);
			}
		}

		FInputAxisKeyMapping newInputAxisKeyMapping = FInputAxisKeyMapping(keyName,
			In_InputChord.Key, In_KeyScale);
		_inputSettings->AddAxisMapping(newInputAxisKeyMapping);
	}
}
