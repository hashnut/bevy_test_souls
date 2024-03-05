// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDMinimap.h"

#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "UMG/Public/Components/WidgetSwitcher.h"

#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"
#include "Management/ScopeGame/GsSpaceCrackManager.h"
#include "Management/ScopeGame/GsMinimapManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsObserverManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateDungeon.h"
#include "GameFlow/GameContents/ContentsGame/Dungeon/GsDungeonStateHandler.h"
#include "GameFlow/GameContents/ContentsGame/Dungeon/GsBattleArenaDungeonState.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaData.h"
#include "GameFlow/Stage/StageGame/GsStageManagerGame.h"
#include "GameFlow/Stage/StageGame/GsStageGameGuildDungeon.h"
#include "GameFlow/Stage/GsStageMode.h"


#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"

#include "Map/Minimap/SceneImage/GsMinimapSceneImage.h"
#include "Map/Minimap/MapIcon/GsMinimapIconBase.h"
#include "Map/Minimap/MapIcon/GsMinimapIconHandler.h"
#include "Map/WorldMap/GsWorldMapDefine.h"
#include "Map/WorldMap/GsWorldMapFunc.h"

#include "Message/GsMessageInvasion.h"
#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsBattleArenaMessageParam.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/InterServer/GsSchemaInvadeMapData.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"

#include "UI/UIContent/HUD/Minimap/GsUIMinimapIcon.h"
#include "UI/UIContent/HUD/Minimap/GsUINPCList.h"
#include "UI/UIContent/HUD/Minimap/GsUIMinimapInvasionPanel.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconAbnormality.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipTitleDesc.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UILib/Base/GsUIHUD.h"

#include "GameObject/HUD/GsHUDLocalPlayer.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Skill/GsSkillUIHandler.h"

#include "Minimap/GsUISubNPCList.h"

#include "GsUIHUDBoss.h"
#include "GsUIHUDMainMenu.h"
#include "GsUIHUDBattleArenaEnter.h"
#include "GsUIHUDBattleArenaScoreHud.h"
#include "GsUIHUDBattleArenaRegister.h"

#include "../Dungeon/GsDungeonCommonEnum.h"
#include "../ContentsServer/GsContentsServerHelper.h"

#include "T1Project.h"



void UGsUIHUDMinimap::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	_btnChannel->OnClicked.AddDynamic(this, &UGsUIHUDMinimap::OnClickChannel);
	_btnBookmark->OnClicked.AddDynamic(this, &UGsUIHUDMinimap::OnClickBookmark);
	_btnWorldMap->OnClicked.AddDynamic(this, &UGsUIHUDMinimap::OnClickWorldMap);
	_btnSpaceCrackActive->OnClicked.AddDynamic(this, &UGsUIHUDMinimap::OnClickSpaceCrackActive);
	_btnSpaceCrackDeactive->OnClicked.AddDynamic(this, &UGsUIHUDMinimap::OnClickSpaceCrackDeactive);
	_btnContribution->OnClicked.AddDynamic(this, &UGsUIHUDMinimap::OnClickContribution);

	_btnInvasion->OnClicked.AddDynamic(this, &UGsUIHUDMinimap::OnClickGoToInvasionInfo);
	_btnPreInvasion->OnClicked.AddDynamic(this, &UGsUIHUDMinimap::OnClickGoToInvasionInfo);
	_btnInterPortal->OnClicked.AddDynamic(this, &UGsUIHUDMinimap::OnClickInterPortal);
}

void UGsUIHUDMinimap::OpenCurrentWorldMap()
{
	//if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::WORLDMAP))
	//{
	//	GSUnlock()->ShowLockTicker(EGsUnlockContentsType::WORLDMAP);
	//	return;
	//}

	//// 개인 이벤트 채널이면 티커출력후 안넘어감
	//// 싱글 필드면 안넘어감
	//// 관전 모드면 안넘어감
	//// or guild dungeon block
	//if (GLevel()->IsMapType(MapType::INSTANCE_SINGLE) == true ||
	//	GLevel()->IsMapType(MapType::SINGLE_FIELD) == true ||
	//	GLevel()->IsMapType(MapType::GUILD_DUNGEON) == true ||
	//	GSObserver()->GetIsObserveStart() == true)
	//{
	//	FText findText;
	//	FText::FindText(TEXT("WorldMapText"), TEXT("ErrorInvalidRegion"), findText);
	//	FGsUIHelper::TrayMessageTicker(findText);
	//	return;
	//}
	
	// 언락 컨텐츠와 가능 타입 맵, 관전 모드 체크 한번에 하자
	if(FGsWorldMapFunc::IsRegionMapOpenPossible() == false)
	{
		return;	
	}

	// 현재 내위치 지역맵으로 이동(level 데이터 접근)
	if (GMode())
	{
		if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				if (FGsGameStateRegionMap* regionMap =
					contents->GetContentsStateObject<FGsGameStateRegionMap>(FGsContentsMode::InGame::ContentsRegionMap))
				{
					regionMap->SetMapChangeType(EGsWorldMapChangeType::TO_REGIONMAP_CLICK_MINIMAP);
				}
			}
		}
	}

	// 지역맵으로 이동
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_REGIONMAP);
}

void UGsUIHUDMinimap::SetAreaNameText(const FText& InAreaName)
{
	if (nullptr != _textAreaName)
	{
		FText mapName = InAreaName;

		if (GGameData()->IsInvadeWorld())
		{
			FText InvadePrefix;
			FText::FindText(TEXT("InvadeText"), TEXT("InvadePrefix"), InvadePrefix);
			mapName = FText::FromString(InvadePrefix.ToString() + InAreaName.ToString());
		}

		_textAreaName->SetText(mapName);
	}
}

// 기본 맵 이름 세팅
void UGsUIHUDMinimap::SetDefaultMapName()
{
	int mapId = GLevel()->GetCurrentLevelId();
	const FGsSchemaMapData* mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(mapId);
	if (mapData == nullptr)
	{
		return;
	}

	SetAreaNameText(mapData->MapStringId);

	int invadeType = GGameData()->IsInvadeWorld() ? static_cast<int>(InvasionState::Invade) :
			GSInterServer()->IsValidInvasionMap(mapId) ? static_cast<int>(InvasionState::Go) :
			static_cast<int>(InvasionState::NoGo);
	SetInvasionMapState(invadeType);
}

void UGsUIHUDMinimap::SetEnvIcon()
{

	int mapId = GLevel()->GetCurrentLevelId();

	int currentIconIndex = 0;
	//https://jira.com2us.com/jira/browse/CHR-16740
#ifdef USE_ENV_ICON
	// 1. check map env
	const FGsSchemaPassivitySet* mapPassivitySet = GetEnvPassivitySetCurrentMap();
	if (mapPassivitySet &&
		false == mapPassivitySet->hideUI)
	{
		UGsUIIconAbnormality* icon = GetEnvIconByIndex(currentIconIndex);
		if (icon != nullptr)
		{
			icon->SetData(mapPassivitySet);
			icon->OnPressIcon.Clear();
			icon->OnPressIcon.AddUObject(this, &UGsUIHUDMinimap::OnPressIconEnv);

			icon->_targetTooltip = GetEnvTooltipByIndex(currentIconIndex++);
		}
	}
	// 2. check invasion
	// 2.1 invade state(in other server)
	if (GGameData()->IsInvadeWorld() == true)
	{
		const FGsSchemaPassivitySet* invadeMapPassivitySet = GetInvadeEnvPassivitySetCurrentMap( true);
		if (invadeMapPassivitySet &&
			false == invadeMapPassivitySet->hideUI)
		{
			UGsUIIconAbnormality* icon = GetEnvIconByIndex(currentIconIndex);
			if (icon != nullptr)
			{
				icon->SetData(invadeMapPassivitySet);
				icon->OnPressIcon.Clear();
				icon->OnPressIcon.AddUObject(this, &UGsUIHUDMinimap::OnPressIconInvasionInvadeEnv);

				icon->_targetTooltip = GetEnvTooltipByIndex(currentIconIndex++);
			}
		}
	}
	// 2.2 defense state(invade possible time)
	else if (GSInterServer()->IsValidInvasionMap(mapId) == true)
	{
		const FGsSchemaPassivitySet* invadeMapPassivitySet = GetInvadeEnvPassivitySetCurrentMap( false);
		if (invadeMapPassivitySet &&
			false == invadeMapPassivitySet->hideUI)
		{
			UGsUIIconAbnormality* icon = GetEnvIconByIndex(currentIconIndex);
			if (icon != nullptr)
			{
				icon->SetData(invadeMapPassivitySet);
				icon->OnPressIcon.Clear();
				icon->OnPressIcon.AddUObject(this, &UGsUIHUDMinimap::OnPressIconInvasionDefenseEnv);

				icon->_targetTooltip = GetEnvTooltipByIndex(currentIconIndex++);
			}
		}
	}
	// 2.3 defense state(invade impossible time)-> no invasion icon
#endif
	// hide remain icon
	for (int i = currentIconIndex; _arrayIconSelectorEnv.Num() > i; ++i)
	{
		UGsUIIconAbnormality* icon = GetEnvIconByIndex(i);
		if (nullptr == icon)
		{
			continue;
		}
		icon->SetVisibility(ESlateVisibility::Collapsed);
	}

}
UGsUIIconAbnormality* UGsUIHUDMinimap::GetEnvIconByIndex(int In_index)
{
	if (_arrayIconSelectorEnv.Num() <= In_index)
	{
		return nullptr;
	}
	UGsHorizontalBoxIconSelector* selector = _arrayIconSelectorEnv[In_index];

	if (nullptr == selector)
	{
		return nullptr;
	}

	UGsUIIconAbnormality* icon = Cast<UGsUIIconAbnormality>(selector->GetChildAt(0));
	if (nullptr == icon)
	{
		icon = Cast<UGsUIIconAbnormality>(selector->CreateIcon());
		selector->AddChild(icon);
	}

	return icon;
}

// 아이콘 붙이기
void UGsUIHUDMinimap::OnAttachIcon( UGsMinimapIconBase* In_icon)
{
	if (nullptr == In_icon)
	{
		return;
	}
	UGsUIIconBase* uiIcon = In_icon->GetIcon();

	if (uiIcon == nullptr)
	{
		return;
	}

	UCanvasPanelSlot* canvasPanelSlot = _iconRoot->AddChildToCanvas(uiIcon);
	// 이걸 저정해놔야 위치값 조절 할수 있음.....
	In_icon->SetCanvasPanelSlot(canvasPanelSlot);
}

void UGsUIHUDMinimap::OnAttachIconWidget(UGsUIIconBase* In_iconWidget)
{
	_testPanelSlot = _iconRoot->AddChildToCanvas(In_iconWidget);
}
void UGsUIHUDMinimap::OnIconPos(const FVector2D& In_pos)
{
	if (_testPanelSlot == nullptr)
	{
		return;
	}
	_testPanelSlot->SetPosition(In_pos);
}
void UGsUIHUDMinimap::NativeConstruct()
{
	Super::NativeConstruct();

	ClearAllMessage();

	FGsMessageHolder* msg = GMessage();
	
	_delegateAttachIcon = msg->GetUIMinimapIcon().AddUObject(
		MessageContentMinimap::ATTACH_ICON, this, &UGsUIHUDMinimap::OnAttachIcon);
	_delegateAttachIconWidget = msg->GetUIMinimapIconWidget().AddUObject(
		MessageContentMinimap::ATTACH_WIDGET, this, &UGsUIHUDMinimap::OnAttachIconWidget);
	_delegateIconPos = msg->GetUIMinimapIconPos().AddUObject(
		MessageContentMinimap::SETPOS_ICON, this, &UGsUIHUDMinimap::OnIconPos);

	//game object message
	MGameObject& msgGameObject = GMessage()->GetGameObject();
		
	_gameObjectMsgArray.Add(msg->GetGameObject().AddUObject(
		MessageGameObject::MINIMAP_SET_CHANNEL, this, &UGsUIHUDMinimap::OnSetChannel)
	);
	_gameObjectMsgArray.Add(msg->GetGameObject().AddUObject(
		MessageGameObject::MINIMAP_SPACE_CRACK_BUTTON_ACTIVE, this, &UGsUIHUDMinimap::OnSpaceCrackButtonActive)
	);
	_gameObjectMsgArray.Add(msg->GetGameObject().AddUObject(
		MessageGameObject::MINIMAP_CONTRIBUTION_BUTTON_ACTIVE, this, &UGsUIHUDMinimap::OnContributionButtonActive)
	);
	_gameObjectMsgArray.Emplace(
		msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this,
			&UGsUIHUDMinimap::OnLocalPlayerSpawnComplete));
	
	_gameObjectMsgArray.Emplace(
		msg->GetGameObject().AddUObject(MessageGameObject::MINIMAP_ELITE_DUNGEON_ACTIVE, this,
			&UGsUIHUDMinimap::OnEliteDungeonActive));
	_gameObjectMsgArray.Emplace(
		msg->GetGameObject().AddUObject(MessageGameObject::MINIMAP_SPAWN_INTER_PORTAL, this,
			&UGsUIHUDMinimap::OnSpawnInterPortal));
	_gameObjectMsgArray.Emplace(
		msg->GetGameObject().AddUObject(MessageGameObject::MINIMAP_DESPAWN_INTER_PORTAL, this,
			&UGsUIHUDMinimap::OnDespawnInterPortal));

	//ui message
	MUI& msgUI = GMessage()->GetUI();
	_uiMsgArray.Add(msg->GetUI().AddUObject(
		MessageUI::RECEIVE_CHANNEL_LIST, this, &UGsUIHUDMinimap::OnRecieveChannelList));

	_uiMsgArray.Add(msg->GetUI().AddUObject(
		MessageUI::INVADE_MAPSTATE_UPDATE, this, &UGsUIHUDMinimap::OnUpdateInvadeMapState));

	_delegateQuestNpcList = msg->GetContentsHud().AddUObject(
		MessageContentHud::INVALIDATE_QUEST, this, &UGsUIHUDMinimap::InvalidateQuest);

	_invasionMsg.Emplace(
		msg->GetInvasion().AddUObject(MessageInvasion::INVALID_INVASION_USER_COUNT, this,
			&UGsUIHUDMinimap::OnInvalidUserCount));

	/**
	 * Battle Arena
	 */
	_battleArenaMsg.Emplace(
		msg->GetBattleArena().AddUObject(MessageContentBattleArena::UPDATE_STATE_INFO, this,
			&UGsUIHUDMinimap::OnInvalidBattleArena));

	if (UGsMinimapManager* minimapManager = GSMinimap())
	{
		// 매니징단에 등록
		minimapManager->GetSceneImage()->SetSceneImageRoot(_sceneImageRoot, _sizeCanvas);
	}

	SetDefaultMapName();

	for (UGsUITooltipTitleDesc* tooltip : _arrayTooltipEnv)
	{
		if (nullptr == tooltip)
		{
			continue;
		}
		tooltip->Close();
	}

	_canvasSpaceCrack->SetVisibility(ESlateVisibility::Collapsed);
	_btnInterPortal->SetVisibility(ESlateVisibility::Collapsed);

	// npc 아이콘 갱신(안전지대인지 체크후 visible 처리)
	EGsMinimapAreaType safeType = FGsSkillUIHandler::GetSaftyZoneType();
	SetSafeZone(safeType);
}

void UGsUIHUDMinimap::NativeDestruct()
{
	ClearAllMessage();
	

	for (UGsHorizontalBoxIconSelector* selector : _arrayIconSelectorEnv)
	{
		if (selector == nullptr)
		{
			continue;
		}
		selector->RemoveAllChildren();
	}

	_bIsRequestChannelList = false;

	Super::NativeDestruct();
}

void UGsUIHUDMinimap::ClearAllMessage()
{
	FGsMessageHolder* msg = GMessage();
	if (_delegateAttachIcon.Value.IsValid())
	{
		msg->GetUIMinimapIcon().Remove(_delegateAttachIcon);
	}	
	if (_delegateAttachIconWidget.Value.IsValid())
	{
		msg->GetUIMinimapIconWidget().Remove(_delegateAttachIconWidget);
	}
	if (_delegateIconPos.Value.IsValid())
	{
		msg->GetUIMinimapIconPos().Remove(_delegateIconPos);
	}	

	MGameObject& msgGameObject = GMessage()->GetGameObject();
	for (MsgGameObjHandle& handle : _gameObjectMsgArray)
	{
		msgGameObject.Remove(handle);
	}
	_gameObjectMsgArray.Empty();

	MUI& msgUI = GMessage()->GetUI();
	for (MsgUIHandle& handle : _uiMsgArray)
	{
		msgUI.Remove(handle);
	}
	_uiMsgArray.Empty();

	for (auto iter : _invasionMsg)
	{
		msg->GetInvasion().Remove(iter);
	}
	_invasionMsg.Empty();

	for (auto iter : _battleArenaMsg)
	{
		msg->GetBattleArena().Remove(iter);
	}
	_battleArenaMsg.Empty();

	if (_delegateQuestNpcList.Value.IsValid())
	{
		msg->GetContentsHud().Remove(_delegateQuestNpcList);
	}
}

void UGsUIHUDMinimap::InvalidContribution(EGsUIHUDMode inMod)
{
	if (inMod == EGsUIHUDMode::MENU)
	{
		_btnContribution->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		_btnContribution->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	FGsStageManagerGame* stage = gameFlow->GetStageManagerGame();
	if (nullptr == stage)
	{
		_btnContribution->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	if (stage->GetCurrentStageMode() != FGsStageMode::Game::GUILD_DUNGEON)
	{
		_btnContribution->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	_btnContribution->SetVisibility(ESlateVisibility::Visible);
}

void UGsUIHUDMinimap::InvalidateAllInternal()
{
	// 현재 채널 갱신
	SetChannel(GGameData()->GetCurrentChannel());

	SetEnvIcon();
}

bool UGsUIHUDMinimap::UpdateManagerTick(float InDeltaTime)
{
	if (false == Super::UpdateManagerTick(InDeltaTime))
	{
		return false;
	}

	if (GGameData()->IsInvadeWorld())
	{
		if (_isCheckRemainTime) // 침공 남은 시간
		{
			FTimespan remainTime = _InvasionEnableTime - FGsTimeSyncUtil::GetServerNowDateTime();
			SetInvasionRemainTime(remainTime);

			if (remainTime <= 0)
			{
				_isCheckMap = false;
				_isCheckRemainTime = false;
				_panelInvasionNoti->SetVisibility(ESlateVisibility::Collapsed);
				_InvasionEnableTime = FDateTime(0);
			}
		}
	}
	else
	{
		if (_isCheckMap) // 침공가능 맵
		{
			FTimespan remainTime = _InvasionEnableTime - FGsTimeSyncUtil::GetServerNowDateTime();

			if (_InvasionEnableTime.GetTicks() > 0 && remainTime.GetTotalMinutes() <= 10)
			{
				_panelInvasionNoti->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				_switchInvasionNoti->SetActiveWidgetIndex(0);

				_isCheckRemainTime = true;
				_isCheckMap = false;
			}
		}
		else if (_isCheckRemainTime) // 침공전 10분전, 침공가능시간
		{
			FTimespan remainTime = _InvasionEnableTime - FGsTimeSyncUtil::GetServerNowDateTime();
			SetInvasionRemainTime(remainTime);

			if (remainTime <= 0)
			{
				_isCheckMap = false;
				_isCheckRemainTime = false;
				_panelInvasionNoti->SetVisibility(ESlateVisibility::Collapsed);
				_InvasionEnableTime = FDateTime(0);
			}
		}
	}

	return true;
}

void UGsUIHUDMinimap::SetHUDMode(EGsUIHUDMode InMode)
{
	Super::SetHUDMode(InMode);

	if (EGsUIHUDMode::NORMAL == InMode)
	{
		_btnBookmark->SetVisibility(ESlateVisibility::Visible);		
	}
	else
	{
		_btnBookmark->SetVisibility(ESlateVisibility::Hidden);		
	}

	if (GGameData()->IsInvadeWorld())
	{
		_btnBookmark->SetVisibility(ESlateVisibility::Collapsed);
		// To be deleted
		//_canvasSpaceCrack->SetVisibility(ESlateVisibility::Collapsed);

		_invasionPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (nullptr != _bgWidgetSwitcher)
			_bgWidgetSwitcher->SetActiveWidgetIndex(1);
	}
	else
	{
		_invasionPanel->SetVisibility(ESlateVisibility::Collapsed);
		if(nullptr != _bgWidgetSwitcher)
			_bgWidgetSwitcher->SetActiveWidgetIndex(0);
	}

	if (FGsContentsServerHelper::IsLocatedOnBattleArenaGuildWar())
	{	// 2023/12/7 PKT - 길드 아레나에서는 즐겨찾기를 꺼준다.
		_btnBookmark->SetVisibility(ESlateVisibility::Collapsed);
	}
}

// npc list 액티브 변경
void UGsUIHUDMinimap::SetActiveDebugList(bool In_isActive)
{

	int index = (In_isActive == true) ? 1 : 0;
	_switcherMinimap->SetActiveWidgetIndex(index);

	if (In_isActive &&
		nullptr != _npcList)
	{
		_npcList->SetDebugListData();
	}
}

// 채널 클릭
void UGsUIHUDMinimap::OnClickChannel()
{
	// 리스트 요청 쿨타임 체크
	if (GGameData()->SendRequestChannelList())
	{
		// 요청 보내기 성공. OnRecieveChannelList 에서 받은 후 false 처리
		_bIsRequestChannelList = true;
	}
	else
	{
		// 쿨타임 중일 경우, 바로 팝업을 열고, 이전 데이터를 보여준다
		if (UGsUIManager* uiMgr = GUI())
		{
			uiMgr->Open(TEXT("PopupChannelList"));
		}
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMinimap::OnClickBookmark()
{
	ClickedBookmark();
}
void UGsUIHUDMinimap::ClickedBookmark()
{
	if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::TELEPORT, true))
	{
		// TODO: 기억장소 구현
		if (UGsUIManager* uiMgr = GUI())
		{
			uiMgr->Open(TEXT("PopupBookmark"));
		}
	}
	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

// 채널 표시
void UGsUIHUDMinimap::SetChannel(int In_channel)
{
	MapType currMapType = GLevel()->GetMapType();

	// 일반 필드만 채널 리스트 조회 가능
	if (MapType::PUBLIC_FIELD == currMapType || 
		MapType::PUBLIC_FIELD == currMapType ||
		MapType::PUBLIC_DUNGEON == currMapType)
	{
		// [B1] | ejrrb10| 최대 채널 갯수가 1개를 넘지 않을 경우, 채널 표시를 하지 않는다 (C2URWQ-5029)
		if (GLevel()->GetTotalChannelCount() <= 1)
		{
			_btnChannel->SetVisibility(ESlateVisibility::Collapsed);
			return;
		}

		_btnChannel->SetVisibility(ESlateVisibility::Visible);

		// {0} 채널
		FText textFormat;
		FText::FindText(TEXT("MinimapText"), TEXT("UIChannel"), textFormat);

		_textCurrentChannel = FText::Format(textFormat, In_channel);
	}
	else
	{
		_btnChannel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

// 안전지역 텍스트 표시
void UGsUIHUDMinimap::SetSafeZone(EGsMinimapAreaType In_isSafe)
{
	// https://jira.com2us.com/jira/browse/C2URWQ-2404
	// 1. check map balance  table data enable pk 
	// - enable pk false -> safe
	// - enable pk true -> in value 

	bool isSafeMap = true;
	if (GetCurrentMapEnablePK())
	{
		switch (In_isSafe)
		{
		case EGsMinimapAreaType::SAFE:
		case EGsMinimapAreaType::CHAOS:
		{
			isSafeMap = true;
		}			
			break;
		case EGsMinimapAreaType::PVP:
		{
			isSafeMap = false;
		}
			break;
		}	
	}

	FText msg;
	switch (In_isSafe)
	{
	case EGsMinimapAreaType::SAFE:
		FText::FindText(TEXT("MinimapText"), TEXT("UIDisablePK"), msg);
		break;
	case EGsMinimapAreaType::PVP:
		FText::FindText(TEXT("MinimapText"), TEXT("UIEnablePK"), msg);
		break;
	case EGsMinimapAreaType::CHAOS:
		FText::FindText(TEXT("MinimapText"), TEXT("UIEnablePKChaos"), msg);
		break;
	default:
		break;
	}

	if (_textSafeZone != nullptr)
	{
		_textSafeZone->SetText(msg);
	}
#ifdef NOT_USE_CODE
	SetNpcListButtonActive(isSafeMap);
#endif
}

// 채널 표시
void UGsUIHUDMinimap::OnSetChannel(const IGsMessageParam* In_param)
{
	const FGsPrimitiveInt32* param = In_param->Cast<const FGsPrimitiveInt32>();
	int channel = param->_data;
	SetChannel(channel);
}

void UGsUIHUDMinimap::OnRecieveChannelList(const IGsMessageParam*)
{
	if (_bIsRequestChannelList)
	{
		// 채널 이동 팝업 열기
		if (UGsUIManager* uiMgr = GUI())
		{
			uiMgr->Open(TEXT("PopupChannelList"));
		}

		_bIsRequestChannelList = false;
	}
}
// 월드맵 클릭
void UGsUIHUDMinimap::OnClickWorldMap()
{
	ClickedWorldMap();
}
void UGsUIHUDMinimap::ClickedWorldMap()
{
	OpenCurrentWorldMap();
	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMinimap::OnClickSpaceCrackActive()
{
	// TODO: 시공의 틈새 정보팝업창
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->Open(TEXT("PopupSpaceCrackDetailInfo"));
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMinimap::OnClickSpaceCrackDeactive()
{
	// TODO: 시공의 틈새 정보팝업창
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->Open(TEXT("PopupSpaceCrackDetailInfo"));
	}

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMinimap::OnClickContribution()
{
	bool isActive = GUI()->IsActiveWidget(TEXT("PopupContribution"));
	if (isActive)
	{
		GUI()->CloseByKeyName(TEXT("PopupContribution"));		
	}
	else
	{
		// TODO: 시공의 틈새 정보팝업창
		GUI()->OpenAndGetWidget(TEXT("PopupContribution"));

		//get contribution
		FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
		if (nullptr == gameFlow)
		{
			return;
		}

		FGsStageManagerGame* stageMgr = gameFlow->GetStageManagerGame();
		if (nullptr == stageMgr)
		{
			return;
		}

		if (stageMgr->GetCurrentStageMode() == FGsStageMode::Game::GUILD_DUNGEON)
		{
			GMessage()->GetGuildDungeon().SendMessage(MessageContentGuildDungeon::UPDATE_GUILD_DUNGEON_RANKING, nullptr);
		}
		else if (stageMgr->GetCurrentStageMode() == FGsStageMode::Game::AGIT_DUNGEON)
		{
			GMessage()->GetGuildDungeon().SendMessage(MessageContentGuildDungeon::UPDATE_AGIT_RAID_RANKING, nullptr);
		}
	}	

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDMinimap::OnClickGoToInvasionInfo()
{
	GMessage()->GetContents().SendMessage(MessageContents::REQ_ACTIVE_WINDOW_INVADE);
}

void UGsUIHUDMinimap::OnClickInterPortal()
{
	// TEXT: 지역 어딘가에 갈라진 전당의 틈이 생성되어있습니다.
	FText textMsg;
	if (FText::FindText(TEXT("DungeonText"), TEXT("Notice_EliteDungeon_HiddenPortalMinimap"), textMsg))
	{
		FGsUIHelper::TrayMessageTicker(textMsg);
	}
}

// 시공의틈새 킬수 있는지(버튼 활성화)
void UGsUIHUDMinimap::OnSpaceCrackButtonActive(const IGsMessageParam* In_param)
{
	// To be deleted - >_canvasSpaceCrack
	return;
	const FGsUIMsgParamSpaceCrackMinimapIcon* param = In_param->Cast<const FGsUIMsgParamSpaceCrackMinimapIcon>();
	bool isActive = param->_isActive;
	bool isShow = param->_isShow;
	bool isRedDot = param->_isRedDot;

	if (isShow && false == GGameData()->IsInvadeWorld())
	{
		_canvasSpaceCrack->SetVisibility(ESlateVisibility::Visible);

		_switcherSpaceCrack->SetActiveWidgetIndex(static_cast<int>(isActive));		
		_spaceCrackRedDot->SetVisibility(
			isRedDot ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	else
	{
		_canvasSpaceCrack->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIHUDMinimap::OnContributionButtonActive(const IGsMessageParam* In_param)
{
	const FGsUIMsgParamContributionMinimapIcon* param = In_param->Cast<const FGsUIMsgParamContributionMinimapIcon>();
	bool isActive = param->_isActive;
	if (isActive)
	{
		SetSideSwitcher(EGsMinimapSideButtonMode::GuildDungeon);
	}
	else
	{
		SetSideSwitcher(EGsMinimapSideButtonMode::Field);
	}
}

void UGsUIHUDMinimap::InvalidateQuest()
{
	int index = _switcherMinimap->GetActiveWidgetIndex();

	if (index == QUEST_NPC_LIST_WIDGET_INDEX)
	{
		if (_questNpcList)
		{
			_questNpcList->SetNpcListData();
		}
	}	
}

void UGsUIHUDMinimap::OnUpdateInvadeMapState(const IGsMessageParam* In_param)
{
	if (const auto param = In_param->Cast<const FGsUIMsgParamInt>())
	{
		SetInvasionMapState(param->_data);
	}
}

void UGsUIHUDMinimap::OnEliteDungeonActive(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (const FGsUIMsgParamBool* param = InParam->Cast<const FGsUIMsgParamBool>())
		{
			if (param->_data)
			{
				SetSideSwitcher(EGsMinimapSideButtonMode::InterServer);
			}
			else
			{
				SetSideSwitcher(EGsMinimapSideButtonMode::Field);
			}
		}
	}
}

void UGsUIHUDMinimap::OnSpawnInterPortal(const IGsMessageParam*)
{
	_btnInterPortal->SetVisibility(ESlateVisibility::Visible);
}

void UGsUIHUDMinimap::OnDespawnInterPortal(const IGsMessageParam*)
{
	_btnInterPortal->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIHUDMinimap::OnInvalidBattleArena(const IGsMessageParam* InParam)
{
	const FGsBattleArenaStateInfo* param = StaticCast<const FGsBattleArenaStateInfo*>(InParam);
	if (nullptr == param)
	{
		_battleArenaRegisterHUD->SetVisibility(ESlateVisibility::Collapsed);
		_battleArenaEnterHUD->SetVisibility(ESlateVisibility::Collapsed);
		_arenaBattleScoreBoardHud->SetVisibility(ESlateVisibility::Collapsed);

		return;
	}

	_battleArenaRegisterHUD->SetData(param->_state, param->_endTime);
	_battleArenaEnterHUD->SetData(param->_state, param->_endTime);
	_arenaBattleScoreBoardHud->SetData(param->_mapId, param->_state, param->_endTime, param->_redTeamScore, param->_blueTeamScore);
}

void UGsUIHUDMinimap::SetInvasionMapState(int state)
{
	auto invadeType = static_cast<InvasionState>(state);

	_textInvadeInfo->SetVisibility(ESlateVisibility::Collapsed);

	_isCheckMap = false;
	_isCheckRemainTime = false;
	_panelInvasionNoti->SetVisibility(ESlateVisibility::Collapsed);
	_InvasionEnableTime = FDateTime(0);

	if (invadeType == InvasionState::Invade)
	{
		auto curTime = FGsTimeSyncUtil::GetServerNowDateTime();
		auto invasionRemainTime = GSInterServer()->GetInvasionRemainStayTime();
		if (!invasionRemainTime.IsZero())
		{
			FDateTime endTime = _InvasionEnableTime = curTime + invasionRemainTime;

			_panelInvasionNoti->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			_switchInvasionNoti->SetActiveWidgetIndex(1);

			_isCheckRemainTime = true;
		}
	}
	else if (auto mapInfo = GSInterServer()->GetCurrentInvasionMapInfo().Pin())
	{
		if (invadeType == InvasionState::NoGo)
		{
			_isCheckMap = true;

			auto curTime = FGsTimeSyncUtil::GetServerNowDateTime();
			FDateTime startTime = _InvasionEnableTime = GSInterServer()->GetNextInvasionEnableTime();
			FTimespan remainTime = startTime - curTime;

			if (startTime.GetTicks() > 0 && remainTime.GetTotalMinutes() <= 10)
			{
				_panelInvasionNoti->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				_switchInvasionNoti->SetActiveWidgetIndex(0);

				_isCheckRemainTime = true;
				_isCheckMap = false;
			}
		}
		else if (invadeType == InvasionState::Go)
		{
			auto curTime = FGsTimeSyncUtil::GetServerNowDateTime();
			FDateTime endTime = _InvasionEnableTime = mapInfo->GetInvasionEndTime();
			FTimespan remainTime = endTime - curTime;

			if (endTime.GetTicks() > 0 && remainTime > 0)
			{
				_panelInvasionNoti->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				_switchInvasionNoti->SetActiveWidgetIndex(1);

				_isCheckRemainTime = true;
			}
		}
	}
}

void UGsUIHUDMinimap::SetInvasionRemainTime(FTimespan remainTime)
{
	if (!remainTime.IsZero())
	{
		auto remainText = FString::Printf(TEXT("%02d:%02d"), int32(remainTime.GetTotalMinutes()), remainTime.GetSeconds());
		_textInvasionTime = FText::FromString(remainText);

		//FText InvadeAllowTime_Hour;
		//FText InvadeAllowTime;
		//FText InvadeAllowTime_Second;
		//FText::FindText(TEXT("InvadeText"), TEXT("InvadeAllowTime_Hour"), InvadeAllowTime_Hour);
		//FText::FindText(TEXT("InvadeText"), TEXT("InvadeAllowTime"), InvadeAllowTime);
		//FText::FindText(TEXT("InvadeText"), TEXT("InvadeAllowTime_Second"), InvadeAllowTime_Second);

		//int hour = remainTime.GetTotalHours();
		//int min = remainTime.GetMinutes();
		//int sec = remainTime.GetSeconds();

		//if (hour > 0)
		//{
		//	inTextblock->SetText(FText::Format(InvadeAllowTime, { hour, min }));
		//}
		//else if (min > 0)
		//{
		//	inTextblock->SetText(FText::Format(InvadeAllowTime, { min, sec }));
		//}
		//else if (sec > 0)
		//{
		//	inTextblock->SetText(FText::Format(InvadeAllowTime_Second, { sec }));
		//}
	}
}

const FGsSchemaPassivitySet* UGsUIHUDMinimap::GetEnvPassivitySetCurrentMap() const
{
	const FGsSchemaMapData* mapData = GLevel()->GetCurrentMapInfo();
	if (mapData == nullptr)
	{
		return nullptr;
	}

	const FGsSchemaMapBalance* mapBalanceData = mapData->balanceId.GetRow();
	if (nullptr == mapBalanceData)
	{
		return nullptr;
	}

	if (false == mapBalanceData->passivityIdList.IsValidIndex(mapBalanceData->passivityIdListIndexEnv))
	{
		return nullptr;
	}
	
	return mapBalanceData->passivityIdList[mapBalanceData->passivityIdListIndexEnv].GetRow();
}
bool UGsUIHUDMinimap::GetCurrentMapEnablePK()
{
	const FGsSchemaMapData* mapData = GLevel()->GetCurrentMapInfo();
	if (mapData == nullptr)
	{
		return false;
	}

	const FGsSchemaMapBalance* mapBalanceData = mapData->balanceId.GetRow();
	if (nullptr == mapBalanceData)
	{
		return false;
	}

	return mapBalanceData->EnablePK;
}

const FGsSchemaPassivitySet* UGsUIHUDMinimap::GetInvadeEnvPassivitySetCurrentMap(bool In_isInvade) const
{
	int mapId = GLevel()->GetCurrentLevelId();
	const UGsTableInvadeMapData* table = Cast<UGsTableInvadeMapData>(FGsSchemaInvadeMapData::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}
	TArray<const FGsSchemaInvadeMapData*> arrayData;
	table->GetAllRows<FGsSchemaInvadeMapData>(arrayData);

	const FGsSchemaInvadeMapData* invadeMapTbl = nullptr;
	for (const FGsSchemaInvadeMapData* iter : arrayData)
	{
		if (nullptr == iter)
		{
			continue;
		}
		const FGsSchemaMapData* mapData = iter->mapId.GetRow();
		if (mapData == nullptr)
		{
			continue;
		}
		if (mapData->id == mapId)
		{
			invadeMapTbl = iter;
			break;
		}
	}

	if (invadeMapTbl == nullptr)
	{
		return nullptr;
	}

	int findIndex = (In_isInvade == true) ? invadeMapTbl->invadePassivityIdListIndexEnv : invadeMapTbl->defensePassivityIdListIndexEnv;
	TArray<FGsSchemaPassivitySetRow> passivityList = (In_isInvade == true) ? invadeMapTbl->invadePassivityIdList : invadeMapTbl->defensePassivityIdList;

	if (passivityList.Num() == 0)
	{
		return nullptr;
	}

	if (false == passivityList.IsValidIndex(findIndex))
	{
		return nullptr;
	}

	// 지정된 인덱스의 아이콘만 표시한다
	return passivityList[findIndex].GetRow();
}

void UGsUIHUDMinimap::OnPressIconEnv(class UGsUIIconAbnormality* InIcon)
{
	if (InIcon == nullptr ||
		InIcon->_targetTooltip == nullptr)
	{
		return;
	}

	UGsUITooltipTitleDesc* tooltip = Cast<UGsUITooltipTitleDesc>(InIcon->_targetTooltip);

	if (tooltip == nullptr)
	{
		return;
	}
	if (tooltip->IsOpen())
	{
		tooltip->Close();
		return;
	}
	
	const FGsSchemaPassivitySet* passivitySet = GetEnvPassivitySetCurrentMap();
	if (passivitySet)
	{
		tooltip->SetTitleColor(FGsUIColorHelper::GetColorNameTextPassivity(
			passivitySet->isBuff, PassivityOwnerType::MAP));
		tooltip->SetTitleText(passivitySet->nameText);
		tooltip->SetDescText(passivitySet->descText);
	}
	else
	{
		tooltip->SetDefaultColor();

		// TEXT: 없음
		FText textTitle;
		FText::FindText(TEXT("EnvironmentText"), TEXT("EnvNone"), textTitle);
		tooltip->SetTitleText(textTitle);

		// TEXT: 효과 없음
		FText textDesc;
		FText::FindText(TEXT("EnvironmentText"), TEXT("DebuffNone"), textDesc);
		tooltip->SetDescText(textDesc);
	}

	tooltip->Open();
}
UGsUITooltipTitleDesc* UGsUIHUDMinimap::GetEnvTooltipByIndex(int In_index)
{
	if (_arrayTooltipEnv.Num() <= In_index)
	{
		return nullptr;
	}

	return _arrayTooltipEnv[In_index];
}
void UGsUIHUDMinimap::OnPressIconInvasionInvadeEnv(class UGsUIIconAbnormality* InIcon)
{
	if (InIcon == nullptr ||
		InIcon->_targetTooltip == nullptr)
	{
		return;
	}

	UGsUITooltipTitleDesc* tooltip = Cast<UGsUITooltipTitleDesc>(InIcon->_targetTooltip);

	if (tooltip == nullptr)
	{
		return;
	}

	if (tooltip->IsOpen())
	{
		tooltip->Close();
		return;
	}

	const FGsSchemaPassivitySet* passivitySet = GetInvadeEnvPassivitySetCurrentMap( true);
	if (passivitySet)
	{
		tooltip->SetTitleColor(FGsUIColorHelper::GetColorNameTextPassivity(
			passivitySet->isBuff, PassivityOwnerType::MAP));
		tooltip->SetTitleText(passivitySet->nameText);
		tooltip->SetDescText(passivitySet->descText);
	}
	else
	{
		tooltip->SetDefaultColor();

		// TEXT: 없음
		FText textTitle;
		FText::FindText(TEXT("EnvironmentText"), TEXT("EnvNone"), textTitle);
		tooltip->SetTitleText(textTitle);

		// TEXT: 효과 없음
		FText textDesc;
		FText::FindText(TEXT("EnvironmentText"), TEXT("DebuffNone"), textDesc);
		tooltip->SetDescText(textDesc);
	}

	tooltip->Open();
}
void UGsUIHUDMinimap::OnPressIconInvasionDefenseEnv(class UGsUIIconAbnormality* InIcon)
{
	if (InIcon == nullptr ||
		InIcon->_targetTooltip == nullptr)
	{
		return;
	}

	UGsUITooltipTitleDesc* tooltip = Cast<UGsUITooltipTitleDesc>(InIcon->_targetTooltip);

	if (tooltip == nullptr)
	{
		return;
	}

	if (tooltip->IsOpen())
	{
		tooltip->Close();
		return;
	}
	
	const FGsSchemaPassivitySet* passivitySet = GetInvadeEnvPassivitySetCurrentMap( false);
	if (passivitySet)
	{
		tooltip->SetTitleColor(FGsUIColorHelper::GetColorNameTextPassivity(
			passivitySet->isBuff, PassivityOwnerType::MAP));
		tooltip->SetTitleText(passivitySet->nameText);
		tooltip->SetDescText(passivitySet->descText);
	}
	else
	{
		tooltip->SetDefaultColor();

		// TEXT: 없음
		FText textTitle;
		FText::FindText(TEXT("EnvironmentText"), TEXT("EnvNone"), textTitle);
		tooltip->SetTitleText(textTitle);

		// TEXT: 효과 없음
		FText textDesc;
		FText::FindText(TEXT("EnvironmentText"), TEXT("DebuffNone"), textDesc);
		tooltip->SetDescText(textDesc);
	}

	tooltip->Open();
}


void UGsUIHUDMinimap::InvalidUserCount()
{
	int32 currentUserCount = GSInterServer()->GetTotalInvaderCount();
	//const FString& userCountText = FString::Printf(TEXT("%d/%d"), currentUserCount, _maxUserCount);
	_userCountTextBlock->SetText(FText::AsNumber(currentUserCount));
}

void UGsUIHUDMinimap::InvalidInvasionServerName()
{
	WorldId worldId = GSInterServer()->GetWorldId();
	FString serverName;
	GNet()->TryGetServerName(worldId, serverName);
	_invasionServerTextBlock->SetText(FText::FromString(serverName));
}

void UGsUIHUDMinimap::OnLocalPlayerSpawnComplete(const struct IGsMessageParam* inParam)
{
	_currentMapId = GLevel()->GetCurrentLevelId();
	TWeakPtr<FGsInvadeMap> invadeMap = GSInterServer()->GetInvadeMapData(_currentMapId);
	if (false == invadeMap.IsValid())
	{
		return;
	}

	//_maxUserCount = invadeMap.Pin()->GetMaxInvaderCount();
	InvalidInvasionServerName();
}

void UGsUIHUDMinimap::OnInvalidUserCount(const struct IGsMessageParam*)
{
	InvalidUserCount();
}

void UGsUIHUDMinimap::SetSideSwitcher(EGsMinimapSideButtonMode InMode)
{
	_switcherSideButton->SetActiveWidgetIndex(static_cast<int32>(InMode));

	// 시련의 전당에서도 보스 아이콘 출력되게 요청와서 
	// switcher가 아닌 별도 visible 관리
	// https://jira.com2us.com/jira/browse/C2URWQ-6900

	_bossHud->SetMinimapSideButtonMode(InMode);
}
