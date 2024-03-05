// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDFrameMain.h"
#include "Widget.h"
#include "Components/TextBlock.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "UI/UIContent/HUD/GsUIHUDMainMenu.h"
#include "UI/UIContent/HUD/GsUIHUDNavigationBar.h"
#include "UI/UIContent/HUD/GsUIHUDSkill.h"
#include "UI/UIContent/HUD/GsUIHUDPlayerInfo.h"
#include "UI/UIContent/HUD/GsUIHUDTargetInfo.h"
#include "UI/UIContent/HUD/GsUIHUDCamera.h"
#include "UI/UIContent/HUD/GsUIHUDQuest.h"
#include "UI/UIContent/HUD/GsUIHUDParty.h"
#include "UI/UIContent/HUD/GsUIHUDScan.h"
#include "UI/UIContent/HUD/GsUIHUDMinimap.h"
#include "UI/UIContent/HUD/GsUIHUDSystemInfo.h"
#include "UI/UIContent/HUD/GsUIHUDAuto.h"
#include "UI/UIContent/HUD/GsUIHUDAbnormality.h"
//#include "UI/UIContent/HUD/GsUIHUDModeButton.h"
#include "UI/UIContent/HUD/GsUIHUDPickUp.h"
#include "UI/UIContent/HUD/GsUIHUDChaotic.h"
#include "UI/UIContent/HUD/GsUIHUDQuickSlot.h"
#include "UI/UIContent/HUD/GsUIHUDQuestDistance.h"
#include "UI/UIContent/HUD/GsUIHUDSideTab.h"
#include "UI/UIContent/HUD/GsUIHUDConsumable.h"
#include "UI/UIContent/HUD/GsUIHUDSocialEmojiButton.h"
#include "UI/UIContent/HUD/GsUIHUDChat.h"
#include "UI/UIContent/HUD/GsUIHUDPropTouchProgress.h"
#include "UI/UIContent/HUD/GsUIHUDSpiritShot.h"
#include "UI/UIContent/HUD/GsUIHUDBless.h"
#include "UI/UIContent/HUD/GsUIHUDDungeonTime.h"
#include "UI/UIContent/HUD/GsUIHUDGainEffect.h"
#include "UI/UIContent/HUD/GsUIHUDScanButton.h"
#include "UI/UIContent/HUD/GsUIHUDOpenPopupChat.h"
#include "UI/UIContent/HUD/Contribution/GsUIHUDContribution.h"
#include "UI/UIContent/HUD/GsUIHUDOpenPopupChat.h"
#include "UI/UIContent/HUD/Penalty/GsUIHUDPenalty.h"
#include "UI/UIContent/HUD/GsUIHUDSaveBattery.h"
#include "UI/UIContent/HUD/GsUIHUDEventActionMessageProgressBar.h"
#include "UI/UIContent/HUD/GsUIHUDEventActionMessageNotify.h"
#include "UI/UIContent/HUD/GsUIHUDPartyTargetButton.h"
#include "UI/UIContent/HUD/GsUIHUDArenaTime.h"
#include "UI/UIContent/HUD/GsUIHUDCounterAttackButton.h"
#include "UI/UIContent/HUD/GsUIHUDSkillTargetSelect.h"
#include "UI/UIContent/HUD/GsUIHUDPreset.h"
#include "UI/UIContent/HUD/Preset/GsUIPresetSelect.h"
#include "UI/UIContent/HUD/GsUIHUDPartyInfoButton.h"
#include "UI/UIContent/HUD/GsUIHUDEventActionMessageProgressBarEX.h"
#include "UI/UIContent/HUD/GsUIHUDGameRating.h"

#include "Input/GsInputEventMsgBase.h"

#include "DataCenter/Public/Option/GsOptionEnumType.h"

#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/GsMessageStage.h"
#include "Message/MessageParam/GsStageMessageParam.h"
#include "Message/GsMessageGameObject.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "Input/GsInputBindingLocalPlayer.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Skill/GsSkillUIHandler.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "UTIL/GsEnum.h"
#include "DateTime.h"

#include "Data/GsDataContainManager.h"



void UGsUIHUDFrameMain::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Mode 제어를 위한 등록
	_hudList.Emplace(_mainMenu);
	_hudList.Emplace(_navigationBar);
	_hudList.Emplace(_skillUI);
	_hudList.Emplace(_playerInfoUI);
	_hudList.Emplace(_targetInfoUI);
	_hudList.Emplace(_cameraUI);
	_hudList.Emplace(_questUI);
	_hudList.Emplace(_chat);
	// side 탭보다 위에 있어야함
	// GsUIHudFrame의  SetHUDMode에서 다켜버림
	// side 탭의 SetHUDMode에서 다시 select 처리를 마지막으로 받자
	_hudList.Emplace(_partyUI);
	_hudList.Emplace(_scanUI);
	_hudList.Emplace(_consumableMenu);
	_hudList.Emplace(_spiritShot);
	_hudList.Emplace(_minimap);
	_hudList.Emplace(_systemInfo);
	_hudList.Emplace(_autoUI);
	_hudList.Emplace(_abnormalityUI);
	//_hudList.Emplace(_hudModeButton);
	_hudList.Emplace(_chatButton);
	_hudList.Emplace(_pickUpButton);
	_hudList.Emplace(_chaoticButton);
	_hudList.Emplace(_quickSlot);
	_hudList.Emplace(_questDistance);
	_hudList.Emplace(_propTouchProgress);
	_hudList.Emplace(_sideTab);
	_hudList.Emplace(_penaltyHud);
	_hudList.Emplace(_emojiButton);
	//_hudList.Emplace(_blessHud);
	_hudList.Emplace(_dungeonTimeHud);	
	_hudList.Emplace(_gainExpEffect);
	_hudList.Emplace(_gainGoldEffect);
	_hudList.Emplace(_gainMonsterKnowledgeEffect);
	_hudList.Emplace(_scanButtonUI);
	_hudList.Emplace(_hudSaveBattery);
	_hudList.Emplace(_hudEventActionMessageProgressBar);
	_hudList.Emplace(_hudEventActionMessageNotify);
	_hudList.Emplace(_partyTargetButton);
	_hudList.Emplace(_hudArenaTime);
	_hudList.Emplace(_counterAttackButton);
	_hudList.Emplace(_skillTargetSelect);
	_hudList.Emplace(_presetButton);
	_hudList.Emplace(_partyInfoButtonUI);	
	_hudList.Emplace(_hudEventActionMessageProgressBarEX);
//	_hudList.Emplace(_gameRatingUI); //모드에 따라 동작하지 않음 IsHide 상속함수가 아니어서 리스트에서 뺀다

	// 이벤트 등록
	_sideTab->OnClickSideTab.BindUObject(this, &UGsUIHUDFrameMain::OnClickSideTab);
}

void UGsUIHUDFrameMain::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();

	// 보스방 등에서 카메라를 꺼버리는 경우 등이 있으므로 리셋한다.
	if (_cameraUI)
	{
		_cameraUI->Reset();
	}

	_sideTab->SetSelectedIndex(UGsUIHUDSideTab::EGsHUDSideTabType::QUEST);

	if (_propTouchProgress)
	{
		_propTouchProgress->SetVisibility(ESlateVisibility::Hidden);
	}

	if (_presetButton)
	{
		if (UGsDataContainManager* dataManager = GData())
		{
			if (UGsGlobalConstant* globalData = dataManager->GetGlobalData())
			{
				_presetButton->SetVisibility(globalData->_showPreset ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
			}
		}

		_presetButton->SetPresetSelect(_presetSelect);
	}

	GMessage()->GetUI().SendMessage(MessageUI::INVALID_PENALTY, nullptr);

	CheckVisibleNavigationBar();
}

void UGsUIHUDFrameMain::NativeDestruct()
{
	UnbindMessages();	

	Super::NativeDestruct();
}

void UGsUIHUDFrameMain::OnManagerTick(float InDeltaTime)
{
	// 보일 때만 틱이 돌도록
	if (false == IsVisible())
	{
		return;
	}

	for (UGsUIHUD* iter : _hudList)
	{
		iter->UpdateManagerTick(InDeltaTime);
	}

	_gameRatingUI->UpdateManagerTick(InDeltaTime);

#if !UE_BUILD_SHIPPING
	// 서버 Revision 정보 
	FString strServerRevisionInfo;
	if (FGsNetManager* netManager = GNet())
	{
		strServerRevisionInfo = netManager->GetServerRevisionInfo();
	}

	// 현재 시간
	FString strDateTime = FDateTime::Now().ToString().Append(TEXT("\n"));

	// 내 캐릭터 기본 위치정보 추가
	FString strLocation;
	if (UGsGameObjectManager* gameObjectManager = GSGameObject())
	{
		if (UGsGameObjectBase* localObject = gameObjectManager->FindObject(EGsGameObjectType::LocalPlayer))
		{
			const FVector& localLocation = localObject->GetLocation();
			strLocation = FString::Printf(TEXT("LocalPlayer Location : %.1f, %.1f, %.1f\n"), localLocation.X, localLocation.Y, localLocation.Z);
		}
	}

	FString strMapId;
	if (UGsLevelManager* levelManager = GLevel())
	{
		strMapId = FString::Printf(TEXT("MapID : %d\n"), GLevel()->GetCurrentLevelId());
	}

	_textBlockQA->SetVisibility(ESlateVisibility::Collapsed);
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		if (gameUserSettings->GetHudSetting(EGsOptionHud::IS_SHOW_SERVER_REVISION_AND_LOCATION) == 1)
		{
			_textBlockQA->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			_textBlockQA->SetText(FText::FromString(strDateTime + strMapId + strLocation + strServerRevisionInfo));
		}
	}
#endif
}

void UGsUIHUDFrameMain::BindMessages()
{
	MGameObject& msgObject = GMessage()->GetGameObject();
	_gameObjectHandlerList.Emplace(msgObject.AddUObject(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE,
		this, &UGsUIHUDFrameMain::OnChangeSafetyMode));

	// Stage messages
	MStageParam& msgStage = GMessage()->GetStageParam();
	_msgHandleStageList.Emplace(msgStage.AddUObject(MessageStage::ENTER_TOWN_CALLBACK,
		this, &UGsUIHUDFrameMain::OnChangeSafetyMode));
	_msgHandleStageList.Emplace(msgStage.AddUObject(MessageStage::LEAVE_TOWN_CALLBACK,
		this, &UGsUIHUDFrameMain::OnChangeSafetyMode));
	_msgHandleStageList.Emplace(msgStage.AddUObject(MessageStage::ENTER_CHAOS_CALLBACK,
		this, &UGsUIHUDFrameMain::OnChangeSafetyMode));
	_msgHandleStageList.Emplace(msgStage.AddUObject(MessageStage::LEAVE_CHAOS_CALLBACK,
		this, &UGsUIHUDFrameMain::OnChangeSafetyMode));
	_msgHandleStageList.Emplace(msgStage.AddUObject(MessageStage::ENTER_SAFETY_CALLBACK,
		this, &UGsUIHUDFrameMain::OnChangeSafetyMode));
	_msgHandleStageList.Emplace(msgStage.AddUObject(MessageStage::LEAVE_SAFETY_CALLBACK,
		this, &UGsUIHUDFrameMain::OnChangeSafetyMode));

	MInvasion& msgInvasion = GMessage()->GetInvasion();
	_messageInvasionList.Emplace(msgInvasion.AddUObject(MessageInvasion::INVAION_START,
		this, &UGsUIHUDFrameMain::OnChangeSafetyMode));
	_messageInvasionList.Emplace(msgInvasion.AddUObject(MessageInvasion::INVAION_FINISH,
		this, &UGsUIHUDFrameMain::OnChangeSafetyMode));

	// UI messages
	MUI& msgUI = GMessage()->GetUI();
	_msgHandleUIList.Emplace(msgUI.AddUObject(MessageUI::INVALID_PENALTY,
		this, &UGsUIHUDFrameMain::OnChangePenalty));
	_msgHandleUIList.Emplace(msgUI.AddUObject(MessageUI::PREVENTION_GAME_OVERINDULGENCE,
		this, &UGsUIHUDFrameMain::OnPreventGameOverIndulgence));

	//FGsMessageHolder* msg = GMessage();
	//_inputParamDelegate = msg->GetWindowInput().AddUObject(MessageInput::Window_keyboard_Action,
	//	this, &UGsUIHUDFrameMain::OnWindowKeyMapping);
}

void UGsUIHUDFrameMain::UnbindMessages()
{
	MStageParam& msgStage = GMessage()->GetStageParam();
	for (MsgStageHandle& msgStageHandle : _msgHandleStageList)
	{
		msgStage.Remove(msgStageHandle);
	}
	_msgHandleStageList.Empty();

	MUI& msgUI = GMessage()->GetUI();
	for (MsgUIHandle& msgUIHandle : _msgHandleUIList)
	{
		msgUI.Remove(msgUIHandle);
	}
	_msgHandleUIList.Empty();

	MInvasion& msgInvasion = GMessage()->GetInvasion();
	for (MsgInvasionHandle& msgInvasionHandle : _messageInvasionList)
	{
		msgInvasion.Remove(msgInvasionHandle);
	}
	_messageInvasionList.Empty();

	MGameObject& msgGameObject = GMessage()->GetGameObject();
	for (MsgGameObjHandle& msgGmaeObjectHandle : _gameObjectHandlerList)
	{
		msgGameObject.Remove(msgGmaeObjectHandle);
	}
	_gameObjectHandlerList.Empty();

	//FGsMessageHolder* messageMgr = GMessage();
	//messageMgr->GetWindowInput().Remove(_inputParamDelegate);
}

bool UGsUIHUDFrameMain::OnBack()
{
	return _mainMenu->OnBack();
}

void UGsUIHUDFrameMain::SetHUDMode(EGsUIHUDMode InMode)
{
	Super::SetHUDMode(InMode);

	CheckVisibleNavigationBar();
}

void UGsUIHUDFrameMain::InvalidateAllMenu()
{
	for (UGsUIHUD* hud : _hudList)
	{
		if (nullptr != hud)
		{
			hud->InvalidateAll();
		}		
	}
}

void UGsUIHUDFrameMain::InvalidSafetyZone()
{	
	_minimap->SetSafeZone(FGsSkillUIHandler::GetSaftyZoneType());
}

void UGsUIHUDFrameMain::CheckVisibleNavigationBar()
{
	// 타겟과 상단 재화UI가 겹치는 것을 방지하기 위한 로직
	if (_targetInfoUI->IsOpened())
	{
		_navigationBar->SetVisibility(ESlateVisibility::Collapsed);

		if (UGsUIManager* uiMgr = GUI())
		{
			if (_targetInfoUI->IsHideMode(uiMgr->GetHUDMode()))
			{
				_targetInfoUI->SetVisibility(ESlateVisibility::Collapsed);
				return;
			}
		}

		_targetInfoUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_targetInfoUI->SetVisibility(ESlateVisibility::Collapsed);

		if (UGsUIManager* uiMgr = GUI())
		{
			if (_navigationBar->IsHideMode(uiMgr->GetHUDMode()))
			{
				_navigationBar->SetVisibility(ESlateVisibility::Collapsed);
				return;
			}
		}

		_navigationBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UGsUIHUDFrameMain::OnClickSideTab(int32 InButtonIndex, bool bInIsSelected)
{
	if (static_cast<int32>(UGsUIHUDSideTab::EGsHUDSideTabType::QUEST) == InButtonIndex)
	{
		_questUI->SetVisibility((bInIsSelected) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
	else if (static_cast<int32>(UGsUIHUDSideTab::EGsHUDSideTabType::SCAN) == InButtonIndex)
	{
		ESlateVisibility oldVisible =  _scanUI->GetVisibility();
		ESlateVisibility newVisible = (bInIsSelected) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;

		bool isVisibleChange = (oldVisible != newVisible)? true: false;
		
		_scanUI->SetVisibility(newVisible);
		
		if (bInIsSelected == true &&
			_scanButtonUI != nullptr &&
			isVisibleChange == true)
		{			
			// 쿨타임이면 안함
			if(_scanButtonUI->GetIsCoolTimeOn()== false)
			{ 
				_scanButtonUI->StartScanByUI();	
			}
		}
	}
}

void UGsUIHUDFrameMain::OnChangeSafetyMode(const IGsMessageParam* InParam)
{
	InvalidSafetyZone();
}

void UGsUIHUDFrameMain::OnChangePenalty(const IGsMessageParam* InParam)
{
	_penaltyHud->InvalidateAll();
}

void UGsUIHUDFrameMain::OnPreventGameOverIndulgence(const IGsMessageParam* InParam)
{
	if (const FGsUIMsgParamFloat* data = InParam->Cast<const FGsUIMsgParamFloat>())
	{
		_gameRatingUI->SetVisibleTime(data->_data);
	}
}
void UGsUIHUDFrameMain::SetHide(EGsUIHideFlags InHideFlags)
{
	Super::SetHide(InHideFlags);

	if (true == EnumHasAnyFlags(EGsUIHideFlags::UI_HIDE_HUD, InHideFlags))
	{
		_scanButtonUI->OnHide();
	}
}

ENABLE_ENUM_OPERATORS(EGsKeyMappingType)
// 이미 ENUM_RANGE_BY_COUNT 를 선언했으므로, IsValidRange 에 대한 오버로딩을 추가하겠음
//ENUM_RANGE_BY_FIRST_AND_LAST(EGsKeyMappingType, EGsKeyMappingType::KEY_MAPPING_SKILLSLOT_1, EGsKeyMappingType::KEY_MAPPING_SKILLSLOT_12)

void UGsUIHUDFrameMain::OnKeyboardActionOptionKey(EGsKeyMappingType InKeyMappingType, EInputEvent InEventType)
{
	if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectLocalPlayer* local = player->CastGameObject<UGsGameObjectLocalPlayer>())
		{
			if (UGsInputBindingLocalPlayer* inputBinder = local->GetInputBinder())
			{
				// 인풋이 막혀있으면 누르지 않음
				if (inputBinder->IsInputBlock(InKeyMappingType))
				{
					return;
				}
			}
		}
	}	

	static auto OnKeyboardSkill = [](EGsKeyMappingType InKeyMappingType, EInputEvent InEventType) {

		if (InEventType < 0 || InEventType > IE_MAX) return false;
		if (InKeyMappingType == EGsKeyMappingType::KEY_MAPPING_NORMALATTACK)
			return true;

		// 스킬 슬롯에 대한 단축키 입력 처리가 하드코딩 되어 있다;
		// 이런 식으로 처리를 하게 되면, 스킬 슬롯이 추가될 때마다 하드코딩을 다시 해야 한다.
		// 이 부분은 개선이 필요함.. 코스튬 스킬 추가하다가 발견 (CHR-20222)
		return IsValidRange(InKeyMappingType, EGsKeyMappingType::KEY_MAPPING_SKILLSLOT_1, EGsKeyMappingType::KEY_MAPPING_COSTUMESKILL_1);
	};

	if (OnKeyboardSkill(InKeyMappingType, InEventType))
	{
		_skillUI->OnKeyboardEvent(InKeyMappingType, InEventType);
	}
	else //InEventType
	{
		if (InEventType == EInputEvent::IE_Pressed)
		{
			return;
		}
		switch (InKeyMappingType)
		{
		case EGsKeyMappingType::KEY_MAPPING_SCAN:
		{
			if (_scanButtonUI != nullptr)
			{
				_scanButtonUI->StartScanByUI();
			}
		}
		break;
		case EGsKeyMappingType::KEY_MAPPING_SWITCH_SPIRITSHOT:
		{
			if (_spiritShot != nullptr)
			{
				_spiritShot->ClickedSpiritShot();
			}
		}
		break;
		case EGsKeyMappingType::KEY_MAPPING_SWITCH_AUTO:
		{
			if (_autoUI != nullptr)
			{
				_autoUI->ClickedAutoButton();
			}
		}
		break;
		case EGsKeyMappingType::KEY_MAPPING_CAM_CHANGE:
		{
			if (_cameraUI != nullptr)
			{
				_cameraUI->ClickedCameraViewChange();
			}
		}
		break;
		case EGsKeyMappingType::KEY_MAPPING_MINIMAP:
		{
			if (_minimap != nullptr)
			{
				_minimap->ClickedWorldMap();
			}
		}
		break;
		case EGsKeyMappingType::KEY_MAPPING_BOOKMARK:
		{
			if (_minimap != nullptr)
			{
				_minimap->ClickedBookmark();
			}
		}
		break;
		case EGsKeyMappingType::KEY_MAPPING_CHARACTER:
		{
			if (_playerInfoUI != nullptr)
			{
				_playerInfoUI->OnClickPlayerDetailInfo();
			}
		}
		break;
		case EGsKeyMappingType::KEY_MAPPING_SAVEMODE:
		{
			if (_hudSaveBattery != nullptr)
			{
				_hudSaveBattery->OnClickSaveBattery();
			}
		}
		break;
		case EGsKeyMappingType::KEY_MAPPING_PICKUP:
		{
			if (_pickUpButton != nullptr)
			{
				_pickUpButton->OnClickPickUp();
			}
		}
		break;
		case EGsKeyMappingType::KEY_MAPPING_SWITCH_AUTOPOTION:
		{
			if (_consumableMenu != nullptr)
			{
				_consumableMenu->OnClickItemWindowsKey();
			}
		}
		break;
		case  EGsKeyMappingType::KEY_MAPPING_HUD_TAP_PREV:
		{
			if (_sideTab != nullptr)
			{
				_sideTab->OnClickPrevTab();				
			}
		}
		break;
		case  EGsKeyMappingType::KEY_MAPPING_HUD_TAP_NEXT:
		{
			if (_sideTab != nullptr)
			{
				_sideTab->OnClickNextTab();
			}
		}
		break;
		case EGsKeyMappingType::KEY_MAPPING_TARGET_CHANGE:
		{
			GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_INPUT_TARGET_CHANGE, nullptr);
		}
		break;
		default:
			break;
		}

		if (UGsUIManager* uiMgr = GUI())
		{
			uiMgr->OnKeyboardActionOptionKey(InKeyMappingType, InEventType);
		}
	}
}

void UGsUIHUDFrameMain::OnKeyboardSystemActionKey(EGsKeyMappingType InKeyMappingType, EInputEvent InEventType)
{
	if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectLocalPlayer* local = player->CastGameObject<UGsGameObjectLocalPlayer>())
		{
			if (UGsInputBindingLocalPlayer* inputBinder = local->GetInputBinder())
			{
				// 인풋이 막혀있으면 누르지 않음
				if (inputBinder->IsInputBlock(InKeyMappingType))
				{
					return;
				}
			}
		}
	}

	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->OnKeyboardActionOptionKey(InKeyMappingType, InEventType);
	}
}

/*
void UGsUIHUDFrameMain::OnWindowKeyMapping(const IGsMessageParam* InParam)
{
	const FGsInputEventMsgWindowKey* inputEventMsgWindowKey = InParam->Cast<const FGsInputEventMsgWindowKey>();

	switch (static_cast<EGsKeyMappingType>(inputEventMsgWindowKey->WindowKeyType))
	{

	case EGsKeyMappingType::KEY_MAPPING_SCAN:
	{
		if (_scanButtonUI != nullptr)
		{
			_scanButtonUI->StartScanByUI();
		}
	}
	break;
	case EGsKeyMappingType::KEY_MAPPING_SWITCH_SPIRITSHOT:
	{
		if (_spiritShot != nullptr)
		{
			_spiritShot->ClickedSpiritShot();
		}
	}
	break;
	case EGsKeyMappingType::KEY_MAPPING_SWITCH_AUTO:
	{
		if (_autoUI != nullptr)
		{
			_autoUI->ClickedAutoButton();
		}
	}
	break;
	case EGsKeyMappingType::KEY_MAPPING_CAM_CHANGE:
	{
		if (_cameraUI != nullptr)
		{
			_cameraUI->ClickedCameraViewChange();
		}
	}
	break;
	case EGsKeyMappingType::KEY_MAPPING_MINIMAP:
	{
		if (_minimap != nullptr)
		{
			_minimap->ClickedWorldMap();
		}
	}
	break;
	case EGsKeyMappingType::KEY_MAPPING_BOOKMARK:
	{
		if (_minimap != nullptr)
		{
			_minimap->ClickedBookmark();
		}
	}
	break;
	default:
		break;
	}
}*/