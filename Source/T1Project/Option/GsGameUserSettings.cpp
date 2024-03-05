// Fill out your copyright notice in the Description page of Project Settings.

#include "GsGameUserSettings.h"

#include "Engine.h"
#include "EngineModule.h"
#include "RendererInterface.h"
#include "UnrealMathUtility.h"
#include "UTIL/GsGameObjectUtil.h"

#include "Cheat/GsCheatManager.h"
#include "Item/GsItemManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "AI/Data/GsAIDefine.h"
#include "AI/ContentsCondition/GsAIContentsConditionAutoReturn.h"
#include "AI/ContentsCondition/GsAIContentsConditionDefault.h"

#include "Scan/GsScanDefine.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Target/AutoTarget/GsTargetRule.h"

#include "Message/GsMessageUI.h"
#include "Message/GsMessageContentHud.h"
#include "Message/MessageParam/GsHiveMessageParam.h"

#include "UI/ScaleRule/GsUIScaleRule.h"
#include "UI/UIContent/Popup/GsUIPopupResolutionList.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "GameFramework/GameUserSettings.h"
#include "UI/UIContent/Popup/GsUIPopupYesNoResolution.h"
#include "Data/GsDataContainManager.h"
#include "DataSchema/KeyMapping/GsSchemaKeyMapping.h"
#include "GameFramework/PlayerInput.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "HAL/PlatformApplicationMisc.h"
#include "DeviceProfiles/DeviceProfileManager.h"
#include "Regex.h"
#include "DeviceProfiles/DeviceProfile.h"
#include "Text.h"
#include "Runtime/DataCenter/Public/DataSchema/Option/GsSchemaOptionConfig.h"
#include "UTIL/GsTableUtil.h"
#include "SceneViewport.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Algo/Transform.h"
#include "SlateApplication.h"
#include "SViewport.h"
#include "GsGameOptions.h"
#include "Management/ScopeGame/GsStatChangeNotifyManager.h"

#include "DataSchema/Option/GsSchemaOptionOnOff.h"

#include "GameObject/Fairy/GsFairyHandlerPlayer.h"
#include "ActorEx/GsCharacterPlayer.h"
#include "ActorEx/GsWeaponActor.h"

/*
*/
UGsGameUserSettings::UGsGameUserSettings(const FObjectInitializer& ObjectInitializer)
	: 
	Super(ObjectInitializer)
{
}

void UGsGameUserSettings::ApplyVsync()
{
	if (IsVSyncDirty())
	{
		SaveConfigVsync();
	}
}

void UGsGameUserSettings::SaveConfigVsync()
{
	FString Name = TEXT("r.VSync");

	auto CVar = IConsoleManager::Get().FindConsoleVariable(*Name);
	CVar->Set(IsVSyncEnabled(), ECVF_SetByCode);
}

/*
	참고) GsGameInstance::Initialize에서 호출됨

	1. 엔진이 읽어들여놓은 config 값들을 UI와 통신하기 위한 데이터화
	2. 엔진이 기본적으로 하지 않은 적용들을 진행
*/  
#if WITH_EDITOR || PLATFORM_WINDOWS
void UGsGameUserSettings::FirstRunSetting()
{
	bool FindFirstRunSetting = true;
	FindFirstRunSetting = GConfig->GetBool(INI_SECTION_USERINFO, TEXT("FirstRunSetting"), FindFirstRunSetting, GGameUserSettingsIni);
	if (FindFirstRunSetting == false)
	{
		GSLOG(Warning, TEXT("[GsGameUserSettings] FirstRunSetting Call"));

		RunHardwareBenchmark();
		ApplyHardwareBenchmarkResults();
		_graphicGrade = 4;
		int32 result = GetOverallScalabilityLevel();
		if (-1 != result)
		{
			_graphicGrade = result + 1;
		}
		GConfig->SetInt(INI_SECTION_GRAPHIC, TEXT("GraphicGrade"), _graphicGrade, GGameUserSettingsIni);

		SetVSyncEnabled(true);
		GConfig->SetInt(INI_SECTION_GRAPHIC, TEXT("WindowVSyncEnabled"), static_cast<int32>(true), GGameUserSettingsIni);

		FindFirstRunSetting = true;
		GConfig->SetBool(INI_SECTION_USERINFO, TEXT("FirstRunSetting"), FindFirstRunSetting, GGameUserSettingsIni);
	}
}
#endif 

#if PLATFORM_ANDROID || PLATFORM_IOS
void UGsGameUserSettings::FirstRunSettingMobile()
{
	GSLOG(Warning, TEXT("Call FirstRunSettingMobile"));
	bool FindFirstRunSetting = false;
	FindFirstRunSetting = GConfig->GetBool(INI_SECTION_USERINFO, TEXT("FirstRunSetting"), FindFirstRunSetting, GGameUserSettingsIni);
	if (FindFirstRunSetting == false)
	{
		GSLOG(Warning, TEXT("[GsGameUserSettings] FirstRunSetting Call"));

		UDeviceProfileManager& ProfileManager = UDeviceProfileManager::Get();

		UDeviceProfile* ActiveProfile = ProfileManager.GetActiveProfile();

		if (nullptr == ActiveProfile)
		{
			GSLOG(Warning, TEXT("nullptr == ActiveProfile"));
			return;
		}
		if (nullptr == ActiveProfile->Parent)
		{
			GSLOG(Warning, TEXT("nullptr == ActiveProfile->BaseDeviceProfile"));
			return;
		}
		
		bool bIsMatch = false;
		// 부모찾기
		UDeviceProfile* parent = Cast<UDeviceProfile>(ActiveProfile->Parent);
		FRegexPattern Pattern = FRegexPattern(TEXT("(Low)|(Mid)|(High)|(Ultra)"));
		do
		{
			GSLOG(Warning, TEXT("--------------------------------------------"));
			GSLOG(Warning, TEXT("[BaseProfileName] ->[%s]"), *parent->BaseProfileName);
			GSLOG(Warning, TEXT("--------------------------------------------"));
						
			FRegexMatcher Matcher(Pattern, parent->BaseProfileName);
			bIsMatch = Matcher.FindNext();
			if (false == bIsMatch)
			{
				parent = Cast<UDeviceProfile>(parent->Parent);
			}
		} while ((false == bIsMatch) && (nullptr != parent));
		
		if(false == bIsMatch)
		{
			GSLOG(Warning, TEXT("Cant Find Match String"));
			return;
		}

		if (nullptr == parent)
		{
			GSLOG(Warning, TEXT("nullptr == parent"));
			return;
		}

		if (parent->BaseProfileName.Len() != 0)
		{
			GSLOG(Warning, TEXT("Selected [%s]"), *parent->BaseProfileName);
		}
		

		if (parent->BaseProfileName.Contains(TEXT("Low"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
		{
			_graphicGrade = 1;
		}
		else if (parent->BaseProfileName.Contains(TEXT("Mid"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
		{
			_graphicGrade = 2;
		}
		else if (parent->BaseProfileName.Contains(TEXT("High"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
		{
			_graphicGrade = 3;
		}
		else if(parent->BaseProfileName.Contains(TEXT("Ultra"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
		{
			_graphicGrade = 3;
		}
		else
		{
			GSLOG(Warning, TEXT("Can't Find Grande"));
			return;
		}		

		GSLOG(Warning, TEXT("FirstRunSettingMobile Selected Grapic Select[%d]"), _graphicGrade);
		GConfig->SetInt(INI_SECTION_GRAPHIC, TEXT("GraphicGrade"), _graphicGrade, GGameUserSettingsIni);		
	}
}
#endif

void UGsGameUserSettings::OnInitialize()
{
	RegisterMessages();

#if WITH_EDITOR || PLATFORM_WINDOWS
	FirstRunSetting();
#endif
#if PLATFORM_ANDROID || PLATFORM_IOS
	FirstRunSettingMobile();
#endif

	LoadGraphicSettings();	
	ApplyGraphicSettings();

	LoadAudioSettings();
	ApplyAudioSettings();

	LoadUserInfoSettings();
	LoadCombatSetting();

	LoadChatOptionSetting();

	LoadBlessItemOptionSetting();

	LoadChatMacro();

	LoadOutputSetting();

	LoadAlertSetting();

	LoadMarketSetting();

	LoadCameraModeBackupSetting();

	LoadScanSetting();

	LoadSaveBatterySetting();

	LoadAutoSetting();

	LoadInputSetting();

	LoadHudSetting();

	LoadCommunityEventRedDotSetting();

	LoadCompositionOption();

	//LoadSystemMessageOption();
}

void UGsGameUserSettings::Finalize()
{
	UnRegisterMessages();
}

void UGsGameUserSettings::RegisterMessages()
{
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		_systemMsgHandle
			= MessageHolder->GetSystemParam().AddUObject(MessageSystem::HIVE_PUSH
				, this, &UGsGameUserSettings::OnHivePush);		
	}
	
#if WITH_EDITOR || PLATFORM_WINDOWS
	//_handleResize = FViewport::ViewportResizedEvent.AddUObject(this, &UGsGameUserSettings::OnResizeViewport);
#endif
}

void UGsGameUserSettings::UnRegisterMessages()
{
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		MessageHolder->GetSystemParam().Remove(_systemMsgHandle);	
	}

#if WITH_EDITOR || PLATFORM_WINDOWS
	if (_handleResize.IsValid())
	{
		FViewport::ViewportResizedEvent.Remove(_handleResize);
	}
#endif
}

#if WITH_EDITOR || PLATFORM_WINDOWS
FIntPoint UGsGameUserSettings::CheckSupportedFullscreenResolution(FIntPoint In_CurResolution)
{
	TArray<FIntPoint> resolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(resolutions);

	auto resolution = GetLastConfirmedScreenResolution();

	bool Check = resolutions.Contains(In_CurResolution);
	if (Check)
		return In_CurResolution;
	else
	{
		// 최대 해상도를 찾기 위해 정렬
		resolutions.Sort([](const FIntPoint& A, const FIntPoint& B) {
			return A.Size() > B.Size();
			});
			
		if (0 >= resolutions.Num())
		{ 			
			return FIntPoint(resolution.X, resolution.Y);
		}

		return resolutions[0];
	}
}

FIntPoint UGsGameUserSettings::GetMaxFullscreenResolution(FIntPoint In_CurResolution)
{
	TArray<FIntPoint> resolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(resolutions);

	auto resolution = GetLastConfirmedScreenResolution();

	// 최대 해상도를 찾기 위해 정렬
	resolutions.Sort([](const FIntPoint& A, const FIntPoint& B) {
		return A.Size() > B.Size();
		});

	if (0 >= resolutions.Num())
	{
		return FIntPoint(resolution.X, resolution.Y);
	}

	return resolutions[0];
}
//
//void UGsGameUserSettings::LoadWindowResolutionList()
//{
//	_fullscreenResolutions.Empty();
//	_windowedResolutions.Empty();
//
//	UKismetSystemLibrary::GetSupportedFullscreenResolutions(_fullscreenResolutions);
//
//	TArray<FIntPoint> windowedResolutions;
//	UKismetSystemLibrary::GetConvenientWindowedResolutions(windowedResolutions);
//
//	const float AspectRatio = 16.0 / 9.0;
//
//	for (auto& iter : windowedResolutions)
//	{
//		const bool isFind = _fullscreenResolutions.ContainsByPredicate([iter](FIntPoint e) {
//			return e == iter;
//			});
//
//		if (isFind)
//		{
//			_windowedResolutions.Emplace(iter);
//		}
//	}
//
//	// X제곱 + Y제곱의 제곱근(숫자 오버플로우 대비)으로 비교한다.
//	_fullscreenResolutions.Sort([](const FIntPoint& A, const FIntPoint& B) {
//		return A.Size() > B.Size();
//		});
//
//	_windowedResolutions.Sort([](const FIntPoint& A, const FIntPoint& B) {
//		return A.Size() > B.Size();
//		});
//}

void UGsGameUserSettings::GetFullscreenResolutions(TArray<FIntPoint>& OutFullscreenResolutions)
{
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(OutFullscreenResolutions);
	
	OutFullscreenResolutions.Sort([](const FIntPoint& A, const FIntPoint& B) {
		return A.Size() > B.Size();
		});
}

void UGsGameUserSettings::GetWindowedResolutions(TArray<FIntPoint>& OutWindowedResolutions)
{
	TArray<FIntPoint> fullscreenResolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(fullscreenResolutions);

	TArray<FIntPoint> windowedResolutions;
	UKismetSystemLibrary::GetConvenientWindowedResolutions(windowedResolutions);
	
	for (auto& iter : windowedResolutions)
	{
		const bool isFind = fullscreenResolutions.ContainsByPredicate([iter](FIntPoint e) {
			return e == iter;
			});

		if (isFind)
		{
			OutWindowedResolutions.Emplace(iter);
		}
	}

	OutWindowedResolutions.Sort([](const FIntPoint& A, const FIntPoint& B) {
		return A.Size() > B.Size();
		});
}

#endif

void UGsGameUserSettings::OnHivePush(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
		return;

	const FGsHivePushMessageParam* Param = InParam->Cast<const FGsHivePushMessageParam>();
	if (nullptr == Param)
		return;	

	GSLOG(Warning, TEXT("[GsGameUserSettings] OnHivePush PushGetRemote, ConsoleAlert: %d"), Param->_allAlert);
	GSLOG(Warning, TEXT("[GsGameUserSettings] OnHivePush PushGetRemote, NightAlert: %d"), Param->_night);

	bool pushNight = true;
	bool pushNotice = true;
#if !WITH_EDITOR
	if (UGsHiveManager* hiveMgr = GHive())
	{
		GSLOG(Log, TEXT("UGsGameUserSettings::PushGetRemote recv"));
		if (hiveMgr->IsInit())
		{
			pushNight = hiveMgr->IsPushNight();
			pushNotice = hiveMgr->IsPushNotice();

			GSLOG(Log, TEXT("[GsOptionManager] Hive PushSetRemote, ConsoleAlert: %d"), pushNight);
			GSLOG(Log, TEXT("[GsOptionManager] Hive PushSetRemote, NightAlert: %d"), pushNotice);
		}
	}
#endif

	//bool pushAll = (true == pushNight || true == pushNotice)? true : false;

	//_alertTable[EGsOptionAlert::ALL_ALERT] = static_cast<int32>(pushAll);
	_alertTable[EGsOptionAlert::CONSOLE_ALERT] = static_cast<int32>(pushNotice);
	_alertTable[EGsOptionAlert::CONSOLE_ALERT_NIGHT] = static_cast<int32>(pushNight);
}

void UGsGameUserSettings::Update()
{	
}

void UGsGameUserSettings::Update(float inTick)
{
	OtherPCRenderingUpdate(inTick);
}

void UGsGameUserSettings::OtherPCRenderingUpdate(float InDelta)
{
	static const float CheckTime = 1.0f; // 1초마다 체크

	OPCRLastAccessTime += InDelta;
	if (OPCRLastAccessTime >= CheckTime)
	{
		ApplyOtherPCRendering();
		OPCRLastAccessTime = .0f;
	}
}

// 현재 EGsOptionCombat 에서 사용중인 데이터는 QUICKSLOT_PAGE_INDEX 타입밖에 없으므로
// 추후 다른 타입 저장 필요 시, 추가해주세요
void UGsGameUserSettings::LoadCombatSetting()
{
	// QuickSlot PageIndex
	int32 value = 0;
	GConfig->GetInt(INI_SECTION_COMBAT, TEXT("QuickSlotPageIndex"), value, GGameUserSettingsIni);
	_combatTable.Emplace(EGsOptionCombat::QUICKSLOT_PAGE_INDEX, value);

	// Inventory LongPress Enable
	int32 stateValue = 0;
	GConfig->GetInt(INI_SECTION_COMBAT, TEXT("InventoryLongPressState"), stateValue, GGameUserSettingsIni);
	_combatTable.Emplace(EGsOptionCombat::INVENTROY_LONGPRESS_ENABLE, stateValue);

	// 매너 모드
	int32 mannerModeValue = 0;
	GConfig->GetInt(INI_SECTION_COMBAT, TEXT("MannerMode"), mannerModeValue, GGameUserSettingsIni);
	_combatTable.Emplace(EGsOptionCombat::MANNER_MODE, mannerModeValue);

	// 수동 이동시, 자동 전투 유지
	int32 nonAutoMoveKeepAutoValue = 0;
	GConfig->GetInt(INI_SECTION_COMBAT, TEXT("NonAutoMoveKeepAuto"), nonAutoMoveKeepAutoValue, GGameUserSettingsIni);
	_combatTable.Emplace(EGsOptionCombat::NONAUTO_MOVE_KEEP_AUTO, nonAutoMoveKeepAutoValue);

	// 데미지 플로터 출력
	int32 damageFloaterValue = 0;
	GConfig->GetInt(INI_SECTION_COMBAT, TEXT("DamageFloater"), damageFloaterValue, GGameUserSettingsIni);
	_combatTable.Emplace(EGsOptionCombat::DAMAGE_FLOATER, damageFloaterValue);

	// 드롭 아이템 이름 출력
	int32 drapItemNameValue = 0;
	GConfig->GetInt(INI_SECTION_COMBAT, TEXT("DropItemName"), drapItemNameValue, GGameUserSettingsIni);
	_combatTable.Emplace(EGsOptionCombat::DROP_ITEM_NAME, drapItemNameValue);

	// NPC 이름 출력
	int32 npcNameValue = 0;
	GConfig->GetInt(INI_SECTION_COMBAT, TEXT("NpcName"), npcNameValue, GGameUserSettingsIni);
	_combatTable.Emplace(EGsOptionCombat::NPC_NAME, npcNameValue);

	// NPC 기능 아이콘 출력
	int32 npcFunctionIconValue = 0;
	GConfig->GetInt(INI_SECTION_COMBAT, TEXT("NpcFunctionIcon"), npcFunctionIconValue, GGameUserSettingsIni);
	_combatTable.Emplace(EGsOptionCombat::NPC_FUNCTION_ICON, npcFunctionIconValue);

	// PC 이름 출력
	int32 playerNameValue = 0;
	GConfig->GetInt(INI_SECTION_COMBAT, TEXT("PlayerName"), playerNameValue, GGameUserSettingsIni);
	_combatTable.Emplace(EGsOptionCombat::PLAYER_NAME, playerNameValue);

	// 수동 스캔 시 스캔(음파) 이펙트 표현
	int32 playerScanEffectValue = 0;
	GConfig->GetInt(INI_SECTION_COMBAT, TEXT("PlayerScanEffect"), playerScanEffectValue, GGameUserSettingsIni);
	_combatTable.Emplace(EGsOptionCombat::PLAYER_SCAN_EFFECT, playerScanEffectValue);

	// NPC 말풍선 출력
	int32 npcTalkBalloonValue = 0;
	GConfig->GetInt(INI_SECTION_COMBAT, TEXT("NpcTalkBalloon"), npcTalkBalloonValue, GGameUserSettingsIni);
	_combatTable.Emplace(EGsOptionCombat::NPC_TALK_BALLOON, npcTalkBalloonValue);

	// NPC function ui name
	int32 npcFunctionUINameValue = 0;
	GConfig->GetInt(INI_SECTION_COMBAT, TEXT("NpcFunctionUIName"), npcFunctionUINameValue, GGameUserSettingsIni);
	_combatTable.Emplace(EGsOptionCombat::NPC_FUNCTION_UI_NAME, npcFunctionUINameValue);

	// keep target , touch terrain
	int32 keepTargetGroundTouch = 0;
	GConfig->GetInt(INI_SECTION_COMBAT, TEXT("KeepTargetGroundTouch"), keepTargetGroundTouch, GGameUserSettingsIni);
	_combatTable.Emplace(EGsOptionCombat::KEEP_TARGET_GROUND_TOUCH, keepTargetGroundTouch);

	int32 AutoSkillUseAddTargetButton = 0;
	GConfig->GetInt(INI_SECTION_COMBAT, TEXT("AutoSkillUseAddTargetButton"), AutoSkillUseAddTargetButton, GGameUserSettingsIni);
	_combatTable.Emplace(EGsOptionCombat::AUTOSKILL_USE_ADDTARGETBUTTON, AutoSkillUseAddTargetButton);

	int32 combatPowerUI = (int)FGsStatChangeNotifyManager::EGsCombatPowerUI::ShowOnlyCombatPower;
	GConfig->GetInt(INI_SECTION_COMBAT, TEXT("CombatPowerUI"), combatPowerUI, GGameUserSettingsIni);
	_combatTable.Emplace(EGsOptionCombat::COMBAT_POWER_UI, combatPowerUI);

	int32 itemSlotExpansion = 0;
	if (const FGsSchemaOptionOnOff* option = UGsTableUtil::FindRow<FGsSchemaOptionOnOff>(FName(TEXT("ItemSlotExpansion"))))
	{
		itemSlotExpansion = option->OnOff ? 1 : 0;
	}
	
	GConfig->GetInt(INI_SECTION_COMBAT, TEXT("ItemSlotExpansion"), itemSlotExpansion, GGameUserSettingsIni);
	_combatTable.Emplace(EGsOptionCombat::ITEM_SLOT_EXPANSION, itemSlotExpansion);
}

// 함수명대로 로드 및 셋팅에만 집중 적용은 ApplyGraphicSettings 에서...
void UGsGameUserSettings::LoadGraphicSettings()
{
	GSLOG(Warning, TEXT("UGsGameUserSettings::LoadGraphicSettings() Call"));

//#if WITH_EDITOR || PLATFORM_WINDOWS
//	LoadWindowResolutionList();
//#endif

	// 그래픽 등급 로드
	_graphicGrade = 2;
	GConfig->GetInt(INI_SECTION_GRAPHIC, TEXT("GraphicGrade"), _graphicGrade, GGameUserSettingsIni);
	_graphicTable.Emplace(EGsOptionGraphic::GRAPHIC_GRADE, _graphicGrade);

	GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::GRAPHIC_GRADE _graphicGrade : %d"), _graphicGrade);

#if WITH_EDITOR || PLATFORM_WINDOWS
	// 해상도 설정을 사용자로 변경
	int32 WindowResolution = 0;
	GConfig->GetInt(INI_SECTION_GRAPHIC, TEXT("WindowResolution"), WindowResolution, GGameUserSettingsIni);
	_graphicTable.Emplace(EGsOptionGraphic::WINDOW_RESOLUTION, WindowResolution);

	GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::WINDOW_RESOLUTION WindowResolution : %d"), WindowResolution);
#endif

#if WITH_EDITOR || PLATFORM_WINDOWS
	// FQualityLevels Load, Table Set
	_graphicTable.Emplace(EGsOptionGraphic::VIEW_DISTANCE, GetViewDistanceQuality());
	GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::VIEW_DISTANCE : %d"), GetViewDistanceQuality());

	_graphicTable.Emplace(EGsOptionGraphic::POST_PROCESS, GetPostProcessingQuality());
	GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::POST_PROCESS : %d"), GetPostProcessingQuality());

	_graphicTable.Emplace(EGsOptionGraphic::SHADOW, GetShadowQuality());
	GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::SHADOW : %d"), GetShadowQuality());
#else
	int uiViewDistanceQuality = 0;	
	GConfig->GetInt(INI_SECTION_GRAPHIC, TEXT("UIViewDistanceQuality"), uiViewDistanceQuality, GGameUserSettingsIni);
	_graphicTable.Emplace(EGsOptionGraphic::VIEW_DISTANCE, uiViewDistanceQuality);
	GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::VIEW_DISTANCE : %d"), uiViewDistanceQuality);

	int uiPostProcessQuality = 0;
	GConfig->GetInt(INI_SECTION_GRAPHIC, TEXT("UIPostProcessQuality"), uiPostProcessQuality, GGameUserSettingsIni);
	_graphicTable.Emplace(EGsOptionGraphic::POST_PROCESS, uiPostProcessQuality);
	GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::POST_PROCESS : %d"), uiPostProcessQuality);

	int uiShadowQuality = 0;
	GConfig->GetInt(INI_SECTION_GRAPHIC, TEXT("UIShadowQuality"), uiShadowQuality, GGameUserSettingsIni);
	_graphicTable.Emplace(EGsOptionGraphic::SHADOW, uiShadowQuality);
	GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::SHADOW : %d"), uiShadowQuality);
#endif

#if WITH_EDITOR || PLATFORM_WINDOWS
	// Antialising mode, VSync
	int32 windowAntiAliasing = 0;
	bool isFindWindowAntiAliasing = GConfig->GetInt(INI_SECTION_GRAPHIC, TEXT("WindowAntiAliasing"), windowAntiAliasing, GGameUserSettingsIni);
	if (isFindWindowAntiAliasing)
	{		
		_graphicTable.Emplace(EGsOptionGraphic::WINDOW_ANTIALIASING, windowAntiAliasing);

		GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::WINDOW_ANTIALIASING windowAntiAliasing : %d"), windowAntiAliasing);
	}
	else
	{
		_graphicTable.Emplace(EGsOptionGraphic::WINDOW_ANTIALIASING, GetAntiAliasing());

		GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::WINDOW_ANTIALIASING : %d"), GetAntiAliasing());
	}

	int32 windowVSyncEnabled = 1;
	bool isFindWindowVSyncEnabled = GConfig->GetInt(INI_SECTION_GRAPHIC, TEXT("WindowVSyncEnabled"), windowVSyncEnabled, GGameUserSettingsIni);
	if (isFindWindowVSyncEnabled)
	{
		_graphicTable.Emplace(EGsOptionGraphic::WINDOW_VSYNCENABLED, windowVSyncEnabled);

		GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::WINDOW_VSYNCENABLED windowVSyncEnabled : %d"), windowVSyncEnabled);
	}
	else
	{
		_graphicTable.Emplace(EGsOptionGraphic::WINDOW_VSYNCENABLED, static_cast<int32>(IsVSyncEnabled()));

		GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::WINDOW_VSYNCENABLED : %d"), static_cast<int32>(IsVSyncEnabled()));
	}
	
	_graphicTable.Emplace(EGsOptionGraphic::ANTIALIASING_QUALITY, GetAntiAliasingQuality());

	GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::ANTIALIASING_QUALITY : %d"), GetAntiAliasingQuality());
#endif

	// 프레임수 제한
	int32 frameRateLimit = 3;
	GConfig->GetInt(INI_SECTION_GRAPHIC, TEXT("FrameRateLimit"), frameRateLimit, GGameUserSettingsIni);
	frameRateLimit = FMath::Clamp(frameRateLimit, 0, MAX_OPT_GRAPHIC_LEVELS - 1);
	_graphicTable.Emplace(EGsOptionGraphic::FRAME_RATE, frameRateLimit);

	GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::FRAME_RATE : %d"), frameRateLimit);

#if WITH_EDITOR || PLATFORM_WINDOWS
	// 풀스크린 모드 셋팅	
	int32 WindowScreenMode = static_cast<int32>(GetFullscreenMode());
	--WindowScreenMode;
	_graphicTable.Emplace(EGsOptionGraphic::WINDOW_SCREENMODE, WindowScreenMode);

	GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::WINDOW_SCREENMODE : %d"), WindowScreenMode);
	
	int32 resolutionX = 0;
	int32 resolutionY = 0;
	GConfig->GetInt(INI_SECTION_GRAPHIC, TEXT("gsWindowResolutionX"), resolutionX, GGameUserSettingsIni);
	GConfig->GetInt(INI_SECTION_GRAPHIC, TEXT("gsWindowResolutionY"), resolutionY, GGameUserSettingsIni);

	if (resolutionX == 0 && resolutionY == 0)
	{
		FIntPoint resolutionPoint = GetScreenResolution();
		resolutionX = resolutionPoint.X;
		resolutionY = resolutionPoint.Y;
	}
	_graphicTable.Emplace(EGsOptionGraphic::WINDOW_RESOLUTION_X, resolutionX);
	_graphicTable.Emplace(EGsOptionGraphic::WINDOW_RESOLUTION_Y, resolutionY);

	GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::WINDOW_RESOLUTION_X : %d"), resolutionX);
	GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::WINDOW_RESOLUTION_Y : %d"), resolutionY);
#endif

#if WITH_EDITOR || PLATFORM_WINDOWS
	//_graphicTable.Emplace(EGsOptionGraphic::ANTIALIASING_QUALITY, GetAntiAliasingQuality());
	_graphicTable.Emplace(EGsOptionGraphic::TEXTURE, GetTextureQuality());
	_graphicTable.Emplace(EGsOptionGraphic::EFFECT, GetVisualEffectQuality());
	_graphicTable.Emplace(EGsOptionGraphic::FOLIAGE, GetFoliageQuality());
	_graphicTable.Emplace(EGsOptionGraphic::SHADING_QUALITY, GetShadingQuality());

	GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::TEXTURE : %d"), GetTextureQuality());
	GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::EFFECT : %d"), GetVisualEffectQuality());
	GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::FOLIAGE : %d"), GetFoliageQuality());
	GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::SHADING_QUALITY : %d"), GetShadingQuality());
#endif

	// 그외 항목들(엔진 옵션이 있지 않은 것들, 커스터마이징 추가한 것들) 초기값이 정해져 있지 않거나 ini파일에 있는거 같음, 초기 설정을 정해주면 좋을거 같긴함
	{
		int32 particleQuality = 0;
		GConfig->GetInt(INI_SECTION_GRAPHIC, TEXT("ParticleQuality"), particleQuality, GGameUserSettingsIni);
		_graphicTable.Emplace(EGsOptionGraphic::PARTICLE_QUALITY, particleQuality);

		GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::PARTICLE_QUALITY : %d"), particleQuality);

		int32 screenPercentage = 0;
		GConfig->GetInt(INI_SECTION_GRAPHIC, TEXT("ScreenPercentage"), screenPercentage, GGameUserSettingsIni);
		screenPercentage = FMath::Clamp(screenPercentage, 0, MAX_OPT_GRAPHIC_LEVELS - 1);
		_graphicTable.Emplace(EGsOptionGraphic::RESOLUTION, screenPercentage);

		GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::RESOLUTION : %d"), screenPercentage);

		// SSAO 단계
		int32 ssaoLevel = 0;
		GConfig->GetInt(INI_SECTION_GRAPHIC, TEXT("SSAO"), ssaoLevel, GGameUserSettingsIni);
		_graphicTable.Emplace(EGsOptionGraphic::SSAO, ssaoLevel);

		GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::SSAO : %d"), ssaoLevel);

#if WITH_EDITOR
		int32 uiScale = 100;
#else //WITH_EDITOR

#if PLATFORM_WINDOWS
		int32 uiScale = 20;
#else //PLATFORM_WINDOWS
		int32 uiScale = 100;
#endif //PLATFORM_WINDOWS
#endif //WITH_EDITOR

		GConfig->GetInt(INI_SECTION_GRAPHIC, TEXT("UIScale"), uiScale, GGameUserSettingsIni);
		_graphicTable.Emplace(EGsOptionGraphic::UISCALE, uiScale);

//#if PLATFORM_ANDROID || PLATFORM_IOS
		// FSR
		int32 bFSR_1 = 0;
		GConfig->GetInt(INI_SECTION_GRAPHIC, TEXT("FSR"), bFSR_1, GGameUserSettingsIni);
		_graphicTable.Emplace(EGsOptionGraphic::FSR_1_ONOFF, bFSR_1);
		GSLOG(Log, TEXT("UGsGameUserSettings Load EGsOptionGraphic::FSR : %d"), bFSR_1);
//#endif

		int32 otherPCRendering = 0;
		GConfig->GetInt(INI_SECTION_GRAPHIC, TEXT("OtherPCRendering"), otherPCRendering, GGameUserSettingsIni);
		otherPCRendering = FMath::Clamp(otherPCRendering, 0, MAX_OPT_GRAPHIC_LEVELS - 1);
		_graphicTable.Emplace(EGsOptionGraphic::OTHER_PC_RENDERING, otherPCRendering);
	}
}

#if PLATFORM_ANDROID || PLATFORM_IOS
void UGsGameUserSettings::ApplyFSRSetting()
{
	GSLOG(Log, TEXT("UGsGameUserSettings ApplyFSRSetting() start call"));

	SetMobileFSR(_graphicTable[EGsOptionGraphic::FSR_1_ONOFF]);
}
#endif

void UGsGameUserSettings::ApplyGraphicSettings()
{
#if WITH_EDITOR || PLATFORM_WINDOWS
	GSLOG(Log, TEXT("UGsGameUserSettings ApplyGraphicSettings() start call"));
	// 프레임수 제한
	int32 frameRateLimit = 0;	

	switch (static_cast<EGsOptionGraphicGrade>(_graphicGrade))
	{
	case EGsOptionGraphicGrade::CUSTOM:
	{		
		SetViewDistanceQuality(_graphicTable[EGsOptionGraphic::VIEW_DISTANCE]);
		SetPostProcessingQuality(_graphicTable[EGsOptionGraphic::POST_PROCESS]);
		SetShadowQuality(_graphicTable[EGsOptionGraphic::SHADOW]);
		SetAntiAliasingQuality(_graphicTable[EGsOptionGraphic::ANTIALIASING_QUALITY]);
		SetTextureQuality(_graphicTable[EGsOptionGraphic::TEXTURE]);
		SetVisualEffectQuality(_graphicTable[EGsOptionGraphic::EFFECT]);
		SetFoliageQuality(_graphicTable[EGsOptionGraphic::FOLIAGE]);
		SetShadingQuality(_graphicTable[EGsOptionGraphic::SHADING_QUALITY]);
	} break;

	case EGsOptionGraphicGrade::LOW:
	{
		GSLOG(Log, TEXT("UGsGameUserSettings Set LOW ====="));

		SetViewDistanceQuality(0);
		SetPostProcessingQuality(0);
		SetShadowQuality(0);
		SetAntiAliasingQuality(0);
		SetTextureQuality(0);
		SetVisualEffectQuality(0);
		SetFoliageQuality(0);
		SetShadingQuality(0);

		_graphicTable[EGsOptionGraphic::RESOLUTION] = 0;
		_graphicTable[EGsOptionGraphic::FRAME_RATE] = 0;
		_graphicTable[EGsOptionGraphic::OTHER_PC_RENDERING] = 0;
	} break;

	case EGsOptionGraphicGrade::MID:
	{		
		SetViewDistanceQuality(1);
		SetPostProcessingQuality(1);
		SetShadowQuality(1);
		SetAntiAliasingQuality(1);
		SetTextureQuality(1);
		SetVisualEffectQuality(1);
		SetFoliageQuality(1);
		SetShadingQuality(1);

		_graphicTable[EGsOptionGraphic::RESOLUTION] = 1;
		_graphicTable[EGsOptionGraphic::FRAME_RATE] = 1;
		_graphicTable[EGsOptionGraphic::OTHER_PC_RENDERING] = 1;
	} break;

	case EGsOptionGraphicGrade::HIGH:
	{
		GSLOG(Log, TEXT("UGsGameUserSettings Set HIGH ====="));

		SetViewDistanceQuality(2);
		SetPostProcessingQuality(2);
		SetShadowQuality(2);
		SetAntiAliasingQuality(2);
		SetTextureQuality(2);
		SetVisualEffectQuality(2);
		SetFoliageQuality(2);
		SetShadingQuality(2);

		_graphicTable[EGsOptionGraphic::RESOLUTION] = 2;
		_graphicTable[EGsOptionGraphic::FRAME_RATE] = 2;
		_graphicTable[EGsOptionGraphic::OTHER_PC_RENDERING] = 2;

		_graphicTable[EGsOptionGraphic::SSAO] = MAX_OPT_SSAO_LEVELS;		
	} break;

	case EGsOptionGraphicGrade::TOP:
	{
		GSLOG(Log, TEXT("UGsGameUserSettings Set TOP ====="));

		SetViewDistanceQuality(3);
		SetPostProcessingQuality(3);
		SetShadowQuality(3);
		SetAntiAliasingQuality(3);
		SetTextureQuality(3);
		SetVisualEffectQuality(3);
		SetFoliageQuality(3);
		SetShadingQuality(3);
		_graphicTable[EGsOptionGraphic::RESOLUTION] = 3;
		_graphicTable[EGsOptionGraphic::FRAME_RATE] = 3;
		_graphicTable[EGsOptionGraphic::OTHER_PC_RENDERING] = 3;

		_graphicTable[EGsOptionGraphic::SSAO] = MAX_OPT_SSAO_LEVELS;
	} break;

	default:
	break;
	}

	ApplyFrameRate();
#if PLATFORM_ANDROID || PLATFORM_IOS
	ApplyFSRSetting();
#endif
	ApplyGraphicResolutionSetting();
	ApplyGraphicAdditionalSettings();
	ApplyAntiAliasing();
	ApplyVSyncEnabled();
	ApplySettings(false);

	GSLOG(Log, TEXT("UGsGameUserSettings ApplyGraphicSettings() end call"));

#else
	bool FindFirstRunSetting = false;
	FindFirstRunSetting = GConfig->GetBool(INI_SECTION_USERINFO, TEXT("FirstRunSetting"), FindFirstRunSetting, GGameUserSettingsIni);	

	GSLOG(Log, TEXT("UGsGameUserSettings ApplyGraphicSettings() start call"));
	// 프레임수 제한
	int32 frameRateLimit = 0;

	switch (static_cast<EGsOptionGraphicGrade>(_graphicGrade))
	{
	case EGsOptionGraphicGrade::CUSTOM:
	{
		GSLOG(Log, TEXT("UGsGameUserSettings Set CUSTOM EGsOptionGraphic::VIEW_DISTANCE : %d"), _graphicTable[EGsOptionGraphic::VIEW_DISTANCE]);
		GSLOG(Log, TEXT("UGsGameUserSettings Set CUSTOM EGsOptionGraphic::POST_PROCESS : %d"), _graphicTable[EGsOptionGraphic::POST_PROCESS]);
		GSLOG(Log, TEXT("UGsGameUserSettings Set CUSTOM EGsOptionGraphic::SHADOW : %d"), _graphicTable[EGsOptionGraphic::SHADOW]);
		
		ScalabilityQuality.ViewDistanceQuality = _graphicTable[EGsOptionGraphic::VIEW_DISTANCE];
		ScalabilityQuality.PostProcessQuality = _graphicTable[EGsOptionGraphic::POST_PROCESS];
		ScalabilityQuality.ShadowQuality = _graphicTable[EGsOptionGraphic::SHADOW];

	} break;

	case EGsOptionGraphicGrade::LOW:
	{
		GSLOG(Log, TEXT("UGsGameUserSettings Set LOW ====="));		

		ScalabilityQuality.ViewDistanceQuality = 0;
		ScalabilityQuality.PostProcessQuality = 0;
		ScalabilityQuality.ShadowQuality = 0;		

		_graphicTable[EGsOptionGraphic::RESOLUTION] = 0;
		_graphicTable[EGsOptionGraphic::FRAME_RATE] = 0;
		_graphicTable[EGsOptionGraphic::OTHER_PC_RENDERING] = 0;
	} break;

	case EGsOptionGraphicGrade::MID:
	{
		GSLOG(Log, TEXT("UGsGameUserSettings Set MID ====="));

		ScalabilityQuality.ViewDistanceQuality = 1;
		ScalabilityQuality.PostProcessQuality = 1;
		ScalabilityQuality.ShadowQuality = 1;

		_graphicTable[EGsOptionGraphic::RESOLUTION] = 1;

		if (FindFirstRunSetting == false)
			_graphicTable[EGsOptionGraphic::FRAME_RATE] = 0;
		else
			_graphicTable[EGsOptionGraphic::FRAME_RATE] = 1;

		_graphicTable[EGsOptionGraphic::OTHER_PC_RENDERING] = 1;
	} break;

	case EGsOptionGraphicGrade::HIGH:
	{
		GSLOG(Log, TEXT("UGsGameUserSettings Set HIGH ====="));

		ScalabilityQuality.ViewDistanceQuality = 2;
		ScalabilityQuality.PostProcessQuality = 2;
		ScalabilityQuality.ShadowQuality = 2;

		_graphicTable[EGsOptionGraphic::RESOLUTION] = 2;

		if (FindFirstRunSetting == false)
			_graphicTable[EGsOptionGraphic::FRAME_RATE] = 0;
		else
			_graphicTable[EGsOptionGraphic::FRAME_RATE] = 2;

		_graphicTable[EGsOptionGraphic::OTHER_PC_RENDERING] = 2;

		_graphicTable[EGsOptionGraphic::SSAO] = MAX_OPT_SSAO_LEVELS;
	} break;

	case EGsOptionGraphicGrade::TOP:
	{
		GSLOG(Log, TEXT("UGsGameUserSettings Set TOP ====="));

		ScalabilityQuality.ViewDistanceQuality = 3;
		ScalabilityQuality.PostProcessQuality = 3;
		ScalabilityQuality.ShadowQuality = 3;

		_graphicTable[EGsOptionGraphic::RESOLUTION] = 3;

		if (FindFirstRunSetting == false)
			_graphicTable[EGsOptionGraphic::FRAME_RATE] = 0;
		else
			_graphicTable[EGsOptionGraphic::FRAME_RATE] = 3;

		_graphicTable[EGsOptionGraphic::OTHER_PC_RENDERING] = 3;

		_graphicTable[EGsOptionGraphic::SSAO] = MAX_OPT_SSAO_LEVELS;
	} break;

	default:
		break;
	}

	GSLOG(Log, TEXT("UGsGameUserSettings Set, NextLoad EGsOptionGraphic::VIEW_DISTANCE : %d"), ScalabilityQuality.ViewDistanceQuality);
	GSLOG(Log, TEXT("UGsGameUserSettings Set, NextLoad EGsOptionGraphic::POST_PROCESS : %d"), ScalabilityQuality.PostProcessQuality);
	GSLOG(Log, TEXT("UGsGameUserSettings Set, NextLoad EGsOptionGraphic::SHADOW : %d"), ScalabilityQuality.ShadowQuality);

	ApplyFrameRate();
#if PLATFORM_ANDROID || PLATFORM_IOS
	ApplyFSRSetting();
#endif

	ApplyGraphicResolutionSetting();
	ApplyGraphicAdditionalSettings();	

	ApplyNonResolutionSettings();
	ApplyGraphicScalabiltySettings();
	
	GSLOG(Log, TEXT("UGsGameUserSettings ApplyGraphicSettings() end call"));
#endif
}


void UGsGameUserSettings::LoadAudioSettings()
{
	// Whole
	{
		// On/Off
		bool wholeOn = true;
		GConfig->GetBool(INI_SECTION_AUDIO, TEXT("WholeOn"), wholeOn, GGameUserSettingsIni);
		_audioTable.Emplace(EGsOptionAudio::WHOLE_ON, wholeOn);

		// Volume
		int32 wholeVolume = 0;
		GConfig->GetInt(INI_SECTION_AUDIO, TEXT("WholeVolume"), wholeVolume, GGameUserSettingsIni);
		wholeVolume = FMath::Clamp(wholeVolume, 0, MAX_OPT_AUDIO_VOLUME);
		_audioTable.Emplace(EGsOptionAudio::WHOLE_VOLUME, wholeVolume);
	}
	// Music
	{
		// On/Off
		bool musicOn = true;
		GConfig->GetBool(INI_SECTION_AUDIO, TEXT("MusicOn"), musicOn, GGameUserSettingsIni);
		_audioTable.Emplace(EGsOptionAudio::MUSIC_ON, musicOn);

		// Volume
		int32 musicVolume = 0;
		GConfig->GetInt(INI_SECTION_AUDIO, TEXT("MusicVolume"), musicVolume, GGameUserSettingsIni);
		musicVolume = FMath::Clamp(musicVolume, 0, MAX_OPT_AUDIO_VOLUME);
		_audioTable.Emplace(EGsOptionAudio::MUSIC_VOLUME, musicVolume);
	}
	// Effect
	{
		// On/Off
		bool effectOn = true;
		GConfig->GetBool(INI_SECTION_AUDIO, TEXT("EffectOn"), effectOn, GGameUserSettingsIni);
		_audioTable.Emplace(EGsOptionAudio::EFFECT_ON, effectOn);

		// Volume
		int32 effectVolume = 0;
		GConfig->GetInt(INI_SECTION_AUDIO, TEXT("EffectVolume"), effectVolume, GGameUserSettingsIni);
		effectVolume = FMath::Clamp(effectVolume, 0, MAX_OPT_AUDIO_VOLUME);
		_audioTable.Emplace(EGsOptionAudio::EFFECT_VOLUME, effectVolume);
	}
	// Notice
	{
		// On/Off
		bool noticeOn = true;
		GConfig->GetBool(INI_SECTION_AUDIO, TEXT("NoticeOn"), noticeOn, GGameUserSettingsIni);
		_audioTable.Emplace(EGsOptionAudio::NOTICE_ON, noticeOn);

		// Volume
		int32 noticeVolume = 0;
		GConfig->GetInt(INI_SECTION_AUDIO, TEXT("NoticeVolume"), noticeVolume, GGameUserSettingsIni);
		noticeVolume = FMath::Clamp(noticeVolume, 0, MAX_OPT_AUDIO_VOLUME);
		_audioTable.Emplace(EGsOptionAudio::NOTICE_VOLUME, noticeVolume);
	}

	// On/Off
	bool focusVolumeMultipler = false;
	GConfig->GetBool(INI_SECTION_AUDIO, TEXT("FocusVolumeMultipler"), focusVolumeMultipler, GGameUserSettingsIni);
	_audioTable.Emplace(EGsOptionAudio::FOCUS_VOLUME_MULTIPLER, focusVolumeMultipler);	
}

void UGsGameUserSettings::LoadUserInfoSettings()
{
	FString vidName;
	GConfig->GetString(INI_SECTION_USERINFO, TEXT("VidName"), vidName, GGameUserSettingsIni);
	_userInfoTable.Emplace(EGsOptionUserInfo::VID_NAME, vidName);

	FString autoLogin;
	GConfig->GetString(INI_SECTION_USERINFO, TEXT("AutoLogin"), autoLogin, GGameUserSettingsIni);
	_userInfoTable.Emplace(EGsOptionUserInfo::AUTO_LOGIN, autoLogin);	

	FString serverName;
	GConfig->GetString(INI_SECTION_USERINFO, TEXT("ServerName"), serverName, GGameUserSettingsIni);
	_userInfoTable.Emplace(EGsOptionUserInfo::LAST_ENTER_WORLD_NAME, serverName);

	FString backToLobby;
	GConfig->GetString(INI_SECTION_USERINFO, TEXT("BackToLobby"), backToLobby, GGameUserSettingsIni);
	_userInfoTable.Emplace(EGsOptionUserInfo::BACKTO_LOBBY, backToLobby);

	FString lastEnterCharacter;
	GConfig->GetString(INI_SECTION_USERINFO, TEXT("LastEnterCharacter"), lastEnterCharacter, GGameUserSettingsIni);
	_userInfoTable.Emplace(EGsOptionUserInfo::LAST_ENTER_CHARACTER, lastEnterCharacter);

	FString loginID;
	GConfig->GetString(INI_SECTION_USERINFO, TEXT("LoginID"), loginID, GGameUserSettingsIni);
	_userInfoTable.Emplace(EGsOptionUserInfo::LOGIN_ID, loginID);

	FString loginPassword;
	GConfig->GetString(INI_SECTION_USERINFO, TEXT("LoginPassword"), loginPassword, GGameUserSettingsIni);
	_userInfoTable.Emplace(EGsOptionUserInfo::LOGIN_PASSWORD, loginPassword);

	FString language;
	if (false == GConfig->GetString(INI_SECTION_USERINFO, TEXT("Language"), language, GGameUserSettingsIni))
	{
		language = TEXT("-1");
	}
	_userInfoTable.Emplace(EGsOptionUserInfo::LANGUAGE, language);

	FString languageVoice;
	if (false == GConfig->GetString(INI_SECTION_USERINFO, TEXT("LanguageVoice"), languageVoice, GGameUserSettingsIni))
	{
		languageVoice = TEXT("-1");
	}
	_userInfoTable.Emplace(EGsOptionUserInfo::LANGUAGE_VOICE, languageVoice);
}

void UGsGameUserSettings::LoadChatOptionSetting()
{
	_chatTable.Empty();
	_chatGroupTable.Empty();

	for (auto optionType : {
			TPair<EGsOptionChat, FString>{ EGsOptionChat::NORMAL_CHAT, TEXT("ChatEnableNormal") },
			TPair<EGsOptionChat, FString>{ EGsOptionChat::SERVER_CHAT, TEXT("ChatEnableWorld") },
			TPair<EGsOptionChat, FString>{ EGsOptionChat::PARTY_CHAT, TEXT("ChatEnableParty") },
			TPair<EGsOptionChat, FString>{ EGsOptionChat::ALLYGUILD_CHAT, TEXT("ChatEnableAllience") },
			TPair<EGsOptionChat, FString>{ EGsOptionChat::GUILD_CHAT, TEXT("ChatEnableKnights") },
			TPair<EGsOptionChat, FString>{ EGsOptionChat::WHISPER, TEXT("ChatEnableWhisper") },
			TPair<EGsOptionChat, FString>{ EGsOptionChat::SYSTEM, TEXT("ChatEnableSystem") },
			TPair<EGsOptionChat, FString>{ EGsOptionChat::INVADE_CHAT, TEXT("ChatEnableInvasion") },
			TPair<EGsOptionChat, FString>{ EGsOptionChat::EMOJI_POPUP, TEXT("ChatEnableEmojiPpoup") },
			TPair<EGsOptionChat, FString>{ EGsOptionChat::TALK_POPUP, TEXT("ChatEnableTalkPopup") } })
	{
		bool optionValue = 0;
		if (!GConfig->GetBool(INI_SECTION_CHATOPT, *(optionType.Value), optionValue, GGameUserSettingsIni))
			optionValue = true;

		_chatTable.Emplace(optionType.Key, optionValue);
	}

	TArray<FString> strRooms;
	GConfig->GetArray(INI_SECTION_CHATOPT, TEXT("GroupChatDisabled"), strRooms, GGameUserSettingsIni);

	Algo::TransformIf(strRooms, _chatGroupTable, [](FString str) { return str.IsNumeric(); }, [](FString str) { return  FCString::Atoi64(*str); });
}

void UGsGameUserSettings::LoadBlessItemOptionSetting()
{
	_autoUseBlessRestore.Empty();
	_autoUseBlessRestore.Add(EGsOptionBlessItem::USE_TEAR, false);
	_autoUseBlessRestore.Add(EGsOptionBlessItem::USE_STGMATA, false);

	for (auto optionType : {
			TPair<EGsOptionBlessItem, FString>{ EGsOptionBlessItem::USE_TEAR, TEXT("AutoUseBlessTearItem") },
			TPair<EGsOptionBlessItem, FString>{ EGsOptionBlessItem::USE_STGMATA, TEXT("AutoUseBlessStigmataItem") } })
	{
		bool optionValue = false;
		GConfig->GetBool(INI_SECTION_BLESSOPT, *(optionType.Value), optionValue, GGameUserSettingsIni);
		_autoUseBlessRestore.Emplace(optionType.Key, optionValue);
	}
}

void UGsGameUserSettings::LoadChatMacro()
{
	_chatMacro.Empty();

	for (int index = 0; index < 10; index++)
	{
		FString key = FString::Format(TEXT("ChatMacro{0}"), { index });

		FString macroValue;
		GConfig->GetString(INI_SECTION_CHATMACRO, *key, macroValue, GGameUserSettingsIni);
		_chatMacro.Emplace(index, FText::FromString(macroValue));
	}
}

void UGsGameUserSettings::LoadOutputSetting()
{
	// 출력
	bool summpnSkipOn = false;
	GConfig->GetBool(INI_SECTION_OUTPUT, TEXT("SummonSkip"), summpnSkipOn, GGameUserSettingsIni);
	_outputTable.Emplace(EGsOptionOutput::SUMMON_SKIP, summpnSkipOn);

	bool summpnSkipComposeOn = false;
	GConfig->GetBool(INI_SECTION_OUTPUT, TEXT("SummonSkipCompose"), summpnSkipComposeOn, GGameUserSettingsIni);
	_outputTable.Emplace(EGsOptionOutput::SUMMON_SKIP_COMPOSE, summpnSkipComposeOn);

	bool questSkipOn = true;
	GConfig->GetBool(INI_SECTION_OUTPUT, TEXT("QuestSkip"), questSkipOn, GGameUserSettingsIni);
	_outputTable.Emplace(EGsOptionOutput::QUEST_SKIP, questSkipOn);


	bool isUseCameraShake = true;
	GConfig->GetBool(INI_SECTION_OUTPUT, TEXT("IsUseCameraShake"), isUseCameraShake, GGameUserSettingsIni);
	_outputTable.Emplace(EGsOptionOutput::IS_USE_CAMERA_SHAKE, isUseCameraShake);

#if WITH_EDITOR || PLATFORM_WINDOWS
	_outputTable.Emplace(EGsOptionOutput::WINDOW_KEY_MAPPTIN, false);
#endif

	bool questAutoTeleport = true;
	GConfig->GetBool(INI_SECTION_OUTPUT, TEXT("QuestAutoTeleport"), questAutoTeleport, GGameUserSettingsIni);
	_outputTable.Emplace(EGsOptionOutput::QUEST_AUTO_TELEPORT, questAutoTeleport);
}

void UGsGameUserSettings::LoadAlertSetting()
{
	_alertTable.Empty();

	bool bAllAlert = true;
	GConfig->GetBool(INI_SECTION_NOTICE, TEXT("AllAlert"), bAllAlert, GGameUserSettingsIni);
	_alertTable.Emplace(EGsOptionAlert::ALL_ALERT, bAllAlert);

	bool bAllGameAlert = true;
	GConfig->GetBool(INI_SECTION_NOTICE, TEXT("AllGameAlert"), bAllGameAlert, GGameUserSettingsIni);
	_allGameAlert = bAllGameAlert;

	bool bConsoleAlert = true;
	GConfig->GetBool(INI_SECTION_NOTICE, TEXT("ConsoleAlert"), bConsoleAlert, GGameUserSettingsIni);
	_alertTable.Emplace(EGsOptionAlert::CONSOLE_ALERT, bConsoleAlert);

	bool bConsoleAlertNight = true;
	GConfig->GetBool(INI_SECTION_NOTICE, TEXT("ConsoleAlertNight"), bConsoleAlertNight, GGameUserSettingsIni);
	_alertTable.Emplace(EGsOptionAlert::CONSOLE_ALERT_NIGHT, bConsoleAlertNight);
		
	bool bItemAddAlert = true;
	GConfig->GetBool(INI_SECTION_NOTICE, TEXT("ItemAddAlert"), bItemAddAlert, GGameUserSettingsIni);
	_alertTable.Emplace(EGsOptionAlert::ITEM_ADD_ALERT, bItemAddAlert);
}

void UGsGameUserSettings::LoadMarketSetting()
{
	GConfig->GetArray(INI_SECTION_MARKET, TEXT("Search"), _marketSearchList, GGameUserSettingsIni);
}
void UGsGameUserSettings::LoadCameraModeBackupSetting()
{
	_cameraModeBackup.Empty();

	float isBackup = 0.0f;
	GConfig->GetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("IsBackup"), isBackup, GGameUserSettingsIni);
	_cameraModeBackup.Emplace(EGsOptionCameraModeBackup::IS_BACKUP, isBackup);

	float camMode = 0.0f;
	GConfig->GetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("CamMode"), camMode, GGameUserSettingsIni);
	_cameraModeBackup.Emplace(EGsOptionCameraModeBackup::CAM_MODE, camMode);

	float zoomVal1 = 0.0f;
	GConfig->GetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("ZoomVal1"), zoomVal1, GGameUserSettingsIni);
	_cameraModeBackup.Emplace(EGsOptionCameraModeBackup::ZOOM_VAL_1, zoomVal1);

	float zoomVal2 = 0.0f;
	GConfig->GetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("ZoomVal2"), zoomVal2, GGameUserSettingsIni);
	_cameraModeBackup.Emplace(EGsOptionCameraModeBackup::ZOOM_VAL_2, zoomVal2);

	float zoomVal3 = 0.0f;
	GConfig->GetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("ZoomVal3"), zoomVal3, GGameUserSettingsIni);
	_cameraModeBackup.Emplace(EGsOptionCameraModeBackup::ZOOM_VAL_3, zoomVal3);

	float pitchVal1 = 0.0f;
	GConfig->GetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("PitchVal1"), pitchVal1, GGameUserSettingsIni);
	_cameraModeBackup.Emplace(EGsOptionCameraModeBackup::PITCH_VAL_1, pitchVal1);

	float pitchVal2 = 0.0f;
	GConfig->GetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("PitchVal2"), pitchVal2, GGameUserSettingsIni);
	_cameraModeBackup.Emplace(EGsOptionCameraModeBackup::PITCH_VAL_2, pitchVal2);

	float pitchVal3 = 0.0f;
	GConfig->GetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("PitchVal3"), pitchVal3, GGameUserSettingsIni);
	_cameraModeBackup.Emplace(EGsOptionCameraModeBackup::PITCH_VAL_3, pitchVal3);

	float quatX = 0.0f;
	GConfig->GetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("QuatX"), quatX, GGameUserSettingsIni);
	_cameraModeBackup.Emplace(EGsOptionCameraModeBackup::QUAT_X, quatX);

	float quatY = 0.0f;
	GConfig->GetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("QuatY"), quatY, GGameUserSettingsIni);
	_cameraModeBackup.Emplace(EGsOptionCameraModeBackup::QUAT_Y, quatY);

	float quatZ = 0.0f;
	GConfig->GetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("QuatZ"), quatZ, GGameUserSettingsIni);
	_cameraModeBackup.Emplace(EGsOptionCameraModeBackup::QUAT_Z, quatZ);

	float quatW = 0.0f;
	GConfig->GetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("QuatW"), quatW, GGameUserSettingsIni);
	_cameraModeBackup.Emplace(EGsOptionCameraModeBackup::QUAT_W, quatW);

	float fixedQuarterYawType = 0.0f;
	GConfig->GetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("FixedQuarterYawType"), fixedQuarterYawType, GGameUserSettingsIni);
	_cameraModeBackup.Emplace(EGsOptionCameraModeBackup::FIXED_QUARTER_YAW_TYPE, fixedQuarterYawType);

}

void UGsGameUserSettings::LoadScanSetting()
{
	_scanTable.Empty();

	int32 detectType = 0;
	// return이 0이면 가져오는데 실패이다
	if (GConfig->GetInt(INI_SECTION_SCAN, TEXT("DetectType"), detectType, GGameUserSettingsIni) == 0)
	{
		detectType = FGsScaneFunc::GetDefaultScanDetectType();
	}
	_scanTable.Emplace(EGsOptionScan::DETECT_TYPE, detectType);

	int32 isAutoScan = 0;
	if (GConfig->GetInt(INI_SECTION_SCAN, TEXT("IsAutoScan"), isAutoScan, GGameUserSettingsIni) == 0)
	{
		// 명시적으로 로드 실패시 off
		isAutoScan = 0;
	}
	_scanTable.Emplace(EGsOptionScan::IS_AUTO_SCAN, isAutoScan);

	int32 scanTimmerSecond = 0;
	if (GConfig->GetInt(INI_SECTION_SCAN, TEXT("ScanTimmerSecond"), scanTimmerSecond, GGameUserSettingsIni) == 0)
	{
		// 명시적으로 로드 실패시 첫번째 인덱스(3초)
		scanTimmerSecond = 0;
	}

	_scanTable.Emplace(EGsOptionScan::SCAN_TIMMER_SECOND, scanTimmerSecond);

	int32 detectGroupType = 0;
	if (GConfig->GetInt(INI_SECTION_SCAN, TEXT("DetectGroupType"), detectGroupType, GGameUserSettingsIni) == 0)
	{
		detectGroupType = FGsScaneFunc::GetDefaultScanDetectGroupType();
	}
	_scanTable.Emplace(EGsOptionScan::DETECT_GROUP_TYPE, detectGroupType);
}

void UGsGameUserSettings::LoadSaveBatterySetting()
{
	_saveBatteryTable.Empty();
	
	int32 timerMinute = 0;
	GConfig->GetInt(INI_SECTION_SAVEBATTERY, TEXT("SavebatteryTimerMinute"), timerMinute, GGameUserSettingsIni);
	_saveBatteryTable.Emplace(EGsOptionSaveBattery::SAVEBATTERY_TIMER_MINUTE, timerMinute);

	int32 isPvpAutoOff = 1;
	GConfig->GetInt(INI_SECTION_SAVEBATTERY, TEXT("IsPvpAutoOff"), isPvpAutoOff, GGameUserSettingsIni);
	_saveBatteryTable.Emplace(EGsOptionSaveBattery::IS_PVP_AUTO_OFF, isPvpAutoOff);
}

void UGsGameUserSettings::LoadAutoSetting()
{
	_autoTable.Empty();

	float autoSkillManaLimitRate = 0.0f;
	GConfig->GetFloat(INI_SECTION_AUTO, TEXT("AutoSkillManaLimitRate"), autoSkillManaLimitRate, GGameUserSettingsIni);
	_autoTable.Emplace(EGsOptionAuto::AUTO_SKILL_MANA_LIMIT_RATE, autoSkillManaLimitRate);

	float isAlwasysUseAutoBuffSkill = 0.0f;
	GConfig->GetFloat(INI_SECTION_AUTO, TEXT("IsAlwasysUseAutoBuffSkill"), isAlwasysUseAutoBuffSkill, GGameUserSettingsIni);
	_autoTable.Emplace(EGsOptionAuto::IS_ALWAYS_USE_AUTO_BUFF_SKILL, isAlwasysUseAutoBuffSkill);

	float isAutoPartyAccept = 0.0f;
	GConfig->GetFloat(INI_SECTION_AUTO, TEXT("IsAutoPartyAccept"), isAutoPartyAccept, GGameUserSettingsIni);
	_autoTable.Emplace(EGsOptionAuto::IS_AUTO_PARTY_ACCEPT, isAutoPartyAccept);

	float autoItemLootOtherUsers = 0.0f;
	GConfig->GetFloat(INI_SECTION_AUTO, TEXT("AutoItemLootOtherUsers"), autoItemLootOtherUsers, GGameUserSettingsIni);
	_autoTable.Emplace(EGsOptionAuto::AUTO_ITEM_LOOT_OTHER_USERS, autoItemLootOtherUsers);
}

void UGsGameUserSettings::LoadInputSetting()
{
	_inputTable.Empty();

	int32 isUseTouchMove = 0;
	GConfig->GetInt(INI_SECTION_INPUT, TEXT("IsUseTouchMove"), isUseTouchMove, GGameUserSettingsIni);
	_inputTable.Emplace(EGsOptionInput::IS_USE_TOUCH_MOVE, isUseTouchMove);
}

void UGsGameUserSettings::LoadHudSetting()
{
	_hudTable.Empty();

	int32 isShow = 1;
	GConfig->GetInt(INI_SECTION_HUD, TEXT("IsShowServerRevisionAndLocation"), isShow, GGameUserSettingsIni);
	_hudTable.Emplace(EGsOptionHud::IS_SHOW_SERVER_REVISION_AND_LOCATION, isShow);
}

void UGsGameUserSettings::LoadCommunityEventRedDotSetting()
{
	_communityEventRedDot.Empty();

	if (FGsGameDataManager* dataManager = GGameData())
	{		
		if (const FGsNetUserData* UserData = dataManager->GetUserData())
		{
			FString EventKey = FString::Format(TEXT("CommunityEventRedDot[{0}]"), { UserData->mUserDBId });

			GConfig->GetArray(INI_SECTION_COMMUNITYEVENT_REDDOT, *EventKey, _communityEventRedDot, GGameUserSettingsIni);
		}
	}
}	

void UGsGameUserSettings::LoadCompositionOption()
{
	GConfig->GetBool(INI_SECTION_COMPOSITION, TEXT("FairyLeftLock"), _composeFairy.leftLock, GGameUserSettingsIni);
	GConfig->GetBool(INI_SECTION_COMPOSITION, TEXT("FairyRightLock"), _composeFairy.rightLock, GGameUserSettingsIni);

	GConfig->GetBool(INI_SECTION_COMPOSITION, TEXT("CostumeLeftLock"), _composeCostume.leftLock, GGameUserSettingsIni);
	GConfig->GetBool(INI_SECTION_COMPOSITION, TEXT("CostumeRightLock"), _composeCostume.rightLock, GGameUserSettingsIni);
}

void UGsGameUserSettings::SaveGraphicSetting(const TCHAR* key, int32 val)
{
	GConfig->SetInt(INI_SECTION_GRAPHIC, key, val, GGameUserSettingsIni);
}

void UGsGameUserSettings::SaveAudioSetting(const TCHAR* key, int32 val)
{
	GConfig->SetInt(INI_SECTION_AUDIO, key, val, GGameUserSettingsIni);
}

void UGsGameUserSettings::SaveUserInfoSetting(const TCHAR* key, const FString& val)
{
	GConfig->SetString(INI_SECTION_USERINFO, key, *val, GGameUserSettingsIni);
}

void UGsGameUserSettings::SaveUserCombatSetting(const TCHAR* key, int32 val)
{
	GConfig->SetInt(INI_SECTION_COMBAT, key, val, GGameUserSettingsIni);
}

void UGsGameUserSettings::SaveChatOptionSetting(const TCHAR* key, bool val)
{
	GConfig->SetBool(INI_SECTION_CHATOPT, key, val, GGameUserSettingsIni);
}

void UGsGameUserSettings::SaveChatMacro(const TCHAR* key, const FText& macro)
{
	GConfig->SetString(INI_SECTION_CHATMACRO, key, *(macro.ToString()), GGameUserSettingsIni);
}

void UGsGameUserSettings::SaveOutputSetting(const TCHAR* key, int32 val)
{
	GConfig->SetInt(INI_SECTION_OUTPUT, key, val, GGameUserSettingsIni);
}

void UGsGameUserSettings::SaveAlertSetting(const TCHAR* key, int32 val)
{
	GConfig->SetInt(INI_SECTION_NOTICE, key, val, GGameUserSettingsIni);
}

void UGsGameUserSettings::SaveMarketSetting()
{
	GConfig->SetArray(INI_SECTION_MARKET, TEXT("Search"), _marketSearchList, GGameUserSettingsIni);
}

void UGsGameUserSettings::SaveSaveBatterySetting(const TCHAR* key, int32 val)
{
	GConfig->SetInt(INI_SECTION_SAVEBATTERY, key, val, GGameUserSettingsIni);
}
void UGsGameUserSettings::SaveAutoSetting(const TCHAR* key, float val)
{
	GConfig->SetFloat(INI_SECTION_AUTO, key, val, GGameUserSettingsIni);
}
void UGsGameUserSettings::SaveInputSetting(const TCHAR* key, int32 val)
{
	GConfig->SetInt(INI_SECTION_INPUT, key, val, GGameUserSettingsIni);
}

void UGsGameUserSettings::SaveHudSetting(const TCHAR* key, int32 val)
{
	GConfig->SetInt(INI_SECTION_HUD, key, val, GGameUserSettingsIni);
}

void UGsGameUserSettings::SaveCommunityEventRedDotSetting()
{
	if (FGsGameDataManager* dataManager = GGameData())
	{
		if (const FGsNetUserData* UserData = dataManager->GetUserData())
		{
			FString EventKey = FString::Format(TEXT("CommunityEventRedDot[{0}]"), { GGameData()->GetUserData()->mUserDBId });

			GConfig->SetArray(INI_SECTION_COMMUNITYEVENT_REDDOT, *EventKey, _communityEventRedDot, GGameUserSettingsIni);
		}
	}
}

void UGsGameUserSettings::SaveCompositionSetting(const TCHAR* key, bool isVal)
{
	GConfig->SetBool(INI_SECTION_COMPOSITION, key, isVal, GGameUserSettingsIni);
}

void UGsGameUserSettings::SaveCameraModeBackupSetting(EGsCameraMode In_mode, TMap<EGsCameraMode, float> In_dicZoomVal, 
	TMap<EGsCameraMode, float> In_dicPitchVal,
	FQuat In_quat,
	EGsCameraFixedQuarterYawType In_fixedQaurterYawType)
{
	_cameraModeBackup[EGsOptionCameraModeBackup::IS_BACKUP] = true;
	_cameraModeBackup[EGsOptionCameraModeBackup::CAM_MODE] = (float)In_mode;


	for (auto iter: In_dicZoomVal)
	{
		switch (iter.Key)
		{
		case EGsCameraMode::BackView:
		{
			_cameraModeBackup[EGsOptionCameraModeBackup::ZOOM_VAL_1] = iter.Value;
			break;
		}
		case EGsCameraMode::Quarter:
		{
			_cameraModeBackup[EGsOptionCameraModeBackup::ZOOM_VAL_2] = iter.Value;
			break;
		}
		case EGsCameraMode::FixedQuarter:
		{
			_cameraModeBackup[EGsOptionCameraModeBackup::ZOOM_VAL_3] = iter.Value;
			break;
		}
		}
	}

	for (auto iter : In_dicPitchVal)
	{
		switch (iter.Key)
		{
		case EGsCameraMode::BackView:
		{
			_cameraModeBackup[EGsOptionCameraModeBackup::PITCH_VAL_1] = iter.Value;
			break;
		}
		case EGsCameraMode::Quarter:
		{
			_cameraModeBackup[EGsOptionCameraModeBackup::PITCH_VAL_2] = iter.Value;
			break;
		}
		case EGsCameraMode::FixedQuarter:
		{
			_cameraModeBackup[EGsOptionCameraModeBackup::PITCH_VAL_3] = iter.Value;
			break;
		}
		}
	}

	_cameraModeBackup[EGsOptionCameraModeBackup::QUAT_X] = In_quat.X;
	_cameraModeBackup[EGsOptionCameraModeBackup::QUAT_Y] = In_quat.Y;
	_cameraModeBackup[EGsOptionCameraModeBackup::QUAT_Z] = In_quat.Z;
	_cameraModeBackup[EGsOptionCameraModeBackup::QUAT_W] = In_quat.W;

	_cameraModeBackup[EGsOptionCameraModeBackup::FIXED_QUARTER_YAW_TYPE] = (float)In_fixedQaurterYawType;


	GConfig->SetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("IsBackup"), 1.0f, GGameUserSettingsIni);
	GConfig->SetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("CamMode"), (float)In_mode, GGameUserSettingsIni);

	GConfig->SetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("ZoomVal1"), In_dicZoomVal[EGsCameraMode::BackView], GGameUserSettingsIni);
	GConfig->SetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("ZoomVal2"), In_dicZoomVal[EGsCameraMode::Quarter], GGameUserSettingsIni);
	GConfig->SetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("ZoomVal3"), In_dicZoomVal[EGsCameraMode::FixedQuarter], GGameUserSettingsIni);

	GConfig->SetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("PitchVal1"), In_dicPitchVal[EGsCameraMode::BackView], GGameUserSettingsIni);
	GConfig->SetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("PitchVal2"), In_dicPitchVal[EGsCameraMode::Quarter], GGameUserSettingsIni);
	GConfig->SetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("PitchVal3"), In_dicPitchVal[EGsCameraMode::FixedQuarter], GGameUserSettingsIni);

	GConfig->SetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("QuatX"), In_quat.X, GGameUserSettingsIni);
	GConfig->SetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("QuatY"), In_quat.Y, GGameUserSettingsIni);
	GConfig->SetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("QuatZ"), In_quat.Z, GGameUserSettingsIni);
	GConfig->SetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("QuatW"), In_quat.W, GGameUserSettingsIni);

	GConfig->SetFloat(INI_SECTION_CAMERA_MODE_BACKUP, TEXT("FixedQuarterYawType"), (float)In_fixedQaurterYawType, GGameUserSettingsIni);
}

void UGsGameUserSettings::SaveScanSetting(uint32 In_detectGroupType, uint32 In_detectType, bool In_isAutoScan, int In_scanTimmerIndex)
{	
	_scanTable[EGsOptionScan::DETECT_TYPE]			= (int32)In_detectType;
	_scanTable[EGsOptionScan::IS_AUTO_SCAN]			= (int32)In_isAutoScan;
	_scanTable[EGsOptionScan::SCAN_TIMMER_SECOND]	= In_scanTimmerIndex;
	_scanTable[EGsOptionScan::DETECT_GROUP_TYPE]	= (int32)In_detectGroupType;

	GConfig->SetInt(INI_SECTION_SCAN, TEXT("DetectType"), In_detectType, GGameUserSettingsIni);
	GConfig->SetInt(INI_SECTION_SCAN, TEXT("IsAutoScan"), (int32)In_isAutoScan, GGameUserSettingsIni);
	GConfig->SetInt(INI_SECTION_SCAN, TEXT("ScanTimmerSecond"), In_scanTimmerIndex, GGameUserSettingsIni);
	GConfig->SetInt(INI_SECTION_SCAN, TEXT("DetectGroupType"), In_detectGroupType, GGameUserSettingsIni);
}

void UGsGameUserSettings::SaveUserSettings()
{
	// 나머지 부분을 ini 파일에 쓰기
#if 0
	SaveConfig(CPF_Config, *GGameUserSettingsIni);
	UpdateDefaultConfigFile(*GGameUserSettingsIni);
#else
	SaveConfig();
#if !PLATFORM_WINDOWS
	UpdateDefaultConfigFile();
#endif
#endif
	SaveSettings();

	if (nullptr == GScope())
	{
		return;
	}

	FGsMessageHolder* Message = GMessage();
	if (nullptr != Message)
	{
		Message->GetUI().SendMessage(MessageUI::OPTION_SAVE_TO_CLIENT_COMPLETED, nullptr);
	}
}

//void UGsGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
//{
//	ApplyNonResolutionSettings();
//	RequestUIUpdate();
//	SaveUserSettings();
//}

/*
	Graphic 옵션 중 해상도는 별도로
*/
void UGsGameUserSettings::ApplyGraphicResolutionSetting()
{
	GSLOG(Log, TEXT("UGsGameUserSettings Apply ApplyGraphicResolutionSetting() call"));

	IConsoleVariable* resolutionCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
	if (ensure(resolutionCVar))
	{
		int32 val = _graphicTable[EGsOptionGraphic::RESOLUTION];
		resolutionCVar->Set(ResolutionValueArray[val], ECVF_SetByGameSetting);

		GSLOG(Log, TEXT("UGsGameUserSettings Apply resolutionCVar : %d"), val);
	}
}

void UGsGameUserSettings::ApplyFrameRate()
{
	GSLOG(Log, TEXT("UGsGameUserSettings Apply ApplyFrameRate() call"));

	int32 val = _graphicTable[EGsOptionGraphic::FRAME_RATE];
	val = FMath::Clamp(val, 0, MAX_OPT_GRAPHIC_LEVELS - 1);
	SetFrameRateLimit(static_cast<float>(FrameRateValueArray[val]));
	SetFrameRateLimitCVar(GetFrameRateLimit());

	GSLOG(Log, TEXT("UGsGameUserSettings Apply FrameRateLimit : %f"), static_cast<float>(FrameRateValueArray[val]));
	GSLOG(Log, TEXT("UGsGameUserSettings Apply FrameRateLimitCVar : %f"), GetFrameRateLimit());
}


#if WITH_EDITOR || PLATFORM_WINDOWS
void UGsGameUserSettings::ApplyAntiAliasing()
{
	GSLOG(Log, TEXT("UGsGameUserSettings Apply ApplyAntiAliasing() call"));

	int32 val = _graphicTable[EGsOptionGraphic::WINDOW_ANTIALIASING];
	SetAntiAliasing(val);

	GSLOG(Log, TEXT("UGsGameUserSettings Apply ApplyAntiAliasing : %d"), val);
}

void UGsGameUserSettings::ApplyVSyncEnabled()
{
	GSLOG(Log, TEXT("UGsGameUserSettings Apply ApplyVSyncEnabled() call"));

	int32 val = _graphicTable[EGsOptionGraphic::WINDOW_VSYNCENABLED];
	SetVSyncEnabled(static_cast<bool>(val));
	ApplyVsync();

	GSLOG(Log, TEXT("UGsGameUserSettings Apply VSyncEnabled : %d"), val);
	GSLOG(Log, TEXT("UGsGameUserSettings Apply ApplyVsync() call"));
}
#endif

/*
	Graphic 옵션 중 Additional 항목들 적용
*/
void UGsGameUserSettings::ApplyGraphicAdditionalSettings()
{
	GSLOG(Log, TEXT("UGsGameUserSettings Apply ApplyGraphicAdditionalSettings() call"));

	// SSAO
	IConsoleVariable* ssaoCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AmbientOcclusionLevels"));
	if (ensure(ssaoCVar))
	{
		int32 optVal = _graphicTable[EGsOptionGraphic::SSAO];
		ssaoCVar->Set((optVal == 0) ? 0 : MAX_OPT_SSAO_LEVELS, ECVF_SetByCode);
		GetRendererModule().UpdateStaticDrawLists();

		GSLOG(Log, TEXT("UGsGameUserSettings Apply SSAO : %d"), optVal);
	}
}

/*
	Graphic 옵션 중 UE4 지원 옵션들 적용/저장
*/
void UGsGameUserSettings::ApplyGraphicScalabiltySettings()
{
	GSLOG(Log, TEXT("UGsGameUserSettings Apply ApplyGraphicScalabiltySettings() call"));

//	ScalabilityQuality.ViewDistanceQuality = _graphicTable[EGsOptionGraphic::VIEW_DISTANCE];
//	ScalabilityQuality.PostProcessQuality = _graphicTable[EGsOptionGraphic::POST_PROCESS];
//	ScalabilityQuality.ShadowQuality = _graphicTable[EGsOptionGraphic::SHADOW];
//
//#if WITH_EDITOR || PLATFORM_WINDOWS
//	ScalabilityQuality.AntiAliasingQuality = _graphicTable[EGsOptionGraphic::ANTIALIASING_QUALITY];
//	ScalabilityQuality.TextureQuality = _graphicTable[EGsOptionGraphic::TEXTURE];
//	ScalabilityQuality.EffectsQuality = _graphicTable[EGsOptionGraphic::EFFECT];
//	ScalabilityQuality.FoliageQuality = _graphicTable[EGsOptionGraphic::FOLIAGE];
//	ScalabilityQuality.ShadingQuality = _graphicTable[EGsOptionGraphic::SHADING_QUALITY];
//#endif

	// Scalablity 값 설정
	//SetViewDistanceQuality(_graphicTable[EGsOptionGraphic::VIEW_DISTANCE]);
	//SetPostProcessingQuality(_graphicTable[EGsOptionGraphic::POST_PROCESS]);
	//SetShadowQuality(_graphicTable[EGsOptionGraphic::SHADOW]);

	// 먼저 Scalablity 값을 적용부터 하고
	if (GEngine->IsInitialized())
	{
		Scalability::SetQualityLevels(ScalabilityQuality, true);
	}

	// 저장
	Scalability::SaveState(GGameUserSettingsIni);
	UpdateResolutionQuality();
}

/*
	Graphic 옵션은 외부에서 이 함수로 값 설정
	참고) 개별 UI 조작이 아니라 옵션창을 닫을 때 한번에 적용
*/
void UGsGameUserSettings::SetGraphicSettings(const TMap<EGsOptionGraphic, int32>& graphicOpts, bool bImmediateApply)
{
	if (0 == graphicOpts.Num())
		return;

	for (auto& iter : graphicOpts)
	{
		EGsOptionGraphic type = iter.Key;
		if (_graphicTable.Contains(type))
		{
			_graphicTable[type] = iter.Value;
		}
	}

	_graphicGrade = graphicOpts.FindRef(EGsOptionGraphic::GRAPHIC_GRADE);
	if (0 >= _graphicGrade)
	{
		for (const auto& elem : graphicOpts)
		{
			EGsOptionGraphic optType = elem.Key;
			int32 optValue = elem.Value;

			SetGraphicSettingByType(optType, optValue);
		}
	}
	else
	{
		SetGraphicSettingByType(EGsOptionGraphic::GRAPHIC_GRADE, _graphicGrade);

		// UIScale값은 Grade와 상관 없이 저장
		if (const int32* uiScale = graphicOpts.Find(EGsOptionGraphic::UISCALE))
		{
			SetGraphicSettingByType(EGsOptionGraphic::UISCALE, *uiScale);
		}


#if WITH_EDITOR || PLATFORM_WINDOWS
		for (int i = (int)EGsOptionGraphic::WINDOW_RESOLUTION; i < (int)EGsOptionGraphic::MAX; ++i)
		{
			if (const int32* value = graphicOpts.Find((EGsOptionGraphic)i))
			{
				SetGraphicSettingByType((EGsOptionGraphic)i, * value);
			}
		}
#endif
	}
	
	ApplyGraphicSettings();

	// R3: HUD에서만 비율을 적용하기로 하며 주석처리. 처음부터 적용해야 하는 경우, 주석을 풀어야 함 
	// UIScale은 그래픽 품질과 크게 관계 없이 로드한 값 그대로 사용
	//float uiScaleValue = GetNormalizedUIScaleValue();
	//UGsUIScaleRule::SetUIScaleValue(uiScaleValue);
}

void UGsGameUserSettings::SetGraphicSettings(EGsOptionGraphicGrade GraphicGrade)
{
	_graphicGrade = static_cast<int32>(GraphicGrade);
	ApplyGraphicSettings();
}

/*
*/
void UGsGameUserSettings::SetGraphicSettingByType(EGsOptionGraphic optType, int32 optVal)
{
	if (optType >= EGsOptionGraphic::MAX)
		return;
	
	switch (optType)
	{
	// 기본 항목들
	case EGsOptionGraphic::RESOLUTION:
		SaveGraphicSetting(TEXT("ScreenPercentage"), optVal);
		break;
	case EGsOptionGraphic::FRAME_RATE:
		SaveGraphicSetting(TEXT("FrameRateLimit"), optVal);
		break;
	case EGsOptionGraphic::OTHER_PC_RENDERING:
		SaveGraphicSetting(TEXT("OtherPCRendering"), optVal);
		break;
	// GraphicGrade
	case EGsOptionGraphic::GRAPHIC_GRADE:
		SaveGraphicSetting(TEXT("GraphicGrade"), optVal);
		break;
	// WZ Additional
	case EGsOptionGraphic::SSAO:
		optVal = FMath::Clamp(optVal, 0, MAX_OPT_SSAO_LEVELS - 1);
		SaveGraphicSetting(TEXT("SSAO"), optVal);
		break;
	case EGsOptionGraphic::UISCALE:
		SaveGraphicSetting(TEXT("UIScale"), optVal);
		break;
	case EGsOptionGraphic::PARTICLE_QUALITY:
		SaveGraphicSetting(TEXT("ParticleQuality"), optVal);
		break;
	case EGsOptionGraphic::VIEW_DISTANCE:
		SaveGraphicSetting(TEXT("UIViewDistanceQuality"), optVal);
		break;		
	case EGsOptionGraphic::POST_PROCESS:
		SaveGraphicSetting(TEXT("UIPostProcessQuality"), optVal);
		break;
	case EGsOptionGraphic::SHADOW:
		SaveGraphicSetting(TEXT("UIShadowQuality"), optVal);
		break;
	case EGsOptionGraphic::WINDOW_RESOLUTION:
		break;
	case EGsOptionGraphic::WINDOW_RESOLUTION_X:
	{
#if WITH_EDITOR || PLATFORM_WINDOWS
		SaveGraphicSetting(TEXT("gsWindowResolutionX"), optVal);
#endif
	}
		break;
	case EGsOptionGraphic::WINDOW_RESOLUTION_Y:
	{
#if WITH_EDITOR || PLATFORM_WINDOWS		
		SaveGraphicSetting(TEXT("gsWindowResolutionY"), optVal);
#endif
	}
		break;
	case EGsOptionGraphic::WINDOW_SCREENMODE:
//#if WITH_EDITOR || PLATFORM_WINDOWS
//		SaveGraphicSetting(TEXT("WindowScreenMode"), optVal);
//#endif
		break;
	case EGsOptionGraphic::WINDOW_VSYNCENABLED:
#if WITH_EDITOR || PLATFORM_WINDOWS
		SaveGraphicSetting(TEXT("WindowVSyncEnabled"), optVal);
#endif
		break;
	case EGsOptionGraphic::WINDOW_ANTIALIASING:
#if WITH_EDITOR || PLATFORM_WINDOWS
		SaveGraphicSetting(TEXT("WindowAntiAliasing"), optVal);
#endif
		break;
	case EGsOptionGraphic::FSR_1_ONOFF:
#if PLATFORM_ANDROID || PLATFORM_IOS
		SaveGraphicSetting(TEXT("FSR"), optVal);
#endif
		break;
	default:
		break;
	}
}

const FGsSchemaOptionConfig* UGsGameUserSettings::GetOptionConfig()
{
	if (nullptr != _optionConfig)
	{
		return _optionConfig;
	}

	// 1개 열밖에 없음.
	const FGsSchemaOptionConfig* row = nullptr;
	if (const UGsTable* table = FGsSchemaOptionConfig::GetStaticTable())
	{
		if (table->FindRow(TEXT("0"), row))
		{
			return _optionConfig = row;
		}
	}

#if WITH_EDITOR
	GSLOG(Error, TEXT("[Option] Fail to get FGsSchemaLOptionConfig."));
#endif
	return nullptr;
}


#if WITH_EDITOR || PLATFORM_WINDOWS
void UGsGameUserSettings::SetWindowResolution(int32 optVal)
{
	FIntPoint currScreenResolution = GetScreenResolution();	

	TArray<FIntPoint> outResolutions;
	GetResolutions(outResolutions);

	int currIndex = outResolutions.Find(currScreenResolution);
	bool isManual = (currIndex == -1) ? true : false;

	TPair<FName, FSoftObjectPath> pathList = GUI()->GetPathPopupResolutionList();
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidgetByPath(pathList.Key, pathList.Value);
	if (widget.IsValid())
	{
		UGsUIPopupResolutionList* popup = Cast<UGsUIPopupResolutionList>(widget.Get());
		if (popup)
		{
			TArray<FText> resolutionTextList;
			for (FIntPoint& point : outResolutions)
			{
				resolutionTextList.Add(FText::FromString(FString::Printf(TEXT("%dx%d"), point.X, point.Y)));
			}

			// 지원 해상도 중 현재 항목이 지금 없다면 수동조작으로 변경한거
			if (isManual)
			{
				FText findText;
				FText::FindText(TEXT("OptionText"), TEXT("CustomValue"), findText);
				resolutionTextList.Add(findText);
				currIndex = resolutionTextList.Num() - 1;
			}

			popup->SetData(resolutionTextList, currIndex, [currIndex, outResolutions, this](int index) {
				if (outResolutions.IsValidIndex(index))
					{
						auto size = outResolutions[index];

						GSLOG(Log, TEXT("UGsGameUserSettings::SetWindowResolution ResolutionX : %d, ResolutionY : %d"), size.X, size.Y);
						//RequestResolutionChange(size.X, size.Y, EWindowMode::Windowed);
						//ExecuteConsoleCommand(*FString::Printf(TEXT("r.SetRes %.3f x %.3f"), size.X, size.Y));
						SetScreenResolution(size);
						UpdateApplyResolutionSettings(false);

						OnPopupYesNoResolution(currIndex, index);
					}
				}
			);
		}
	}
}

void UGsGameUserSettings::UpdateApplyResolutionSettings(bool bCheckForCommandLineOverrides)
{
	GSLOG(Log, TEXT("===== UGameUserSettings::UpdateApplyResolutionSettings call ====="));

	FGsUIHelper::ShowBlockUI();
	FGsUIHelper::SetBlockUISetting(FText::GetEmpty(), true, 1.0f);

	/*ApplyResolutionSettings(bCheckForCommandLineOverrides);
	RequestUIUpdate();*/

	ApplySettings(bCheckForCommandLineOverrides);
}

void UGsGameUserSettings::OnPopupYesNoResolution(int In_Index, int In_SelectIndex)
{
	TPair<FName, FSoftObjectPath> pathYesNo = GUI()->GetPathPopupResolutionYesNo();

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidgetByPath(pathYesNo.Key, pathYesNo.Value);
	if (!widget.IsValid())
	{
		GSLOG(Error, TEXT("!widget.IsValid()"));
		return;
	}

	auto mode = GetFullscreenMode();
	auto resolution = GetScreenResolution();

	TArray<FIntPoint> outResolutions;
	GetResolutions(outResolutions);

	if (UGsUIPopupYesNoResolution* popup = Cast<UGsUIPopupYesNoResolution>(widget.Get()))
	{	
		FText message1;
		FText::FindText(TEXT("OptionText"), TEXT("Resolution_ChangePopUp"), message1);
		FText message2 = FText::GetEmpty();

		TFunction<void(bool)> InCallback = [mode, resolution, this](bool InIsOk)
		{
			if (true == InIsOk)
			{
				_graphicTable[EGsOptionGraphic::WINDOW_RESOLUTION_X] = resolution.X;
				_graphicTable[EGsOptionGraphic::WINDOW_RESOLUTION_Y] = resolution.Y;					

				SetGraphicSettingByType(EGsOptionGraphic::WINDOW_RESOLUTION_X, resolution.X);
				SetGraphicSettingByType(EGsOptionGraphic::WINDOW_RESOLUTION_Y, resolution.Y);

				GSLOG(Log, TEXT("UGsGameUserSettings::OnPopupYesNoResolution ok ResolutionX : %d, ResolutionY : %d"), 
					resolution.X, resolution.Y);

				ConfirmVideoMode();
			}
			else
			{
				TPair<FName, FSoftObjectPath> pathList = GUI()->GetPathPopupResolutionList();
				TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(pathList.Key);
				if (widget.IsValid())
				{
					UGsUIPopupResolutionList* uiPopupResolutionList = Cast<UGsUIPopupResolutionList>(widget.Get());
					if (uiPopupResolutionList)
					{
						uiPopupResolutionList->ApplyPreIndexButton();
					}
				}

				auto preConfirmedResolution = GetLastConfirmedScreenResolution();

				_graphicTable[EGsOptionGraphic::WINDOW_RESOLUTION_X] = preConfirmedResolution.X;
				_graphicTable[EGsOptionGraphic::WINDOW_RESOLUTION_Y] = preConfirmedResolution.Y;

				SetGraphicSettingByType(EGsOptionGraphic::WINDOW_RESOLUTION_X, preConfirmedResolution.X);
				SetGraphicSettingByType(EGsOptionGraphic::WINDOW_RESOLUTION_Y, preConfirmedResolution.Y);

				GSLOG(Log, TEXT("UGsGameUserSettings::OnPopupYesNoResolution cancel ResolutionX : %d, ResolutionY : %d"), 
					preConfirmedResolution.X, preConfirmedResolution.Y);

				RevertVideoMode();
			}

			UpdateApplyResolutionSettings(false);
			//FGsUIHelper::ShowBlockUI();
			//FGsUIHelper::SetBlockUISetting(FText::GetEmpty(), true, 1.0f);
			//ApplySettings(false);

			GMessage()->GetSystem().SendMessage(MessageSystem::WINDOW_RESOLUTION_UPDATE);
		};

		popup->SetData(message1, message2, InCallback);
	}
}

bool UGsGameUserSettings::GetMainResolutionIndex(IN FIntPoint inResolutionSize, OUT int& outIndex)
{
	FIntPoint windowResolutionMode1 = FIntPoint::ZeroValue;
	FIntPoint windowResolutionMode2 = FIntPoint::ZeroValue;
	FIntPoint windowResolutionMode3 = FIntPoint::ZeroValue;

	if (const FGsSchemaOptionConfig* config = GetOptionConfig())
	{
		windowResolutionMode1 = config->_windowResolutionMode1;
		if (windowResolutionMode1 == inResolutionSize)
		{
			outIndex = 0;
			return true;
		}

		windowResolutionMode2 = config->_windowResolutionMode2;
		if (windowResolutionMode2 == inResolutionSize)
		{
			outIndex = 1;
			return true;
		}

		windowResolutionMode3 = config->_windowResolutionMode3;
		if (windowResolutionMode3 == inResolutionSize)
		{
			outIndex = 2;
			return true;
		}
	}

	return false;
}

void UGsGameUserSettings::GetResolutions(TArray<FIntPoint>& OutResolutions)
{
	if (_graphicTable[EGsOptionGraphic::WINDOW_SCREENMODE] + 1 == static_cast<int>(EWindowMode::Windowed))
	{
		GetWindowedResolutions(OutResolutions);
	}
	else/* if (_graphicTable[EGsOptionGraphic::WINDOW_SCREENMODE] == static_cast<int>(EWindowMode::Fullscreen))*/
	{
		GetFullscreenResolutions(OutResolutions);
	}
}

void UGsGameUserSettings::OnPopupYesNoResolution(EGsOptionWindowResolution In_Type, FIntPoint curResolution, FIntPoint newResolution)
{
}

void UGsGameUserSettings::SetWindowScreenMode(int32 optVal)
{
	GSLOG(Log, TEXT("===== UGameUserSettings::UpdateApplyResolutionSettings call ====="));

	if (_graphicTable[EGsOptionGraphic::WINDOW_SCREENMODE] == optVal)
	{
		return;
	}

	GSLOG(Log, TEXT("UGsGameUserSettings::SetWindowScreenMode EWindowMode : %d"), optVal);
	GSLOG(Log, TEXT("UGsGameUserSettings::SetWindowScreenMode EWindowMode::Fullscreen : %d, ResolutionSizeY : %d"), ResolutionSizeX, ResolutionSizeY);
	GSLOG(Log, TEXT("UGsGameUserSettings::SetWindowScreenMode _graphicTable[EGsOptionGraphic::WINDOW_RESOLUTION_X] : %d, _graphicTable[EGsOptionGraphic::WINDOW_RESOLUTION_Y] : %d"),
	_graphicTable[EGsOptionGraphic::WINDOW_RESOLUTION_X], _graphicTable[EGsOptionGraphic::WINDOW_RESOLUTION_Y]);

	_graphicTable[EGsOptionGraphic::WINDOW_SCREENMODE] = optVal;
	SetGraphicSettingByType(EGsOptionGraphic::WINDOW_SCREENMODE, optVal);

	FGsUIHelper::ShowBlockUI();
	FGsUIHelper::SetBlockUISetting(FText::GetEmpty(), true, 1.0f);
	
	EWindowMode::Type selectMode = EWindowMode::Windowed;
	if (optVal + 1 == static_cast<int>(EWindowMode::Windowed))
	{
		//_graphicTable[EGsOptionGraphic::WINDOW_RESOLUTION] = (int32)EGsOptionWindowResolution::USER_SETTING;

		TArray<FIntPoint> outWindowedResolutions;
		GetWindowedResolutions(outWindowedResolutions);
		if (0 < outWindowedResolutions.Num())
		{
			auto size = outWindowedResolutions[0];
			if (ResolutionSizeX != size.X || ResolutionSizeY != size.Y)
			{
				SetGraphicSettingByType(EGsOptionGraphic::WINDOW_RESOLUTION, (int32)EGsOptionWindowResolution::USER_SETTING);
				
				_graphicTable[EGsOptionGraphic::WINDOW_RESOLUTION_X] = size.X;
				_graphicTable[EGsOptionGraphic::WINDOW_RESOLUTION_Y] = size.Y;

				SetScreenResolution(size);
				//UpdateApplyResolutionSettings(false);
			}
		}
	}
	else if (optVal + 1 == static_cast<int>(EWindowMode::Fullscreen))
	{
		selectMode = EWindowMode::WindowedFullscreen;		
	}
	else if (optVal + 1 == static_cast<int>(EWindowMode::WindowedFullscreen))
	{
		selectMode = EWindowMode::WindowedFullscreen;
	}
	
	SetFullscreenMode(selectMode);
	ConfirmVideoMode();
	//ApplySettings(false);
	UpdateApplyResolutionSettings(false);

	GMessage()->GetSystem().SendMessage(MessageSystem::WINDOW_SCREENMODE_UPDATE);
}

int UGsGameUserSettings::GetResolutionsNum()
{ 
	TArray<FIntPoint> outResolutions;
	GetResolutions(outResolutions);

	return outResolutions.Num();
}
#endif

/*
	초기화할 때 모든 Audio 옵션 적용
*/
void UGsGameUserSettings::ApplyAudioSettings()
{
	// 전체 오디오
	ApplyAudioSettingWholeVolume();
	ApplyAudioSettingWholeOn();
	// 음악
	ApplyAudioSettingMusicVolume();
	ApplyAudioSettingMusicOn();
	// 효과음
	ApplyAudioSettingEffectVolume();
	ApplyAudioSettingEffectOn();
	// 알림음
	ApplyAudioSettingNoticeVolume();
	ApplyAudioSettingNoticeOn();
	// 게임이 활성화되었을 때만 소리 재생
	ApplyAudioFocusVolumeMultipler();
}

void UGsGameUserSettings::ApplyAudioSettingWholeOn()
{
	if (UGsSoundManager* soundMgr = GSound())
	{
		if (UGsSoundMixerController* mixer = soundMgr->GetOrCreateSoundMixerController())
		{
			int32 val = _audioTable[EGsOptionAudio::WHOLE_ON];

			if (1 == val)
			{
				val = _audioTable[EGsOptionAudio::WHOLE_VOLUME];
			}

			mixer->SetVolume(EGsSoundVolumeType::Master, (0 == val) ? 0.0f : val * 0.01f);
			//mixer->SetVolume(EGsSoundVolumeType::MasterLobby, (0 == val) ? 0.0f : val * 0.01f);
		}
	}
}

void UGsGameUserSettings::ApplyAudioSettingWholeVolume()
{
	int32 isOnOff = _audioTable[EGsOptionAudio::WHOLE_ON];
	if (static_cast<bool>(isOnOff) == false)
		return;

	if (UGsSoundManager* soundMgr = GSound())
	{
		if (UGsSoundMixerController* mixer = soundMgr->GetOrCreateSoundMixerController())
		{			
			int32 val = _audioTable[EGsOptionAudio::WHOLE_VOLUME];

			mixer->SetVolume(EGsSoundVolumeType::Master, val * 0.01f);
			//mixer->SetVolume(EGsSoundVolumeType::MasterLobby, val * 0.01f);
		}
	}
}

void UGsGameUserSettings::ApplyAudioSettingMusicOn()
{
	if (UGsSoundManager* soundMgr = GSound())
	{
		if (UGsSoundMixerController* mixer = soundMgr->GetOrCreateSoundMixerController())
		{
			int32 val = _audioTable[EGsOptionAudio::MUSIC_ON];

			if (1 == val)
			{
				val = _audioTable[EGsOptionAudio::MUSIC_VOLUME];
			}

			mixer->SetVolume(EGsSoundVolumeType::Music, (0 == val) ? 0.0f : val * 0.01f);
		}
	}
}

void UGsGameUserSettings::ApplyAudioSettingMusicVolume()
{
	int32 isOnOff = _audioTable[EGsOptionAudio::WHOLE_ON];
	if (static_cast<bool>(isOnOff) == false)
		return;

	isOnOff = _audioTable[EGsOptionAudio::MUSIC_ON];
	if (static_cast<bool>(isOnOff) == false)
		return;

	if (UGsSoundManager* soundMgr = GSound())
	{
		if (UGsSoundMixerController* mixer = soundMgr->GetOrCreateSoundMixerController())
		{
			int32 val = _audioTable[EGsOptionAudio::MUSIC_VOLUME];
			mixer->SetVolume(EGsSoundVolumeType::Music, val * 0.01f);
		}
	}
}

void UGsGameUserSettings::ApplyAudioSettingEffectOn()
{
	if (UGsSoundManager* soundMgr = GSound())
	{
		if (UGsSoundMixerController* mixer = soundMgr->GetOrCreateSoundMixerController())
		{
			int32 val = _audioTable[EGsOptionAudio::EFFECT_ON];

			if (1 == val)
			{
				val = _audioTable[EGsOptionAudio::EFFECT_VOLUME];
			}

			mixer->SetVolume(EGsSoundVolumeType::Game, (0 == val) ? 0.0f : val * 0.01f);
		}
	}
}

void UGsGameUserSettings::ApplyAudioSettingEffectVolume()
{
	int32 isOnOff = _audioTable[EGsOptionAudio::WHOLE_ON];
	if (static_cast<bool>(isOnOff) == false)
		return;

	isOnOff = _audioTable[EGsOptionAudio::EFFECT_ON];
	if (static_cast<bool>(isOnOff) == false)
		return;

	if (UGsSoundManager* soundMgr = GSound())
	{
		if (UGsSoundMixerController* mixer = soundMgr->GetOrCreateSoundMixerController())
		{
			int32 val = _audioTable[EGsOptionAudio::EFFECT_VOLUME];
			mixer->SetVolume(EGsSoundVolumeType::Game, val * 0.01f);
		}
	}
}

void UGsGameUserSettings::ApplyAudioSettingNoticeOn()
{
	if (UGsSoundManager* soundMgr = GSound())
	{
		if (UGsSoundMixerController* mixer = soundMgr->GetOrCreateSoundMixerController())
		{
			int32 val = _audioTable[EGsOptionAudio::NOTICE_ON];

			if (1 == val)
			{
				val = _audioTable[EGsOptionAudio::NOTICE_VOLUME];
			}

			mixer->SetVolume(EGsSoundVolumeType::Notice, (0 == val) ? 0.0f : val * 0.01f);
		}
	}
}

void UGsGameUserSettings::ApplyAudioSettingNoticeVolume()
{
	int32 isOnOff = _audioTable[EGsOptionAudio::WHOLE_ON];
	if (static_cast<bool>(isOnOff) == false)
		return;

	isOnOff = _audioTable[EGsOptionAudio::NOTICE_ON];
	if (static_cast<bool>(isOnOff) == false)
		return;

	if (UGsSoundManager* soundMgr = GSound())
	{
		if (UGsSoundMixerController* mixer = soundMgr->GetOrCreateSoundMixerController())
		{
			int32 val = _audioTable[EGsOptionAudio::NOTICE_VOLUME];
			mixer->SetVolume(EGsSoundVolumeType::Notice, val * 0.01f);
		}
	}
}

void UGsGameUserSettings::ApplyAudioFocusVolumeMultipler()
{
	int32 isOnOff = _audioTable[EGsOptionAudio::FOCUS_VOLUME_MULTIPLER];
	FApp::SetUnfocusedVolumeMultiplier(isOnOff == 0 ? 1 : 0);
}

void UGsGameUserSettings::SetAudioSettingByType(EGsOptionAudio optType, int32 optVal)
{
	if (optType >= EGsOptionAudio::MAX)
		return;

	// 같은 값이면 무시
	if (_audioTable[optType] == optVal)
		return;

	// 우선 변경된 값을 적용부터
	switch (optType)
	{
	// Whole
	case EGsOptionAudio::WHOLE_ON:
		_audioTable[optType] = optVal == 0 ? 0 : 1;
		SaveAudioSetting(TEXT("WholeOn"), _audioTable[optType]);
		ApplyAudioSettingWholeOn();
		break;
	case EGsOptionAudio::WHOLE_VOLUME:
		_audioTable[optType] = FMath::Clamp(optVal, 0, MAX_OPT_AUDIO_VOLUME);
		SaveAudioSetting(TEXT("WholeVolume"), _audioTable[optType]);
		ApplyAudioSettingWholeVolume();
		break;
	// Music
	case EGsOptionAudio::MUSIC_ON:
		_audioTable[optType] = optVal == 0 ? 0 : 1;
		SaveAudioSetting(TEXT("MusicOn"), _audioTable[optType]);
		ApplyAudioSettingMusicOn();
		break;
	case EGsOptionAudio::MUSIC_VOLUME:
		_audioTable[optType] = FMath::Clamp(optVal, 0, MAX_OPT_AUDIO_VOLUME);
		SaveAudioSetting(TEXT("MusicVolume"), _audioTable[optType]);
		ApplyAudioSettingMusicVolume();
		break;
	// Effect
	case EGsOptionAudio::EFFECT_ON:
		_audioTable[optType] = optVal == 0 ? 0 : 1;
		SaveAudioSetting(TEXT("EffectOn"), _audioTable[optType]);
		ApplyAudioSettingEffectOn();
		break;
	case EGsOptionAudio::EFFECT_VOLUME:
		_audioTable[optType] = FMath::Clamp(optVal, 0, MAX_OPT_AUDIO_VOLUME);
		SaveAudioSetting(TEXT("EffectVolume"), _audioTable[optType]);
		ApplyAudioSettingEffectVolume();
		break;
	// notice
	case EGsOptionAudio::NOTICE_ON:
		_audioTable[optType] = optVal == 0 ? 0 : 1;
		SaveAudioSetting(TEXT("NoticeOn"), _audioTable[optType]);
		ApplyAudioSettingNoticeOn();
		break;
	case EGsOptionAudio::NOTICE_VOLUME:
		_audioTable[optType] = FMath::Clamp(optVal, 0, MAX_OPT_AUDIO_VOLUME);
		SaveAudioSetting(TEXT("NoticeVolume"), _audioTable[optType]);
		ApplyAudioSettingNoticeVolume();
		break;
	case EGsOptionAudio::FOCUS_VOLUME_MULTIPLER:
		_audioTable[optType] = optVal == 0 ? 0 : 1;
		SaveAudioSetting(TEXT("FocusVolumeMultipler"), _audioTable[optType]);
		ApplyAudioFocusVolumeMultipler();
		break;
	}
}

int32 UGsGameUserSettings::GetAudioSettingByType(EGsOptionAudio optType)
{
	if (_audioTable.Contains(optType))
	{
		return _audioTable[optType];
	}

	return 0;
}

bool UGsGameUserSettings::SetGraphicGrade(EGsOptionGraphic optType, int32 optVal)
{
	if (_graphicTable[optType] == optVal)
	{
		return false;
	}

	_graphicTable[optType] = optVal;
	return true;
}

bool UGsGameUserSettings::SetGraphicCustom(EGsOptionGraphic optType, int32 optVal)
{	
	TMap<EGsOptionGraphic, int32> graphicTable;
	GetGraphicSettings(graphicTable);

	if (graphicTable[optType] == optVal)
	{
		return false;
	}

	for (auto& iter : graphicTable)
	{
		EGsOptionGraphic type = iter.Key;
		if (_graphicTable.Contains(type))
		{
			_graphicTable[type] = iter.Value;
		}
	}	
	_graphicTable[EGsOptionGraphic::GRAPHIC_GRADE] = (int32)EGsOptionGraphicGrade::CUSTOM;
	_graphicTable[optType] = optVal;

	return true;
}

bool UGsGameUserSettings::SetAlert(EGsOptionAlert optType, int32 optVal, bool inIsAllChange)
{
	//모두알림 on
	//하위 모든 알림을 켠다.
	if (EGsOptionAlert::ALL_ALERT == optType)
	{
		if (inIsAllChange)
		{			
			for (int i = 0; i < _alertTable.Num(); ++i)
			{
				_alertTable[static_cast<EGsOptionAlert>(i)] = optVal;
			}
		}
		else
		{
			_alertTable[optType] = optVal;
		}
	}
	else
	{
		_alertTable[optType] = optVal;
	}

	return true;
}

void UGsGameUserSettings::SetUserInfoSettingByType(EGsOptionUserInfo optType, FString optVal, bool bImmediate)
{
	if (optType >= EGsOptionUserInfo::MAX)
		return;

	GSLOG(Log, TEXT("UGsGameUserSettings::SetUserInfoSettingByType Type : %d, value : %s"), optType, *optVal);

	// 같은 값이면 무시
	if (_userInfoTable[optType] == optVal)
		return;

	// 우선 변경된 값을 적용부터
	switch (optType)
	{
	case EGsOptionUserInfo::VID_NAME:
		_userInfoTable[optType] = optVal;
		SaveUserInfoSetting(TEXT("VidName"), _userInfoTable[optType]);
		break;
	case EGsOptionUserInfo::AUTO_LOGIN:
		_userInfoTable[optType] = optVal;
		SaveUserInfoSetting(TEXT("AutoLogin"), _userInfoTable[optType]);
		break;
	case EGsOptionUserInfo::LAST_ENTER_WORLD_NAME:
		_userInfoTable[optType] = optVal;
		SaveUserInfoSetting(TEXT("ServerName"), _userInfoTable[optType]);
		break;
	case EGsOptionUserInfo::BACKTO_LOBBY:
		_userInfoTable[optType] = TEXT("0");
		SaveUserInfoSetting(TEXT("BackToLobby"), _userInfoTable[optType]);
		break;
	case EGsOptionUserInfo::LAST_ENTER_CHARACTER:
		_userInfoTable[optType] = optVal;
		SaveUserInfoSetting(TEXT("LastEnterCharacter"), _userInfoTable[optType]);
		break;
	case EGsOptionUserInfo::LOGIN_ID:
		_userInfoTable[optType] = optVal;
		SaveUserInfoSetting(TEXT("LoginID"), _userInfoTable[optType]);
		break;
	case EGsOptionUserInfo::LOGIN_PASSWORD:
		_userInfoTable[optType] = optVal;
		SaveUserInfoSetting(TEXT("LoginPassword"), _userInfoTable[optType]);
		break;
	case EGsOptionUserInfo::LANGUAGE:
		_userInfoTable[optType] = optVal;
		SaveUserInfoSetting(TEXT("Language"), _userInfoTable[optType]);
		break;
	case EGsOptionUserInfo::LANGUAGE_VOICE:
		_userInfoTable[optType] = optVal;
		SaveUserInfoSetting(TEXT("LanguageVoice"), _userInfoTable[optType]);
		break;
	default:
		break;
	}

	if (true == bImmediate)
	{
		SaveUserSettings();
	}
}

void UGsGameUserSettings::ApplyCharacterTopGraphicSettings()
{
#if WITH_EDITOR || PLATFORM_WINDOWS
	undoResolution = _graphicTable[EGsOptionGraphic::RESOLUTION];
	undoParticleQuality = _graphicTable[EGsOptionGraphic::PARTICLE_QUALITY];
	undoEffect = _graphicTable[EGsOptionGraphic::EFFECT];

	IConsoleVariable* resolutionCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
	if (ensure(resolutionCVar))
		resolutionCVar->Set(ResolutionValueArray[3], ECVF_SetByGameSetting);	
	
	GEngine->Exec(nullptr, *FString::Printf(TEXT("sg.EffectsQuality %d"), 3));

	_graphicTable.Emplace(EGsOptionGraphic::PARTICLE_QUALITY, 3);
#else
	// 크래쉬가 자주 발생한다는 요청으로 모바일에서는 해당 기능 제외(일단은 다시 살릴수도 있기에 주석처리만)
	/*undoResolution = _graphicTable[EGsOptionGraphic::RESOLUTION];
	undoParticleQuality = _graphicTable[EGsOptionGraphic::PARTICLE_QUALITY];

	IConsoleVariable* resolutionCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
	if (ensure(resolutionCVar))
		resolutionCVar->Set(ResolutionValueArray[2], ECVF_SetByGameSetting);

	_graphicTable.Emplace(EGsOptionGraphic::PARTICLE_QUALITY, 2);*/
#endif
}

void UGsGameUserSettings::UndoApplyCharacterTopGraphicSettings()
{
#if WITH_EDITOR || PLATFORM_WINDOWS
	IConsoleVariable* resolutionCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
	if (ensure(resolutionCVar))
		resolutionCVar->Set(ResolutionValueArray[undoResolution], ECVF_SetByGameSetting);	
	
	GEngine->Exec(nullptr, *FString::Printf(TEXT("sg.EffectsQuality %d"), undoEffect));

	_graphicTable.Emplace(EGsOptionGraphic::PARTICLE_QUALITY, undoParticleQuality);
#else
	/*IConsoleVariable* resolutionCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
	if (ensure(resolutionCVar))
		resolutionCVar->Set(ResolutionValueArray[undoResolution], ECVF_SetByGameSetting);	

	_graphicTable.Emplace(EGsOptionGraphic::PARTICLE_QUALITY, undoParticleQuality);*/
#endif
}

void UGsGameUserSettings::GetGraphicSettings(OUT TMap<EGsOptionGraphic, int32>& outGraphicData)
{
	GSLOG(Log, TEXT("UGsGameUserSettings GetGraphicSettings() start call"));

	EGsOptionGraphicGrade grade = static_cast<EGsOptionGraphicGrade>(_graphicTable[EGsOptionGraphic::GRAPHIC_GRADE]);

	// 해상도 비율
	int32 screenPercentage = 0;
	// 프레임수 제한
	int32 frameRateLimit = 0;
	// 다른 캐리터 표현정도
	int32 otherPCRendering = 0;
	// SSAO 단계 특수효과 - SSAO (0~2)
	int32 ssaoLevel = 0;
	int32 uiScale = 100;
	if (int32* prevUIScaleValue = _graphicTable.Find(EGsOptionGraphic::UISCALE))
	{
		uiScale = *prevUIScaleValue;
	}

	// 파티클 시그니피컨스 세팅레벨
	int32 particleQuality = 0;

	// 시야거리 품질 (0~3)
	int32 viewDistanceQuality = 0;
	// 포스트프로세스 품질 (0~3)
	int32 postProcessQuality = 0;
	// 그림자 품질 (0~3)
	int32 shadowQuality = 0;

#if WITH_EDITOR || PLATFORM_WINDOWS
	int32 antiAliasingQuality = 0;
	int32 textureQuality = 0;
	int32 effectsQuality = 0;
	int32 foliageQuality = 0;
	int32 shadingQuality = 0;
#endif

	/*
	*	1. 기존 하는 제거 중 / 상 / 최상을 아래와 같이 변경
			하(기존 중) / 중(기존 상) / 상(기존 최상) /최상(PC버전에서만 사용)/사용자지정
		2. 디폴트옵션: 모바일은 중, PC버전은 최상
	*/
	switch (grade)
	{
	case EGsOptionGraphicGrade::CUSTOM:
	{
		outGraphicData = _graphicTable;
		return;
	}
	break;
	case EGsOptionGraphicGrade::LOW:
	{		
		viewDistanceQuality = 0;
		postProcessQuality = 0;
		shadowQuality = 0;	

#if WITH_EDITOR || PLATFORM_WINDOWS
		antiAliasingQuality = 0;
		textureQuality = 0;
		effectsQuality = 0;
		foliageQuality = 0;
		shadingQuality = 0;
#endif

		screenPercentage = 0;
		frameRateLimit = 0;
		otherPCRendering = 0;
		particleQuality = 0;

//		ScalabilityQuality.ViewDistanceQuality = 0;
//		ScalabilityQuality.PostProcessQuality = 0;
//		ScalabilityQuality.ShadowQuality = 0;
//#if WITH_EDITOR || PLATFORM_WINDOWS
//		ScalabilityQuality.AntiAliasingQuality = 0;
//		ScalabilityQuality.TextureQuality = 0;
//		ScalabilityQuality.EffectsQuality = 0;
//		ScalabilityQuality.FoliageQuality = 0;
//		ScalabilityQuality.ShadingQuality = 0;
//#endif

		
	}
	break;
	case EGsOptionGraphicGrade::MID:
	{
		viewDistanceQuality = 1;
		postProcessQuality = 1;
		shadowQuality = 1;

#if WITH_EDITOR || PLATFORM_WINDOWS
		antiAliasingQuality = 1;
		textureQuality = 1;
		effectsQuality = 1;
		foliageQuality = 1;
		shadingQuality = 1;
#endif

		screenPercentage = 1;
		frameRateLimit = 1;
		otherPCRendering = 1;
		particleQuality = 1;

//		ScalabilityQuality.ViewDistanceQuality = 1;
//		ScalabilityQuality.PostProcessQuality = 1;
//		ScalabilityQuality.ShadowQuality = 1;
//#if WITH_EDITOR || PLATFORM_WINDOWS
//		ScalabilityQuality.AntiAliasingQuality = 1;
//		ScalabilityQuality.TextureQuality = 1;
//		ScalabilityQuality.EffectsQuality = 1;
//		ScalabilityQuality.FoliageQuality = 1;
//		ScalabilityQuality.ShadingQuality = 1;
//#endif
	}
	break;
	case EGsOptionGraphicGrade::HIGH:
	{
//		ScalabilityQuality.ViewDistanceQuality = 2;
//		ScalabilityQuality.PostProcessQuality = 2;
//		ScalabilityQuality.ShadowQuality = 2;
//#if WITH_EDITOR || PLATFORM_WINDOWS
//		ScalabilityQuality.AntiAliasingQuality = 2;
//		ScalabilityQuality.TextureQuality = 2;
//		ScalabilityQuality.EffectsQuality = 2;
//		ScalabilityQuality.FoliageQuality = 2;
//		ScalabilityQuality.ShadingQuality = 2;
//#endif

		viewDistanceQuality = 2;
		postProcessQuality = 2;
		shadowQuality = 2;

#if WITH_EDITOR || PLATFORM_WINDOWS
		antiAliasingQuality = 2;
		textureQuality = 2;
		effectsQuality = 2;
		foliageQuality = 2;
		shadingQuality = 2;
#endif

		screenPercentage = 2;
		frameRateLimit = 2;
		otherPCRendering = 2;
		particleQuality = 2;
	}
	break;
	case EGsOptionGraphicGrade::TOP:
	{
//		ScalabilityQuality.ViewDistanceQuality = 3;
//		ScalabilityQuality.PostProcessQuality = 3;
//		ScalabilityQuality.ShadowQuality = 3;
//#if WITH_EDITOR || PLATFORM_WINDOWS
//		ScalabilityQuality.AntiAliasingQuality = 3;
//		ScalabilityQuality.TextureQuality = 3;
//		ScalabilityQuality.EffectsQuality = 3;
//		ScalabilityQuality.FoliageQuality = 3;
//		ScalabilityQuality.ShadingQuality = 3;
//#endif
		viewDistanceQuality =3;
		postProcessQuality = 3;
		shadowQuality = 3;

#if WITH_EDITOR || PLATFORM_WINDOWS
		antiAliasingQuality = 3;
		textureQuality = 3;
		effectsQuality = 3;
		foliageQuality = 3;
		shadingQuality = 3;
#endif

		screenPercentage = 3;
		frameRateLimit = 3;
		otherPCRendering = 3;
		particleQuality = 3;
	}
	break;
	default:
	break;
	}
	
	outGraphicData.Emplace(EGsOptionGraphic::GRAPHIC_GRADE, (int32)grade);
	screenPercentage = FMath::Clamp(screenPercentage, 0, MAX_OPT_GRAPHIC_LEVELS - 1);
	outGraphicData.Emplace(EGsOptionGraphic::RESOLUTION, screenPercentage);
	frameRateLimit = FMath::Clamp(frameRateLimit, 0, MAX_OPT_GRAPHIC_LEVELS - 1);
	outGraphicData.Emplace(EGsOptionGraphic::FRAME_RATE, frameRateLimit);
	outGraphicData.Emplace(EGsOptionGraphic::OTHER_PC_RENDERING, otherPCRendering);
	outGraphicData.Emplace(EGsOptionGraphic::VIEW_DISTANCE, viewDistanceQuality);
	outGraphicData.Emplace(EGsOptionGraphic::POST_PROCESS, postProcessQuality);
	outGraphicData.Emplace(EGsOptionGraphic::SHADOW, shadowQuality);
	outGraphicData.Emplace(EGsOptionGraphic::SSAO, ssaoLevel);
	outGraphicData.Emplace(EGsOptionGraphic::UISCALE, uiScale);
	outGraphicData.Emplace(EGsOptionGraphic::PARTICLE_QUALITY, particleQuality);

#if WITH_EDITOR || PLATFORM_WINDOWS
	outGraphicData.Emplace(EGsOptionGraphic::EFFECT, effectsQuality);
	outGraphicData.Emplace(EGsOptionGraphic::TEXTURE, textureQuality);
	outGraphicData.Emplace(EGsOptionGraphic::FOLIAGE, foliageQuality);
	outGraphicData.Emplace(EGsOptionGraphic::ANTIALIASING_QUALITY, antiAliasingQuality);
	outGraphicData.Emplace(EGsOptionGraphic::SHADING_QUALITY, shadingQuality);
#endif

#if WITH_EDITOR || PLATFORM_WINDOWS
	outGraphicData.Emplace(EGsOptionGraphic::WINDOW_RESOLUTION, _graphicTable[EGsOptionGraphic::WINDOW_RESOLUTION]);
	outGraphicData.Emplace(EGsOptionGraphic::WINDOW_SCREENMODE, _graphicTable[EGsOptionGraphic::WINDOW_SCREENMODE]);
	outGraphicData.Emplace(EGsOptionGraphic::WINDOW_VSYNCENABLED, _graphicTable[EGsOptionGraphic::WINDOW_VSYNCENABLED]);
	outGraphicData.Emplace(EGsOptionGraphic::WINDOW_ANTIALIASING, _graphicTable[EGsOptionGraphic::WINDOW_ANTIALIASING]);
#endif

#if PLATFORM_ANDROID || PLATFORM_IOS
	outGraphicData.Emplace(EGsOptionGraphic::FSR_1_ONOFF, _graphicTable[EGsOptionGraphic::FSR_1_ONOFF]);

	bool FindFirstRunSetting = false;
	FindFirstRunSetting = GConfig->GetBool(INI_SECTION_USERINFO, TEXT("FirstRunSetting"), FindFirstRunSetting, GGameUserSettingsIni);
	if (FindFirstRunSetting == false)
	{
		//_graphicGrade = 0;
		//GConfig->SetInt(INI_SECTION_GRAPHIC, TEXT("GraphicGrade"), _graphicGrade, GGameUserSettingsIni);
		//_graphicTable[EGsOptionGraphic::GRAPHIC_GRADE] = (int32)EGsOptionGraphicGrade::CUSTOM;

		outGraphicData.Emplace(EGsOptionGraphic::FRAME_RATE, 0);
		outGraphicData.Emplace(EGsOptionGraphic::GRAPHIC_GRADE, 0);

		FindFirstRunSetting = true;
		GConfig->SetBool(INI_SECTION_USERINFO, TEXT("FirstRunSetting"), FindFirstRunSetting, GGameUserSettingsIni);
	}
#endif

	GSLOG(Log, TEXT("UGsGameUserSettings GetGraphicSettings() end call"));
}

void UGsGameUserSettings::GetGraphicSettingByType(EGsOptionGraphic optType, OUT int32& outOptVal)
{
	if (false == _graphicTable.Contains(optType))
	{
		return;
	}

	outOptVal = _graphicTable[optType];
}

float UGsGameUserSettings::GetNormalizedUIScaleValue()
{
	// 0~100 세팅 값을 0.6~1 사이의 값으로 보간하여 적용
	if (int32* valuePtr = _graphicTable.Find(EGsOptionGraphic::UISCALE))
	{
		return FMath::Lerp(0.6f, 1.f, (*valuePtr) * 0.01f);
	}

	return 1.f;
}

const FString UGsGameUserSettings::GetUserInfoSetting(EGsOptionUserInfo optType) const
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("===== FString UGsGameUserSettings::GetUserInfoStting in  type : %d ====="), optType);
#endif
	for (int i = 0; i < static_cast<uint8>(EGsOptionUserInfo::MAX); ++i)
	{
		if (i < _userInfoTable.Num())
		{
			FString str = _userInfoTable[(EGsOptionUserInfo)i];
			if (str.IsEmpty())
			{
			}
			else
			{
				GSLOG(Log, TEXT("UGsGameUserSettings::GetUserInfoStting type : %d, value : %s"), i, *str);
			}
		}
	}

	return *_userInfoTable[optType];
}

const int32 UGsGameUserSettings::GetCombatSetting(EGsOptionCombat optType) const
{
	// FIX: 게임 시작하자마자 뻗는 문제로 체크로직 추가
	if (false == _combatTable.Contains(optType))
	{
		return 0;
	}

	// 추가 타입 데이터 필요 시, case 추가 할것
	switch (optType)
	{
	case EGsOptionCombat::QUICKSLOT_PAGE_INDEX:
		return _combatTable[EGsOptionCombat::QUICKSLOT_PAGE_INDEX];
	case EGsOptionCombat::INVENTROY_LONGPRESS_ENABLE:
		return _combatTable[EGsOptionCombat::INVENTROY_LONGPRESS_ENABLE];
	case EGsOptionCombat::MANNER_MODE:
		return _combatTable[EGsOptionCombat::MANNER_MODE];
	case EGsOptionCombat::NONAUTO_MOVE_KEEP_AUTO:
		return _combatTable[EGsOptionCombat::NONAUTO_MOVE_KEEP_AUTO];
	case EGsOptionCombat::DAMAGE_FLOATER:
		return _combatTable[EGsOptionCombat::DAMAGE_FLOATER];
	case EGsOptionCombat::DROP_ITEM_NAME:
		return _combatTable[EGsOptionCombat::DROP_ITEM_NAME];
	case EGsOptionCombat::NPC_NAME:
		return _combatTable[EGsOptionCombat::NPC_NAME];
	case EGsOptionCombat::NPC_FUNCTION_ICON:
		return _combatTable[EGsOptionCombat::NPC_FUNCTION_ICON];
	case EGsOptionCombat::PLAYER_NAME:
		return _combatTable[EGsOptionCombat::PLAYER_NAME];
	case EGsOptionCombat::PLAYER_SCAN_EFFECT:
		return _combatTable[EGsOptionCombat::PLAYER_SCAN_EFFECT];
	case EGsOptionCombat::NPC_TALK_BALLOON:
		return _combatTable[EGsOptionCombat::NPC_TALK_BALLOON];
	case EGsOptionCombat::NPC_FUNCTION_UI_NAME:
		return _combatTable[EGsOptionCombat::NPC_FUNCTION_UI_NAME];
	case EGsOptionCombat::KEEP_TARGET_GROUND_TOUCH:
		return _combatTable[EGsOptionCombat::KEEP_TARGET_GROUND_TOUCH];
	case EGsOptionCombat::AUTOSKILL_USE_ADDTARGETBUTTON:
		return _combatTable[EGsOptionCombat::AUTOSKILL_USE_ADDTARGETBUTTON];
	case EGsOptionCombat::COMBAT_POWER_UI:
		return _combatTable[EGsOptionCombat::COMBAT_POWER_UI];
	case EGsOptionCombat::ITEM_SLOT_EXPANSION:
		return _combatTable[EGsOptionCombat::ITEM_SLOT_EXPANSION];
	default:
		break;
	}

	// FIX: 사용하는 곳에서 Enum캐스팅 등 문제될 여지가 있음. 관련 사용자들의 예외처리가 필요함.
	return 0;
	//return -1;
}

void UGsGameUserSettings::SetCombatSettingByType(EGsOptionCombat optType, int32 optVal, bool bImmediate)
{
	if (optType >= EGsOptionCombat::MAX)
		return;

	if (_combatTable[optType] == optVal)
		return;

	switch (optType)
	{
	case EGsOptionCombat::QUICKSLOT_PAGE_INDEX:  // 실제 데이터 저장 할때 case 문 추가 필요
	{
		_combatTable[optType] = optVal;
		SaveUserCombatSetting(TEXT("QuickSlotPageIndex"), optVal);
		break;
	}
	case EGsOptionCombat::INVENTROY_LONGPRESS_ENABLE:
	{
		_combatTable[optType] = optVal;
		SaveUserCombatSetting(TEXT("InventoryLongPressState"), optVal);
		break;
	}
	case EGsOptionCombat::MANNER_MODE:
	{
		_combatTable[optType] = optVal;

		GSAI()->SetMannerMode((EGsAIBattleOptionMannerMode)optVal);
		SaveUserCombatSetting(TEXT("MannerMode"), optVal);
		break;
	}
	case EGsOptionCombat::NONAUTO_MOVE_KEEP_AUTO:
	{
		_combatTable[optType] = optVal;

		bool isNonAutoMoveKeepAuto = (optVal == 0) ? false : true;
		GSAI()->SetNonAutoMoveKeepAuto(isNonAutoMoveKeepAuto);
		SaveUserCombatSetting(TEXT("NonAutoMoveKeepAuto"), optVal);
		break;
	}
	case EGsOptionCombat::DAMAGE_FLOATER:
	{
		_combatTable[optType] = optVal;
		SaveUserCombatSetting(TEXT("DamageFloater"), optVal);
		break;
	}
	case EGsOptionCombat::DROP_ITEM_NAME:
	{
		_combatTable[optType] = optVal;

		UGsGameObjectUtil::UpdateWidgetShowNameOption(EGsGameObjectType::DropObject);

		SaveUserCombatSetting(TEXT("DropItemName"), optVal);
		break;
	}
	case EGsOptionCombat::NPC_NAME:
	{
		_combatTable[optType] = optVal;

		UGsGameObjectUtil::UpdateWidgetShowNameOption(EGsGameObjectType::NonPlayer);

		SaveUserCombatSetting(TEXT("NpcName"), optVal);
		break;
	}
	case EGsOptionCombat::NPC_FUNCTION_ICON:
	{
		_combatTable[optType] = optVal;

		UGsGameObjectUtil::UpdateWidgetShowInteractionButtonOption();

		SaveUserCombatSetting(TEXT("NpcFunctionIcon"), optVal);
		break;
	}
	case EGsOptionCombat::PLAYER_NAME:
	{
		_combatTable[optType] = optVal;

		UGsGameObjectUtil::UpdateWidgetShowNameOption(EGsGameObjectType::Player);

		SaveUserCombatSetting(TEXT("PlayerName"), optVal);
		break;
	}
	case EGsOptionCombat::PLAYER_SCAN_EFFECT:
	{
		_combatTable[optType] = optVal;
		SaveUserCombatSetting(TEXT("PlayerScanEffect"), optVal);
		break;
	}
	case EGsOptionCombat::NPC_TALK_BALLOON:
	{
		_combatTable[optType] = optVal;
		SaveUserCombatSetting(TEXT("NpcTalkBalloon"), optVal);
		break;
	}
	case EGsOptionCombat::NPC_FUNCTION_UI_NAME:
	{
		_combatTable[optType] = optVal;
		SaveUserCombatSetting(TEXT("NpcFunctionUIName"), optVal);
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_NPCFUNCTION);
		break;
	}
	case EGsOptionCombat::KEEP_TARGET_GROUND_TOUCH:
	{
		_combatTable[optType] = optVal;
		SaveUserCombatSetting(TEXT("KeepTargetGroundTouch"), optVal);
		break;
	}
	case EGsOptionCombat::AUTOSKILL_USE_ADDTARGETBUTTON:
	{
		_combatTable[optType] = optVal;
		SaveUserCombatSetting(TEXT("AutoSkillUseAddTargetButton"), optVal);
		break;
	}
	case EGsOptionCombat::COMBAT_POWER_UI:
	{
		_combatTable[optType] = optVal;
		SaveUserCombatSetting(TEXT("CombatPowerUI"), optVal);
		break;
	}
	case EGsOptionCombat::ITEM_SLOT_EXPANSION:
	{
		_combatTable[optType] = optVal;
		SaveUserCombatSetting(TEXT("ItemSlotExpansion"), optVal);
		break;
	}
	default:
		bImmediate = false;
		break;
	}

	if (true == bImmediate)
	{
		SaveUserSettings();
	}
}

void UGsGameUserSettings::ApplyAutoCombatRange()
{
	ApplyTargetList();
}
void UGsGameUserSettings::ApplyItemLootGrade()
{
	//
}

void UGsGameUserSettings::ApplyTargetList()
{
	if (UGsGameObjectManager* objectManager = GSGameObject())
	{
		if (UGsGameObjectBase* gameObject = objectManager->FindObject(EGsGameObjectType::LocalPlayer))
		{
			FGsTargetHandlerLocalPlayer* targetHandler = gameObject->GetCastTarget<FGsTargetHandlerLocalPlayer>();
			FGsTargetRule* targetRule = targetHandler->GetTargetRule();
			targetRule->ApplyTargetList();
		}
	}
}

const bool UGsGameUserSettings::GetChatOption(EGsOptionChat type) const
{
	if (type >= EGsOptionChat::MAX) return false;

	return _chatTable[type];
}

void UGsGameUserSettings::SetChatOption(EGsOptionChat type, bool optVal, bool isSave)
{
	static TMap< EGsOptionChat, FString> options = {
				TPair<EGsOptionChat, FString>{ EGsOptionChat::NORMAL_CHAT, TEXT("ChatEnableNormal") },
				TPair<EGsOptionChat, FString>{ EGsOptionChat::SERVER_CHAT, TEXT("ChatEnableWorld") },
				TPair<EGsOptionChat, FString>{ EGsOptionChat::PARTY_CHAT, TEXT("ChatEnableParty") },
				TPair<EGsOptionChat, FString>{ EGsOptionChat::ALLYGUILD_CHAT, TEXT("ChatEnableAllience") },
				TPair<EGsOptionChat, FString>{ EGsOptionChat::GUILD_CHAT, TEXT("ChatEnableKnights") },
				TPair<EGsOptionChat, FString>{ EGsOptionChat::WHISPER, TEXT("ChatEnableWhisper") },
				TPair<EGsOptionChat, FString>{ EGsOptionChat::SYSTEM, TEXT("ChatEnableSystem") }, 
				TPair<EGsOptionChat, FString>{ EGsOptionChat::INVADE_CHAT, TEXT("ChatEnableInvasion") },
				TPair<EGsOptionChat, FString>{ EGsOptionChat::EMOJI_POPUP, TEXT("ChatEnableEmojiPpoup") },
				TPair<EGsOptionChat, FString>{ EGsOptionChat::TALK_POPUP, TEXT("ChatEnableTalkPopup") } };

	if (type < EGsOptionChat::MAX && _chatTable[type] != optVal)
	{
		_chatTable[type] = optVal;
		SaveChatOptionSetting(*options[type], optVal);
	}

	if (isSave)		SaveUserSettings();
}

const int UGsGameUserSettings::GetGroupChatOptions(TArray<uint64>& options) const
{
	options.Append(_chatGroupTable);
	return options.Num();
}

const bool UGsGameUserSettings::GetGroupChatOption(uint64 roomId) const 
{
	return _chatGroupTable.Find(roomId) != INDEX_NONE ? false : true;
}

void UGsGameUserSettings::SetGroupChatOption(uint64 roomId, bool optVal, bool isSave)
{
	if (optVal) _chatGroupTable.Remove(roomId);
	else _chatGroupTable.Emplace(roomId);

	TArray<FString> strRooms;
	Algo::TransformIf(_chatGroupTable, strRooms, [](uint64 roomId) { return roomId > 0; }, [](uint64 roomId) { return FString::Format(TEXT("{0}"), { roomId }); });

	GConfig->SetArray(INI_SECTION_CHATOPT, TEXT("GroupChatDisabled"), strRooms, GGameUserSettingsIni);

	if (isSave)		SaveUserSettings();
}

void UGsGameUserSettings::ClearGroupChatOption()
{
	_chatGroupTable.Reset();
}

const FText& UGsGameUserSettings::GetChatMacro(int8 index) const
{
	return _chatMacro[index];
}

void UGsGameUserSettings::SetChatMacro(int8 index, const FText& text, bool isSave)
{
	_chatMacro.Emplace(index, text);

	FString key = FString::Format(TEXT("ChatMacro{0}"), { index });
	SaveChatMacro(*key, text);

	if (isSave)		SaveUserSettings(); 
}

const bool UGsGameUserSettings::GetAutoUseTear() const
{
	return _autoUseBlessRestore[EGsOptionBlessItem::USE_TEAR];
}

const bool UGsGameUserSettings::GetAuroUseStigmata() const
{
	return _autoUseBlessRestore[EGsOptionBlessItem::USE_STGMATA];
}

void UGsGameUserSettings::SetAutoUseBlessItem(bool tearOptVal, bool stigmataOptVal)
{
	_autoUseBlessRestore[EGsOptionBlessItem::USE_TEAR] = tearOptVal;
	_autoUseBlessRestore[EGsOptionBlessItem::USE_STGMATA] = stigmataOptVal;

	static TMap<EGsOptionBlessItem, FString> options = {
			TPair<EGsOptionBlessItem, FString>{ EGsOptionBlessItem::USE_TEAR, TEXT("AutoUseBlessTearItem") },
			TPair<EGsOptionBlessItem, FString>{ EGsOptionBlessItem::USE_STGMATA, TEXT("AutoUseBlessStigmataItem") }
	};

	for(auto iter : _autoUseBlessRestore)
		GConfig->SetBool(INI_SECTION_BLESSOPT, *options[iter.Key], iter.Value, GGameUserSettingsIni);

	SaveUserSettings();
}

void UGsGameUserSettings::SetOutputSetting(EGsOptionOutput In_optionType, int32 In_val, bool bImmediate)
{
	if (In_optionType >= EGsOptionOutput::MAX)
		return;

	if (_outputTable[In_optionType] == In_val)
		return;

	switch (In_optionType)
	{
	case EGsOptionOutput::SUMMON_SKIP:  // 실제 데이터 저장 할때 case 문 추가 필요
	{
		_outputTable[In_optionType] = In_val;
		SaveOutputSetting(TEXT("SummonSkip"), In_val);
		break;
	}
	case EGsOptionOutput::SUMMON_SKIP_COMPOSE:
	{
		_outputTable[In_optionType] = In_val;
		SaveOutputSetting(TEXT("SummonSkipCompose"), In_val);
		break;
	}
	case EGsOptionOutput::QUEST_SKIP:
	{
		_outputTable[In_optionType] = In_val;
		SaveOutputSetting(TEXT("QuestSkip"), In_val);
		break;
	}
	case EGsOptionOutput::IS_USE_CAMERA_SHAKE:
	{
		_outputTable[In_optionType] = In_val;
		SaveOutputSetting(TEXT("IsUseCameraShake"), In_val);
		break;
	}
	case EGsOptionOutput::QUEST_AUTO_TELEPORT:
	{
		_outputTable[In_optionType] = In_val;
		SaveOutputSetting(TEXT("QuestAutoTeleport"), In_val);
		break;
	}
	default:
		bImmediate = false;
		break;
	}

	if (true == bImmediate)
	{
		SaveUserSettings();
	}
}
const int32 UGsGameUserSettings::GetOutputSetting(EGsOptionOutput In_optionType) const
{
	// 추가 타입 데이터 필요 시, case 추가 할것
	switch (In_optionType)
	{
	case EGsOptionOutput::SUMMON_SKIP:
		return _outputTable[EGsOptionOutput::SUMMON_SKIP];
		break;
	case EGsOptionOutput::SUMMON_SKIP_COMPOSE:
		return _outputTable[EGsOptionOutput::SUMMON_SKIP_COMPOSE];
		break;
	case EGsOptionOutput::QUEST_SKIP:
		return _outputTable[EGsOptionOutput::QUEST_SKIP];
		break;
	case EGsOptionOutput::IS_USE_CAMERA_SHAKE:
		return _outputTable[EGsOptionOutput::IS_USE_CAMERA_SHAKE];
		break;
	case EGsOptionOutput::QUEST_AUTO_TELEPORT:
		return _outputTable[EGsOptionOutput::QUEST_AUTO_TELEPORT];
		break;
	}

	return -1;
}

void UGsGameUserSettings::SetAllAlertSetting(bool InValue)
{
	int32* valuePtr = _alertTable.Find(EGsOptionAlert::ALL_ALERT);
	if (nullptr == valuePtr)
	{
		return;
	}

	*valuePtr = InValue;

	SaveAlertSetting(TEXT("AllAlert"), InValue);
}

void UGsGameUserSettings::SetAllGameAlert(bool InValue)
{
	_allGameAlert = InValue;
	SaveAlertSetting(TEXT("AllGameAlert"), InValue);
}

bool UGsGameUserSettings::GetFairyLockOption(bool isLeft)
{
	return isLeft ? _composeFairy.leftLock : _composeFairy.rightLock;
}

bool UGsGameUserSettings::GetCostumeLockOption(bool isLeft)
{
	return isLeft ? _composeCostume.leftLock : _composeCostume.rightLock;
}

void UGsGameUserSettings::SetFairyLockOption(bool left, bool right)
{
	_composeFairy.leftLock = left;
	_composeFairy.rightLock = right;

	SaveCompositionSetting(TEXT("FairyLeftLock"), left);
	SaveCompositionSetting(TEXT("FairyRightLock"), right);

	SaveUserSettings();
}

void UGsGameUserSettings::SetCostumeLockOption(bool left, bool right)
{
	_composeCostume.leftLock = left;
	_composeCostume.rightLock = right;

	SaveCompositionSetting(TEXT("CostumeLeftLock"), left);
	SaveCompositionSetting(TEXT("CostumeRightLock"), right);

	SaveUserSettings();
}

void UGsGameUserSettings::SetAlertSetting(int32 InConsoleAlertValue, int32 InConsoleAlertNightValue)
{
	bool pushNight = true;
	bool pushNotice = true;
#if !WITH_EDITOR
	if (UGsHiveManager* hiveMgr = GHive())
	{
		GSLOG(Log, TEXT("UGsGameUserSettings::SetAlertSetting call"));
		if (hiveMgr->IsInit())
		{
			pushNight = hiveMgr->IsPushNight();
			pushNotice = hiveMgr->IsPushNotice();

			GSLOG(Log, TEXT("[GsOptionManager] Get Hive PushSetRemote, ConsoleAlert: %d"), pushNight);
			GSLOG(Log, TEXT("[GsOptionManager] Get Hive PushSetRemote, NightAlert: %d"), pushNotice);

			if (pushNight != static_cast<bool>(InConsoleAlertNightValue) ||
				pushNotice != static_cast<bool>(InConsoleAlertValue))
			{
				hiveMgr->PushSetRemote(static_cast<bool>(InConsoleAlertValue), static_cast<bool>(InConsoleAlertNightValue));

				GSLOG(Log, TEXT("[GsOptionManager] Set Hive PushSetRemote, ConsoleAlert: %d"), InConsoleAlertNightValue);
				GSLOG(Log, TEXT("[GsOptionManager] Set Hive PushSetRemote, NightAlert: %d"), InConsoleAlertValue);

				_alertTable[EGsOptionAlert::CONSOLE_ALERT] = InConsoleAlertValue;
				_alertTable[EGsOptionAlert::CONSOLE_ALERT_NIGHT] = InConsoleAlertNightValue;
			}
		}
	}
#else
	_alertTable[EGsOptionAlert::CONSOLE_ALERT] = InConsoleAlertValue;
	_alertTable[EGsOptionAlert::CONSOLE_ALERT_NIGHT] = InConsoleAlertNightValue;
#endif
}


const int32 UGsGameUserSettings::GetAlertSetting(EGsOptionAlert InType) const
{	
	if (const int* valuePtr = _alertTable.Find(InType))
	{
		return *valuePtr;
	}

	return 0;
}

void UGsGameUserSettings::SetSearchString(const FString& InStr, bool bIsAdd)
{	
	if (InStr.IsEmpty())
	{
		return;
	}

	if (bIsAdd)
	{
		// 있으면 이전 정보 삭제
		if (_marketSearchList.Contains(InStr))
		{
			_marketSearchList.Remove(InStr);
		}

		// 최근 내역이 앞에 올 수 있게 앞에다가 추가함
		_marketSearchList.Insert(InStr, 0);

		// 최대 20개만 저장. 넘어가면 뒤부터 삭제. 중요하진 않아서 하드코딩함
		if (20 < _marketSearchList.Num())
		{
			_marketSearchList.RemoveAt(_marketSearchList.Num() - 1);
		}
	}
	else
	{
		for (int32 i = 0; i < _marketSearchList.Num(); ++i)
		{
			if (InStr == _marketSearchList[i])
			{
				_marketSearchList.RemoveAt(i);
				break;
			}
		}
	}

	SaveMarketSetting();
}


const float UGsGameUserSettings::GetCameraModeBackupData(EGsOptionCameraModeBackup In_type) const
{

	if (_cameraModeBackup.Contains(In_type))
	{
		return _cameraModeBackup[In_type];
	}

	return -1;
}

const int32 UGsGameUserSettings::GetScanBackupData(EGsOptionScan In_type)const
{
	if (_scanTable.Contains(In_type))
	{
		return _scanTable[In_type];
	}

	return -1;
}

void UGsGameUserSettings::SetSaveBatterySetting(EGsOptionSaveBattery InType, int32 InVal, bool bImmediate /*= true*/)
{
	if (EGsOptionSaveBattery::MAX <= InType)
		return;

	int32* valuePtr = _saveBatteryTable.Find(InType);
	if (nullptr == valuePtr)
		return;

	*valuePtr = InVal;

	switch (InType)
	{
	case EGsOptionSaveBattery::SAVEBATTERY_TIMER_MINUTE:
		SaveSaveBatterySetting(TEXT("SavebatteryTimerMinute"), InVal);
		break;
	case EGsOptionSaveBattery::IS_PVP_AUTO_OFF:
		SaveSaveBatterySetting(TEXT("IsPvpAutoOff"), InVal);
		break;
	default:
		bImmediate = false;
		break;
	}

	if (bImmediate)
	{
		SaveUserSettings();
	}
}

const int32 UGsGameUserSettings::GetSaveBatterySetting(EGsOptionSaveBattery InType) const
{
	if (_saveBatteryTable.Contains(InType))
	{
		return _saveBatteryTable[InType];
	}

	return -1;
}
void UGsGameUserSettings::SetAutoSetting(EGsOptionAuto In_optionType, float In_val, bool bImmediate)
{
	if (In_optionType >= EGsOptionAuto::MAX)
		return;

	if (_autoTable[In_optionType] == In_val)
		return;

	switch (In_optionType)
	{
	case EGsOptionAuto::AUTO_SKILL_MANA_LIMIT_RATE: 
	{
		_autoTable[In_optionType] = In_val;
		SaveAutoSetting(TEXT("AutoSkillManaLimitRate"), In_val);
		break;
	}
	case EGsOptionAuto::IS_ALWAYS_USE_AUTO_BUFF_SKILL:
	{
		_autoTable[In_optionType] = In_val;
		SaveAutoSetting(TEXT("IsAlwasysUseAutoBuffSkill"), In_val);
		break;
	}
	case EGsOptionAuto::IS_AUTO_PARTY_ACCEPT:
	{
		_autoTable[In_optionType] = In_val;
		SaveAutoSetting(TEXT("IsAutoPartyAccept"), In_val);
		break;
	}
	case EGsOptionAuto::AUTO_ITEM_LOOT_OTHER_USERS:
	{
		_autoTable[In_optionType] = In_val;
		SaveAutoSetting(TEXT("AutoItemLootOtherUsers"), In_val);
		break;
	}
	default:
		bImmediate = false;
		break;
	}

	if (true == bImmediate)
	{
		SaveUserSettings();
	}
}
const float UGsGameUserSettings::GetAutoSetting(EGsOptionAuto In_optionType)const
{
	// 추가 타입 데이터 필요 시, case 추가 할것
	switch (In_optionType)
	{
	case EGsOptionAuto::AUTO_SKILL_MANA_LIMIT_RATE:
		return _autoTable[EGsOptionAuto::AUTO_SKILL_MANA_LIMIT_RATE];
		break;
	case EGsOptionAuto::IS_ALWAYS_USE_AUTO_BUFF_SKILL:
		return _autoTable[EGsOptionAuto::IS_ALWAYS_USE_AUTO_BUFF_SKILL];
		break;
	case EGsOptionAuto::IS_AUTO_PARTY_ACCEPT:
		return _autoTable[EGsOptionAuto::IS_AUTO_PARTY_ACCEPT];
		break;
	case EGsOptionAuto::AUTO_ITEM_LOOT_OTHER_USERS:
		return _autoTable[EGsOptionAuto::AUTO_ITEM_LOOT_OTHER_USERS];
		break;
	}

	return -1;
}

void UGsGameUserSettings::SetInputSetting(EGsOptionInput In_optionType, int32 In_val, bool bImmediate)
{
	if (In_optionType >= EGsOptionInput::MAX)
		return;

	if (_inputTable[In_optionType] == In_val)
		return;

	switch (In_optionType)
	{
	case EGsOptionInput::IS_USE_TOUCH_MOVE:
	{
		_inputTable[In_optionType] = In_val;
		SaveInputSetting(TEXT("IsUseTouchMove"), In_val);
		break;
	}
	default:
		bImmediate = false;
		break;
	}

	if (true == bImmediate)
	{
		SaveUserSettings();
	}
}

const int32 UGsGameUserSettings::GetInputSetting(EGsOptionInput In_optionType)const
{
	// 추가 타입 데이터 필요 시, case 추가 할것
	switch (In_optionType)
	{
	case EGsOptionInput::IS_USE_TOUCH_MOVE:
		return _inputTable[EGsOptionInput::IS_USE_TOUCH_MOVE];
	}

	return -1;
}

void UGsGameUserSettings::SetHudSetting(EGsOptionHud In_optionType, int32 In_val, bool bImmediate /*= true*/)
{
	if (In_optionType >= EGsOptionHud::MAX)
		return;

	if (_hudTable[In_optionType] == In_val)
		return;

	switch (In_optionType)
	{
	case EGsOptionHud::IS_SHOW_SERVER_REVISION_AND_LOCATION:
	{
		_hudTable[In_optionType] = In_val;
		SaveHudSetting(TEXT("IsShowServerRevisionAndLocation"), In_val);
		break;
	}
	default:
		bImmediate = false;
		break;
	}

	if (true == bImmediate)
	{
		SaveUserSettings();
	}
}

const int32 UGsGameUserSettings::GetHudSetting(EGsOptionHud In_optionType) const
{
	switch (In_optionType)
	{
	case EGsOptionHud::IS_SHOW_SERVER_REVISION_AND_LOCATION:
		return _hudTable[EGsOptionHud::IS_SHOW_SERVER_REVISION_AND_LOCATION];
	}

	return -1;
}

int UGsGameUserSettings::GetAllCommunityInfo(TArray<FBannerInfo>& dataList)
{
	_communityEventRedDot.Reset();
	LoadCommunityEventRedDotSetting();

	Algo::TransformIf(_communityEventRedDot, dataList, [](auto str) {
		TArray<FString> parseArray;
		str.ParseIntoArray(parseArray, TEXT(" "));
		return 	parseArray.IsValidIndex(4);

		}, [](auto str) {
			TArray<FString> parseArray;
			str.ParseIntoArray(parseArray, TEXT(" "));

			FBannerInfo info;
			if (parseArray.IsValidIndex(0)) info.id = FCString::Atoi64(*parseArray[0]);
			if (parseArray.IsValidIndex(1)) FDateTime::Parse(parseArray[1], info.tmStart);
			if (parseArray.IsValidIndex(2)) FDateTime::Parse(parseArray[2], info.tmEnd);
			if (parseArray.IsValidIndex(3)) info.valid = parseArray[3].StartsWith(TEXT("TRUE"));
			if (parseArray.IsValidIndex(4)) info.reddot = parseArray[4].StartsWith(TEXT("TRUE"));

			return info;
		});
	return dataList.Num();
}

void UGsGameUserSettings::SetAllCommunityInfo(TArray<FBannerInfo> dataList)
{
	_communityEventRedDot.Reset();
	Algo::Transform(dataList, _communityEventRedDot, [](auto e) {
		FString parseText = FString::Format(TEXT("{0} {1} {2} {3} {4}"),
			{ e.id, e.tmStart.ToString(), e.tmEnd.ToString(), e.valid ? TEXT("TRUE") : TEXT("FALSE"), e.reddot ? TEXT("TRUE") : TEXT("FALSE") });
		return parseText;
		});

	SaveCommunityEventRedDotSetting();
	SaveUserSettings();
}

#if WITH_EDITOR || PLATFORM_WINDOWS
int UGsGameUserSettings::GetWindowScreenMode()
{
	EWindowMode::Type screenMode = GetFullscreenMode();
	int mode = static_cast<int>(screenMode) - 1;
	_graphicTable.Emplace(EGsOptionGraphic::WINDOW_SCREENMODE, mode);

	return mode;
}

int UGsGameUserSettings::GetWindowResolution()
{
	return 0;
}

void UGsGameUserSettings::OnResizeViewport(class FViewport* InViewport, uint32 InNum)
{	
}

void UGsGameUserSettings::OnGameWindowMoved(const TSharedRef<SWindow>& WindowBeingMoved)
{
}

void UGsGameUserSettings::CreateWindowMovedEvent()
{
	return;

	if (GEngine)
	{
		if (GEngine->GameViewport)
		{
			auto Window = GEngine->GameViewport->GetWindow();

			Window->SetOnWindowMoved(FOnWindowMoved::CreateUObject(this, &UGsGameUserSettings::OnGameWindowMoved));
		}
	}
}

void UGsGameUserSettings::TestGameWindowMoved()
{
	return;
	if (GEngine)
	{
		if (GEngine->GameViewport)
		{
			auto Window = GEngine->GameViewport->GetWindow();

			Window->SetOnWindowMoved(FOnWindowMoved::CreateUObject(this, &UGsGameUserSettings::OnGameWindowMoved));
		}
	}
}

void UGsGameUserSettings::TestWindowScreenMode()
{	
	/*if (_fullscreenResolutions.IsValidIndex(0))
	{
		auto size = _fullscreenResolutions[0];
		SetScreenResolution(size);
		ConfirmVideoMode();
		ApplySettings(false);

		_isResolutionDirty = true;
	}*/
}
#endif

#if PLATFORM_ANDROID || PLATFORM_IOS
void UGsGameUserSettings::SetMobileFSR(int32 inFlag)
{
	//int32 FSR = inFlag;
	static IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Mobile.FSR.Enabled"));
	if(CVar != nullptr)
		CVar->Set(inFlag);
}

bool UGsGameUserSettings::GetMobileFSR()
{
	static const auto CVarFSREnbaled = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.Mobile.FSR.Enabled"));
	bool bEnableFSR = CVarFSREnbaled && CVarFSREnbaled->GetValueOnRenderThread() == 1;

	return bEnableFSR;
}
#endif

// 다른 캐릭터 표현 정도 - OtherPCRendering : 10명, 25명, 50명, 100명
// remotepc, fairy, weapon에 적용하지만 이번 2월 빌드에는 faity만 적용(다음 스텝에 적용되면 주석해제)
void  UGsGameUserSettings::ApplyOtherPCRendering()
{
	GSLOG(Log, TEXT("Call ApplyOtherPCRendering"));

	if (GSGameObject() == nullptr) return;

	int32 otherPCRenderingNums = OtherPCRenderingValueArray[_graphicTable[EGsOptionGraphic::OTHER_PC_RENDERING]];

	TArray<UGsGameObjectBase*> remotePlayerArr = GSGameObject()->FindObjectArray(EGsGameObjectType::RemotePlayer);
	if (otherPCRenderingNums >= remotePlayerArr.Num())
	{
		for (auto Obj : remotePlayerArr)
		{
			UGsGameObjectPlayer* playerObj = Cast<UGsGameObjectPlayer>(Obj);
			if (playerObj == nullptr)
				continue;

			AGsCharacterPlayer* playerActor = Cast<AGsCharacterPlayer>(playerObj->GetCharacter());
			if (playerActor)
			{
				//playerActor->CallBlueprintApplyLodLevelDown(false);
				if(playerObj->GetFairyHandler() != nullptr)
					if(playerObj->GetFairyHandler()->GetFairyActor() != nullptr)
						playerObj->GetFairyHandler()->GetFairyActor()->CallBlueprintApplyLodLevelDown(false);

				/*if (playerActor->GetWeapon())
					playerActor->GetWeapon()->CallBlueprintApplyLodLevelDown(false);*/
			}
		}
	}
	else
	{
		UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer));
		if (localPlayer == nullptr) return;

		// 거리 정렬은 내위치 기준으로
		const FVector2D& captureVec2D = localPlayer->GetLocation2D();
		remotePlayerArr.Sort([captureVec2D](const UGsGameObjectBase& lhs, const UGsGameObjectBase& rhs) {
			return (lhs.GetLocation2D() - captureVec2D).SizeSquared() <
				(rhs.GetLocation2D() - captureVec2D).SizeSquared();
		});

		for (int i = 0; i < remotePlayerArr.Num(); ++i)
		{
			UGsGameObjectPlayer* playerObj = Cast<UGsGameObjectPlayer>(remotePlayerArr[i]);
			if (playerObj == nullptr)
				continue;

			if (i < otherPCRenderingNums)
			{
				AGsCharacterPlayer* playerActor = Cast<AGsCharacterPlayer>(playerObj->GetCharacter());
				if (playerActor)
				{
					//playerActor->CallBlueprintApplyLodLevelDown(false);
					if (playerObj->GetFairyHandler() != nullptr)
						if (playerObj->GetFairyHandler()->GetFairyActor() != nullptr)
							playerObj->GetFairyHandler()->GetFairyActor()->CallBlueprintApplyLodLevelDown(false);

					/*if (playerActor->GetWeapon())
						playerActor->GetWeapon()->CallBlueprintApplyLodLevelDown(false);*/
				}
			}
			else
			{
				AGsCharacterPlayer* playerActor = Cast<AGsCharacterPlayer>(playerObj->GetCharacter());
				if (playerActor)
				{
					//playerActor->CallBlueprintApplyLodLevelDown(true);
					if (playerObj->GetFairyHandler() != nullptr)
						if (playerObj->GetFairyHandler()->GetFairyActor() != nullptr)
							playerObj->GetFairyHandler()->GetFairyActor()->CallBlueprintApplyLodLevelDown(true);

					/*if (playerActor->GetWeapon())
						playerActor->GetWeapon()->CallBlueprintApplyLodLevelDown(true);*/
				}
			}
		}
	}
}