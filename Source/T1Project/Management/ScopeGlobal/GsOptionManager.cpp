// Fill out your copyright notice in the Description page of Project Settings.


#include "GsOptionManager.h"
#include "T1Project.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Object.h"
#include "GsTable.h"

#include "GsSchemaEnums.h"

#include "ActorEx/GsCharacterBase.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/HUD/GsHUDLocalPlayer.h"

#include "Option/GsSchemaOptionCategory.h"
#include "Option/GsOptionFunc.h"
#include "Option/GsSchemaOptionSlider.h"
#include "Option/GsSchemaOptionOnOff.h"
#include "Option/GsSchemaOptionButton.h"
#include "Option/GsSchemaOptionDivision.h"
#include "Option/GsSchemaOptionItemGroup.h"
#include "Option/GsSchemaOptionItem.h"
#include "Option/GsSchemaOptionToggleGroup.h"
#include "Option/GsSchemaOptionButtonGroup.h"
#include "Option/GsSchemaOptionCheckGroup.h"


#include "TimerManager.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageSystem.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/GsMessageContents.h"

#include "Option/GsServerOption.h"
#include "Option/GsSchemaOptionText.h"
#include "Option/GsGameUserSettings.h"
#include "Option/GsDevelopmentOption.h"
#include "Option/GsInputSettings.h"

#include "GsHiveManager.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UTIL/GsGameObjectUtil.h"
#include "GsUIManager.h"
#include "GameFramework/GameUserSettings.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UTIL/GsEnum.h"
#include "Option/GsGameOptions.h"

ENUM_RANGE_BY_FIRST_AND_LAST(OptionSystemMessage, OptionSystemMessage::MIN, OptionSystemMessage::MAX)
ENABLE_ENUM_OPERATORS(OptionSystemMessage)

void UGsOptionManager::Initialize()
{	
	_userSettings = static_cast<UGsGameUserSettings*>(GEngine->GetGameUserSettings());
	_userSettings->OnInitialize();

	if (nullptr == _inputSettings)
	{
		_inputSettings = TUniquePtr<FGsInputSettings>(new FGsInputSettings());
	}

	if (nullptr == _serverOption)
	{
		_serverOption = TUniquePtr<FGsServerOption>(new FGsServerOption());
		_serverOption->Initialize();
	}

	if (nullptr == _developmentOption)
	{
		_developmentOption = TUniquePtr<FGsDevelopmentOption>(new FGsDevelopmentOption());
		_developmentOption->Initialize(_userSettings);
	}

	_options = UGsGameOptions::GetGameUserOptions();

	// 오디오 옵션 셋팅 정보
	InitAudioDatas();
	// 그래픽 옵션 셋팅 정보
	InitGraphicDatas();
	// 유저 정보
	InitUserDatas();
	// 전투 정보
	InitCombatDatas();
	// 전투 정보
	InitCombatTargetDatas();
	// 출력 정보 초기화
	InitOutputDatas();
	// 알림 정보
	InitAlertDatas();
	// 게임 알림 정보
	InitGameAlertData();
	// 절전 모드 정보
	InitSaveBatteryDatas();
	// 오토 정보
	InitAutoDatas();
	// 인풋 정보
	InitInputDatas();

	// 개발 옵션 모드
	InitDevelopmentDatas();

	FGsMessageHolder* message = GMessage();

	_listSystemDelegate.Emplace(
		message->GetSystem().AddUObject(MessageSystem::ENTER_INGAME, this, &UGsOptionManager::OnEnterGame));
	_listSystemDelegate.Emplace(
		message->GetSystem().AddUObject(MessageSystem::EXIT_GAME, this, &UGsOptionManager::ClearAllMenuDatas));
	_listSystemDelegate.Emplace(
		message->GetSystem().AddUObject(MessageSystem::BACKTO_TITLE, this, &UGsOptionManager::ClearAllMenuDatas));
#if WITH_EDITOR || PLATFORM_WINDOWS
	_listSystemDelegate.Emplace(
		message->GetSystem().AddUObject(MessageSystem::WINDOW_RESOLUTION_UPDATE, this, &UGsOptionManager::OnWindowResolutionUpdate));
	_listSystemDelegate.Emplace(
		message->GetSystem().AddUObject(MessageSystem::WINDOW_SCREENMODE_UPDATE, this, &UGsOptionManager::OnWindowScreenModeUpdate));
#endif
}

void UGsOptionManager::Finalize()
{
	_audioOptDatas.Empty();
	_graphicOptDatas.Empty();
	_userInfoOptDatas.Empty();
	_combatOptDatas.Empty();
	_outputOptDatas.Empty();
	_alertOptDatas.Empty();
	_alertServerOptDatas.Empty();
	_alertLoginOptDatas.Empty();
	_alertNotificationOptDatas.Empty();
	_alertCombatNotificationOptDatas.Empty();
	_alertOfflineOptDatas.Empty();
	_alertPKBookOptData.Empty();
	_saveBatteryOptDatas.Empty();
	_autoOptDatas.Empty();
	_autoServerOptDatas.Empty();
	_inputOptDatas.Empty();
	_serverOptionAutoDrop.Empty();
	_serverOptionPickupItem.Empty();
	_serverOptionAutoDecompose.Empty();

	if (nullptr != _inputSettings)
	{
		_inputSettings->Finalize();
		_inputSettings = nullptr;
	}

	// 서버옵션
	if (nullptr != _serverOption)
	{
		_serverOption->Finalize();
		_serverOption = nullptr;
	}

	if (nullptr != _developmentOption)
	{
		_developmentOption->Finalize();
		_developmentOption = nullptr;
	}

	if (_listSystemDelegate.Num() != 0)
	{
		for (auto iter : _listSystemDelegate)
		{
			GMessage()->GetSystem().Remove(iter);
		}
		_listSystemDelegate.Empty();
	}

	if (nullptr != _options)
	{
		_options->Reset();
	}

	if (nullptr != _userSettings)
	{
		_userSettings->Finalize();
	}
}

void UGsOptionManager::InitializeAfterResourceDownload()
{
	if (nullptr != _inputSettings)
	{		
		_inputSettings->Initialize();
	}

	SaveAll();
}

void UGsOptionManager::OnEnterGame()
{
	GSLOG(Log, TEXT("UGsOptionManager::OnEnterGame() s"));
	ClearAllMenuDatas();

	SetServerOptionData();
}

void UGsOptionManager::ClearAllMenuDatas()
{
	_menuDatas.Empty();
}

void UGsOptionManager::Update(float inTick)
{
	// USER_SETTING_UPDATE_TICK_INTERVAL의 주기로 GameUserSetting의 Update 호출
	if (0 == (++_updateTickCount % USER_SETTING_UPDATE_TICK_INTERVAL))
	{
		_userSettings->Update();
		_updateTickCount = 0;
	}
}

void UGsOptionManager::InitAudioDatas()
{	
	_audioOptDatas.Empty();
	const TMap<EGsOptionAudio, int32>& audioTable = _userSettings->GetAudioSettings();
	for (const auto& iter : audioTable)
	{
		EGsOptionAudio audioType = iter.Key;
		int32 val = iter.Value;
		switch (audioType)
		{
		case EGsOptionAudio::WHOLE_ON:
			{
				_audioOptDatas.Emplace(EGsOptionItem::MasterOn,
										FGsOptionData<EGsOptionAudio, int32>(audioType, val,
										[this](EGsOptionAudio InOptType, int32 InVal){SaveAudio(InOptType, InVal);}));			
			}
			break;
		case EGsOptionAudio::WHOLE_VOLUME:
			{
				_audioOptDatas.Emplace(EGsOptionItem::MasterVolume,
										FGsOptionData<EGsOptionAudio, int32>(audioType, val,
										[this](EGsOptionAudio InOptType, int32 InVal){SaveAudio(InOptType, InVal);}));
			}
			break;
		case EGsOptionAudio::MUSIC_ON:
			{
				_audioOptDatas.Emplace(EGsOptionItem::BgmOn,
										FGsOptionData<EGsOptionAudio, int32>(audioType, val,
										[this](EGsOptionAudio InOptType, int32 InVal) { SaveAudio(InOptType, InVal); }));
			}
			break;
		case EGsOptionAudio::MUSIC_VOLUME:
			{
				_audioOptDatas.Emplace(EGsOptionItem::BgmVolume,
										FGsOptionData<EGsOptionAudio, int32>(audioType, val,
										[this](EGsOptionAudio InOptType, int32 InVal) { SaveAudio(InOptType, InVal); }));
			}
			break;
		case EGsOptionAudio::EFFECT_ON:
			{
				_audioOptDatas.Emplace(EGsOptionItem::EffectOn,
										FGsOptionData<EGsOptionAudio, int32>(audioType, val,
										[this](EGsOptionAudio InOptType, int32 InVal) { SaveAudio(InOptType, InVal); }));
			}
			break;
		case EGsOptionAudio::EFFECT_VOLUME:
			{
				_audioOptDatas.Emplace(EGsOptionItem::EffectVolume,
										FGsOptionData<EGsOptionAudio, int32>(audioType, val,
										[this](EGsOptionAudio InOptType, int32 InVal) { SaveAudio(InOptType, InVal); }));
			}
			break;
		case EGsOptionAudio::NOTICE_ON:
		{
			_audioOptDatas.Emplace(EGsOptionItem::AlertVolumeOn,
				FGsOptionData<EGsOptionAudio, int32>(audioType, val,
					[this](EGsOptionAudio InOptType, int32 InVal) { SaveAudio(InOptType, InVal); }));
		}
		break;
		case EGsOptionAudio::NOTICE_VOLUME:
		{
			_audioOptDatas.Emplace(EGsOptionItem::AlertVolume,
				FGsOptionData<EGsOptionAudio, int32>(audioType, val,
					[this](EGsOptionAudio InOptType, int32 InVal) { SaveAudio(InOptType, InVal); }));
		}
		break;
		case EGsOptionAudio::FOCUS_VOLUME_MULTIPLER:
		{
			_audioOptDatas.Emplace(EGsOptionItem::Focus_Volume_Multipler,
				FGsOptionData<EGsOptionAudio, int32>(audioType, val,
					[this](EGsOptionAudio InOptType, int32 InVal) { SaveAudio(InOptType, InVal); }));
		}
		break;
		default:
			break;
		}
	}
}

void UGsOptionManager::InitGraphicDatas()
{
	_graphicOptDatas.Empty();
	TMap<EGsOptionGraphic, int32> graphicTable;
	_userSettings->GetGraphicSettings(graphicTable);
	for (const auto& iter : graphicTable)
	{
		EGsOptionGraphic graphicType = iter.Key;
		int32 val = iter.Value;
		switch (graphicType)
		{
		case EGsOptionGraphic::GRAPHIC_GRADE:
			_graphicOptDatas.Emplace(EGsOptionItem::Graphic_Total, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
				[this](EGsOptionGraphic InOptType, int32 InVal) {SaveGraphicGrade(InOptType, InVal);}));
			break;
		case EGsOptionGraphic::RESOLUTION: 
			_graphicOptDatas.Emplace(EGsOptionItem::Resolution, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
				[this](EGsOptionGraphic InOptType, int32 InVal) {SaveGraphicEtc(InOptType, InVal);}));
			break;
		case EGsOptionGraphic::FRAME_RATE: 
			_graphicOptDatas.Emplace(EGsOptionItem::FrameRate, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
				[this](EGsOptionGraphic InOptType, int32 InVal) {SaveGraphicEtc(InOptType, InVal);}));
			break;
		case EGsOptionGraphic::VIEW_DISTANCE: 
			_graphicOptDatas.Emplace(EGsOptionItem::ViewDistance, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
				[this](EGsOptionGraphic InOptType, int32 InVal) {SaveGraphicEtc(InOptType, InVal);}));
			break;
		case EGsOptionGraphic::POST_PROCESS: 
			_graphicOptDatas.Emplace(EGsOptionItem::PostProcess, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
				[this](EGsOptionGraphic InOptType, int32 InVal) {SaveGraphicEtc(InOptType, InVal);}));
			break;
		case EGsOptionGraphic::SHADOW: 
			_graphicOptDatas.Emplace(EGsOptionItem::Shadow, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
				[this](EGsOptionGraphic InOptType, int32 InVal) {SaveGraphicEtc(InOptType, InVal);}));
			break;
		case EGsOptionGraphic::SSAO: 
			_graphicOptDatas.Emplace(EGsOptionItem::SSAO, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
				[this](EGsOptionGraphic InOptType, int32 InVal) {SaveGraphicEtc(InOptType, InVal);}));
			break;
		case EGsOptionGraphic::UISCALE:
			_graphicOptDatas.Emplace(EGsOptionItem::UIScale, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
				[this](EGsOptionGraphic InOptType, int32 InVal) {SaveGraphicUIScale(InOptType, InVal);}));
			break;
		case EGsOptionGraphic::PARTICLE_QUALITY:
				_graphicOptDatas.Emplace(EGsOptionItem::ParticleQuality, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
					[this](EGsOptionGraphic InOptType, int32 InVal) {SaveGraphicEtc(InOptType, InVal);}));
			break;
		case EGsOptionGraphic::WINDOW_RESOLUTION:
#if WITH_EDITOR || PLATFORM_WINDOWS
			_graphicOptDatas.Emplace(EGsOptionItem::Window_Resolution, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
				[this](EGsOptionGraphic InOptType, int32 InVal) {SaveWindowResolution(InOptType, InVal);}));
#endif
			break;
		case EGsOptionGraphic::WINDOW_SCREENMODE:
#if WITH_EDITOR || PLATFORM_WINDOWS
			_graphicOptDatas.Emplace(EGsOptionItem::Window_ScreenMode, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
				[this](EGsOptionGraphic InOptType, int32 InVal) {SaveWindowScreenMode(InOptType, InVal);}));
#endif
			break;
		case EGsOptionGraphic::WINDOW_VSYNCENABLED:
#if WITH_EDITOR || PLATFORM_WINDOWS
			_graphicOptDatas.Emplace(EGsOptionItem::Window_SetVSyncEnabled, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
				[this](EGsOptionGraphic InOptType, int32 InVal) {SaveGraphicEtc(InOptType, InVal);}));
#endif
			break;
		case EGsOptionGraphic::WINDOW_ANTIALIASING:
#if WITH_EDITOR || PLATFORM_WINDOWS
			_graphicOptDatas.Emplace(EGsOptionItem::Window_AntiAliasing, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
				[this](EGsOptionGraphic InOptType, int32 InVal) {SaveGraphicEtc(InOptType, InVal);}));
#endif
			break;
		case EGsOptionGraphic::ANTIALIASING_QUALITY:
			_graphicOptDatas.Emplace(EGsOptionItem::AntiAliasingQuality, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
				[this](EGsOptionGraphic InOptType, int32 InVal) {SaveGraphicEtc(InOptType, InVal);}));
			break;
		case EGsOptionGraphic::SHADING_QUALITY:
			_graphicOptDatas.Emplace(EGsOptionItem::ShadingQuality, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
				[this](EGsOptionGraphic InOptType, int32 InVal) {SaveGraphicEtc(InOptType, InVal);}));
			break;
		case EGsOptionGraphic::EFFECT:
#if WITH_EDITOR || PLATFORM_WINDOWS
			_graphicOptDatas.Emplace(EGsOptionItem::Effect, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
				[this](EGsOptionGraphic InOptType, int32 InVal) {SaveGraphicEtc(InOptType, InVal);}));
#endif
			break;
		case EGsOptionGraphic::TEXTURE:
#if WITH_EDITOR || PLATFORM_WINDOWS
			_graphicOptDatas.Emplace(EGsOptionItem::Texture, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
				[this](EGsOptionGraphic InOptType, int32 InVal) {SaveGraphicEtc(InOptType, InVal);}));
#endif
			break;
		case EGsOptionGraphic::FOLIAGE:
#if WITH_EDITOR || PLATFORM_WINDOWS
			_graphicOptDatas.Emplace(EGsOptionItem::Foliage, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
				[this](EGsOptionGraphic InOptType, int32 InVal) {SaveGraphicEtc(InOptType, InVal);}));
#endif
			break;
		case EGsOptionGraphic::FSR_1_ONOFF:
			_graphicOptDatas.Emplace(EGsOptionItem::Graphic_FSR, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
				[this](EGsOptionGraphic InOptType, int32 InVal) {SaveGraphicEtc(InOptType, InVal);}));
			break;
		case EGsOptionGraphic::OTHER_PC_RENDERING:
			_graphicOptDatas.Emplace(EGsOptionItem::OtherPCRendering, FGsOptionData<EGsOptionGraphic, int32>(graphicType, val,
				[this](EGsOptionGraphic InOptType, int32 InVal) {SaveGraphicEtc(InOptType, InVal); }));
			break;
		default:
			break;
		}
	}
}


void UGsOptionManager::ReLoadGraphicDatas()
{	
	UGsGameUserSettings* Setting = GGameUserSettings();
	if (nullptr == Setting)
		return;

	TMap<EGsOptionGraphic, int32> graphicTable;
	_userSettings->GetGraphicSettings(graphicTable);
	
	for (auto& iter : _graphicOptDatas)
	{
		FGsOptionData<EGsOptionGraphic, int32>& data = iter.Value;
		
		if (graphicTable.Contains(data._saveType))
		{
			int32 value = graphicTable.FindRef(data._saveType);
			data.SetValueData(value);
		}		
	}
}

void UGsOptionManager::InitUserDatas()
{
	_userInfoOptDatas.Empty();
	const TMap<EGsOptionUserInfo, FString>& userTable = _userSettings->GetUserInfoSettings();
	for (const auto& iter : userTable)
	{
		EGsOptionUserInfo userInfoType = iter.Key;
		int32 val = FCString::Atoi(*iter.Value);
			   
		switch (userInfoType)
		{
		case EGsOptionUserInfo::AUTO_LOGIN:
			_userInfoOptDatas.Emplace(EGsOptionItem::AutoLogin, FGsOptionData<EGsOptionUserInfo, int32>(userInfoType, val));
			break;
		case EGsOptionUserInfo::LANGUAGE:
			_userInfoOptDatas.Emplace(EGsOptionItem::Language, FGsOptionData<EGsOptionUserInfo, int32>(userInfoType, val));
			break;
		case EGsOptionUserInfo::LANGUAGE_VOICE:
			_userInfoOptDatas.Emplace(EGsOptionItem::LanguageVoice, FGsOptionData<EGsOptionUserInfo, int32>(userInfoType, val));
			break;
		default:
			break;
		}
	}
}    

// 전투 옵션 데이터 세팅
void UGsOptionManager::InitCombatDatas()
{
	_combatOptDatas.Empty();
	const TMap<EGsOptionCombat, int32>& combatTable = _userSettings->GetCombatSettings();
	for (const auto& iter : combatTable)
	{
		EGsOptionCombat combatType = iter.Key;
		int32 val = iter.Value;

		switch (combatType)
		{
		case EGsOptionCombat::INVENTROY_LONGPRESS_ENABLE:
			_combatOptDatas.Emplace(EGsOptionItem::InventoryLongPressEnable, FGsOptionData<EGsOptionCombat, int32>(combatType, val));
			break;
		case EGsOptionCombat::MANNER_MODE:
			_combatOptDatas.Emplace(EGsOptionItem::Battle_MannerMode, FGsOptionData<EGsOptionCombat, int32>(combatType, val));
			break;
		case EGsOptionCombat::NONAUTO_MOVE_KEEP_AUTO:
			_combatOptDatas.Emplace(EGsOptionItem::Battle_NonAutoMoveKeepAuto, FGsOptionData<EGsOptionCombat, int32>(combatType, val));
			break;
		case EGsOptionCombat::DAMAGE_FLOATER:
			_combatOptDatas.Emplace(EGsOptionItem::Battle_DamageFloater, FGsOptionData<EGsOptionCombat, int32>(combatType, val));
			break;
		case EGsOptionCombat::DROP_ITEM_NAME:
			_combatOptDatas.Emplace(EGsOptionItem::Battle_DropItemName, FGsOptionData<EGsOptionCombat, int32>(combatType, val));
			break;

		case EGsOptionCombat::NPC_NAME:
			_combatOptDatas.Emplace(EGsOptionItem::Battle_NPCName, FGsOptionData<EGsOptionCombat, int32>(combatType, val));
			break;
		case EGsOptionCombat::NPC_FUNCTION_ICON:
			_combatOptDatas.Emplace(EGsOptionItem::Battle_NPCFunctionIcon, FGsOptionData<EGsOptionCombat, int32>(combatType, val));
			break;
		case EGsOptionCombat::PLAYER_NAME:
			_combatOptDatas.Emplace(EGsOptionItem::Battle_PlayerName, FGsOptionData<EGsOptionCombat, int32>(combatType, val));
			break;
		case EGsOptionCombat::PLAYER_SCAN_EFFECT:
			_combatOptDatas.Emplace(EGsOptionItem::Battle_ScanEffect, FGsOptionData<EGsOptionCombat, int32>(combatType, val));
			break;
		case EGsOptionCombat::NPC_TALK_BALLOON:
			_combatOptDatas.Emplace(EGsOptionItem::Battle_NPCTalkBalloon, FGsOptionData<EGsOptionCombat, int32>(combatType, val));
			break;
		case EGsOptionCombat::NPC_FUNCTION_UI_NAME:
			_combatOptDatas.Emplace(EGsOptionItem::Npc_Function_UI_Name, FGsOptionData<EGsOptionCombat, int32>(combatType, val));
			break;
		case EGsOptionCombat::KEEP_TARGET_GROUND_TOUCH:
			_combatOptDatas.Emplace(EGsOptionItem::KeepTarget_GroundTouch, FGsOptionData<EGsOptionCombat, int32>(combatType, val));
			break;
		case EGsOptionCombat::AUTOSKILL_USE_ADDTARGETBUTTON:
			_combatOptDatas.Emplace(EGsOptionItem::AutoSkillUse_AddTargetButton, FGsOptionData< EGsOptionCombat, int32>(combatType, val));
			break;
		case EGsOptionCombat::COMBAT_POWER_UI:
			_combatOptDatas.Emplace(EGsOptionItem::CombatPower, FGsOptionData< EGsOptionCombat, int32>(combatType, val));
			break;
		case EGsOptionCombat::ITEM_SLOT_EXPANSION:
			_combatOptDatas.Emplace(EGsOptionItem::ItemSlotExpansion, FGsOptionData< EGsOptionCombat, int32>(combatType, val));
			break;
		}
	}
}

void UGsOptionManager::InitCombatTargetDatas()
{
	_autoCombatOptTargetDatas.Empty();
	_manualCombatOptTargetDatas.Empty();
	
	_autoCombatOptTargetDatas.Emplace(EGsOptionItem::NPC_BattleMon,
		FGsOptionData<CombatOptionType, int32>(CombatOptionType::ATTACK_ME_NPC, (int)CombatOptionType::ATTACK_ME_NPC));
	_autoCombatOptTargetDatas.Emplace(EGsOptionItem::NPC_QuestMon,
		FGsOptionData<CombatOptionType, int32>(CombatOptionType::QUEST_TARGET, (int)CombatOptionType::QUEST_TARGET));
	_autoCombatOptTargetDatas.Emplace(EGsOptionItem::NPC_AgrresiveMon,
		FGsOptionData<CombatOptionType, int32>(CombatOptionType::FIRST_NPC, (int)CombatOptionType::FIRST_NPC));

	_manualCombatOptTargetDatas.Emplace(EGsOptionItem::PC_BattleCharacter_Manual,
		FGsOptionData<CombatOptionType, int32>(CombatOptionType::ATTACKED_ME_NPC, (int)CombatOptionType::ATTACKED_ME_NPC));
	_manualCombatOptTargetDatas.Emplace(EGsOptionItem::PC_Enermy_manual,
		FGsOptionData<CombatOptionType, int32>(CombatOptionType::ENEMY_ME, (int)CombatOptionType::ENEMY_ME));
	_manualCombatOptTargetDatas.Emplace(EGsOptionItem::NPC_BattleMon_Manual,
		FGsOptionData<CombatOptionType, int32>(CombatOptionType::ATTACK_ME_NPC, (int)CombatOptionType::ATTACK_ME_NPC));
	_manualCombatOptTargetDatas.Emplace(EGsOptionItem::NPC_QuestMon_Manual,
		FGsOptionData<CombatOptionType, int32>(CombatOptionType::QUEST_TARGET, (int)CombatOptionType::QUEST_TARGET));
	_manualCombatOptTargetDatas.Emplace(EGsOptionItem::NPC_AgrresiveMon_Manual,
		FGsOptionData<CombatOptionType, int32>(CombatOptionType::FIRST_NPC, (int)CombatOptionType::FIRST_NPC));
}
// 출력 초기화
void UGsOptionManager::InitOutputDatas()
{
	_outputOptDatas.Empty();
	const TMap<EGsOptionOutput, int32>& outputTable = _userSettings->GetOutputSettings();
	for (const auto& iter : outputTable)
	{
		EGsOptionOutput outputType = iter.Key;
		int32 val = iter.Value;

		switch (outputType)
		{
		case EGsOptionOutput::SUMMON_SKIP:
			_outputOptDatas.Emplace(EGsOptionItem::SummonSkip, FGsOptionData<EGsOptionOutput, int32>(outputType, val));
			break;
		case EGsOptionOutput::SUMMON_SKIP_COMPOSE:
			_outputOptDatas.Emplace(EGsOptionItem::SummonSkipCompose, FGsOptionData<EGsOptionOutput, int32>(outputType, val));
			break;
		case EGsOptionOutput::QUEST_SKIP:
			_outputOptDatas.Emplace(EGsOptionItem::Quest_Skip, FGsOptionData<EGsOptionOutput, int32>(outputType, val));
			break;
		case EGsOptionOutput::IS_USE_CAMERA_SHAKE:
			_outputOptDatas.Emplace(EGsOptionItem::Camera_Shake, FGsOptionData<EGsOptionOutput, int32>(outputType, val));
			break;
		case EGsOptionOutput::WINDOW_KEY_MAPPTIN:
#if WITH_EDITOR || PLATFORM_WINDOWS
			_outputOptDatas.Emplace(EGsOptionItem::Window_Key_Mapping, FGsOptionData<EGsOptionOutput, int32>(EGsOptionOutput::WINDOW_KEY_MAPPTIN, 0,
				[this](EGsOptionOutput InOptType, int32 InVal) {OnKeyMapping(EGsOptionItem::Window_Key_Mapping, 0);}));
#endif
			break;
		case EGsOptionOutput::QUEST_AUTO_TELEPORT:
			_outputOptDatas.Emplace(EGsOptionItem::OptionAutoQuestTeleport, FGsOptionData<EGsOptionOutput, int32>(outputType, val));
			break;
		}
	}
}

void UGsOptionManager::InitAlertDatas()
{
	_alertOptDatas.Empty();
	const TMap<EGsOptionAlert, int32>& alertTable = _userSettings->GetAlertSettings();
	for (const TPair<EGsOptionAlert, int32>& iter : alertTable)
	{
		EGsOptionAlert alertType = iter.Key;
		int32 val = iter.Value;
		switch (iter.Key)
		{
		case EGsOptionAlert::ALL_ALERT:
			_alertOptDatas.Emplace(EGsOptionItem::AllAlert, FGsOptionData<EGsOptionAlert, int32>(alertType, val,
				[this](EGsOptionAlert InOptType, int32 InVal) {SaveAlert(EGsOptionItem::AllAlert, InVal);}));
			break;
		case EGsOptionAlert::CONSOLE_ALERT:
			_alertOptDatas.Emplace(EGsOptionItem::ConsoleAlert, FGsOptionData<EGsOptionAlert, int32>(alertType, val,
				[this](EGsOptionAlert InOptType, int32 InVal) {SaveAlert(EGsOptionItem::ConsoleAlert, InVal);}));
			break;
		case EGsOptionAlert::CONSOLE_ALERT_NIGHT:
			_alertOptDatas.Emplace(EGsOptionItem::ConsoleAlert_Night, FGsOptionData<EGsOptionAlert, int32>(alertType, val,
				[this](EGsOptionAlert InOptType, int32 InVal) {SaveAlert(EGsOptionItem::ConsoleAlert_Night, InVal);}));
			break;
		case EGsOptionAlert::MAX:
			break;
		default:
			break;
		}
	}
}

void UGsOptionManager::InitServerAlertDatas()
{
	_alertServerOptDatas.Empty();
	FGsServerOption* serverOption = _serverOption.Get();
	if (nullptr != serverOption)
	{
		int32 val = static_cast<int32>(serverOption->GetHiddenNameGetItem());
		_alertServerOptDatas.Emplace(EGsOptionItem::ItemGainGlobalMessgeNameHide, FGsOptionData<EGsOptionAlert, int32>(EGsOptionAlert::ITEMGAIN_GLOBAL_MESSGE_NAME_HIDE, val,
			[this](EGsOptionAlert InOptType, int32 InVal) {SaveHiddenNameGetItem(EGsOptionItem::ItemGainGlobalMessgeNameHide, InVal);}));

	}
}

void UGsOptionManager::InitGameAlertData()
{
	_isAllGameAlert = _userSettings->GetAllGameAlert();
}

void UGsOptionManager::InitAlertLoginDatas()
{
	_alertLoginOptDatas.Empty();

	FGsServerOption* serverOption = _serverOption.Get();
	if (nullptr != serverOption)
	{
		const LoginAlram& loginAlram = serverOption->GetOptionLoginAlert();

		_alertLoginOptDatas.Emplace(EGsOptionItem::ConnectAlert,
			FGsOptionData<EGsOptionLoginAlert, int32>(EGsOptionLoginAlert::CONNECT_ALERT, loginAlram.mLoginAlramOn,
				[this](EGsOptionLoginAlert InOptType, int32 InVal) {SaveLoginAlert(EGsOptionItem::ConnectAlert, InVal);}));

		_alertLoginOptDatas.Emplace(EGsOptionItem::FriendAlert,
			FGsOptionData<EGsOptionLoginAlert, int32>(EGsOptionLoginAlert::BUDDY_ALERT, loginAlram.mBuddyLoginOn,
				[this](EGsOptionLoginAlert InOptType, int32 InVal) {SaveLoginAlert(EGsOptionItem::FriendAlert, InVal);}));

		_alertLoginOptDatas.Emplace(EGsOptionItem::GuildAlert,
			FGsOptionData<EGsOptionLoginAlert, int32>(EGsOptionLoginAlert::GUILD_ALERT, loginAlram.mGuildMemberLoginOn,
				[this](EGsOptionLoginAlert InOptType, int32 InVal) {SaveLoginAlert(EGsOptionItem::GuildAlert, InVal);}));
	}
}

void UGsOptionManager::InitAlertNotificationDatas()
{
	_alertNotificationOptDatas.Empty();

	FGsServerOption* serverOption = _serverOption.Get();
	if (nullptr != serverOption)
	{
		const ClientNotification& clientNotification = serverOption->GetOptionNotificationAlert();

		_alertNotificationOptDatas.Emplace(EGsOptionItem::EquanimityAlert,
			FGsOptionData<EGsOptionNotificationAlert, float>(EGsOptionNotificationAlert::EQUANIMITY_ALERT, clientNotification.mRepeat,
				[this](EGsOptionNotificationAlert InOptType, float InVal) {SaveNotificationAlert(EGsOptionItem::EquanimityAlert, InVal);}));
		_alertNotificationOptDatas.Emplace(EGsOptionItem::EquanimityAlertIntervalTime,
			FGsOptionData<EGsOptionNotificationAlert, float>(EGsOptionNotificationAlert::EQUANIMITY_ALERT_INTERVALTIME, clientNotification.mTimePeriodms / 1000,
				[this](EGsOptionNotificationAlert InOptType, float InVal) {SaveNotificationAlert(EGsOptionItem::EquanimityAlertIntervalTime, InVal);}));
		_alertNotificationOptDatas.Emplace(EGsOptionItem::AutoPotionAlert,
			FGsOptionData<EGsOptionNotificationAlert, float>(EGsOptionNotificationAlert::AUTO_POTION_ALERT, clientNotification.mAutoPotionLowNotification.mAlertCount,
				[this](EGsOptionNotificationAlert InOptType, float InVal) {SaveNotificationAlert(EGsOptionItem::AutoPotionAlert, InVal);}));
		_alertNotificationOptDatas.Emplace(EGsOptionItem::HPPotionRingerAlert,
			FGsOptionData<EGsOptionNotificationAlert, float>(EGsOptionNotificationAlert::HP_POTION_RINGER_ALERT, clientNotification.mAutoPotionLowNotification.mSoundOn,
				[this](EGsOptionNotificationAlert InOptType, float InVal) {SaveNotificationAlert(EGsOptionItem::HPPotionRingerAlert, InVal);}));
		_alertNotificationOptDatas.Emplace(EGsOptionItem::HPPotionVibrateAlert,
			FGsOptionData<EGsOptionNotificationAlert, float>(EGsOptionNotificationAlert::HP_POTION_VIBRATE_ALERT, clientNotification.mAutoPotionLowNotification.mVibrationOn,
				[this](EGsOptionNotificationAlert InOptType, float InVal) {SaveNotificationAlert(EGsOptionItem::HPPotionVibrateAlert, InVal);}));
		_alertNotificationOptDatas.Emplace(EGsOptionItem::WeightAlert,
			FGsOptionData<EGsOptionNotificationAlert, float>(EGsOptionNotificationAlert::WEIGHT_ALERT, clientNotification.mWeightNotification.mAlertPercent * 0.01f,
				[this](EGsOptionNotificationAlert InOptType, float InVal) {SaveNotificationAlert(EGsOptionItem::WeightAlert, InVal);}));
		_alertNotificationOptDatas.Emplace(EGsOptionItem::WeightRingerAlert,
			FGsOptionData<EGsOptionNotificationAlert, float>(EGsOptionNotificationAlert::WEIGHT_RINGER_ALERT, clientNotification.mWeightNotification.mSoundOn,
				[this](EGsOptionNotificationAlert InOptType, float InVal) {SaveNotificationAlert(EGsOptionItem::WeightRingerAlert, InVal);}));
		_alertNotificationOptDatas.Emplace(EGsOptionItem::WeightVibrateAlert,
			FGsOptionData<EGsOptionNotificationAlert, float>(EGsOptionNotificationAlert::WEIGHT_VIBRATE_ALERT, clientNotification.mWeightNotification.mVibrationOn,
				[this](EGsOptionNotificationAlert InOptType, float InVal) {SaveNotificationAlert(EGsOptionItem::WeightVibrateAlert, InVal);}));
		_alertNotificationOptDatas.Emplace(EGsOptionItem::DeathAlert,
			FGsOptionData<EGsOptionNotificationAlert, float>(EGsOptionNotificationAlert::DEATH_ALERT, 0,
				[this](EGsOptionNotificationAlert InOptType, float InVal) {SaveNotificationAlert(EGsOptionItem::DeathAlert, InVal);}));
		_alertNotificationOptDatas.Emplace(EGsOptionItem::DeathRingerAlert,
			FGsOptionData<EGsOptionNotificationAlert, float>(EGsOptionNotificationAlert::DEATH_RINGER_ALERT, clientNotification.mDeadNotification.mSoundOn,
				[this](EGsOptionNotificationAlert InOptType, float InVal) {SaveNotificationAlert(EGsOptionItem::DeathRingerAlert, InVal);}));
		_alertNotificationOptDatas.Emplace(EGsOptionItem::DeathVibrateAlert,
			FGsOptionData<EGsOptionNotificationAlert, float>(EGsOptionNotificationAlert::DEATH_VIBRATE_ALERT, clientNotification.mDeadNotification.mVibrationOn,
				[this](EGsOptionNotificationAlert InOptType, float InVal) {SaveNotificationAlert(EGsOptionItem::DeathVibrateAlert, InVal);}));
	}
}

void UGsOptionManager::InitAlertCombatNotificationDatas()
{
	_alertCombatNotificationOptDatas.Empty();

	FGsServerOption* serverOption = _serverOption.Get();
	if (nullptr != serverOption)
	{
		const ClientNotification& clientNotification = serverOption->GetOptionNotificationAlert();

		_alertCombatNotificationOptDatas.Emplace(EGsOptionItem::CombatStateAlert,
			FGsOptionData<EGsOptionNotificationAlert, float>(EGsOptionNotificationAlert::COMBAT_STATE_ALERT, clientNotification.mCombatRepeat,
				[this](EGsOptionNotificationAlert InOptType, float InVal) {SaveCombatNotificationAlert(EGsOptionItem::CombatStateAlert, InVal); }));
		_alertCombatNotificationOptDatas.Emplace(EGsOptionItem::CombatStateAlertIntervalTime,
			FGsOptionData<EGsOptionNotificationAlert, float>(EGsOptionNotificationAlert::COMBAT_STATE_ALERT_INTERVALTIME, clientNotification.mCombatTimePeriodms / 1000,
				[this](EGsOptionNotificationAlert InOptType, float InVal) {SaveCombatNotificationAlert(EGsOptionItem::CombatStateAlertIntervalTime, InVal); }));
		_alertCombatNotificationOptDatas.Emplace(EGsOptionItem::PvpAttackedAlert,
			FGsOptionData<EGsOptionNotificationAlert, float>(EGsOptionNotificationAlert::PVP_ALERT, 0,
				[this](EGsOptionNotificationAlert InOptType, float InVal) {SaveCombatNotificationAlert(EGsOptionItem::PvpAttackedAlert, InVal); }));
		_alertCombatNotificationOptDatas.Emplace(EGsOptionItem::PvpAttackedRingerAlert,
			FGsOptionData<EGsOptionNotificationAlert, float>(EGsOptionNotificationAlert::PVP_RINGER_ALERT, clientNotification.mAutoKillTakeDamageNotification.mSoundOn,
				[this](EGsOptionNotificationAlert InOptType, float InVal) {SaveCombatNotificationAlert(EGsOptionItem::PvpAttackedRingerAlert, InVal); }));
		_alertCombatNotificationOptDatas.Emplace(EGsOptionItem::PvpAttackedVibrateAlert,
			FGsOptionData<EGsOptionNotificationAlert, float>(EGsOptionNotificationAlert::PVP_VIBRATE_ALERT, clientNotification.mAutoKillTakeDamageNotification.mVibrationOn,
				[this](EGsOptionNotificationAlert InOptType, float InVal) {SaveCombatNotificationAlert(EGsOptionItem::PvpAttackedVibrateAlert, InVal); }));
		_alertCombatNotificationOptDatas.Emplace(EGsOptionItem::HPRateAlert,
			FGsOptionData<EGsOptionNotificationAlert, float>(EGsOptionNotificationAlert::HP_RATE_ALERT, clientNotification.mHPDangerousNotification.mAlertPercent * 0.01f,
				[this](EGsOptionNotificationAlert InOptType, float InVal) {SaveCombatNotificationAlert(EGsOptionItem::HPRateAlert, InVal); }));
		_alertCombatNotificationOptDatas.Emplace(EGsOptionItem::HPRateRingerAlert,
			FGsOptionData<EGsOptionNotificationAlert, float>(EGsOptionNotificationAlert::HP_RATE_RINGER_ALERT, clientNotification.mHPDangerousNotification.mSoundOn,
				[this](EGsOptionNotificationAlert InOptType, float InVal) {SaveCombatNotificationAlert(EGsOptionItem::HPRateRingerAlert, InVal); }));
		_alertCombatNotificationOptDatas.Emplace(EGsOptionItem::HPRateVibrateAlert,
			FGsOptionData<EGsOptionNotificationAlert, float>(EGsOptionNotificationAlert::HP_RATE_VIBRATE_ALERT, clientNotification.mHPDangerousNotification.mVibrationOn,
				[this](EGsOptionNotificationAlert InOptType, float InVal) {SaveCombatNotificationAlert(EGsOptionItem::HPRateVibrateAlert, InVal); }));
	}
}

void UGsOptionManager::InitOfflineAlertDatas()
{
	_alertOfflineOptDatas.Empty();
	FGsServerOption* serverOption = _serverOption.Get();
	if (nullptr != serverOption)
	{
		const HivePushNotification& optionOfflineAlert = serverOption->GetOptionOfflineAlert();

		_alertOfflineOptDatas.Emplace(EGsOptionItem::OfflinePlayAlert,
			FGsOptionData<EGsOptionOfflineAlert, float>(EGsOptionOfflineAlert::OFFLINE_PLAY_ALERT, optionOfflineAlert.mRepeat,
				[this](EGsOptionOfflineAlert InOptType, float InVal) {SaveOfflineAlert(EGsOptionItem::OfflinePlayAlert, InVal);}));
		_alertOfflineOptDatas.Emplace(EGsOptionItem::OfflinePlayAlertIntervalTime,
			FGsOptionData<EGsOptionOfflineAlert, float>(EGsOptionOfflineAlert::OFFLINE_PLAY_ALERT_INTERVALTIME, optionOfflineAlert.mTimePeriodms / 1000,
			[this](EGsOptionOfflineAlert InOptType, float InVal) {SaveOfflineAlert(EGsOptionItem::OfflinePlayAlertIntervalTime, InVal);}));
		_alertOfflineOptDatas.Emplace(EGsOptionItem::OfflineInvenAlert,
			FGsOptionData<EGsOptionOfflineAlert, float>(EGsOptionOfflineAlert::OFFLINE_INVEN_ALERT, optionOfflineAlert.mFullInventory,
				[this](EGsOptionOfflineAlert InOptType, float InVal) {SaveOfflineAlert(EGsOptionItem::OfflineInvenAlert, InVal);}));
		_alertOfflineOptDatas.Emplace(EGsOptionItem::OfflineWeightAlert,
			FGsOptionData<EGsOptionOfflineAlert, float>(EGsOptionOfflineAlert::OFFLINE_WEIGHT_ALERT, optionOfflineAlert.mWeightOver,
				[this](EGsOptionOfflineAlert InOptType, float InVal) {SaveOfflineAlert(EGsOptionItem::OfflineWeightAlert, InVal);}));
		_alertOfflineOptDatas.Emplace(EGsOptionItem::OfflinePlayfinishedAlert,
			FGsOptionData<EGsOptionOfflineAlert, float>(EGsOptionOfflineAlert::OFFLINE_PLAYFINISHED_ALERT, optionOfflineAlert.mFinishOfflinePlay,
				[this](EGsOptionOfflineAlert InOptType, float InVal) {SaveOfflineAlert(EGsOptionItem::OfflinePlayfinishedAlert, InVal);}));
		_alertOfflineOptDatas.Emplace(EGsOptionItem::OfflineDeathAlert,
			FGsOptionData<EGsOptionOfflineAlert, float>(EGsOptionOfflineAlert::OFFLINE_DEATHALERT, optionOfflineAlert.mDead,
				[this](EGsOptionOfflineAlert InOptType, float InVal) {SaveOfflineAlert(EGsOptionItem::OfflineDeathAlert, InVal);}));
		_alertOfflineOptDatas.Emplace(EGsOptionItem::OfflineDungeonTimeOutAlert,
			FGsOptionData<EGsOptionOfflineAlert, float>(EGsOptionOfflineAlert::OFFLINE_DUNGEON_TIMEOUT_ALERT, optionOfflineAlert.mFinishDungeon,
				[this](EGsOptionOfflineAlert InOptType, float InVal) {SaveOfflineAlert(EGsOptionItem::OfflineDungeonTimeOutAlert, InVal);}));
		_alertOfflineOptDatas.Emplace(EGsOptionItem::OfflineAttackedAlert,
			FGsOptionData<EGsOptionOfflineAlert, float>(EGsOptionOfflineAlert::OFFLINE_ATTACKED_ALERT, optionOfflineAlert.mTakeDamageByPVP,
				[this](EGsOptionOfflineAlert InOptType, float InVal) {SaveOfflineAlert(EGsOptionItem::OfflineAttackedAlert, InVal);}));
		
		// 2023/4/28 PKT - 장착 가능한 장비 아이템 획득 알림
		float floatItemGrade = StaticCast<float>(FGsOptionFunc::ConvertAutoItemGainServerEnumToOptionEnum(StaticCast<AutoDropItemGrade>(optionOfflineAlert.mNotiItemGrade)));
		_alertOfflineOptDatas.Emplace(EGsOptionItem::OfflineEquipItemAcquisitionAlert,
			FGsOptionData<EGsOptionOfflineAlert, float>(EGsOptionOfflineAlert::OFFLINE_EQUIP_ITEM_ACQUISITION_ALERT, floatItemGrade,
				[this](EGsOptionOfflineAlert InOptType, float InVal) {SaveOfflineAlert(EGsOptionItem::OfflineEquipItemAcquisitionAlert, InVal); }));
		// 2023/4/28 PKT - 스킬북 획득 알림
		float floatSkillBookGrade = StaticCast<float>(FGsOptionFunc::ConvertAutoItemGainServerEnumToOptionEnum(StaticCast<AutoDropItemGrade>(optionOfflineAlert.mNotiSkillBookGrade)));
		_alertOfflineOptDatas.Emplace(EGsOptionItem::OfflineSkillBookAcquisitionAlert,
			FGsOptionData<EGsOptionOfflineAlert, float>(EGsOptionOfflineAlert::OFFLINE_SKILL_BOOK_ACQUISITION_ALERT, floatSkillBookGrade,
				[this](EGsOptionOfflineAlert InOptType, float InVal) {SaveOfflineAlert(EGsOptionItem::OfflineSkillBookAcquisitionAlert, InVal); }));
		// 2023/4/28 PKT - 거래소 등록 물품 판매 완료
		_alertOfflineOptDatas.Emplace(EGsOptionItem::OfflineMarketSaleCompletedAlert,
			FGsOptionData<EGsOptionOfflineAlert, float>(EGsOptionOfflineAlert::OFFLINE_MARKET_SALE_COMPLETED_ALERT, StaticCast<float>(optionOfflineAlert.mMarketItemSell),
				[this](EGsOptionOfflineAlert InOptType, float InVal) {SaveOfflineAlert(EGsOptionItem::OfflineMarketSaleCompletedAlert, InVal); }));
	}
}

void UGsOptionManager::InitPKBookAlertDatas()
{
	_alertPKBookOptData.Empty();

	FGsServerOption* serverOption = _serverOption.Get();
	if (nullptr != serverOption)
	{
		const MonitoringTargetNotification& PKBookOption = serverOption->GetPKBookOption();

		// 2022/09/30 PKT - 경계 대상 근접 알림
		_alertPKBookOptData.Emplace(EGsOptionItem::Target_Detection_Alert,
			FGsOptionData<EGsOptionPKBookAlert, int32>(EGsOptionPKBookAlert::CLOSE_TARGET_DETECTION_ALERT, 0,
				[this](EGsOptionPKBookAlert InOptType, int32 InVal) {SavePKBookAlert(EGsOptionItem::Target_Detection_Alert, InVal); }));

		// 2022/09/30 PKT - 경계 대상 근접 탐지 거리
		_alertPKBookOptData.Emplace(EGsOptionItem::Target_Detection_Alert_Distance,
			FGsOptionData<EGsOptionPKBookAlert, int32>(EGsOptionPKBookAlert::CLOSE_TARGET_DETECTION_ALERT_DISTANCE, PKBookOption.mNotificationRange,
				[this](EGsOptionPKBookAlert InOptType, int32 InVal) {SavePKBookAlert(EGsOptionItem::Target_Detection_Alert_Distance, InVal); }));

		// 2022/09/30 PKT - 경계 대상 근접 알림 주기
		_alertPKBookOptData.Emplace(EGsOptionItem::Target_Detection_Alert_Interval,
			FGsOptionData<EGsOptionPKBookAlert, int32>(EGsOptionPKBookAlert::CLOSE_TARGET_DETECTION_ALERT_INTERVAL, PKBookOption.mNotificationCycleSecond,
				[this](EGsOptionPKBookAlert InOptType, int32 InVal) {SavePKBookAlert(EGsOptionItem::Target_Detection_Alert_Interval, InVal); }));

		// 2022/09/30 PKT - 경계 대상 근접 알림 소리
		_alertPKBookOptData.Emplace(EGsOptionItem::Target_Detection_Ringer_Alert,
			FGsOptionData<EGsOptionPKBookAlert, int32>(EGsOptionPKBookAlert::CLOSE_TARGET_DETECTION_RINGER_ALERT, PKBookOption.mSoundOn,
				[this](EGsOptionPKBookAlert InOptType, int32 InVal) {SavePKBookAlert(EGsOptionItem::Target_Detection_Ringer_Alert, InVal); }));

		// 2022/09/30 PKT - 경계 대상 근접 알림 진동
		_alertPKBookOptData.Emplace(EGsOptionItem::Target_Detection_Vibrate_Alert,
			FGsOptionData<EGsOptionPKBookAlert, int32>(EGsOptionPKBookAlert::CLOSE_TARGET_DETECTION_VIBRATE_ALERT, PKBookOption.mVibrationOn,
				[this](EGsOptionPKBookAlert InOptType, int32 InVal) {SavePKBookAlert(EGsOptionItem::Target_Detection_Vibrate_Alert, InVal); }));
	}
}



void UGsOptionManager::InitSaveBatteryDatas()
{
	_saveBatteryOptDatas.Empty();
	const TMap<EGsOptionSaveBattery, int32>& saveBatteryTable = _userSettings->GetSaveBatterySettings();
	for (const TPair<EGsOptionSaveBattery, int32>& iter : saveBatteryTable)
	{
		switch (iter.Key)
		{
		case EGsOptionSaveBattery::SAVEBATTERY_TIMER_MINUTE:
			_saveBatteryOptDatas.Emplace(EGsOptionItem::SaveBattery_Start, FGsOptionData<EGsOptionSaveBattery, int32>(iter.Key, iter.Value));
			break;
		case EGsOptionSaveBattery::IS_PVP_AUTO_OFF:
			_saveBatteryOptDatas.Emplace(EGsOptionItem::SaveBattery_AutoQuit, FGsOptionData<EGsOptionSaveBattery, int32>(iter.Key, iter.Value));
			break;
		}
	}
}

void UGsOptionManager::InitAutoDatas()
{
	_autoOptDatas.Empty();
	const TMap<EGsOptionAuto, float>& autoTable = _userSettings->GetAutoSettings();
	for (const auto& iter : autoTable)
	{
		EGsOptionAuto autoType = iter.Key;
		float val = iter.Value;

		switch (autoType)
		{
		case EGsOptionAuto::AUTO_SKILL_MANA_LIMIT_RATE:
			_autoOptDatas.Emplace(EGsOptionItem::AutoSkillUse, FGsOptionData<EGsOptionAuto, float>(autoType, val));
			break;
		case EGsOptionAuto::IS_ALWAYS_USE_AUTO_BUFF_SKILL:
			_autoOptDatas.Emplace(EGsOptionItem::BuffAlwaysUse, FGsOptionData<EGsOptionAuto, float>(autoType, val));
			break;
		case EGsOptionAuto::IS_AUTO_PARTY_ACCEPT:
			_autoOptDatas.Emplace(EGsOptionItem::AutoPartyConfirm, FGsOptionData<EGsOptionAuto, float>(autoType, val));
			break;
		case EGsOptionAuto::AUTO_ITEM_LOOT_OTHER_USERS:
			_autoOptDatas.Emplace(EGsOptionItem::AnotherUserItemGain, FGsOptionData<EGsOptionAuto, float>(autoType, val));
			break;
		}
	}
}

void UGsOptionManager::InitServerAutoDatas()
{
	_autoServerOptDatas.Empty();
	FGsServerOption* serverOption = _serverOption.Get();
	if (nullptr != serverOption)
	{
		_autoServerOptDatas.Emplace(EGsOptionItem::PersonalTradingAutoReject, FGsOptionData<EGsOptionAuto, float>(EGsOptionAuto::PERSONAL_TRADING_AUTO_REJECT, serverOption->GetPersonalTradingAutoReject()));
	}
}

void UGsOptionManager::InitDevelopmentDatas()
{
	_developmentGraphicOptDatas.Empty();
	const TMap<EGsDevelopOptionGraphic, int32>& graphicOptions = _developmentOption->GetGraphicOptions();
	for (const auto& iter : graphicOptions)
	{
		EGsDevelopOptionGraphic graphicType = iter.Key;
		float val = iter.Value;

		switch (graphicType)
		{
		case EGsDevelopOptionGraphic::EFFECT:
			_developmentGraphicOptDatas.Emplace(EGsOptionItem::Effect, FGsOptionData<EGsDevelopOptionGraphic, int32>(graphicType, val));
			break;
		case EGsDevelopOptionGraphic::TEXTURE:
			_developmentGraphicOptDatas.Emplace(EGsOptionItem::Texture, FGsOptionData<EGsDevelopOptionGraphic, int32>(graphicType, val));
			break;
		case EGsDevelopOptionGraphic::FOLIAGE:
			_developmentGraphicOptDatas.Emplace(EGsOptionItem::Foliage, FGsOptionData<EGsDevelopOptionGraphic, int32>(graphicType, val));
			break;
		case EGsDevelopOptionGraphic::OUTLINE:
			_developmentGraphicOptDatas.Emplace(EGsOptionItem::Outline, FGsOptionData<EGsDevelopOptionGraphic, int32>(graphicType, val));
			break;
		case EGsDevelopOptionGraphic::DISABLE_DISTORTION:
			_developmentGraphicOptDatas.Emplace(EGsOptionItem::DisableDistortion, FGsOptionData<EGsDevelopOptionGraphic, int32>(graphicType, val));
			//[this](EGsDevelopOptionGraphic InOptType, int32 InVal) {SaveDevelopmentGraphic(InOptType, InVal);}));
			break;
		case EGsDevelopOptionGraphic::TERRAIN_NORMAL_DISTANCE:
			_developmentGraphicOptDatas.Emplace(EGsOptionItem::TerrainNormalDistance, FGsOptionData<EGsDevelopOptionGraphic, int32>(graphicType, val));
			break;			
		default:
			break;
		}
	}
}

void UGsOptionManager::InitInputDatas()
{
	_inputOptDatas.Empty();
	const TMap<EGsOptionInput, int32>& inputTable = _userSettings->GetInputSettings();
	for (const auto& iter : inputTable)
	{
		EGsOptionInput inputType = iter.Key;
		float val = iter.Value;

		switch (inputType)
		{
		case EGsOptionInput::IS_USE_TOUCH_MOVE:
			_inputOptDatas.Emplace(EGsOptionItem::TouchMove, FGsOptionData<EGsOptionInput, int32>(inputType, val));
			break;
		}
	}
}

const TArray<const FGsSchemaOptionCategory*> UGsOptionManager::GetOptionData(FString inTableName)
{
	// FGT: 접속 끊겨서 로비로 돌아왔을 때 ClearAllMenuDatas 가 안불렸는지 인게임 탭 메뉴가 나오는 이슈있어서 막음
	// jira.com2us.com/jira/browse/C2URWQ-4525
	//if (true == _menuDatas.IsValidIndex(0)) return _menuDatas;
#if WITH_EDITOR || PLATFORM_WINDOWS
	// 현재 윈도우 해상도 및 디스플레이모드
	GetWindowGraphicDatas();
#endif

#if !WITH_EDITOR
	GetHivePushDatas();
#endif

	const UGsTable* table;
	if (inTableName.IsEmpty())
	{
		table = FGsSchemaOptionCategory::GetStaticTable();		
	}
	else
	{
		table = UGsTableManager::GetInstance().GetTable(FGsSchemaOptionCategory::StaticStruct(), inTableName);
	}

	if (nullptr != table)
	{
		_menuDatas.Empty();
		table->GetAllRows<FGsSchemaOptionCategory>(_menuDatas);
	}

	return _menuDatas;
}

const FGsSchemaOptionSlider* UGsOptionManager::GetGsOptionSliderData(const FString& InRowKey)
{	
	if (const UGsTable* table = FGsSchemaOptionSlider::GetStaticTable())
	{
		const FGsSchemaOptionSlider* Row = nullptr;
		table->FindRow<FGsSchemaOptionSlider>(FName(*InRowKey), Row);
		return Row;
	}

	return nullptr;
}

const FGsSchemaOptionOnOff * UGsOptionManager::GetGsOptionOnOffData(const FString& InRowKey)
{	
	if (const UGsTable* table = FGsSchemaOptionOnOff::GetStaticTable())
	{
		const FGsSchemaOptionOnOff* Row = nullptr;
		table->FindRow<FGsSchemaOptionOnOff>(FName(*InRowKey), Row);
		return Row;
	}

	return nullptr;
}

const FGsSchemaOptionToggleGroup* UGsOptionManager::GetGsOptionOnOffGroupData(const FString& InRowKey)
{
	if (const UGsTable* table = FGsSchemaOptionToggleGroup::GetStaticTable())
	{
		const FGsSchemaOptionToggleGroup* Row = nullptr;
		table->FindRow<FGsSchemaOptionToggleGroup>(FName(*InRowKey), Row);
		return Row;
	}

	return nullptr;
}

const FGsSchemaOptionButton* UGsOptionManager::GetGsOptionButtonData(const FString& InRowKey)
{
	if (const UGsTable* table = FGsSchemaOptionButton::GetStaticTable())
	{
		const FGsSchemaOptionButton* Row = nullptr;
		table->FindRow<FGsSchemaOptionButton>(FName(*InRowKey), Row);
		return Row;
	}
	return nullptr;
}

const FGsSchemaOptionButtonGroup* UGsOptionManager::GetGsOptionButtonGroupData(const FString& InRowKey)
{
	if (const UGsTable* table = FGsSchemaOptionButtonGroup::GetStaticTable())
	{
		const FGsSchemaOptionButtonGroup* Row = nullptr;
		
#if PLATFORM_ANDROID || PLATFORM_IOS
		FString mobileItem = InRowKey + TEXT("_Mobile");
		if (table->FindRow<FGsSchemaOptionButtonGroup>(FName(*mobileItem), Row))
		{
		}
		else
		{
			table->FindRow<FGsSchemaOptionButtonGroup>(FName(*InRowKey), Row);
		}
#else
		table->FindRow<FGsSchemaOptionButtonGroup>(FName(*InRowKey), Row);
#endif	
		return Row;
	}
	return nullptr;
}

const FGsSchemaOptionText* UGsOptionManager::GetGsOptionTextData(const FString& InRowKey)
{
	if (const UGsTable* table = FGsSchemaOptionText::GetStaticTable())
	{
		const FGsSchemaOptionText* Row = nullptr;
		table->FindRow<FGsSchemaOptionText>(FName(*InRowKey), Row);
		return Row;
	}
	return nullptr;
}

const struct FGsSchemaOptionCheckGroup* UGsOptionManager::GetGsOptionCheckGroupData(const FString& InRowKey)
{
	if (const UGsTable* table = FGsSchemaOptionCheckGroup::GetStaticTable())
	{
		const FGsSchemaOptionCheckGroup* Row = nullptr;
		table->FindRow<FGsSchemaOptionCheckGroup>(FName(*InRowKey), Row);
		return Row;
	}

	return nullptr;
}

bool UGsOptionManager::GetValue(const EGsOptionItem InType, float& outValue, bool& outIsVisible) const
{
	outIsVisible = true;

	if (const FGsOptionData<EGsOptionAudio, int32>* audioData = _audioOptDatas.Find(InType))
	{
		outValue = audioData->GetData() * 0.01f;
		return true;
	}

	if (const FGsOptionData<EGsOptionGraphic, int32>* graphicData = _graphicOptDatas.Find(InType))
	{
#if WITH_EDITOR || PLATFORM_WINDOWS
		if (InType == EGsOptionItem::Window_Resolution)
		{
			if (_userSettings)
			{
				if (_userSettings->GetFullscreenMode() == EWindowMode::WindowedFullscreen)
				{
					outIsVisible = false;
				}
			}
		}
#endif

		outValue = graphicData->GetData();
		return true;
	}

	if (const FGsOptionData<EGsOptionUserInfo, int32>* userInfoData = _userInfoOptDatas.Find(InType))
	{
		outValue = userInfoData->GetData();
		return true;
	}

	// 급 새로 추가된 ServerOption 참조 형식이라, 급하게 분기처리
	// 추후 별도의 분기로직으로 수정 필요함
	if (InType == EGsOptionItem::AutoPotionUse)
	{
		FGsServerOption* serverOption = _serverOption.Get();
		if (nullptr != serverOption)
		{
			float rate = (float)serverOption->GetUsePotionHpRate();
			float result = rate * 0.01f;
			outValue = result;

			//GSLOG(Warning, TEXT("[UGsOptionManager::GetValue] value : %f"), result);
			return true;
		}
	}

	if (InType == EGsOptionItem::AutoBattleRange)
	{
		FGsServerOption* serverOption = _serverOption.Get();
		if (nullptr != serverOption)
		{
			float rate = (float)serverOption->GetAutoAttackRange();
			outValue = rate;

			return true;
		}
	}

	if (InType == EGsOptionItem::CostumeHairOn)
	{
		FGsServerOption* serverOption = _serverOption.Get();
		if (nullptr != serverOption)
		{
			outValue = serverOption->IsVisibleHelmet() ? 1.f : 0.f;
			return true;
		}
	}
	if (InType == EGsOptionItem::ManualBattle_AutoCounterAttack)
	{
		FGsServerOption* serverOption = _serverOption.Get();
		if (nullptr != serverOption)
		{
			outValue = (serverOption->IsCounterAttackOn() == true)? 1.0f: 0.0f;		
			return true;
		}
	}

	if (InType == EGsOptionItem::Pc_Rvo)
	{
		FGsServerOption* serverOption = _serverOption.Get();
		if (nullptr != serverOption)
		{
			float index = (float)serverOption->GetPcRvoIndex();
			outValue = index;

			return true;
		}
	}

	if (IsValidRange(static_cast<OptionSystemMessage>(InType)))
	{
		outValue = 1.f;

		ItemGrade SystemMessageMax = ItemGrade::LEGEND;
		int32 index = static_cast<int32>(InType) - static_cast<int32>(OptionSystemMessage::MIN) + static_cast<int32>(ItemGrade::NORMAL);
		outValue = _options ? _options->GetSystemMessageGradeOption(index) == true ? 1.f : 0.f : 1.f;

		return true;
	}


	if (const FGsOptionData<EGsOptionCombat, int32>* combatData = _combatOptDatas.Find(InType))
	{
		outValue = combatData->GetData();
		return true;
	}

	if (const FGsOptionData<CombatOptionType, int32>* combatTargetData = _autoCombatOptTargetDatas.Find(InType))
	{
		outValue = _serverOption->IsAutoCombatTargetOption(combatTargetData->GetData()) ? 1.f : 0.f;
		return true;
	}

	if (const FGsOptionData<CombatOptionType, int32>* combatTargetData = _manualCombatOptTargetDatas.Find(InType))
	{
		outValue = _serverOption->IsManualCombatTargetOption(combatTargetData->GetData()) ? 1.f : 0.f;
		return true;
	}

	if (const FGsOptionData<EGsOptionOutput, int32>* outputData = _outputOptDatas.Find(InType))
	{
		outValue = outputData->GetData();
		return true;
	}

	if (const FGsOptionData<EGsOptionAlert, int32>* alertData = _alertOptDatas.Find(InType))
	{
		outValue = alertData->GetData();

		if (InType != EGsOptionItem::AllAlert)
		{
			const FGsOptionData<EGsOptionAlert, int32>* allAlertData = _alertOptDatas.Find(EGsOptionItem::AllAlert);
			if (nullptr != allAlertData)
			{
				int32 value = allAlertData->GetData();
				if (false == static_cast<bool>(value))
				{
					outIsVisible = false;
				}
			}
		}

		return true;
	}

	if (EGsOptionItem::AllNotice ==  InType)
	{
		outValue = static_cast<float>(_isAllGameAlert);
		return true;
	}

	if (const FGsOptionData<EGsOptionAlert, int32>* alertServerData = _alertServerOptDatas.Find(InType))
	{
		outValue = alertServerData->GetData();
		return true;
	}

	if (const FGsOptionData<EGsOptionLoginAlert, int32>* alertLoginData = _alertLoginOptDatas.Find(InType))
	{
		outValue = alertLoginData->GetData();

		if (false == _isAllGameAlert)
		{
			outIsVisible = false;
		}

		if (InType != EGsOptionItem::ConnectAlert)
		{
			const FGsOptionData<EGsOptionLoginAlert, int32>* connectAlertData = _alertLoginOptDatas.Find(EGsOptionItem::ConnectAlert);
			if (nullptr != connectAlertData)
			{
				int32 value = connectAlertData->GetData();
				if (false == static_cast<bool>(value))
				{
					outIsVisible = false;
				}
			}			
		}
		
		return true;
	}

	if (const FGsOptionData<EGsOptionNotificationAlert, float>* alertNotificationData = _alertNotificationOptDatas.Find(InType))
	{
		outValue = alertNotificationData->GetData();

		if (false == _isAllGameAlert)
		{
			outIsVisible = false;
		}

		if (InType != EGsOptionItem::EquanimityAlert)
		{
			const FGsOptionData<EGsOptionNotificationAlert, float>* equanimityAlertData = _alertNotificationOptDatas.Find(EGsOptionItem::EquanimityAlert);
			if (nullptr != equanimityAlertData)
			{
				int32 value = equanimityAlertData->GetData();
				if (false == static_cast<bool>(value))
				{
					outIsVisible = false;
				}
			}		
		}

		return true;
	}

	if (const FGsOptionData<EGsOptionNotificationAlert, float>* alertCombatNotificationData = _alertCombatNotificationOptDatas.Find(InType))
	{
		outValue = alertCombatNotificationData->GetData();

		if (false == _isAllGameAlert)
		{
			outIsVisible = false;
		}

		if (InType != EGsOptionItem::CombatStateAlert)
		{
			const FGsOptionData<EGsOptionNotificationAlert, float>* combatStateAlertData = _alertCombatNotificationOptDatas.Find(EGsOptionItem::CombatStateAlert);
			if (nullptr != combatStateAlertData)
			{
				int32 value = combatStateAlertData->GetData();
				if (false == static_cast<bool>(value))
				{
					outIsVisible = false;
				}
			}			
		}
		
		return true;
	}
	
	if (const FGsOptionData<EGsOptionOfflineAlert, float>* offlineAlertData = _alertOfflineOptDatas.Find(InType))
	{
		outValue = offlineAlertData->GetData();

		if (InType != EGsOptionItem::AllAlert)
		{
			const FGsOptionData<EGsOptionAlert, int32>* allAlertData = _alertOptDatas.Find(EGsOptionItem::AllAlert);
			if (nullptr != allAlertData)
			{
				int32 value = allAlertData->GetData();
				if (false == static_cast<bool>(value))
				{
					outIsVisible = false;
				}
			}			
		}

		if (InType != EGsOptionItem::OfflinePlayAlert)
		{
			const FGsOptionData<EGsOptionOfflineAlert, float>* offlinePlayAlertData = _alertOfflineOptDatas.Find(EGsOptionItem::OfflinePlayAlert);
			if (nullptr != offlinePlayAlertData)
			{
				int32 value = offlinePlayAlertData->GetData();
				if (false == static_cast<bool>(value))
				{
					outIsVisible = false;
				}
			}			
		}

		return true;
	}

	// 2022/09/30 PKT - PK Book Option
	if (const FGsOptionData<EGsOptionPKBookAlert, int32>* alertData = _alertPKBookOptData.Find(InType))
	{
		if (false == _isAllGameAlert)
		{
			outIsVisible = false;
		}

		outValue = alertData->GetData();
		return true;
	}

	if (const FGsOptionData<EGsOptionSaveBattery, int32>* saveBatteryData = _saveBatteryOptDatas.Find(InType))
	{
		outValue = saveBatteryData->GetData();
	}

	if (const FGsOptionData<EGsOptionAuto, float>* autoData = _autoOptDatas.Find(InType))
	{
		outValue = autoData->GetData();
		return true;
	}

	if (const FGsOptionData<EGsOptionAuto, float>* autoData = _autoServerOptDatas.Find(InType))
	{
		outValue = autoData->GetData();
		return true;
	}

	if (const FGsOptionData<EGsOptionInput, int32>* inputData = _inputOptDatas.Find(InType))
	{
		outValue = inputData->GetData();
		return true;
	}

	if (const FGsOptionData<EGsDevelopOptionGraphic, int32>* developmentGraphicOptData = 
													_developmentGraphicOptDatas.Find(InType))
	{
		outValue = developmentGraphicOptData->GetData();
		return true;
	}

	if (const FGsOptionData<EGsOptionServerAutoDrop, int32>* optAutoDropData =
		_serverOptionAutoDrop.Find(InType))
	{
		outValue = optAutoDropData->GetData();		
		return true;
	}

	if (const FGsOptionData<EGsPickUpItemOption, bool>* findOption = _serverOptionPickupItem.Find(InType))
	{
		outValue = findOption->GetData();
		return true;
	}

	if (const FGsOptionData<EGsOptionAutoDecompose, bool>* findOption = _serverOptionAutoDecompose.Find(InType))
	{
		outValue = findOption->GetData();
		return true;
	}

	return false;
}

bool UGsOptionManager::GetValue(const EGsOptionItem InType, FString& outValue, bool& outIsVisible) const
{
	return false;
}

void UGsOptionManager::SetValue(EGsOptionItem InType, float InVal)
{	
	if (FGsOptionData<EGsOptionAudio, int32>* audioData = _audioOptDatas.Find(InType))
	{
		audioData->SetData(InVal * 100);
		return;
	}

	if (FGsOptionData<EGsOptionGraphic, int32>* graphicData = _graphicOptDatas.Find(InType))
	{
		graphicData->SetData(InVal);
		return;
	}

	if (FGsOptionData<EGsDevelopOptionGraphic, int32>* developmentGraphicData = _developmentGraphicOptDatas.Find(InType))
	{
		developmentGraphicData->SetData(InVal);
		return;
	}

	if (FGsOptionData<EGsOptionUserInfo, int32>* userInfoData = _userInfoOptDatas.Find(InType))
	{
		if (EGsOptionItem::Language == InType ||
			EGsOptionItem::LanguageVoice == InType)
		{
			int32 valueInt = static_cast<int32>(InVal);

			if (GLocalization()->IsOptionSave())
			{
				userInfoData->SetData(InVal);
			}
			else
			{
				if (userInfoData->_value != valueInt)
				{
					bool bIsVoice = (EGsOptionItem::LanguageVoice == InType) ? true : false;

					FGsUIMsgParamLanguege param(valueInt, bIsVoice);
					GMessage()->GetUI().SendMessage(MessageUI::OPTION_SELECT_LANGUAGE, &param);
				}
			}

			return;
		}
		userInfoData->SetData(InVal);
		
		return;
	}

	// 급 새로 추가된 ServerOption 참조 형식이라, 급하게 분기처리
	// 추후 별도의 분기로직으로 수정 필요함
	if (InType == EGsOptionItem::AutoPotionUse)
	{	
		FGsServerOption* serverOption = _serverOption.Get();
		if (nullptr != serverOption)
		{
			int32 tempVal = FMath::RoundToInt(InVal * 100);
			UsePotionHpRate result = static_cast<UsePotionHpRate>(tempVal);
			serverOption->SetUsePotionHpRate(result);

			//GSLOG(Warning, TEXT("[UGsOptionManager::SetValue] value : %d"), result);
			return;
		}
	}

	if (InType == EGsOptionItem::CostumeHairOn)
	{
		FGsServerOption* serverOption = _serverOption.Get();
		if (nullptr != serverOption)
		{
			serverOption->SetIsVisibleHelmet(InVal > 0.f);
			return;
		}
	}

	if (InType == EGsOptionItem::AutoBattleRange)
	{
		FGsServerOption* serverOption = _serverOption.Get();
		if (nullptr != serverOption)
		{
			int32 value = (int32)InVal;
			serverOption->SetAutoAttackRange(AutoAttackRange(value));
			return;
		}
	}

	if (InType == EGsOptionItem::Pc_Rvo)
	{
		FGsServerOption* serverOption = _serverOption.Get();
		if (nullptr != serverOption)
		{
			int32 value = (int32)InVal;
			serverOption->SetPcRvo(value);
			return;
		}
	}


	if (InType == EGsOptionItem::ManualBattle_AutoCounterAttack)
	{
		FGsServerOption* serverOption = _serverOption.Get();
		if (nullptr != serverOption)
		{
			serverOption->SetIsCounterAttackOn((InVal == 1.0f)?true:false);
			return;
		}
	}

	if (IsValidRange(static_cast<OptionSystemMessage>(InType)))
	{
		int32 index = static_cast<int32>(InType) - static_cast<int32>(OptionSystemMessage::MIN) + static_cast<int32>(ItemGrade::NORMAL);
		if(_options) _options->SetSystemMessageGradeOption(index, InVal > 0.f);
		return;
	}

// 	if (InType == EGsOptionItem::ManualBattle_AutoSkillUse)
// 	{
// 		FGsServerOption* serverOption = _serverOption.Get();
// 		if (nullptr != serverOption)
// 		{
// 			serverOption->SetIsAutoSkillOn((InVal == 1.0f) ? true : false);
// 			return;
// 		}
// 	}

	if (FGsOptionData<EGsOptionCombat, int32>* combatData = _combatOptDatas.Find(InType))
	{
		combatData->SetData(InVal);
		return;
	}

	if (FGsOptionData<CombatOptionType, int32>* combatTargetData = _autoCombatOptTargetDatas.Find(InType))
	{
		_serverOption->SetAutoCombatTargetOption(combatTargetData->GetData(), (InVal > 0.f) ? true : false);
		return;
	}

	if (FGsOptionData<CombatOptionType, int32>* combatTargetData = _manualCombatOptTargetDatas.Find(InType))
	{
		_serverOption->SetManualCombatTargetOption(combatTargetData->GetData(), (InVal > 0.f) ? true : false);
		return;
	}

	if (FGsOptionData<EGsOptionOutput, int32>* outputData = _outputOptDatas.Find(InType))
	{
		outputData->SetData(InVal);
		return;
	}

	if (FGsOptionData<EGsOptionAlert, int32>* alertData = _alertOptDatas.Find(InType))
	{
		alertData->SetData(InVal);
		return;
	}

	if (FGsOptionData<EGsOptionAlert, int32>* alertServerOptData = _alertServerOptDatas.Find(InType))
	{
		alertServerOptData->SetData(InVal);
		return;
	}

	if (EGsOptionItem::AllNotice == InType)
	{
		SetAllGameAlert(InVal);
	}

	if (FGsOptionData<EGsOptionLoginAlert, int32>* alertData = _alertLoginOptDatas.Find(InType))
	{
		alertData->SetData(InVal);
		return;
	}

	if (FGsOptionData<EGsOptionNotificationAlert, float>* alertData = _alertNotificationOptDatas.Find(InType))
	{
		alertData->SetData(InVal);
		return;
	}

	if (FGsOptionData<EGsOptionNotificationAlert, float>* alertData = _alertCombatNotificationOptDatas.Find(InType))
	{
		alertData->SetData(InVal);
		return;
	}

	if (FGsOptionData<EGsOptionOfflineAlert, float>* offlineAlertData = _alertOfflineOptDatas.Find(InType))
	{
		offlineAlertData->SetData(InVal);
		return;
	}

	if (FGsOptionData<EGsOptionPKBookAlert, int32>* pkBookAlertData = _alertPKBookOptData.Find(InType))
	{
		pkBookAlertData->SetData(InVal);
		return;
	}

	if (FGsOptionData<EGsOptionSaveBattery, int32>* saveBatteryData = _saveBatteryOptDatas.Find(InType))
	{
		saveBatteryData->SetData(InVal);
		return;
	}

	if (FGsOptionData<EGsOptionAuto, float>* autoData = _autoOptDatas.Find(InType))
	{
		autoData->SetData(InVal);
		return;
	}

	if (FGsOptionData<EGsOptionAuto, float>* autoData = _autoServerOptDatas.Find(InType))
	{
		autoData->SetData(InVal);
		return;
	}

	if (FGsOptionData<EGsOptionInput, int32>* inputData = _inputOptDatas.Find(InType))
	{
		inputData->SetData(InVal);
		return;
	}

	if (FGsOptionData<EGsOptionServerAutoDrop, int32>* autoDropData = _serverOptionAutoDrop.Find(InType))
	{
		autoDropData->SetData(InVal);
		return;
	}

	if (FGsOptionData<EGsPickUpItemOption, bool>* findOption = _serverOptionPickupItem.Find(InType))
	{
		findOption->SetData((InVal > 0.f) ? true : false);
		return;
	}

	if (FGsOptionData<EGsOptionAutoDecompose, bool>* findOption = _serverOptionAutoDecompose.Find(InType))
	{
		findOption->SetData((InVal > 0.f) ? true : false);
		return;
	}
}

void UGsOptionManager::OnUserInfoFunc(EGsOptionUserInfo InOptType, int32 InVal)
{
	switch (InOptType)
	{
	case EGsOptionUserInfo::BACKTO_LOBBY:
	{
		GMessage()->GetSystem().SendMessage(MessageSystem::EXIT_GAME);	
	}
	break;
	default:
		break;
	}
}

void UGsOptionManager::SaveAudio(EGsOptionAudio InOptType, int32 InVal)
{
	_userSettings->SetAudioSettingByType(InOptType, InVal);
}

void UGsOptionManager::SaveAlert(EGsOptionItem InOptType, int32 InVal)
{
	FText com2usText;
	FText::FindText(TEXT("OptionText"), TEXT("Com2us"), com2usText);

	FString serverTimeText;
	FGsTimeStringHelper::GetTimeStringNYMD(FGsTimeSyncUtil::GetClientNowDateTime(), serverTimeText);

	if(EGsOptionItem::AllAlert == InOptType || EGsOptionItem::ConsoleAlert == InOptType)
	{
		if (true == static_cast<bool>(InVal))
		{
			FText findText;
			if (FText::FindText(TEXT("OptionText"), TEXT("Alert_PushAgree"), findText))
			{
				FGsUIHelper::TrayTickerLauncher(FText::Format(findText, com2usText, FText::FromString(serverTimeText)));
			}			
		}
		else
		{			
			FText findText;
			if (FText::FindText(TEXT("OptionText"), TEXT("Alert_PushDisagree"), findText))
			{
				FGsUIHelper::TrayTickerLauncher(FText::Format(findText, com2usText, FText::FromString(serverTimeText)));
			}
		}
	}

	if (EGsOptionItem::AllAlert == InOptType)
	{
		for (auto& iter : _alertOptDatas)
		{
			FGsOptionData<EGsOptionAlert, int32>& data = iter.Value;
			data.SetValueData(InVal);
		}
		if (FGsOptionData<EGsOptionAlert, int32>* alertOptData = _alertOptDatas.Find(InOptType))
		{
			alertOptData->SetValueData(InVal);
		}
		SaveOfflineAlert(EGsOptionItem::OfflinePlayAlert, InVal);
	}	
	else
	{
		if (EGsOptionItem::ConsoleAlert == InOptType)
		{
			if (FGsOptionData<EGsOptionAlert, int32>* alertOptData = _alertOptDatas.Find(InOptType))
			{
				alertOptData->SetValueData(InVal);
			}

			if (false == static_cast<bool>(InVal))
			{
				if (FGsOptionData<EGsOptionAlert, int32>* consoleAlertNightData = _alertOptDatas.Find(EGsOptionItem::ConsoleAlert_Night))
				{
					consoleAlertNightData->SetValueData(InVal);
				}
			}
		}
		else if (EGsOptionItem::ConsoleAlert_Night == InOptType)
		{
			if (true == static_cast<bool>(InVal))
			{
				if (FGsOptionData<EGsOptionAlert, int32>* consoleAlertData = _alertOptDatas.Find(EGsOptionItem::ConsoleAlert))
				{
					int32 value = consoleAlertData->GetData();
					if (0 >= value)
					{
						if (FGsOptionData<EGsOptionAlert, int32>* consoleAlertNightData = _alertOptDatas.Find(EGsOptionItem::ConsoleAlert_Night))
						{
							consoleAlertNightData->SetValueData(false);
						}
					}
					else
					{
						FText findText;
						if (FText::FindText(TEXT("OptionText"), TEXT("Alert_NightPushAgree"), findText))
						{
							FGsUIHelper::TrayTickerLauncher(FText::Format(findText, com2usText, FText::FromString(serverTimeText)));
						}						
					}
				}
			}
			else
			{
				if (FGsOptionData<EGsOptionAlert, int32>* alertOptData = _alertOptDatas.Find(InOptType))
				{
					alertOptData->SetValueData(InVal);
				}

				FText findText;
				if (FText::FindText(TEXT("OptionText"), TEXT("Alert_NightPushDisagree"), findText))
				{
					FGsUIHelper::TrayTickerLauncher(FText::Format(findText, com2usText, FText::FromString(serverTimeText)));
				}
			}		
		}
	}

	int32 consoleAlertValue = 0;
	FGsOptionData<EGsOptionAlert, int32>* consoleAlertData = _alertOptDatas.Find(EGsOptionItem::ConsoleAlert);
	if (nullptr != consoleAlertData)
	{
		consoleAlertValue = consoleAlertData->GetData();
	}

	int32 consoleAlertNightValue = 0;
	FGsOptionData<EGsOptionAlert, int32>* consoleAlertNightData = _alertOptDatas.Find(EGsOptionItem::ConsoleAlert_Night);
	if (nullptr != consoleAlertNightData)
	{
		consoleAlertNightValue = consoleAlertNightData->GetData();
	}

	if (_userSettings)
	{
		if (EGsOptionItem::AllAlert == InOptType)
		{
			_userSettings->SetAllAlertSetting(static_cast<bool>(InVal));
		}
		_userSettings->SetAlertSetting(consoleAlertValue, consoleAlertNightValue);
	}

	GMessage()->GetUI().SendMessage(MessageUI::OPTION_UI_UPDATE, nullptr);
}

void UGsOptionManager::SaveHiddenNameGetItem(EGsOptionItem InOptType, int32 InVal)
{	
	
	if (FGsOptionData<EGsOptionAlert, int32>* alertOptData = _alertServerOptDatas.Find(InOptType))
	{
		alertOptData->SetValueData(InVal);
	}
}


void UGsOptionManager::SetAllGameAlert(int InValue)
{
	_isAllGameAlert = static_cast<bool>(InValue);
	if (_userSettings)
	{
		_userSettings->SetAllGameAlert(_isAllGameAlert);
	}

	SaveLoginAlert(EGsOptionItem::ConnectAlert, InValue);
	SaveNotificationAlert(EGsOptionItem::EquanimityAlert, InValue);
	SaveCombatNotificationAlert(EGsOptionItem::CombatStateAlert, InValue);
	SavePKBookAlert(EGsOptionItem::Target_Detection_Alert, InValue);

	GMessage()->GetUI().SendMessage(MessageUI::OPTION_UI_UPDATE, nullptr);
}

void UGsOptionManager::SavePushAlert(EGsOptionItem InOptType, int32 InVal)
{	
	// AllAlert 켜면 다 켜여야한다.
	if (EGsOptionItem::AllAlert == InOptType)
	{
		for (auto& iter : _alertOptDatas)
		{
			FGsOptionData<EGsOptionAlert, int32>& data = iter.Value;
			data.SetValueData(InVal);
		}
	}
	else
	{
		if (EGsOptionItem::ConsoleAlert == InOptType)
		{
			if (false == static_cast<bool>(InVal))
			{
				FGsOptionData<EGsOptionAlert, int32>* nightdata = _alertOptDatas.Find(EGsOptionItem::ConsoleAlert_Night);
				nightdata->SetData(InVal);
			}
		}

		if (FGsOptionData<EGsOptionAlert, int32>* data = _alertOptDatas.Find(InOptType))
		{
			data->SetValueData(InVal);
		}
	}

	int32 consoleAlertValue = 0;
	FGsOptionData<EGsOptionAlert, int32>* consoleAlertData = _alertOptDatas.Find(EGsOptionItem::ConsoleAlert);
	if (nullptr != consoleAlertData)
	{
		consoleAlertValue = consoleAlertData->GetData();
	}
	
	int32 consoleAlertNightValue = 0;
	FGsOptionData<EGsOptionAlert, int32>* consoleAlertNightData = _alertOptDatas.Find(EGsOptionItem::ConsoleAlert_Night);
	if (nullptr != consoleAlertNightData)
	{
		consoleAlertNightValue = consoleAlertNightData->GetData();
	}

	_userSettings->SetAlertSetting(consoleAlertValue, consoleAlertNightValue);
	GMessage()->GetUI().SendMessage(MessageUI::OPTION_UI_UPDATE, nullptr);
}

void UGsOptionManager::SaveLoginAlert(EGsOptionItem InOptType, int32 InVal)
{
	if (EGsOptionItem::ConnectAlert == InOptType)
	{
		for (auto& iter : _alertLoginOptDatas)
		{				
			FGsOptionData<EGsOptionLoginAlert, int32>& data = iter.Value;
			data.SetValueData(InVal);
		}
		
		GMessage()->GetUI().SendMessage(MessageUI::OPTION_UI_UPDATE, nullptr);
		return;
	}	

	if (FGsOptionData<EGsOptionLoginAlert, int32>* alertLoginOptData = _alertLoginOptDatas.Find(InOptType))
	{
		alertLoginOptData->SetValueData(InVal);
	}
}

void UGsOptionManager::SaveNotificationAlert(EGsOptionItem InOptType, float InVal)
{
	if (EGsOptionItem::EquanimityAlert == InOptType)
	{
		for (auto& iter : _alertNotificationOptDatas)
		{
			if (iter.Key == EGsOptionItem::EquanimityAlertIntervalTime ||
				/*iter.Key == EGsOptionItem::HPRateAlert ||*/
				iter.Key == EGsOptionItem::AutoPotionAlert)
				continue;

			FGsOptionData<EGsOptionNotificationAlert, float>& data = iter.Value;
			data.SetValueData(InVal);
		}

		GMessage()->GetUI().SendMessage(MessageUI::OPTION_UI_UPDATE, nullptr);
		return;
	}

	if (FGsOptionData<EGsOptionNotificationAlert, float>* notificationOptData = _alertNotificationOptDatas.Find(InOptType))
	{
		notificationOptData->SetValueData(InVal);
	}
}

void UGsOptionManager::SaveCombatNotificationAlert(EGsOptionItem InOptType, float InVal)
{
	if (EGsOptionItem::CombatStateAlert == InOptType)
	{
		for (auto& iter : _alertCombatNotificationOptDatas)
		{
			if (iter.Key == EGsOptionItem::CombatStateAlertIntervalTime ||
				iter.Key == EGsOptionItem::HPRateAlert)
				continue;

			FGsOptionData<EGsOptionNotificationAlert, float>& data = iter.Value;
			data.SetValueData(InVal);
		}

		GMessage()->GetUI().SendMessage(MessageUI::OPTION_UI_UPDATE, nullptr);
		return;
	}

	if (FGsOptionData<EGsOptionNotificationAlert, float>* combatNotificationOptData = _alertCombatNotificationOptDatas.Find(InOptType))
	{
		combatNotificationOptData->SetValueData(InVal);
	}
}

void UGsOptionManager::SaveOfflineAlert(EGsOptionItem InOptType, float InVal)
{
	if (FGsOptionData<EGsOptionOfflineAlert, float>* offlineOptData = _alertOfflineOptDatas.Find(InOptType))
	{
		offlineOptData->SetValueData(InVal);
	}

	if (EGsOptionItem::OfflinePlayAlert == InOptType)
	{
		for (auto& iter : _alertOfflineOptDatas)
		{
			if (iter.Key == EGsOptionItem::OfflinePlayAlertIntervalTime)
				continue;

			FGsOptionData<EGsOptionOfflineAlert, float>& data = iter.Value;
			data.SetValueData(InVal);
		}

		GMessage()->GetUI().SendMessage(MessageUI::OPTION_UI_UPDATE, nullptr);
	}
}

void UGsOptionManager::SavePKBookAlert(EGsOptionItem InOptType, int32 InVal)
{
	if (EGsOptionItem::Target_Detection_Alert == InOptType)
	{
		for (auto& iter : _alertPKBookOptData)
		{
			if (iter.Key == EGsOptionItem::Target_Detection_Alert_Distance || iter.Key == EGsOptionItem::Target_Detection_Alert_Interval)
			{
				continue;
			}				

			FGsOptionData<EGsOptionPKBookAlert, int32>& data = iter.Value;
			data.SetValueData(InVal);
		}

		GMessage()->GetUI().SendMessage(MessageUI::OPTION_UI_UPDATE, nullptr);
		return;
	}

	if (FGsOptionData<EGsOptionPKBookAlert, int32>* option = _alertPKBookOptData.Find(InOptType))
	{
		option->SetValueData(InVal);
	}
}

void UGsOptionManager::SaveGraphicGrade(EGsOptionGraphic InOptType, int32 InVal)
{
	if (_userSettings->SetGraphicGrade(InOptType, InVal))
	{
		_graphicGrade = InVal;
		ReLoadGraphicDatas();
		GMessage()->GetUI().SendMessage(MessageUI::OPTION_UI_UPDATE, nullptr);
	}	
}

void UGsOptionManager::SaveGraphicGradeCustom()
{
	_userSettings->SetGraphicGrade(EGsOptionGraphic::GRAPHIC_GRADE, 0);
	_graphicGrade = 0;

	if (FGsOptionData<EGsOptionGraphic, int32>* graphicData = _graphicOptDatas.Find(EGsOptionItem::Graphic_Total))
	{
		graphicData->SetValueData(0);
	}

	GMessage()->GetUI().SendMessage(MessageUI::OPTION_UI_UPDATE, nullptr);
}

void UGsOptionManager::SaveGraphicUIScale(EGsOptionGraphic InOptType, int32 InVal)
{
	_userSettings->SetGraphicGrade(InOptType, InVal);
}

void UGsOptionManager::SaveGraphicEtc(EGsOptionGraphic InOptType, int32 InVal)
{
	if (0 < _graphicGrade)
	{
		// 커스텀이 아닐때
		if (_userSettings->SetGraphicCustom(InOptType, InVal))
		{
			_graphicGrade = 0;
			ReLoadGraphicDatas();
			GMessage()->GetUI().SendMessage(MessageUI::OPTION_UI_UPDATE, nullptr);
		}
	}
	else
	{
		_userSettings->SetGraphicGrade(InOptType, InVal);
		SaveGraphicGradeCustom();
	}
}

#if WITH_EDITOR || PLATFORM_WINDOWS
void UGsOptionManager::SaveWindowResolution(EGsOptionGraphic InOptType, int32 InVal)
{
	// 사용자 지정이면 -> 팝업
	// 해상도 선택이면 지원가능하면 변경 후 저장
	// 지원이 불가능 하면 불가능 티커 후 OPTION_UI_UPDATE 호출

	_userSettings->SetWindowResolution(InVal);
}

void UGsOptionManager::SaveWindowScreenMode(EGsOptionGraphic InOptType, int32 InVal)
{
	_userSettings->SetWindowScreenMode(InVal);
}
#endif

void UGsOptionManager::SaveAll()
{
	TMap<EGsOptionGraphic, int32> graphicOptTable;

	for (const auto& iter : _graphicOptDatas)
	{
		const FGsOptionData<EGsOptionGraphic, int32>& data = iter.Value;
		graphicOptTable.Emplace(data._saveType, data._value);
	}
	_userSettings->SetGraphicSettings(graphicOptTable, false);

	for (const auto& iter : _userInfoOptDatas)
	{
		const FGsOptionData<EGsOptionUserInfo, int32>& data = iter.Value;
		_userSettings->SetUserInfoSettingByType(data._saveType, FString::FromInt(data._value), false);
	}

	for (const auto& iter: _combatOptDatas)
	{
		const FGsOptionData<EGsOptionCombat, int32>& data = iter.Value;
		_userSettings->SetCombatSettingByType(data._saveType, data._value, false);
	}
	
	for (const auto& iter: _outputOptDatas)
	{
		const FGsOptionData<EGsOptionOutput, int32>& data = iter.Value;
		_userSettings->SetOutputSetting(data._saveType, data._value, false);
	}

	/*for (auto& iter : _alertOptDatas)
	{
		FGsOptionData<EGsOptionAlert, int32>& data = iter.Value;
		_userSettings->SetAlertSetting(data._saveType, data._value, false);
	}*/

	for (auto& iter : _alertServerOptDatas)
	{
		FGsOptionData<EGsOptionAlert, int32>& data = iter.Value;
		_serverOption->SetOptionAlert(data._saveType, data._value);
	}

	for (auto& iter : _alertLoginOptDatas)
	{		
		FGsOptionData<EGsOptionLoginAlert, int32>& data = iter.Value;
		_serverOption->SetOptionLoginAlert(data._saveType, data._value);
	}

	for (auto& iter : _alertNotificationOptDatas)
	{
		FGsOptionData<EGsOptionNotificationAlert, float>& data = iter.Value;
		_serverOption->SetOptionNotificationAlert(data._saveType, data._value);
	}

	for (auto& iter : _alertCombatNotificationOptDatas)
	{
		FGsOptionData<EGsOptionNotificationAlert, float>& data = iter.Value;
		_serverOption->SetOptionNotificationAlert(data._saveType, data._value);
	}

	for (auto& iter : _alertOfflineOptDatas)
	{
		FGsOptionData<EGsOptionOfflineAlert, float>& data = iter.Value;
		_serverOption->SetOptionOfflineAlert(data._saveType, data._value);
	}

	// 2022/09/30 PKT - PK Book은 Server와 동기화를 맞춤
	for (auto& iter : _alertPKBookOptData)
	{
		FGsOptionData<EGsOptionPKBookAlert, int32>& data = iter.Value;
		_serverOption->SetOptionPKBookAlert(data._saveType, data._value);
	}

	for (const auto& iter : _saveBatteryOptDatas)
	{
		const FGsOptionData<EGsOptionSaveBattery, int32>& data = iter.Value;
		_userSettings->SetSaveBatterySetting(data._saveType, data._value, false);
	}

	for (const auto& iter : _autoOptDatas)
	{
		const FGsOptionData<EGsOptionAuto, float>& data = iter.Value;
		_userSettings->SetAutoSetting(data._saveType, data._value, false);
	}

	for (const auto& iter : _autoServerOptDatas)
	{
		const FGsOptionData<EGsOptionAuto, float>& data = iter.Value;
		_serverOption->SetOptionAuto(data._saveType, data._value);
	}

	for (const auto& iter : _inputOptDatas)
	{
		const FGsOptionData<EGsOptionInput, int32>& data = iter.Value;
		_userSettings->SetInputSetting(data._saveType, data._value, false);
	}

	for (const auto& iter : _developmentGraphicOptDatas)
	{
		const FGsOptionData<EGsDevelopOptionGraphic, int32>& data = iter.Value;
		_developmentOption->SetGraphicOpt(data._saveType, data._value);
	}

	for (auto& iter : _serverOptionAutoDrop)
	{
		FGsOptionData<EGsOptionServerAutoDrop, int32>& data = iter.Value;
		if (data._saveType == EGsOptionServerAutoDrop::ItemGrade)
		{
			// need convert
			// diff data, ui align
			AutoDropItemGrade itemGrade = FGsOptionFunc::ConvertAutoItemGainOptionEnumToServerEnum(data._value);
			_serverOption->SetDropItemGrade(itemGrade);
		}
		else if (data._saveType == EGsOptionServerAutoDrop::ItemWeight)
		{
			_serverOption->SetAutoDropItemWeight(data._value);
		}		
	}

	for (auto& iter : _serverOptionPickupItem)
	{
		FGsOptionData<EGsPickUpItemOption, bool>& data = iter.Value;
		_serverOption->SetPickupItemOption(data._saveType, data._value);
	}

	for (auto& iter : _serverOptionAutoDecompose)
	{
		FGsOptionData<EGsOptionAutoDecompose, bool>& data = iter.Value;
		_serverOption->SetAutoDecomposeOption(data._saveType, data._value);
	
	}

	_options->SaveOptionConfig();
	_serverOption->SendToServerGameOption();
	_userSettings->SaveUserSettings();
}

void UGsOptionManager::OptionRefresh()
{
	if (UGsOptionManager* optionManager = GOption())
	{
		optionManager->SaveAll();
	}
}

bool UGsOptionManager::IsVisible(EGsOptionItem InType)
{
	if (InType == EGsOptionItem::Logout)
	{
#if WITH_EDITOR
		return false;
#else
		if (UGsHiveManager* hiveMgr = GHive())
		{
			return hiveMgr->IsGuest() ? false : true;
		}
#endif
	}
	else if (InType == EGsOptionItem::Window_Resolution)
	{
		if (_userSettings)
		{
			if (_userSettings->GetFullscreenMode() != EWindowMode::Windowed)
			{
				return false;
			}
		}
	}
	return true;
}

bool UGsOptionManager::GetCustomTitleValue(EGsOptionItem InType, OUT FText& outValue)
{
	outValue = FText::GetEmpty();

	if (InType == EGsOptionItem::Logout)
	{
#if WITH_EDITOR		
#else
		if (UGsHiveManager* hiveMgr = GHive())
		{
			outValue = FText::FromString(FString::Printf(TEXT("%llu"), hiveMgr->GetPlayerID()));
			return true;
		}
#endif		
	}

	return false;
}
void UGsOptionManager::SetIsShowHpbar(bool In_val)
{
	_isShowHPbar = In_val;

	// update widget hpbar show/ hide
	UGsGameObjectUtil::UpdateWidgetShowHPBar();

	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (local && local->GetCharacter())
	{
		if (APlayerController* controller = Cast<APlayerController>(local->GetCharacter()->GetController()))
		{
			if (AGsHUDLocalPlayer* hud = Cast<AGsHUDLocalPlayer>(controller->GetHUD()))
			{
				hud->UpdateTargetShowHPBar();
			}
		}
	}
}

void UGsOptionManager::SetServerOptionData()
{
	InitServerAlertDatas();
	// 로그인 알림 정보
	InitAlertLoginDatas();
	// 인게임 개인 알림 정보
	InitAlertNotificationDatas();
	// 인게임 전투 개인 알림 정보
	InitAlertCombatNotificationDatas();
	// 오프라인 알림 정보
	InitOfflineAlertDatas();
	// 2022/10/05 PKT - PK Book 알림 정보
	InitPKBookAlertDatas();
	// 자동 옵션 서버 정보
	InitServerAutoDatas();
	// server option: drop item
	InitServerAutoDrop();

	InitServerItemPickupOption();
	// 자동 분해 정보
	InitServerAutoDecomposeDatas();
}

#if WITH_EDITOR || PLATFORM_WINDOWS
void UGsOptionManager::OnKeyMapping(EGsOptionItem InOptType, int32 InVal)
{
	GMessage()->GetContents().SendMessage(MessageContents::CLOSE_QUICK_MENU);
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_KEYBOARD_MAPPING);
}

void UGsOptionManager::OnWindowResolutionUpdate()
{
	ReLoadGraphicDatas();
	GMessage()->GetUI().SendMessage(MessageUI::OPTION_UI_UPDATE, nullptr);
}

void UGsOptionManager::OnWindowScreenModeUpdate()
{
	ReLoadGraphicDatas();
	GMessage()->GetUI().SendMessage(MessageUI::OPTION_UI_UPDATE, nullptr);
}

void UGsOptionManager::GetWindowGraphicDatas()
{
	UGsGameUserSettings* Setting = GGameUserSettings();
	if (nullptr == Setting)
		return;

	int screenMode = Setting->GetWindowScreenMode();
	if (_graphicOptDatas.Contains(EGsOptionItem::Window_ScreenMode))
	{
		if (FGsOptionData<EGsOptionGraphic, int32>* graphicData = _graphicOptDatas.Find(EGsOptionItem::Window_ScreenMode))
		{
			graphicData->SetValueData(screenMode);
		}
	}

	int resolution = Setting->GetWindowResolution();
	if (_graphicOptDatas.Contains(EGsOptionItem::Window_Resolution))
	{
		if (FGsOptionData<EGsOptionGraphic, int32>* graphicData = _graphicOptDatas.Find(EGsOptionItem::Window_Resolution))
		{
			graphicData->SetValueData(resolution);
		}
	}
}
#endif

void UGsOptionManager::GetHivePushDatas()
{
	const TMap<EGsOptionAlert, int32>& alertTable = _userSettings->GetAlertSettings();
	for (const TPair<EGsOptionAlert, int32>& iter : alertTable)
	{
		EGsOptionAlert alertType = iter.Key;
		int32 val = iter.Value;
		switch (iter.Key)
		{
		case EGsOptionAlert::CONSOLE_ALERT:
			if (FGsOptionData<EGsOptionAlert, int32>* alertData = _alertOptDatas.Find(EGsOptionItem::ConsoleAlert))
			{
				alertData->SetValueData(val);
			}
			break;
		case EGsOptionAlert::CONSOLE_ALERT_NIGHT:
			if (FGsOptionData<EGsOptionAlert, int32>* alertData = _alertOptDatas.Find(EGsOptionItem::ConsoleAlert_Night))
			{
				alertData->SetValueData(val);
			}
			break;
		default:
			break;
		}
	}
}

bool UGsOptionManager::GetIsValidData(FString inTableName)
{
	const UGsTable* table;
	if (inTableName.IsEmpty())
	{
		table = FGsSchemaOptionCategory::GetStaticTable();
	}
	else
	{
		table = UGsTableManager::GetInstance().GetTable(FGsSchemaOptionCategory::StaticStruct(), inTableName);
	}

	return (nullptr == table)? false : true;
}

void UGsOptionManager::InitServerAutoDrop()
{
	_serverOptionAutoDrop.Empty();

	FGsServerOption* serverOption = _serverOption.Get();
	if (nullptr != serverOption)
	{
		_serverOptionAutoDrop.Emplace(EGsOptionItem::AutoItemGainWeight,
			FGsOptionData<EGsOptionServerAutoDrop, int32>(EGsOptionServerAutoDrop::ItemWeight, 
				(int32)(serverOption->GetAutoDropItemWeight())));

		AutoDropItemGrade serverEnum = serverOption->GetDropItemGrade();
		int optionEnum = FGsOptionFunc::ConvertAutoItemGainServerEnumToOptionEnum(serverEnum);

		_serverOptionAutoDrop.Emplace(EGsOptionItem::AutoItemGain,
			FGsOptionData<EGsOptionServerAutoDrop, int32>(EGsOptionServerAutoDrop::ItemGrade, 
				optionEnum));
	}
}

void UGsOptionManager::InitServerItemPickupOption()
{
	_serverOptionPickupItem.Empty();

	FGsServerOption* serverOption = _serverOption.Get();
	if (nullptr != serverOption)
	{
		_serverOptionPickupItem.Emplace(EGsOptionItem::OptionItemGainNoneEqupConsume_Normal,
			FGsOptionData<EGsPickUpItemOption, bool>(EGsPickUpItemOption::ItemGainCommonConsume, (bool)(serverOption->GetItempickupOption().mItemGainCommonConsume)));

		_serverOptionPickupItem.Emplace(EGsOptionItem::OptionItemGainNoneEqupConsume_Special,
			FGsOptionData<EGsPickUpItemOption, bool>(EGsPickUpItemOption::ItemGainSpecialConsume, (bool)(serverOption->GetItempickupOption().mItemGainSpecialConsume)));

		_serverOptionPickupItem.Emplace(EGsOptionItem::OptionItemGainScrollConsume_Normal,
			FGsOptionData<EGsPickUpItemOption, bool>(EGsPickUpItemOption::ItemGainCommonScroll, (bool)(serverOption->GetItempickupOption().mItemGainCommonScroll)));

		_serverOptionPickupItem.Emplace(EGsOptionItem::OptionItemGainScrollConsume_Special,
			FGsOptionData<EGsPickUpItemOption, bool>(EGsPickUpItemOption::ItemGainSpecialScroll, (bool)(serverOption->GetItempickupOption().mItemGainSpecialScroll)));

		_serverOptionPickupItem.Emplace(EGsOptionItem::OptionItemGainScrollConsume_Enchant,
			FGsOptionData<EGsPickUpItemOption, bool>(EGsPickUpItemOption::ItemGainEnchantScroll, (bool)(serverOption->GetItempickupOption().mItemGainEnchantScroll)));

		_serverOptionPickupItem.Emplace(EGsOptionItem::OptionItemGainOther_Normal,
			FGsOptionData<EGsPickUpItemOption, bool>(EGsPickUpItemOption::ItemGainOtherNormal, (bool)(serverOption->GetItempickupOption().mItemGainOtherNormal)));

		_serverOptionPickupItem.Emplace(EGsOptionItem::OptionItemGainOther_Special,
			FGsOptionData<EGsPickUpItemOption, bool>(EGsPickUpItemOption::ItemGainOtherSpecial, (bool)(serverOption->GetItempickupOption().mItemGainOtherSpecial)));

		_serverOptionPickupItem.Emplace(EGsOptionItem::OptionItemGainOther_Etc,
			FGsOptionData<EGsPickUpItemOption, bool>(EGsPickUpItemOption::ItemGainOtherEtc, (bool)(serverOption->GetItempickupOption().mItemGainOtherEtc)));
	}
}

void UGsOptionManager::InitServerAutoDecomposeDatas()
{
	_serverOptionAutoDecompose.Empty();

	FGsServerOption* serverOption = _serverOption.Get();
	if (nullptr != serverOption)
	{
		_serverOptionAutoDecompose.Emplace(EGsOptionItem::OptionAutoDecomposeFieldBoss,
			FGsOptionData<EGsOptionAutoDecompose, bool>(EGsOptionAutoDecompose::FIELD_BOSS, (bool)(serverOption->GetAutoDecomposeOption().mIsFieldBossDropItem)));

		_serverOptionAutoDecompose.Emplace(EGsOptionItem::OptionAutoDecomposeItemCollection,
			FGsOptionData<EGsOptionAutoDecompose, bool>(EGsOptionAutoDecompose::ITEM_COLLECTION, (bool)(serverOption->GetAutoDecomposeOption().mIsItemCollectAble)));

		_serverOptionAutoDecompose.Emplace(EGsOptionItem::OptionAutoDecomposeEquipNormal,
			FGsOptionData<EGsOptionAutoDecompose, bool>(EGsOptionAutoDecompose::EQUIP_NORMAL, (bool)(serverOption->GetAutoDecomposeOption().mIsEquipNormal)));

		_serverOptionAutoDecompose.Emplace(EGsOptionItem::OptionAutoDecomposeEquipMagic,
			FGsOptionData<EGsOptionAutoDecompose, bool>(EGsOptionAutoDecompose::EQUIP_MAGIC, (bool)(serverOption->GetAutoDecomposeOption().mIsEquipMagic)));

		_serverOptionAutoDecompose.Emplace(EGsOptionItem::OptionAutoDecomposeEquipRare,
			FGsOptionData<EGsOptionAutoDecompose, bool>(EGsOptionAutoDecompose::EQUIP_RARE, (bool)(serverOption->GetAutoDecomposeOption().mIsEquipRare)));

		_serverOptionAutoDecompose.Emplace(EGsOptionItem::OptionAutoDecomposeSkillBookNormal,
			FGsOptionData<EGsOptionAutoDecompose, bool>(EGsOptionAutoDecompose::SKILLBOOK_NORMAL, (bool)(serverOption->GetAutoDecomposeOption().mIsSkillBookNormal)));

		_serverOptionAutoDecompose.Emplace(EGsOptionItem::OptionAutoDecomposeSkillBookMagic,
			FGsOptionData<EGsOptionAutoDecompose, bool>(EGsOptionAutoDecompose::SKILLBOOK_MAGIC, (bool)(serverOption->GetAutoDecomposeOption().mIsSkillBookMagic)));

		_serverOptionAutoDecompose.Emplace(EGsOptionItem::OptionAutoDecomposeSkillBookRare,
			FGsOptionData<EGsOptionAutoDecompose, bool>(EGsOptionAutoDecompose::SKILLBOOK_RARE, (bool)(serverOption->GetAutoDecomposeOption().mIsSkillBookRare)));

		_serverOptionAutoDecompose.Emplace(EGsOptionItem::OptionAutoDecomposeSlateNormal,
			FGsOptionData<EGsOptionAutoDecompose, bool>(EGsOptionAutoDecompose::SLATE_NORMAL, (bool)(serverOption->GetAutoDecomposeOption().mIsMonsterKnowwledgeSlateNormal)));

		_serverOptionAutoDecompose.Emplace(EGsOptionItem::OptionAutoDecomposeSlateMagic,
			FGsOptionData<EGsOptionAutoDecompose, bool>(EGsOptionAutoDecompose::SLATE_MAGIC, (bool)(serverOption->GetAutoDecomposeOption().mIsMonsterKnowwledgeSlateMagic)));

		_serverOptionAutoDecompose.Emplace(EGsOptionItem::OptionAutoDecomposeSlateRare,
			FGsOptionData<EGsOptionAutoDecompose, bool>(EGsOptionAutoDecompose::SLATE_RARE, (bool)(serverOption->GetAutoDecomposeOption().mIsMonsterKnowwledgeSlateRare)));
	}
}

void UGsOptionManager::SetOptionWindowMenuIndex(int in_Index) 
{
	_optionWindowMenuIndex = in_Index;
}

void UGsOptionManager::SetOptionWindowDivisionIndex(int in_Index)
{
	_optionWindowDivisionIndex = in_Index;
}

void UGsOptionManager::SetScrollOffset(float in_Offset)
{
	_optionWindowScrollOffset = in_Offset;
}

int UGsOptionManager::GetOptionWindowMenuIndex()
{ 
	int returnValue = _optionWindowMenuIndex;
	_optionWindowMenuIndex = 0;

	return returnValue;
}

int UGsOptionManager::GetOptionWindowDivisionIndex()
{ 
	int returnValue = _optionWindowDivisionIndex;
	_optionWindowDivisionIndex = 0;

	return returnValue;
}

float UGsOptionManager::GetScrollOffset()
{ 
	float returnValue = _optionWindowScrollOffset;
	_optionWindowScrollOffset = 0.0f;

	return returnValue;
}
