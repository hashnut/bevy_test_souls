// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "UObject/ObjectMacros.h"
#include "Management/GsScopeHolder.h"
#include "GsSchemaEnums.h"
#include "Engine/EngineTypes.h"
#include "Shared/Client/SharedEnums/SharedCombatEnum.h"
#include "Option/GsGameUserSettings.h"
#include "Option/GsServerOption.h"
#include "Option/GsDevelopmentOption.h"
#include "Option/GsInputSettings.h"
#include "Message/GsMessageSystem.h"
#include "Option/GsOptionEnumType.h"
#include "Shared/Client/SharedEnums/SharedGsOptionItemEnum.h"
#include "GsOptionManager.generated.h"

template <typename T, typename A>
struct FGsOptionData
{
	T _saveType;
	A _value;
	TFunction<void(T InOptType, A InVal)> _invalidateFunc;

	FGsOptionData(T InSaveType, A InValue, TFunction<void(T InOptType, A InVal)>
		InInvalidateFunc = nullptr )
		:_saveType(InSaveType), _value(InValue), _invalidateFunc(InInvalidateFunc)
	{
	}

	A GetData() const { return _value; }
	void SetData(A InVal)
	{ 
		_value = InVal;

		if (nullptr != _invalidateFunc)
		{
			_invalidateFunc(_saveType, InVal);
		}		
	}

	void SetValueData(A InVal)
	{
		_value = InVal;
	}
};

/**
 * 게임 옵션
 */
class UGsGameUserSettings;
class FGsInputSettings;
struct FGsSchemaOptionSlider;
struct FGsSchemaOptionOnOff;
struct FGsSchemaOptionButton;
struct FGsSchemaOptionGroup;
struct FGsSchemaOptionCategory;
class FGsServerOption;
struct FGsSchemaOptionText;
struct FGsSchemaOptionButtonGroup;
struct GsSchemaOptionCheckGroup;

UCLASS()
class T1PROJECT_API UGsOptionManager : public UObject,
	public IGsManager
{
	GENERATED_BODY()

protected:
	//UPROPERTY()
	static constexpr uint16 USER_SETTING_UPDATE_TICK_INTERVAL = 30 * 60;

	UGsGameUserSettings*				_userSettings = nullptr;
	uint16								_updateTickCount = 0;

	TUniquePtr<FGsInputSettings>		_inputSettings;

	// 서버에 저장하는 옵션
	TUniquePtr<FGsServerOption>			_serverOption;

	// 개발 모드 적용 테스트용
	TUniquePtr<FGsDevelopmentOption>	_developmentOption;

	// cheat option
	bool _isShowHPbar = false;

	bool _isDevelopmentMode = false;

	UPROPERTY()
	class UGsGameOptions* _options{ nullptr };

protected:
	FTimerHandle _loadhHandle;

	// UI 데이터 스키마
	TArray<const FGsSchemaOptionCategory*>		_menuDatas;

	// UI와 셋팅 정보를 매칭해주는 데이터 (오디오)
	TMap<EGsOptionItem, FGsOptionData<EGsOptionAudio, int32>>			_audioOptDatas;
	// UI와 셋팅 정보를 매칭해주는 데이터 (그래픽)
	TMap<EGsOptionItem, FGsOptionData<EGsOptionGraphic, int32>>			_graphicOptDatas;
	TMap<EGsOptionItem, FGsOptionData<EGsOptionUserInfo, int32>>		_userInfoOptDatas;
	TMap<EGsOptionItem, FGsOptionData<EGsOptionCombat, int32>>			_combatOptDatas;
	TMap<EGsOptionItem, FGsOptionData<CombatOptionType, int32>>			_autoCombatOptTargetDatas;
	TMap<EGsOptionItem, FGsOptionData<CombatOptionType, int32>>			_manualCombatOptTargetDatas;
	TMap<EGsOptionItem, FGsOptionData<EGsOptionOutput, int32>>			_outputOptDatas;

	TMap<EGsOptionItem, FGsOptionData<EGsOptionAlert, int32>>				_alertOptDatas;
	TMap<EGsOptionItem, FGsOptionData<EGsOptionAlert, int32>>				_alertServerOptDatas;
	TMap<EGsOptionItem, FGsOptionData<EGsOptionLoginAlert, int32>>			_alertLoginOptDatas;
	TMap<EGsOptionItem, FGsOptionData<EGsOptionNotificationAlert, float>>	_alertNotificationOptDatas;
	TMap<EGsOptionItem, FGsOptionData<EGsOptionNotificationAlert, float>>	_alertCombatNotificationOptDatas;

	TMap<EGsOptionItem, FGsOptionData<EGsOptionOfflineAlert, float>>		_alertOfflineOptDatas;
	// 2022/09/30 PKT - PK Book Option
	TMap<EGsOptionItem, FGsOptionData<EGsOptionPKBookAlert, int32>>			_alertPKBookOptData;

	TMap<EGsOptionItem, FGsOptionData<EGsOptionSaveBattery, int32>>		_saveBatteryOptDatas;
	TMap<EGsOptionItem, FGsOptionData<EGsOptionAuto, float>>			_autoOptDatas;
	TMap<EGsOptionItem, FGsOptionData<EGsOptionAuto, float>>			_autoServerOptDatas;
	TMap<EGsOptionItem, FGsOptionData<EGsOptionInput, int32>>			_inputOptDatas;

	TMap<EGsOptionItem, FGsOptionData<EGsDevelopOptionGraphic, int32>>	_developmentGraphicOptDatas;

	TMap<EGsOptionItem, FGsOptionData<EGsOptionServerAutoDrop, int32>>	_serverOptionAutoDrop;	

	TMap<EGsOptionItem, FGsOptionData<EGsPickUpItemOption, bool>> _serverOptionPickupItem;
	TMap<EGsOptionItem, FGsOptionData<EGsOptionAutoDecompose, bool>> _serverOptionAutoDecompose;

	MsgSystemHandleArray	_listSystemDelegate;

	int _optionWindowMenuIndex = 0;
	int _optionWindowDivisionIndex = 0;
	float _optionWindowScrollOffset = 0.0f;

	// 그래픽 등급 설정 값
	int _graphicGrade = 0;
	// 게임 전체 알림
	bool _isAllGameAlert = true;

public:
	//IGsManager
	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual void Update(float inTick) override;

protected:
	virtual void InitializeAfterResourceDownload() override;

protected:
	// OnInitialize 시점에 데이터 셋팅
	UFUNCTION()
		void InitAudioDatas();
	UFUNCTION()
		void InitGraphicDatas();
	UFUNCTION()
		void InitUserDatas();
	// 전투 옵션 데이터 세팅
	void InitCombatDatas();
	void InitCombatTargetDatas();
	// 출력 초기화
	void InitOutputDatas();

	void InitAlertDatas();
	void InitServerAlertDatas();
	void InitGameAlertData();
	void InitAlertLoginDatas();
	void InitAlertNotificationDatas();
	void InitAlertCombatNotificationDatas();
	void InitOfflineAlertDatas();
	// 2022/09/30 PKT - Init PK Book Option
	void InitPKBookAlertDatas();

	void InitSaveBatteryDatas();

	void InitAutoDatas();
	void InitServerAutoDatas();
	void InitInputDatas();

	void InitServerAutoDrop();

	void InitServerItemPickupOption();
	void InitServerAutoDecomposeDatas();

	// 개발 옵션 모드
	void InitDevelopmentDatas();
public:
	UFUNCTION(BlueprintCallable, Category = "Option")
		static void OptionRefresh();	// 테스트 용 (리프레쉬 함수 BP에서 호출)

public:
	// 오디오는 상시 저장, 나머지는 창 닫을 때 1번 저장

	void SaveAudio(EGsOptionAudio InOptType, int32 InVal);
	void SaveGraphicGrade(EGsOptionGraphic InOptType, int32 InVal);
	void SaveGraphicGradeCustom();
	void SaveGraphicUIScale(EGsOptionGraphic InOptType, int32 InVal);
	void SaveGraphicEtc(EGsOptionGraphic InOptType, int32 InVal);

	void SaveAll();
#if WITH_EDITOR || PLATFORM_WINDOWS
	void SaveWindowResolution(EGsOptionGraphic InOptType, int32 InVal);
	void SaveWindowScreenMode(EGsOptionGraphic InOptType, int32 InVal);
#endif
	void SaveAlert(EGsOptionItem InOptType, int32 InVal);
	void SaveHiddenNameGetItem(EGsOptionItem InOptType, int32 InVal);
	void SaveLoginAlert(EGsOptionItem InOptType, int32 InVal);
	void SaveNotificationAlert(EGsOptionItem InOptType, float InVal);
	void SaveCombatNotificationAlert(EGsOptionItem InOptType, float InVal);
	void SaveOfflineAlert(EGsOptionItem InOptType, float InVal);
	void SavePKBookAlert(EGsOptionItem InOptType, int32 InVal);
	void SavePushAlert(EGsOptionItem InOptType, int32 InVal);

#if WITH_EDITOR || PLATFORM_WINDOWS
	void OnKeyMapping(EGsOptionItem InOptType, int32 InVal);
#endif

	void ReLoadGraphicDatas();

public:
	// ini
	UGsGameUserSettings* GetGameUserSettingsEx() { return _userSettings; }
	FGsInputSettings* GetInputSettings() { return _inputSettings.Get(); }
	FGsServerOption* GetServerOption() { return _serverOption.Get(); }

	// UI에서 사용하는 스키마
	const TArray<const struct FGsSchemaOptionCategory*> GetOptionData(FString inTableName);
	const struct FGsSchemaOptionSlider* GetGsOptionSliderData(const FString& InRowKey);
	const struct FGsSchemaOptionOnOff* GetGsOptionOnOffData(const FString& InRowKey);
	const struct FGsSchemaOptionToggleGroup* GetGsOptionOnOffGroupData(const FString& InRowKey);
	const struct FGsSchemaOptionButton* GetGsOptionButtonData(const FString& InRowKey);
	const struct FGsSchemaOptionButtonGroup* GetGsOptionButtonGroupData(const FString& InRowKey);
	const struct FGsSchemaOptionText* GetGsOptionTextData(const FString& InRowKey);
	const struct FGsSchemaOptionCheckGroup* GetGsOptionCheckGroupData(const FString& InRowKey);

	// UI 에서 호출 하는 Get/Set
	//bool GetValue(const EGsOptionItem InType, float& outValue) const;
	bool GetValue(const EGsOptionItem InType, float& outValue, bool& outIsVisible) const;
	bool GetValue(const EGsOptionItem InType, FString& outValue, bool& outIsVisible) const;

	void SetValue(EGsOptionItem InType, float InVal);
	// 서버에서 받은 옵션적용
	void SetServerOptionData();

private:
	void OnUserInfoFunc(EGsOptionUserInfo InOptType, int32 InVal);
	void OnEnterGame();
	void ClearAllMenuDatas();

#if WITH_EDITOR || PLATFORM_WINDOWS
	void OnWindowResolutionUpdate();
	void OnWindowScreenModeUpdate();

	void GetWindowGraphicDatas();
#endif

public:
	bool IsVisible(EGsOptionItem InType);
	bool GetCustomTitleValue(EGsOptionItem InType, OUT FText& outValue);

	void SetIsShowHpbar(bool In_val);
	bool GetIsShowHPbar()
	{
		return _isShowHPbar;
	}

	void GetHivePushDatas();

	void SetIsDevelopmentMode(bool In_val) { _isDevelopmentMode = In_val; }
	bool GetIsDevelopmentMode()
	{
		return _isDevelopmentMode;
	}

	int GetOptionWindowMenuIndex();
	int GetOptionWindowDivisionIndex();
	float GetScrollOffset();
	
	void SetOptionWindowMenuIndex(int in_Index);
	void SetOptionWindowDivisionIndex(int in_Index);
	void SetScrollOffset(float in_Offset);
	void SetInItPage() 
	{
		SetOptionWindowMenuIndex(0);
		SetOptionWindowDivisionIndex(0);
		SetScrollOffset(0.0f);
	}

	bool GetIsValidData(FString inTableName);


	void SetAllGameAlert(int InValue);
	int GetAllGameAlert() { return _isAllGameAlert; }
};


#define GOption() UGsScopeHolder::GetGlobalManager<UGsOptionManager>(UGsScopeGlobal::EManagerType::Option)
#define GGameUserSettings() (nullptr == GOption()) ? nullptr : GOption()->GetGameUserSettingsEx()