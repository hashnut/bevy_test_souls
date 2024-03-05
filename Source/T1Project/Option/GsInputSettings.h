// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/PlayerInput.h"
#include "Option/GsOptionEnumType.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "DataSchema/KeyMapping/GsSchemaKeyMapping.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

class UInputSettings;
/**
 * 
 */
class T1PROJECT_API FGsInputSettings
{
protected:
	UInputSettings* _inputSettings = nullptr;

	TArray<FName> _actionNames;
	TArray<FName> _axisNames;

	TArray<FInputActionKeyMapping> _actionBaseMappings;
	TArray<FInputActionKeyMapping> _actionMappings;
	TArray<FInputAxisKeyMapping> _axisMappings;
	bool _isHaveServerKeyMappings = false;
	bool _keyMappingLoaded = false;

	TArray<const FGsSchemaKeyMapping*> _schemaKeyMappings;
	
	TMap<EGsKeyMappingType, FInputActionKeyMapping> _actionBaseKeyMapptings;
	TMap<EGsKeyMappingType, FInputActionKeyMapping> _actionKeyMapptings;
	TMap<EGsKeyMappingType, FInputAxisKeyMapping> _axisKeyMapptings;

public:
	void Initialize();
	void Finalize();

	void InitTblKeyMappingLoad();

	// 게임에 셋팅된 키 얻어오기
	void FillActionNamesAndMappingByName();
	void FillAxisNamesAndMappingByName();

	// 초기 테이블 데이터 게임에 추가셋팅
	void AddInitTblKey(EGsKeyMappingType In_KeyMapptingType, const FGsSchemaKeyMapping* In_SchemaKeyMapping);
	void AddServerKey(EGsKeyMappingType In_KeyMapptingType, FInputChord In_InputChord, float In_KeyScale = 1.0f);

	// Getter
	// Action 데이터
	void GetActionBaseKeyMappingDatas(OUT TMap<EGsKeyMappingType, FInputActionKeyMapping>& Out_InputActionBaseKeyMappings);
	void GetActionKeyMappingDatas(OUT TMap<EGsKeyMappingType, FInputActionKeyMapping>& Out_InputActionKeyMappings);
	// Axis 데이터
	void GetAxisKeyMappingDatas(OUT TMap<EGsKeyMappingType, FInputAxisKeyMapping>& Out_InputAxisKeyMappings);
	// 초기값 테이터
	void GetDefaultKeyMappingDatas(OUT TMap<EGsKeyMappingType, FInputActionKeyMapping>& Out_InputActionBaseKeyMappings,
		OUT TMap<EGsKeyMappingType, FInputActionKeyMapping>& Out_InputActionKeyMappings,
		OUT TMap<EGsKeyMappingType, FInputAxisKeyMapping>& Out_InputAxisKeyMappings);
	// 현재 키맵핑 데이터
	void GetKeyMappingDatas(OUT TMap<EGsKeyMappingType, FInputActionKeyMapping>& Out_InputActionBaseKeyMappings,
		OUT TMap<EGsKeyMappingType, FInputActionKeyMapping>& Out_InputActionKeyMappings,
		OUT TMap<EGsKeyMappingType, FInputAxisKeyMapping>& Out_InputAxisKeyMappings);

	void GetKeyMappingData(EGsKeyMappingType In_Type, FInputChord& Out_InputChord);

	// 타입 => TextName
	void GetKeyNameTextEx(EGsKeyMappingType In_Type, OUT FText& Out_KeyNameText);
	// 타입 => Name
	void GetKeyName(EGsKeyMappingType In_Type, OUT FName& Out_KeyName);
	// Name => 타입
	void GetKeyMappingType(const FName& In_KeyName, OUT EGsKeyMappingType& Out_Type);
	
	// Setter
	void SetHasServerKeyMapping(bool In_isHas) { _isHaveServerKeyMappings = In_isHas; }		
	// 타입 데이터 셋팅
	// ui 셋팅 후 저장
	void SetKeyboardMappingData(EGsKeyMappingType In_Type, FInputChord In_InputChord);

	// 키 중복 체크
	bool IsValidInputKey(EGsKeyMappingType In_Type, FInputChord In_InputChord, OUT EGsKeyMappingType& Out_SameTypeKey);
	// 특수키 체크
	bool HasSpecialKey(FInputChord In_InputChord) const;
	// 서버 데이터 셋팅
	void LoadServerKeyboardMappingData(PD::GC::PKT_GC_ACK_KEYBOARD_MAPPING_LOAD_READ& Packet);
	void RequestKeyboardMapping();
	
	// 중복여부 체크 팝업
	void OpenConsentPopup(FText& In_KeyText, EGsKeyMappingType In_Type, FInputChord In_InputChord);

	// 저장
	// 서버 데이터 전송
	void SaveServerKeyboardMapping();
	void SaveAll();
	void SaveKeyboardMappingData(EGsKeyMappingType In_Type, FInputChord In_InputChord);
};

#define GInputSettings() (nullptr == GOption()? nullptr : GOption()->GetInputSettings())
