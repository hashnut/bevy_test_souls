// Fill out your copyright notice in the Description page of Project Settings.

#include "GsHUDLocalPlayer.h"
#include "T1Project.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsCameraModeManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"

#include "UI/UILib/Define/GsUIDefine.h"
#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/HUD/GsUIHUDSkill.h"
#include "UI/UIContent/HUD/GsUIHUDPlayerInfo.h"
#include "UI/UIContent/HUD/GsUIHUDTargetInfo.h"
#include "UI/UIContent/HUD/GsUIHUDCamera.h"
#include "UI/UIContent/HUD/GsUIHUDConsumable.h"
#include "UI/UIContent/HUD/GsUIHUDQuest.h"
#include "UI/UIContent/HUD/GsUIHUDMinimap.h"
#include "UI/UIContent/HUD/GsUIHUDAuto.h"
#include "UI/UIContent/HUD/GsUIHUDSystemInfo.h"
#include "UI/UIContent/HUD/GsUIHUDSideTab.h"
#include "UI/UIContent/HUD/GsUIHUDChat.h"
#include "UI/UIContent/HUD/GsUIHUDSpiritShot.h"
//#include "UI/UIContent/HUD/GsUIHUDBless.h"
#include "UI/UIContent/HUD/GsUIHUDGainEffect.h"
#include "UI/UIContent/HUD/GsUIHUDCounterAttackButton.h"
#include "UI/UIContent/HUD/GsUIHUDSkillTargetSelect.h"
#include "UI/UIContent/Tray/GsUITrayAreaName.h"
#include "UI/UIContent/Tray/GsUITrayTickerLevelUp.h"
#include "UI/UIContent/Popup/GsUIPopupRevive.h"
#include "UI/GsAreaNameHUD.h"
#include "UI/UIContent/HUD/GsUIHUDArenaTime.h"
#include "UI/UIContent/HUD/GsUIHUDPreset.h"
#include "UI/UIContent/Tray/GsUITrayNavigationBar.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Stat/GsStatBase.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/Camera/GsCameraModeHandler.h"
#include "GameObject/Fence/GsFenceHandler.h"

#include "Item/GsItemManager.h"

#include "Util/GsTableUtil.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsStageMessageParam.h"
#include "Message/MessageParam/GsMinimapMessageParam.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Message/MessageParam/GsMonsterKnowledgeCollection.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"

#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Sound/GsSchemaSoundResData.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Skill/GsSkillUIHandler.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "Currency/GsCurrencyHelper.h"

//#define CRASH_TEST_LOOP
#ifdef CRASH_TEST_LOOP
#include "Net/GsNetSendService.h"
#include "Net/GsNetSendServiceWorld.h"
#endif
#include "DataSchema/KeyMapping/GsSchemaKeyMapping.h"
#include "Components/InputComponent.h"

#include "Sequence/GsSequenceManager.h"
#include "Sequence/GsSequencePlayer.h"
#include "GameObject/Fence/GsServerFence.h"
#include "DataSchema/Map/GsSchemaMapTerritoryInfo.h"
#include "GameObject/Fence/GsTownChaosFence.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "UTIL/GsLevelUtil.h"
#include "Guild/GsGuildHelper.h"


const FName LEVEL_UP_TRAY_NAME = TEXT("TrayTickerLevelUp");

void AGsHUDLocalPlayer::BeginPlay()
{
	Super::BeginPlay();

	FText::FindText(TEXT("InvadeText"), TEXT("InvadeAreaName"), _invasionTownFormatText);

	GMessage()->GetSystem().SendMessage(MessageSystem::AHUD_BEGIN_PLAY);

	Reset();
	
	_uiManager = GUI();
	if (_uiManager.IsValid())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = _uiManager->OpenAndGetWidget(TEXT("HUDMain"));
		if (widget.IsValid())
		{
			_hudWidget = Cast<UGsUIHUDFrameMain>(widget.Get());

#if PLATFORM_WINDOWS || WITH_EDITOR
			SetHudInput();
#endif
		}

		_uiManager->CloseByKeyName(TEXT("TrayNavigationBar"));
		_uiManager->SetHUDMode(EGsUIHUDMode::NORMAL);
		
	}	
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);

	_msgUIHandle = GMessage()->GetUI().AddUObject(MessageUI::SHOW_REVIVE, this, &AGsHUDLocalPlayer::OnShowRevive);

	InitMessageHandler();
	EnableHudInput();
}

void AGsHUDLocalPlayer::EndPlay(const EEndPlayReason::Type in_type)
{
	RemoveMessageHandler();

	if (_hudWidget.IsValid())
	{
		_hudWidget->Close();
	}

	if (_localPlayer.IsValid())
	{
		if (FGsCameraModeHandler* cameraModehandler = _localPlayer->GetCameraModeHandler())
		{
			cameraModehandler->OnEnterStateEvent().RemoveAll(this);
		}
	}

	if (_msgUIHandle.Value.IsValid())
	{
		GMessage()->GetUI().Remove(_msgUIHandle);
	}	

	if (FGsGameFlowManager* mode = GMode())
	{
		if (FGsGameFlowGame* gameFlow = mode->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				contents->OnEnterStateEvent().RemoveAll(this);
			}
		}
	}

	Super::EndPlay(in_type);
}

void AGsHUDLocalPlayer::PostRender()
{
	Super::PostRender();

	return;

	// 뽑기방 내에서 시네마틱 플레이는 ui flag 변경 안한다(이미 세팅된게 날라감...)
	FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler();
	if (summonHandler == nullptr)
	{
		return;
	}

	if (true == summonHandler->GetIsSummonPlay())
	{
		return;
	}	
}

void AGsHUDLocalPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AGsHUDLocalPlayer::Reset()
{
	_uiManager = nullptr;
	_hudWidget = nullptr;
	_localPlayer = nullptr;
	_changeStatHandle.Reset();
	_handleTargetDisplayData.Reset();	
}

void AGsHUDLocalPlayer::SetPlayer(class UGsGameObjectLocalPlayer* InPlayer)
{
	_localPlayer = InPlayer;

	// 주의: 이 타이밍엔 UGsGameObjectLocalPlayer 의 SubClass들이 제대로 세팅되지 않았다.
	// 각종 초기화는 OnLocalPlayerSpawnComplete 시 진행한다.
}

void AGsHUDLocalPlayer::BindingEvent()
{
	if (_localPlayer.IsValid())
	{		
		if (FGsCameraModeHandler* cameraModehandler = _localPlayer->GetCameraModeHandler())
		{
			cameraModehandler->OnEnterStateEvent().RemoveAll(this);
			cameraModehandler->OnEnterStateEvent().AddUObject(this, &AGsHUDLocalPlayer::OnChangeCamera);
		}

		if (FGsGameObjectDataCreature* creatureData = _localPlayer->GetCastData<FGsGameObjectDataCreature>())
		{
			if (TSharedPtr<FGsStatBase> base = creatureData->GetStatBase())
			{
				base->OnChangeStatDelegate.AddUObject(this, &AGsHUDLocalPlayer::OnSetLocalPlayerStat);
			}
		}
	}
}

void AGsHUDLocalPlayer::InitMessageHandler()
{
	FGsMessageHolder* msg = GMessage();
	// Invalidate Event
	// 삭제 코드 확인필요
	MHud& contentsHUD = msg->GetContentsHud();
	_hudHandlerList.Emplace(contentsHUD.AddUObject(MessageContentHud::INVALIDATE_ALL, 
		this, &AGsHUDLocalPlayer::InvalidateAll));
	_hudHandlerList.Emplace(contentsHUD.AddUObject(MessageContentHud::INVALIDATE_CONSUMEABLE,
		this, &AGsHUDLocalPlayer::InvalidateComsumable));
	_hudHandlerList.Emplace(contentsHUD.AddUObject(MessageContentHud::INVALIDATE_PLAYERINFO, 
		this, &AGsHUDLocalPlayer::InvalidatePlayerInfo));
	_hudHandlerList.Emplace(contentsHUD.AddUObject(MessageContentHud::INVALIDATE_QUEST,
		this, &AGsHUDLocalPlayer::InvalidateQuest));
	_hudHandlerList.Emplace(contentsHUD.AddUObject(MessageContentHud::INVALIDATE_SKILL,
		this, &AGsHUDLocalPlayer::InvalidateSkill));
	_hudHandlerList.Emplace(contentsHUD.AddUObject(MessageContentHud::INVALIDATE_SYSTEMINFO,
		this, &AGsHUDLocalPlayer::InvalidateSystemInfo));
	_hudHandlerList.Emplace(contentsHUD.AddUObject(MessageContentHud::INVALIDATE_CHAT,
		this, &AGsHUDLocalPlayer::InvalidateChat));
	_hudHandlerList.Emplace(contentsHUD.AddUObject(MessageContentHud::INVALIDATE_NPCFUNCTION,
		this, &AGsHUDLocalPlayer::InvalidateNPCFunction));
	_hudHandlerList.Emplace(contentsHUD.AddUObject(MessageContentHud::INVALIDATE_ARENA,
		this, &AGsHUDLocalPlayer::InvalidateArena));
	_hudHandlerList.Emplace(contentsHUD.AddUObject(MessageContentHud::INVALIDATE_COUNTERATTACK,
		this, &AGsHUDLocalPlayer::InvalidateCounterAttack));
	_hudHandlerList.Emplace(contentsHUD.AddUObject(MessageContentHud::INVALIDATE_SKILLTARGETSELECT,
		this, &AGsHUDLocalPlayer::InvalidateSkillTargetSelect));
	_hudHandlerList.Emplace(contentsHUD.AddUObject(MessageContentHud::INVALIDATE_PRESET,
		this, &AGsHUDLocalPlayer::InvalidatePreset));

	_itemDelegate = msg->GetItem().AddUObject(MessageItem::LOCKITEM, this, &AGsHUDLocalPlayer::OnLocalReaction);

	// Target HUD Event
	// 삭제 코드 확인필요
	MEnemyHud& enemyHUD = msg->GetEnemyHudInfo();
	_hudHandlerList.Emplace(enemyHUD.AddUObject(MessageContentHud::ACTIVE_ENEMYINFO,
		this, &AGsHUDLocalPlayer::OnEnableTargetHUD));
	_hudHandlerList.Emplace(enemyHUD.AddUObject(MessageContentHud::DEACTIVE_ENEMYINFO,
		this, &AGsHUDLocalPlayer::OnDisableTargetHUD));

	// LocalPlayer
	MUserBasicInfo& basicInfo = msg->GetUserBasicInfo();	
	_userInfoHandlerList.Emplace(basicInfo.AddUObject(MessageUserInfo::EXP,
		this, &AGsHUDLocalPlayer::OnChangePlayerExp));
	_userInfoHandlerList.Emplace(basicInfo.AddUObject(MessageUserInfo::INCREASE_EXP,
		this, &AGsHUDLocalPlayer::OnIncreasePlayerExp));
	_userInfoHandlerList.Emplace(basicInfo.AddUObject(MessageUserInfo::INCREASE_GOLD,
		this, &AGsHUDLocalPlayer::OnIncreaseGold));
	_userInfoHandlerList.Emplace(basicInfo.AddUObject(MessageUserInfo::INCREASE_MONSTER_BOOK_PT,
		this, &AGsHUDLocalPlayer::OnIncreaseMonsterBookPoint));
	_userInfoHandlerList.Emplace(basicInfo.AddUObject(MessageUserInfo::CURRENCY_MAX,
		this, &AGsHUDLocalPlayer::OnMaxCurrencyAmount));

	MMonsterKnowledgeCollection& monsterCollectionInfo = msg->GetMonsterKnowledgeCollection();
	_monsterKnowledeHandler = monsterCollectionInfo.AddUObject(MessageContentMonsterKnowledge::INVALIDATE_CODEX_MAP_GROUP_EXP_UPDATE, this, &AGsHUDLocalPlayer::OnIncreaseCodexMapGroupExp);


#ifdef R3_ARCAS_NOT_USE
	_userInfoHandlerList.Emplace(basicInfo.AddUObject(MessageUserInfo::BLESS_POINT,
		this, &AGsHUDLocalPlayer::OnChangeBlessPoint));
#endif
	MGameObject& msgObject = msg->GetGameObject();
	_gameObjectHandlerList.Emplace(msgObject.AddUObject(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, 
		this, &AGsHUDLocalPlayer::OnLocalPlayerSpawnComplete));
	_gameObjectHandlerList.Emplace(msgObject.AddUObject(MessageGameObject::LOCAL_REVIVE, 
		this, &AGsHUDLocalPlayer::OnLocalPlayerRevive));

	// UI	
	// 삭제 코드 확인필요
	_hudHandlerList.Emplace(msg->GetAreaTriggerEvent().AddUObject(MessageContentHud::AREA_TRIGGER_BEGIN,
		this, &AGsHUDLocalPlayer::OnShowAreaName));
	_hudHandlerList.Emplace(msg->GetAreaTriggerEvent().AddUObject(MessageContentHud::AREA_TRIGGER_END,
		this, &AGsHUDLocalPlayer::OnAreaNameLeave));	

	MUI& msgUI = msg->GetUI();
	_msgUIHandlerList.Emplace(msgUI.AddUObject(MessageUI::CHANGE_HUD_MODE,
		this, &AGsHUDLocalPlayer::OnChangeHUDMode));
	_msgUIHandlerList.Emplace(msgUI.AddUObject(MessageUI::LEVEL_UP,
		this, &AGsHUDLocalPlayer::OnChangeLocalPlayerLevel));

	_msgUIHandlerList.Emplace(msgUI.AddUObject(MessageUI::KEYBOARD_MAPPING_HUD_ENABLE,
		this, &AGsHUDLocalPlayer::OnChangeInputProcessEnable));

	//////////////////////////////////////////////// 빠져야되는 이벤트 ////////////////////////////////////////////////////////////////////////////////////
	// ai off 시 예약 클리어
	_gameObjectHandlerList.Emplace(msgObject.AddUObject(MessageGameObject::AI_MODE_ACTIVE,
		this, &AGsHUDLocalPlayer::OnActiveAI));
	_gameObjectHandlerList.Emplace(msgObject.AddUObject(MessageGameObject::AI_QUEST_AUTO_ACTIVE,
		this, &AGsHUDLocalPlayer::OnActiveAutoQuest));

	_gameObjectHandlerList.Emplace(msgObject.AddUObject(MessageGameObject::PARTY_HUD_SIDE_TAB_UPDATE,
		this, &AGsHUDLocalPlayer::OnPartyHUDSideTabUpdate));

	_gameObjectHandlerList.Emplace(msgObject.AddUObject(MessageGameObject::UPDATE_PARTY_MEMBER,
		this, &AGsHUDLocalPlayer::OnPartyMemberUpdate));

	_gameObjectHandlerList.Emplace(msgObject.AddUObject(MessageGameObject::DEBUG_AI_RESERVE_LIST_SHOW,
		this, &AGsHUDLocalPlayer::OnDebugAIReserveListShow));

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	MStageParam& msgStage = msg->GetStageParam();
	_msgStageHandleList.Emplace(msgStage.AddUObject(MessageStage::ENTER_TOWN_CALLBACK,
		this, &AGsHUDLocalPlayer::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddUObject(MessageStage::LEAVE_TOWN_CALLBACK,
		this, &AGsHUDLocalPlayer::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddUObject(MessageStage::ENTER_CHAOS_CALLBACK,
		this, &AGsHUDLocalPlayer::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddUObject(MessageStage::LEAVE_CHAOS_CALLBACK,
		this, &AGsHUDLocalPlayer::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddUObject(MessageStage::ENTER_SAFETY_CALLBACK,
		this, &AGsHUDLocalPlayer::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddUObject(MessageStage::LEAVE_SAFETY_CALLBACK,
		this, &AGsHUDLocalPlayer::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddUObject(MessageStage::ENTER_ARENA_READY_CALLBACK, // 추가하는 거 맞는지 재확인 필요
		this, &AGsHUDLocalPlayer::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddUObject(MessageStage::LEAVE_ARENA_READY_CALLBACK,
		this, &AGsHUDLocalPlayer::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddUObject(MessageStage::ENTER_ARENA_CALLBACK,
		this, &AGsHUDLocalPlayer::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddUObject(MessageStage::LEAVE_ARENA_CALLBACK,
		this, &AGsHUDLocalPlayer::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddUObject(MessageStage::ENTER_NON_PK_CALLBACK,
		this, &AGsHUDLocalPlayer::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddUObject(MessageStage::LEAVE_NON_PK_CALLBACK,
		this, &AGsHUDLocalPlayer::OnChangeSafetyMode));

}

void AGsHUDLocalPlayer::RemoveMessageHandler()
{
	FGsMessageHolder* message = GMessage();

	//bak1210 : 같은 타입으로 묶어서 핸들링
	for (MsgHudHandle& msgHandler : _hudHandlerList)
	{
		switch (msgHandler.Key)
		{
		case MessageContentHud::INVALIDATE_ALL:
		case MessageContentHud::INVALIDATE_CONSUMEABLE:
		case MessageContentHud::INVALIDATE_PLAYERINFO:
		case MessageContentHud::INVALIDATE_QUEST:
		case MessageContentHud::INVALIDATE_SKILL:
		case MessageContentHud::INVALIDATE_SYSTEMINFO:
		case MessageContentHud::INVALIDATE_CHAT:
			message->GetContentsHud().Remove(msgHandler);
			break;
		case MessageContentHud::ACTIVE_ENEMYINFO:
		case MessageContentHud::DEACTIVE_ENEMYINFO:
			message->GetEnemyHudInfo().Remove(msgHandler);
			break;
		case MessageContentHud::AREA_TRIGGER_BEGIN:
		case MessageContentHud::AREA_TRIGGER_END:
			message->GetAreaTriggerEvent().Remove(msgHandler);
			break;
		default:
			message->GetContentsHud().RemoveIF(msgHandler);
			message->GetEnemyHudInfo().RemoveIF(msgHandler);
			message->GetAreaTriggerEvent().RemoveIF(msgHandler);
			break;
		}
	}
	_hudHandlerList.Empty();


	for (UserInfoHandler& msgHandler : _userInfoHandlerList)
	{
		message->GetUserBasicInfo().Remove(msgHandler);
	}
	_userInfoHandlerList.Empty();

	message->GetMonsterKnowledgeCollection().Remove(_monsterKnowledeHandler);

	for (MsgGameObjHandle& msgHandler : _gameObjectHandlerList)
	{
		message->GetGameObject().Remove(msgHandler);
	}
	_gameObjectHandlerList.Empty();
	

	for (MsgUIHandle& msgHandler : _msgUIHandlerList)
	{
		message->GetUI().Remove(msgHandler);
	}
	_msgUIHandlerList.Empty();

	MStageParam& msgStage = message->GetStageParam();
	for (MsgStageHandle& msgStageHandle : _msgStageHandleList)
	{
		msgStage.Remove(msgStageHandle);
	}
	_msgStageHandleList.Empty();

	message->GetItem().Remove(_itemDelegate);
}

void AGsHUDLocalPlayer::OnLocalPlayerSpawnComplete(const IGsMessageParam*)
{
	// 각종 초기화 진행
	BindingEvent();

#if WITH_EDITOR
	if (APlayerController* playerController = GetWorld()->GetFirstPlayerController())
	{
		playerController->CreateTouchInterface();
	}
#endif // WITH_EDITOR

#if !REVIVE_RECONNECT_TEST
	// UI 전부 켜기
	if (_uiManager.IsValid())
	{
		bool bIsPlayingCinematic = false;
		if (UGsSequencePlayer* cinematic = GSequencePlayer())
		{
			bIsPlayingCinematic = cinematic->IsPlaying();
		}

		if (false == bIsPlayingCinematic)
		{
			_uiManager->ClearHideFlags(EGsUIHideFlags::UI_HIDE_ALL);
		}
		else
		{
			// R3 시네마틱 위로 HUD가 뜨는 이슈 추적위해 로그 삽입
			GSLOG(Warning, TEXT("[TEST_HIDE_FLAG] AGsHUDLocalPlayer::OnLocalPlayerSpawnComplete. bIsPlayingCinematic true."));
		}
	}
#endif

	if (FGsUnlockManager* unlockMgr = GSUnlock())
	{
		if (GGameData()->IsInvadeWorld())
		{
			unlockMgr->LockContentsByClient(UnlockCategory::INVADE);
		}
		else
		{
			unlockMgr->UnlockContentsByClient(UnlockCategory::INVADE);
		}

		if (FGsGuildHelper::IsInAgit())
		{
			unlockMgr->LockContentsByClient(UnlockCategory::AGIT);
		}
		else
		{
			unlockMgr->UnlockContentsByClient(UnlockCategory::AGIT);
		}
	}

	InvalidateAll();

#ifdef CRASH_TEST_LOOP
	UWorld* world = GUI()->GetWorld();
	if (world == nullptr)
	{
		return;
	}

	FTimerDelegate timerCallback;
	timerCallback.BindLambda([] {
		int randVal = FMath::RandRange(0, 1);
		if (randVal == 0)
		{
			FGsNetSendService::SendGatewayReturnToLobby();
		}
		else
		{
			FString sendCMD(TEXT("killme"));
			FGsNetSendService::SendWorldAdminCommandQA(sendCMD);
		}
	});

	FTimerHandle testCrashTimeHandle;
	world->GetTimerManager().SetTimer(testCrashTimeHandle,
		timerCallback, 2.0f, false);
#endif
}

void AGsHUDLocalPlayer::OnChangeLocalPlayerLevel(const IGsMessageParam* inParam)
{
	const FGsUIMsgParamLevelUp* msg = inParam->Cast<const FGsUIMsgParamLevelUp>();
	_preLevel = msg->_preLevel;
	_currentLevel = msg->_currentLevel;

	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_TRAY_LEVEL_UP, this);
	
	// 레벨업 티커 뒤에 언락 티커가 발생할 수 있도록 이곳에서 업데이트 시도
	GSUnlock()->UpdateLockedContentsByLevel();

	// HUD 하단 HP바도 반영을 위해 갱신
	InvalidatePlayerInfo();
}

void AGsHUDLocalPlayer::OnChangePlayerExp(uint64 InExp)
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDSystemInfo* systemInfo = _hudWidget->GetSystemInfo())
		{
			systemInfo->InvalidateExp();
		}
	}
}

void AGsHUDLocalPlayer::OnIncreasePlayerExp(uint64 InExp)
{
	if (_hudWidget.IsValid())
	{
		if (_hudWidget->IsVisible())
		{
			if (UGsUIHUDGainEffect* gainEffect = _hudWidget->GetGainExpEffect())
			{
				gainEffect->SetExp(InExp);
			}
		}
	}
}

void AGsHUDLocalPlayer::OnIncreaseGold(uint64 InAmount)
{
	if (_hudWidget.IsValid())
	{
		if (_hudWidget->IsVisible())
		{
			if (UGsUIHUDGainEffect* gainEffect = _hudWidget->GetGainGoldEffect())
			{
				gainEffect->SetGold(InAmount);
			}
		}
	}
}

void AGsHUDLocalPlayer::OnIncreaseMonsterBookPoint(uint64 InIncreasePoint)
{
	if (_hudWidget.IsValid())
	{
		if (_hudWidget->IsVisible())
		{
			if (UGsUIHUDGainEffect* gainEffect = _hudWidget->GetGainMonsterKnowledgeEffect())
			{	// 2021/12/22 PKT - 지식 포인트 이펙트 수정 이슈로 주석 처리( 추후 개선 )
				//gainEffect->SetMonsterBookPoint(InIncreasePoint);
			}
		}
	}
}

void AGsHUDLocalPlayer::OnIncreaseCodexMapGroupExp(const IGsMessageParam* InParam)
{
	const FGsMessageCodexMapGroupExpUpdate* paramData = static_cast<const FGsMessageCodexMapGroupExpUpdate*>(InParam);
	if (nullptr == paramData)
		return;

	if (_hudWidget.IsValid())
	{
		if (_hudWidget->IsVisible())
		{
			if (UGsUIHUDGainEffect* gainEffect = _hudWidget->GetGainMonsterKnowledgeEffect())
			{	
				gainEffect->SetMonsterBookPoint(paramData->_monsterKnowledgeExp);
			}
		}
	}
}

void AGsHUDLocalPlayer::OnMaxCurrencyAmount(uint64 InCurrencyType)
{
	// TEXT: {0}이/가 최대입니다.
	FText textFormat;
	if (FText::FindText(TEXT("CurrencyText"), TEXT("MaxCurrencyAmount"), textFormat))
	{
		FText textCurrencyName = FGsCurrencyHelper::GetCurrencyLocalizedText(static_cast<CurrencyType>(InCurrencyType));

		FText msg = FText::Format(textFormat, textCurrencyName);
		// 티커 출력
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, msg);
	}
}

#ifdef R3_ARCAS_NOT_USE
void AGsHUDLocalPlayer::OnChangeBlessPoint(uint64 InBlessPoint)
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDBless* blessHud = _hudWidget->GetBlessHud())
		{
			blessHud->OnChangedBlessPoint(InBlessPoint);
		}
	}
}
#endif

void AGsHUDLocalPlayer::OnSetLocalPlayerStat(const FGsStatInfo& InStatInfo)
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDPlayerInfo* playerInfo = _hudWidget->GetPlayerInfo())
		{
			playerInfo->SetStat(InStatInfo);
		}
	}
}

void AGsHUDLocalPlayer::InvalidateComsumable()
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDConsumable* consumableMenu = _hudWidget->GetConsumableMenu())
		{
			consumableMenu->InvalidateAll();
		}

		if (UGsUIHUDSpiritShot* spiritShot = _hudWidget->GetSpiritShot())
		{
			spiritShot->InvalidateAll();
		}
	}
}

void AGsHUDLocalPlayer::OnEnableTargetHUD(TargetHUD& InTargetInfo)
{
	//ShowTargetInfo(&InTargetInfo);

	if (InTargetInfo._target.IsValid())
	{
		if (UGsGameObjectCreature* targetCreature = InTargetInfo._target.Get())
		{
			if (FGsGameObjectDataCreature* creatureData = targetCreature->GetCastData<FGsGameObjectDataCreature>())
			{
				if (TSharedPtr<FGsStatBase> statBase = creatureData->GetStatBase())
				{
					_changeStatHandle = statBase->OnChangeStatDelegate.AddUObject(this, 
						&AGsHUDLocalPlayer::OnChangeTargetStat);
				}
				
				if (_hudWidget.IsValid())
				{
					if (UGsUIHUDTargetInfo* targetInfo = _hudWidget->GetTargetInfo())
					{
						targetInfo->SetData(&InTargetInfo);
					}
				}
			}
		}
	}

	if (_hudWidget.IsValid())
	{
		_hudWidget->CheckVisibleNavigationBar();
	}
}

void AGsHUDLocalPlayer::OnDisableTargetHUD(TargetHUD& InTargetInfo)
{
	if (InTargetInfo._target.IsValid())
	{
		if (UGsGameObjectCreature* targetCreature = InTargetInfo._target.Get())
		{
			if (FGsGameObjectDataCreature* creatureData = targetCreature->GetCastData<FGsGameObjectDataCreature>())
			{
				if (TSharedPtr<FGsStatBase> statBase = creatureData->GetStatBase())
				{
					statBase->OnChangeStatDelegate.Remove(_changeStatHandle);
				}
			}
		}
	}


	if (_hudWidget.IsValid()) 
	{
		if (UGsUIHUDTargetInfo* targetInfo = _hudWidget->GetTargetInfo())
		{
			targetInfo->SetData(nullptr);
		}

		_hudWidget->CheckVisibleNavigationBar();
	}
}

void AGsHUDLocalPlayer::OnChangeTargetStat(const FGsStatInfo& InStatInfo)
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDTargetInfo* targetInfo = _hudWidget->GetTargetInfo())
		{
			targetInfo->SetStat(InStatInfo, true);
		}
	}
}

void AGsHUDLocalPlayer::OnChangeCamera(EGsCameraMode InMode)
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDCamera* cameraUI = _hudWidget->GetCamera())
		{
			cameraUI->SetCameraTypeText(InMode);
		}
	}
}

void AGsHUDLocalPlayer::OnShowAreaName(AGsAreaNameHUD& InAreaInfo)
{	
	SetMiniMapAreaName(InAreaInfo.GetAreaTopName());
	OpenDisplayAreaUI(InAreaInfo.GetAreaTopName());
}
// 지역명 나감
void AGsHUDLocalPlayer::OnAreaNameLeave(AGsAreaNameHUD& InAreaInfo)
{
	ShowCurrentMapInfo();
}
// 현재 맵 기준으로 출력
void AGsHUDLocalPlayer::ShowCurrentMapInfo()
{
	//펜스에 겹치지 않을 경우 AreaName actor가 존재하면 미니맵에 AreaNameHUD 정보 보여줄 것
	//AreaName도 없다면 미니맵에 맵 이름 표시 
	TArray<AGsAreaNameHUD*> areaNameArray;
	if (UGsLevelUtil::TryGetAllOverlappedAreaName(GetWorld(), areaNameArray))
	{
		return;
	}

	SetMinimapDefaultMapName();
	OpenDisplayAreaUICurrentMap();
}

void AGsHUDLocalPlayer::OpenDisplayAreaUI(const FString& InTopName)
{
	_backupTextAreaName = FText::FromString(InTopName);
	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_TRAY_AREA_NAME, this);
}

void AGsHUDLocalPlayer::OpenDisplayAreaUICurrentMap()
{
	int mapId = GLevel()->GetCurrentLevelId();
	const FGsSchemaMapData* mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(mapId);
	if (mapData == nullptr)
	{
		return;
	}

	_backupTextAreaName = mapData->MapStringId;
	
	if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
	{
		eventProgressManager->OnReqStartEvent(EGsEventProgressType::UI_TRAY_AREA_NAME, this);
	}

}

void AGsHUDLocalPlayer::OnShowRevive(const IGsMessageParam*)
{
	if (false == _uiManager.IsValid())
	{
		GSLOG(Error, TEXT("%s"), TEXT("AGsHUDLocalPlayer::OnLocalPlayerDying - ui manager is invalid"));
		return;
	}
	// 현재 맵 타입
	MapType currentMapType = GLevel()->GetCurrentMapType(); 
	if (MapType::PARTY_DUNGEON == currentMapType)
	{	// 2023/3/15 PKT - 파티 던전에서는 사망 창을 파티 던전 결과 창으로 대처 하기 위해 해당 이벤트 Skip..
		// https://jira.com2us.com/jira/browse/C2URWQ-3089
		return;
	}

	_uiManager->CloseAllStack();
	
	// 재접이나 기타 상황을 고려해 한 번더 체크
	// 사망시엔 모든 Flag를 풀어준 후 사망UI 출력
	_uiManager->SetHUDMode(EGsUIHUDMode::NORMAL);
	_uiManager->ClearHideFlags(EGsUIHideFlags::UI_HIDE_ALL);
	_uiManager->SetHideFlags(EGsUIHideFlags::STATE_DIE);

	// 현재 맵이 기사단 던전일때 남은시간 텍스트 표시
	// https://jira.com2us.com/jira/browse/CHR-24638


	UGsUIWidgetBase* widget = GUI()->OpenAndGetWidget(POPUP_REVIVE_NAME).Get();
	UGsUIPopupRevive* reviveWidget = Cast<UGsUIPopupRevive>(widget);
	if (reviveWidget)
	{
		reviveWidget->ShowRevivePopup(false);
		// 길드 던전 퇴장 텍스트 세팅
		reviveWidget->ShowGuildDungeonKickRemainTimeText((currentMapType == MapType::GUILD_DUNGEON)? true: false);
	}
}

void AGsHUDLocalPlayer::OnLocalReaction(struct FGsItemMessageParamBase& InParam)
{
	FGsReactionItemLock* paramData = static_cast<FGsReactionItemLock*>(&InParam);
	if (nullptr == paramData)
		return;

	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDConsumable* consumable = _hudWidget->GetConsumableMenu())
		{
			const FGsItem* itemData = consumable->GetIconItemData();
			consumable->SetEnableButton(!((paramData->_isLock) && (itemData) && (INDEX_NONE != paramData->_lockItemCategorySubList.Find(itemData->GetSubCategory()))));
		}
	}

	/*
	const FGsGameObjectMessageParamReactionLocalPlayer* paramAbnormal =
		InParam->Cast<const FGsGameObjectMessageParamReactionLocalPlayer>();

	if (nullptr == paramAbnormal)
	{
		GSLOG(Error, TEXT("[Stun] paramAbnormal->_data is null."));
		return;
	}

	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDConsumable* consumable = _hudWidget->GetConsumableMenu())
		{
			consumable->SetEnableButton(!paramAbnormal->IsLock(CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT));
		}
	}
	*/
}

void AGsHUDLocalPlayer::OnChangeSafetyMode(const IGsMessageParam* InAreaInfo)
{
	const FGsStageFenceMsgParam* param = InAreaInfo->Cast<const FGsStageFenceMsgParam>();
	if (nullptr == param)
	{
		return;
	}

	const FGsFence* fence = param->_fence;
	if (fence)
	{
		const FGsServerFence* serverFence = static_cast<const FGsServerFence*>(fence);
		if (serverFence)
		{
			const struct FGsSchemaMapTerritoryInfo* territory = serverFence->GetTerritoryData();			
			switch (fence->GetTerritoryType())
			{
			case TerritoryType::TOWN_CHAOS:
			{
				const FGsTownChaosFence* townFence = static_cast<const FGsTownChaosFence*>(serverFence);
				if (townFence)
				{
					if (townFence->IsChaos())
					{
						InvalidChaosUI(territory);
					}
					else
					{
						InvalidTownUI(territory);
					}
				}
			}
			break;
			case TerritoryType::ARENA:
			case TerritoryType::ARENA_READY:
			{
				SetMiniMapAreaName(territory->name.ToString());
				OpenDisplayAreaUI(territory->name.ToString());
			}
			break;
			case TerritoryType::NON_PK:
			case TerritoryType::ARRIVAL:			
			{
				ShowCurrentMapInfo();
			}
			break;
			case TerritoryType::TOWN:
			case TerritoryType::SAFE_ZONE:
			{
				InvalidTownUI(territory);
			}
			break;
			}
		}
	}	
	else
	{		
		ShowCurrentMapInfo();				
	}

	InvalidateSkill();
}


//void AGsHUDLocalPlayer::OnEnableReviveUI(int32 InParam)
//{
//	_bEnableReviveUI = (0 < InParam) ? true : false;
//
//	// 열려있는 것이 있다면 닫는다.
//	if (_uiManager.IsValid())
//	{
//		TWeakObjectPtr<UGsUIWidgetBase> popup = _uiManager->GetWidgetByKey(POPUP_REVIVE_NAME);
//		if (popup.IsValid()) 
//		{
//			popup->Close();
//		}
//	}
//}

// 0: auto ui off, 1: auto ui on
void AGsHUDLocalPlayer::OnAutoUIChange(int32 In_param)
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDAuto* autoUI = _hudWidget->GetAuto())
		{
			autoUI->SetOnOffState((bool)In_param);
		}
	}
}

void AGsHUDLocalPlayer::OnChangeHUDMode(const IGsMessageParam* InParam)
{
	const FGsUIMsgParamInt* param = InParam->Cast<const FGsUIMsgParamInt>();
	if (param)
	{
		EGsUIHUDMode hudMode = static_cast<EGsUIHUDMode>(param->_data);

		if (_hudWidget.IsValid())
		{
			_hudWidget->SetHUDMode(hudMode);
		}
	}
}

void AGsHUDLocalPlayer::OnLocalPlayerRevive(const IGsMessageParam*)
{
	// 주의: 제자리 부활일 때만 여기에 들어온다.

	// 사망으로 Hide된 UI를 켜준다.
	if (UGsUIManager* uiManager = GUI())
	{		
		uiManager->ClearHideFlags(EGsUIHideFlags::UI_HIDE_ALL);
	}

	InvalidateAll();
}

void AGsHUDLocalPlayer::InvalidateAll()
{
	if (_hudWidget.IsValid())
	{
		// 경우에 따라 풀린 HUDMode를 재설정한다.
		if (UGsUIManager* uiManager = GUI())
		{
			_hudWidget->SetHUDMode(uiManager->GetHUDMode());
		}

		_hudWidget->InvalidateAllMenu();
	}
}

void AGsHUDLocalPlayer::InvalidatePlayerInfo()
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDPlayerInfo* playerInfo = _hudWidget->GetPlayerInfo())
		{
			playerInfo->InvalidateAll();
		}
	}
}

void AGsHUDLocalPlayer::InvalidateQuest()
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDQuest* questUI = _hudWidget->GetQuest())
		{
			questUI->InvalidateAll();
		}
	}
}

void AGsHUDLocalPlayer::InvalidateSkill()
{
	if (false == _hudWidget.IsValid())
	{
		return;
	}

	if (UGsUIHUDSkill* skillUI = _hudWidget->GetSkill())
	{
		skillUI->InvalidateAll();
	}
}

void AGsHUDLocalPlayer::InvalidateCamera()
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDCamera* cameraUI = _hudWidget->GetCamera())
		{
			cameraUI->InvalidateAll();
		}
	}
}

void AGsHUDLocalPlayer::InvalidateSystemInfo()
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDSystemInfo* systemInfoUI = _hudWidget->GetSystemInfo())
		{
			systemInfoUI->InvalidateAll();
		}
	}
}

void AGsHUDLocalPlayer::OnActiveAI(const IGsMessageParam* In_isActive)
{
	const FGsPrimitiveInt32* param = In_isActive->Cast<const FGsPrimitiveInt32>();
	bool isActive = static_cast<bool>(param->_data);
	// ai 끄면 예약 초기화
	if (false == isActive)
	{
		//ClearCurrentReservedSlot();
	}
	// ui 갱신
	OnAutoUIChange(param->_data);
}

void AGsHUDLocalPlayer::OnActiveAutoQuest(const IGsMessageParam* In_isActive)
{
	const FGsPrimitiveInt32* param = In_isActive->Cast<const FGsPrimitiveInt32>();
	bool isActive = static_cast<bool>(param->_data);

	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDAuto* autoUI = _hudWidget->GetAuto())
		{
			autoUI->SetQuestAutoState(isActive);
		}
	}
}

void AGsHUDLocalPlayer::InvalidTownUI(const struct FGsSchemaMapTerritoryInfo* inTerritory)
{
#if INVASION_TOWN_BATTLE_DEBUG
	GSLOG(Log, TEXT("invasion town - show town ui "));
#endif

	SetMiniMapAreaName(inTerritory->name.ToString());
	OpenDisplayAreaUI(inTerritory->name.ToString());
}

void AGsHUDLocalPlayer::InvalidChaosUI(const struct FGsSchemaMapTerritoryInfo* inTerritory)
{
#if INVASION_TOWN_BATTLE_DEBUG
	GSLOG(Log, TEXT("invasion town - show chaos ui "));
#endif

	FText invasionTownText = FText::Format(_invasionTownFormatText, inTerritory->name);
	SetMiniMapAreaName(inTerritory->name.ToString());	
	OpenDisplayAreaUI(invasionTownText.ToString());
}

void AGsHUDLocalPlayer::SetMiniMapAreaName(const FString& InTopName)
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDMinimap* miniMapUI = _hudWidget->GetMiniMap())
		{
			miniMapUI->SetAreaNameText(FText::FromString(InTopName));
		}
	}
}

void AGsHUDLocalPlayer::SetMiniMapDebugUI(bool inIsActive)
{

	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDMinimap* miniMapUI = _hudWidget->GetMiniMap())
		{
			miniMapUI->SetActiveDebugList(inIsActive);
		}
	}

}

// 트리거 나가서 기본 맵이름 출력
void AGsHUDLocalPlayer::SetMinimapDefaultMapName()
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDMinimap* miniMapUI = _hudWidget->GetMiniMap())
		{
			miniMapUI->SetDefaultMapName();
		}
	}
}

// 파티 HUD sidetab 갱신
void AGsHUDLocalPlayer::OnPartyHUDSideTabUpdate(const IGsMessageParam*)
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDSideTab* sideTab = _hudWidget->GetSideTab())
		{
			sideTab->InvalidateAll();
		}
	}
}

//채팅 Hud 갱신
void AGsHUDLocalPlayer::InvalidateChat()
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDChat* chatHud = _hudWidget->GetHudChat())
		{
			chatHud->OnChatAdded();
		}
	}
}

// side tab 액티브
void AGsHUDLocalPlayer::SetActiveSideTab(UGsUIHUDSideTab::EGsHUDSideTabType In_type)
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDSideTab* sideTab = _hudWidget->GetSideTab()) 
		{
			EGsUIHUDMode currentHudMode = GUI()->GetHUDMode();
			if (false == sideTab->IsHideMode(currentHudMode))
			{
				sideTab->SetSelectedIndex(In_type);
			}
		}
	}
}

void AGsHUDLocalPlayer::DrawHUD()
{
	Super::DrawHUD();

	FGsInteractionManager* interactionManager = GSInteraction();
	if (interactionManager == nullptr)
	{
		return;
	}
	if (true == interactionManager->GetTestShowInteractionButtonPos())
	{

		FVector2D touchScreenPos = GSInteraction()->GetTestTouchScreenPos();
		DrawRect(FLinearColor::Yellow, touchScreenPos.X, touchScreenPos.Y, 50.0f, 50.0f);

		FVector2D widgetShiftPos = GSInteraction()->GetTestWidgetShiftPos();
		FVector2D widgetSize = GSInteraction()->GetTestWidgetSize();

		DrawRect(FLinearColor::Blue, widgetShiftPos.X, widgetShiftPos.Y, widgetSize.X, widgetSize.Y);
	}
}


void AGsHUDLocalPlayer::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{
	if (false == _uiManager.IsValid())
	{
		return;
	}

	switch (In_startType)
	{
	case EGsEventProgressType::UI_TRAY_AREA_NAME:
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = _uiManager->OpenAndGetWidget(TEXT("TrayArea"));
		if (widget.IsValid())
		{
			UGsUITrayAreaName* areaWidget = Cast<UGsUITrayAreaName>(widget);
			if (nullptr != areaWidget)
			{
				areaWidget->SetAreaName(_backupTextAreaName);
			}
		}
	}
	break;
	case EGsEventProgressType::UI_TRAY_LEVEL_UP:
	{
		if (_currentLevel > _preLevel)
		{
			if (_uiManager->IsExistInQueue(LEVEL_UP_TRAY_NAME, 0))
			{
				_uiManager->CloseByKeyName(LEVEL_UP_TRAY_NAME);
			}
			_uiManager->PushQueue(LEVEL_UP_TRAY_NAME, 0, [=](UGsUIWidgetBase* InWidget)
				{
					UGsUITrayTickerLevelUp* levelUpWidget = Cast<UGsUITrayTickerLevelUp>(InWidget);
					if (levelUpWidget)
					{
						levelUpWidget->SetData(_preLevel, _currentLevel);
					}
				});
		}
	}
		break;
	}
		
}
void AGsHUDLocalPlayer::FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType)
{
	if(false == _uiManager.IsValid())
	{
		return;
	}
	
	switch(In_finishType)
	{
	case EGsEventProgressType::UI_TRAY_AREA_NAME:
	{
		_uiManager->CloseByKeyName(TEXT("TrayArea"));
		break;
	}
	case EGsEventProgressType::UI_TRAY_LEVEL_UP:
	{
		_uiManager->CloseByKeyName(LEVEL_UP_TRAY_NAME);
	}
		break;		
	}
}

void AGsHUDLocalPlayer::UpdateTargetShowHPBar()
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDTargetInfo* targetInfo = _hudWidget->GetTargetInfo())
		{
			targetInfo->UpdateShowHPBar();
		}
	}
}

void AGsHUDLocalPlayer::SetHudInput()
{
	if (InputComponent == nullptr)
	{
		_HudInputComponent = NewObject<UInputComponent>(this);
		_SystemInputComponent = NewObject<UInputComponent>(this);

		InputComponent = _HudInputComponent;

		TArray<const FGsSchemaKeyMapping*> schemaKeyMappingList;
		if (const UGsTableKeyMapping* table = Cast<UGsTableKeyMapping>(FGsSchemaKeyMapping::GetStaticTable()))
		{
			table->GetAllRows<FGsSchemaKeyMapping>(schemaKeyMappingList);

			const FGsSchemaKeyMapping* systemKeyMapData = nullptr;
			if (table->FindRowById(EGsKeyMappingType::KEY_MAPPING_NO, systemKeyMapData))
			{
				for (auto iter : systemKeyMapData->keyInputEvents)
				{
					_SystemInputComponent->BindAction<TDelegate<void(EGsKeyMappingType, EInputEvent)>>(systemKeyMapData->keyName, iter.GetValue(),
						_hudWidget.Get(), &UGsUIHUDFrameMain::OnKeyboardSystemActionKey, systemKeyMapData->keyMappingType, iter.GetValue());
				}
			}
		}

		if (_hudWidget.IsValid())
		{
			for (const FGsSchemaKeyMapping* schemaKeyMapping : schemaKeyMappingList)
			{
				if (nullptr == schemaKeyMapping)
					continue;

				for (auto& e : schemaKeyMapping->keyInputEvents)
				{
					if (EGsOptionKeyBindType::Action == schemaKeyMapping->keyBindType)
					{
						_HudInputComponent->BindAction<TDelegate<void(EGsKeyMappingType, EInputEvent)>>(schemaKeyMapping->keyName, e.GetValue(),
							_hudWidget.Get(), &UGsUIHUDFrameMain::OnKeyboardActionOptionKey, schemaKeyMapping->keyMappingType, e.GetValue());
					}
				}
			}
		}

		if (_isInputEnable) EnableHudInput();
	}
}

void AGsHUDLocalPlayer::EnableHudInput()
{
#if PLATFORM_WINDOWS || WITH_EDITOR
	_isInputEnable = true;
	if (InputComponent)
	{
		if (auto controller = GetWorld()->GetFirstPlayerController())
		{
			DisableInput(controller);

			InputComponent = _HudInputComponent;
			EnableInput(controller);
		}
	}
#endif
}

void AGsHUDLocalPlayer::DisableHudInput()
{
#if PLATFORM_WINDOWS || WITH_EDITOR
	_isInputEnable = false;
	if (InputComponent)
	{
		if (auto controller = GetWorld()->GetFirstPlayerController())
		{
			DisableInput(controller);

			InputComponent = _SystemInputComponent;
			EnableInput(controller);
		}
	}
#endif
}

void AGsHUDLocalPlayer::OnChangeInputProcessEnable(const IGsMessageParam* InParam)
{
	if (const FGsUIMsgParamInputEnable* param = InParam->Cast<const FGsUIMsgParamInputEnable>())
	{
#if PLATFORM_WINDOWS || WITH_EDITOR
		if (param->_isEnable) 
			 EnableHudInput();
		else DisableHudInput();
#endif
	}
}

void AGsHUDLocalPlayer::OnPartyMemberUpdate(const IGsMessageParam* InParam)
{
	const FGsUIMapPartyMemberParam* paramTarget =
		InParam->Cast<const FGsUIMapPartyMemberParam>();

	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDTargetInfo* targetInfo = _hudWidget->GetTargetInfo())
		{
			targetInfo->UpdatePartyMember(paramTarget);
		}
	}
}

void AGsHUDLocalPlayer::InvalidateNPCFunction()
{
	if (false == _hudWidget.IsValid())
	{
		return;
	}

	if (UGsUIHUDSkill* skillUI = _hudWidget->GetSkill())
	{
		skillUI->InvalidateAll();
	}
}

void AGsHUDLocalPlayer::InvalidateArena()
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDArenaTime* arenaUI = _hudWidget->GetArenaTime())
		{
			arenaUI->InvalidateArena();
		}
	}
}
void AGsHUDLocalPlayer::InvalidateCounterAttack()
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDCounterAttackButton* counterAttackButtonUI = _hudWidget->GetCounterAttackButton())
		{
			counterAttackButtonUI->UpdateHudMode();
		}
	}
}

void AGsHUDLocalPlayer::InvalidateSkillTargetSelect()
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDSkillTargetSelect* skillTargetSelect = _hudWidget->GetSkillTargetSelect())
		{
			skillTargetSelect->UpdateVisible();
		}
	}
}

void AGsHUDLocalPlayer::InvalidatePreset()
{
	if (_hudWidget.IsValid())
	{
		if (UGsUIHUDPreset* presetUI = _hudWidget->GetPreset())
		{
			presetUI->InvalidateAll();
		}
	}
}
// 디버그용 예약 리스트 show
void AGsHUDLocalPlayer::OnDebugAIReserveListShow(const IGsMessageParam* In_isActive)
{
	const FGsPrimitiveInt32* param = In_isActive->Cast<const FGsPrimitiveInt32>();
	bool isActive = static_cast<bool>(param->_data);
	SetMiniMapDebugUI(isActive);
}