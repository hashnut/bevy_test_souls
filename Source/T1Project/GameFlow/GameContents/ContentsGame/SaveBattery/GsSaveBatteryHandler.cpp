// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSaveBatteryHandler.h"

#include "Framework/Application/SlateApplication.h"

#include "Data/GsDataContainManager.h"
#include "DataSchema/SaveBattery/GsSchemaSaveBatteryTime.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/Stat/GsStatBase.h"

#include "Item/GsItemManager.h"

#include "Input/GsInputEventMsgBase.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsSaveBatteryManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsObserverManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Message/MessageParam/GsInvenMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "Option/GsGameUserSettings.h"

#include "UI/UIContent/Tray/GsUITraySaveBattery.h"
#include "UI/UIContent/Tray/GsUITrayMovie.h"
#include "UI/UIContent/Tray/SaveBattery/GsUISaveBatteryMode.h"
#include "UI/UIContent/Tray/SaveBattery/GsUIInventorySaveBattery.h"
#include "UI/UIContent/HUD/GsUIHUDSystemInfo.h"
#include "UI/UIContent/HUD/GsUIHUDPlayerInfo.h"
#include "UI/UIContent/HUD/GsUIHUDSpiritShot.h"
#include "UI/UIContent/HUD/GsUIHUDConsumable.h"
#include "UI/UIContent/HUD/GsUIHUDCounterAttackButton.h"

#include "UTIL/GsSoundUtil.h"

#include "Unlock/GsUnlockDefine.h"

#include "Significance/GsGameViewPortClient.h"
#include "Sequence/GsSequencePlayer.h"
#include "Sequence/GsSequenceManager.h"

#include "T1Project.h"


FGsSaveBatteryHandler::~FGsSaveBatteryHandler()
{
}

void FGsSaveBatteryHandler::Init()
{
	FGsMessageHolder* msg = GMessage();
	MContents& contents = msg->GetContents();
	_contentsHandlerList.Emplace(contents.AddRaw(MessageContents::ACTIVE_SAVE_BATTERY,
		this, &FGsSaveBatteryHandler::OnActiveSaveBattery));

	_inputHandlerList.Emplace(msg->GetWindowInput().AddRaw(MessageInput::Window_keyboard_Action,
		+this, &FGsSaveBatteryHandler::OnKeyboardInput));

	_lastInteractionTime = FSlateApplication::Get().GetLastUserInteractionTime();

	InitEnterTime();

	MDungeonHUDTime& msgDungeonHUDTime = msg->GetDungeonHUDTime();
	_dungeonHUDTimeHandlerList.Emplace(msgDungeonHUDTime.AddRaw(MessageContentDungeonHUDTime::HUD_TIME,
		this, &FGsSaveBatteryHandler::OnDungeonTimeSetData));
	_dungeonHUDTimeHandlerList.Emplace(msgDungeonHUDTime.AddRaw(MessageContentDungeonHUDTime::ON_HUD_TIME,
		this, &FGsSaveBatteryHandler::OnDungeonTimePlay));
	_dungeonHUDTimeHandlerList.Emplace(msgDungeonHUDTime.AddRaw(MessageContentDungeonHUDTime::PAUSE_HUD_TIME,
		this, &FGsSaveBatteryHandler::OnDungeonTimePause));
	_dungeonHUDTimeHandlerList.Emplace(msgDungeonHUDTime.AddRaw(MessageContentDungeonHUDTime::OFF_HUD_TIME,
		this, &FGsSaveBatteryHandler::OnDungeonTimeStop));

	MGuildDungeon& msgGuildDungeon = msg->GetGuildDungeon();
	_guildDungeonHandlerList.Emplace(msgGuildDungeon.AddRaw(MessageContentGuildDungeon::ENTER,
		this, &FGsSaveBatteryHandler::OnDungeonTimePlay));
	_guildDungeonHandlerList.Emplace(msgGuildDungeon.AddRaw(MessageContentGuildDungeon::EXIT,
		this, &FGsSaveBatteryHandler::OnDungeonTimeStop));
}

void FGsSaveBatteryHandler::Close()
{
	OnClose();

	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
		return;

	MContents& contents = msg->GetContents();
	for (auto& iter : _contentsHandlerList)
	{
		contents.Remove(iter);
	}
	_contentsHandlerList.Empty();

	MWindowInput& windowInput = msg->GetWindowInput();
	for (auto& iter : _inputHandlerList)
	{
		windowInput.Remove(iter);
	}
	_inputHandlerList.Empty();

	MDungeonHUDTime& msgDungeonHUDTime = msg->GetDungeonHUDTime();
	for (auto& iter : _dungeonHUDTimeHandlerList)
	{
		msgDungeonHUDTime.Remove(iter);
	}
	_dungeonHUDTimeHandlerList.Empty();

	MGuildDungeon& msgGuildDungeon = msg->GetGuildDungeon();
	for (auto& iter : _guildDungeonHandlerList)
	{
		msgGuildDungeon.Remove(iter);
	}
	_guildDungeonHandlerList.Empty();
}

void FGsSaveBatteryHandler::Update(float InDeltaTime)
{
	const int32 SAVEBATTERY_AUTO_ENTER_OFF = 3;
	static double interactionTime = 0;

	if (UGsUIManager* uiManager = GUI())
	{
		if (uiManager->IsActiveWidget(TRAY_SAVEBATTERY_NAME))
			return;
	}

	// 관전 모드에서는 안함
	if (FGsObserverManager* observerManager = GSObserver())
	{
		if (observerManager->GetIsObserveStart())
		{
			return;
		}
	}

	_time += InDeltaTime;

	if (_tick > 0)
	{
		--_tick;
	}

	// 입력 판단
	interactionTime = FSlateApplication::Get().GetLastUserInteractionTime();
	if (_lastInteractionTime < interactionTime)
	{
		if (0 < FSlateApplication::Get().GetPressedMouseButtons().Num() ||
			GSSaveBattery()->IsKeyboardPressed())
		{
			if (true == _requestEvent && _tick <= 0)
			{
				_clickedBeforeStartEvent = true;
			}

			GSSaveBattery()->SetIsKeyboardPressed(false);
			_lastInteractionTime = interactionTime;
			_time = 0.f;
			return;
		}
	}

	// 자동 절전 모드 진입 판단
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		int32 findTimerIndex = gameUserSettings->GetSaveBatterySetting(EGsOptionSaveBattery::SAVEBATTERY_TIMER_MINUTE);
		if (_enterTimeList.IsValidIndex(findTimerIndex) && SAVEBATTERY_AUTO_ENTER_OFF != findTimerIndex)
		{
			if (_enterTimeList[findTimerIndex] * 60.f <= _time)
			{
				OnActiveSaveBattery();
				_time = 0.f;
			}
		}
	}
}

void FGsSaveBatteryHandler::Enter()
{
	if (UGsUIManager* uiManager = GUI())
	{
		// Tray 생성
		TWeakObjectPtr<UGsUIWidgetBase> saveBatteryWindow = uiManager->OpenAndGetWidget(TRAY_SAVEBATTERY_NAME);
		if (saveBatteryWindow.IsValid())
		{
			_trayUI = Cast<UGsUITraySaveBattery>(saveBatteryWindow);
			_trayUI->SetHandler(this);
			
			if (_isDungeonTimeShow)
			{
				_trayUI->PlayRemainTime();
			}
			_trayUI->InvalidateAllInternal();

			if (EGsUIHUDMode::CAMERA == uiManager->GetHUDMode())
			{
				uiManager->SetHUDMode(EGsUIHUDMode::NORMAL);
			}

			_exitType = SaveBatteryExitType::Normal;

			// 절전모드 중 불필요한 UI 정리 (불필요한 자원소모 방지)
			if (UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
			{
				if (UGsGameObjectCreature* creature = Cast<UGsGameObjectCreature>(localPlayer))
				{
					// 열 때 사망 상태일 경우 사망 UI 등이 존재하므로 닫지 않는다.
					if (false == creature->IsDeadState())
					{
						uiManager->CloseAllStack();
						GMessage()->GetContents().SendMessage(MessageContents::CLOSE_QUICK_MENU);
					}
				}
			}

			// 입력 막기
			FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::INPUT_LOCK_ALL, true);
			GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);

			// 메세지 및 이벤트 등록
			RegisterMessages();
			BindEvents();

			// 사운드 음소거. 알림음은 출력 가능
			UGsSoundUtil::MuteAllSound();
			UGsSoundUtil::ResetNoticeSoundVolumeBySettingValue();

			// 프레임 성능 일시 제한
			if (UGsDataContainManager* dataManager = GData())
			{
				if (UGsGlobalConstant* globalData = dataManager->GetGlobalData())
				{
					SetFrameRateLimit(globalData->_saveBatteryMaxFPS);
				}
			}

			// 월드 렌더링 끄기
			SetEnableWorldRendering(false);

			_isEntered = true;

			uiManager->SetHideFlags(EGsUIHideFlags::STATE_SAVE_BATTERY);

			GMessage()->GetSaveBattery().SendMessage(MessageContentSaveBattery::ENTER, nullptr);
		}
		else
		{
			if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
			{
				eventProgressManager->OnReqFinishEvent(EGsEventProgressType::SAVE_BATTERY);
			}
		}
	}	
}

void FGsSaveBatteryHandler::Exit()
{
	if (!_isEntered)
		return;

	_isEntered = false;

	if (nullptr != _trayUI)
	{
		_trayUI->SetHandler(nullptr);
		_trayUI = nullptr;
	}
	

	_time = 0.f;

	// 메세지 및 이벤트 해제
	RemoveMessage();
	UnBindEvents();

	// 입력 막기
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::INPUT_LOCK_ALL, false);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);

	// 사운드 복구
	UGsSoundUtil::ResetAllSoundVolumeBySettingValue();

	// 프레임 상태 복구
	SetFrameRateLimit(_prevFrameRate);

	// 월드 렌더링 켜기
	SetEnableWorldRendering(true);

	// 분해 정보가 있을 경우 채팅 메시지 출력
	if (FGsSaveBatteryManager* saveBatteryManager = GSSaveBattery())
	{
		saveBatteryManager->ShowAndClearAutoDecomposeItemAmount();
	}

	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->ClearHideFlags(EGsUIHideFlags::STATE_SAVE_BATTERY);
	}
	
	if (_isReservedCinematic)
	{
		if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
		{
			eventProgressManager->SetDirtyNextEventForce(true);
			_isReservedCinematic = false;
		}
	}
	else
	{
		switch (_exitType)
		{
		case SaveBatteryExitType::OfflinePlay:
		{
			//if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
			//{
			//	eventProgressManager->ClearAllData();
			//}
			if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
			{
				if (eventProgressManager->GetReservedEventCount() <= 0)
				{
					GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_OFFLINEPLAY);
				}
			}
			break;
		}
		case SaveBatteryExitType::ReturnScroll:
		{
			if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
			{
				eventProgressManager->ClearAllData();
			}

			if (UGsItemManager* itemManager = GItem())
			{
				if (const FGsSchemaItemCommon* returnScrollData = itemManager->FindItemSchemaDataBySubCategory(ItemCategorySub::WARP_TO_TOWN_SCROLL))
				{
					TArray<TWeakPtr<FGsItem>> returnScrolls = itemManager->FindByTID(returnScrollData->id);
					returnScrolls.Sort([](TWeakPtr<FGsItem> itemA, TWeakPtr<FGsItem> itemB)
						{
							return itemB.Pin()->GetAmount() < itemA.Pin()->GetAmount();
						}
					);

					itemManager->TryUseItem(returnScrolls.Last().Pin()->GetDBID());
				}
			}
			
			break;
		}
		}
		_exitType = SaveBatteryExitType::Normal;

		if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
		{
			eventProgressManager->OnReqFinishEvent(EGsEventProgressType::SAVE_BATTERY);
		}
	}

	GMessage()->GetSaveBattery().SendMessage(MessageContentSaveBattery::EXIT, nullptr);
}

void FGsSaveBatteryHandler::OnReconectionEnd()
{
	OnCloseAll();
}

void FGsSaveBatteryHandler::RegisterMessages()
{
	FGsMessageHolder* msg = GMessage();
	MHud& contentsHUD = msg->GetContentsHud();
	_hudHandlerList.Emplace(contentsHUD.AddRaw(MessageContentHud::INVALIDATE_ALL,
		this, &FGsSaveBatteryHandler::InvalidateAll));
	_hudHandlerList.Emplace(contentsHUD.AddRaw(MessageContentHud::INVALIDATE_CONSUMEABLE,
		this, &FGsSaveBatteryHandler::InvalidateComsumable));
	_hudHandlerList.Emplace(contentsHUD.AddRaw(MessageContentHud::INVALIDATE_PLAYERINFO,
		this, &FGsSaveBatteryHandler::InvalidatePlayerInfo));
	_hudHandlerList.Emplace(contentsHUD.AddRaw(MessageContentHud::INVALIDATE_SYSTEMINFO,
		this, &FGsSaveBatteryHandler::InvalidateSystemInfo));
	_hudHandlerList.Emplace(contentsHUD.AddRaw(MessageContentHud::INVALIDATE_COUNTERATTACK,
		this, &FGsSaveBatteryHandler::InvalidateCounterAttack));

	MUserBasicInfo& basicInfo = msg->GetUserBasicInfo();
	_userInfoHandlerList.Emplace(basicInfo.AddRaw(MessageUserInfo::LEVEL,
		this, &FGsSaveBatteryHandler::OnChangeLocalPlayerLevel));
	_userInfoHandlerList.Emplace(basicInfo.AddRaw(MessageUserInfo::EXP,
		this, &FGsSaveBatteryHandler::OnChangePlayerExp));
	_userInfoHandlerList.Emplace(basicInfo.AddRaw(MessageUserInfo::INCREASE_GOLD,
		this, &FGsSaveBatteryHandler::OnIncreaseGold));
	_userInfoHandlerList.Emplace(basicInfo.AddRaw(MessageUserInfo::INCREASE_EXP,
		this, &FGsSaveBatteryHandler::OnIncreasePlayerExp));

	MGameObject& gameObject = msg->GetGameObject();
	_objectIDHandlerList.Emplace(gameObject.AddRaw(MessageGameObject::LOCAL_HIT,
		this, &FGsSaveBatteryHandler::OnHitLocal));
	_objectIDHandlerList.Emplace(gameObject.AddRaw(MessageGameObject::LOCAL_DIE,
		this, &FGsSaveBatteryHandler::OnLocalPlayerDie));
	_objectIDHandlerList.Emplace(gameObject.AddRaw(MessageGameObject::UPDATE_USER_STATUS,
		this, &FGsSaveBatteryHandler::InvalidateMode));

	MItem& item = msg->GetItem();
	_itemHandlerList.Emplace(item.AddRaw(MessageItem::ADDITEMLIST,
		this, &FGsSaveBatteryHandler::OnAddItemList));
	_itemHandlerList.Emplace(item.AddRaw(MessageItem::UPDATEITEM,
		this, &FGsSaveBatteryHandler::OnUpdateItem));
	_itemHandlerList.Emplace(item.AddRaw(MessageItem::REMOVEITEM,
		this, &FGsSaveBatteryHandler::OnRemoveItem));
	_itemHandlerList.Emplace(item.AddRaw(MessageItem::REMOVEITEMS,
		this, &FGsSaveBatteryHandler::OnRemoveItemList));

	MInventory& inventoryEvent = msg->GetInventoryEvent();
	_inventoryHandlerList.Emplace(inventoryEvent.AddRaw(MessageContentInven::UPDATE_ITEM_WEIGHT,
		this, &FGsSaveBatteryHandler::OnUpdateItemWeight));
	_inventoryHandlerList.Emplace(inventoryEvent.AddRaw(MessageContentInven::UPDATE_ITEM_WEIGHT_PENALTY,
		this, &FGsSaveBatteryHandler::OnUpdateWeightPenaltyType));

	MSaveBattery& saveBattery = msg->GetSaveBattery();
	_saveBatteryHandlerList.Emplace(saveBattery.AddRaw(MessageContentSaveBattery::CLOSE,
		this, &FGsSaveBatteryHandler::OnClose));
	_saveBatteryHandlerList.Emplace(saveBattery.AddRaw(MessageContentSaveBattery::CLOSE_ALL,
		this, &FGsSaveBatteryHandler::OnCloseAll));
	_saveBatteryHandlerList.Emplace(saveBattery.AddRaw(MessageContentSaveBattery::CLOSE_UI,
		this, &FGsSaveBatteryHandler::OnCloseUI));
	_saveBatteryHandlerList.Emplace(saveBattery.AddRaw(MessageContentSaveBattery::INVALIDATE_INVEN,
		this, &FGsSaveBatteryHandler::InvalidateInventory));

	MUnlock& msgUnlock = msg->GetUnlock();
	_msgHandleUnlockList.Emplace(msgUnlock.AddRaw(MessageContentUnlock::NOTIFY_CONTENTS_UNLOCK,
		this, &FGsSaveBatteryHandler::OnContentsUnlock));

	MPKBook& msgPKBook = msg->GetPKBook();
	_pkBookHandlerList.Emplace(msgPKBook.AddRaw(MessageContentPKBook::UI_INVALID_DETECTIONTARGET,
		this, &FGsSaveBatteryHandler::OnInvalidateDetectionTargetList));
	_pkBookHandlerList.Emplace(msgPKBook.AddRaw(MessageContentPKBook::UI_INVALID_DETECTIONTARGET_LIST,
		this, &FGsSaveBatteryHandler::OnInvalidateDetectionTargetList));

}

void FGsSaveBatteryHandler::RemoveMessage()
{
	FGsMessageHolder* msg = GMessage();
	MHud& contentsHUD = msg->GetContentsHud();
	for (auto& iter : _hudHandlerList)
	{
		contentsHUD.Remove(iter);
	}
	_hudHandlerList.Empty();

	MUserBasicInfo& basicInfo = msg->GetUserBasicInfo();
	for (auto& iter : _userInfoHandlerList)
	{
		basicInfo.Remove(iter);
	}
	_userInfoHandlerList.Empty();

	MGameObject& gameObject = msg->GetGameObject();
	for (auto& iter : _objectIDHandlerList)
	{
		gameObject.Remove(iter);
	}
	_objectIDHandlerList.Empty();

	MItem& item = msg->GetItem();
	for (auto& iter : _itemHandlerList)
	{
		item.Remove(iter);
	}
	_itemHandlerList.Empty();

	MInventory& inventoryEvent = msg->GetInventoryEvent();
	for (auto& iter : _inventoryHandlerList)
	{
		inventoryEvent.Remove(iter);
	}
	_inventoryHandlerList.Empty();

	MSaveBattery& saveBattery = msg->GetSaveBattery();
	for (auto& iter : _saveBatteryHandlerList)
	{
		saveBattery.Remove(iter);
	}
	_saveBatteryHandlerList.Empty();

	MUnlock& msgUnlock = msg->GetUnlock();
	for (MsgHandleUnlock& handle : _msgHandleUnlockList)
	{
		msgUnlock.Remove(handle);
	}
	_msgHandleUnlockList.Empty();

	MPKBook& msgPKBook = msg->GetPKBook();
	for (MsgPKBookHandle& handle : _pkBookHandlerList)
	{
		msgPKBook.Remove(handle);
	}
	_pkBookHandlerList.Empty();
	
}

void FGsSaveBatteryHandler::BindEvents()
{
	// 캐릭터 상태 이벤트
	UGsGameObjectLocalPlayer* localPlayer = nullptr;
	if (UGsGameObjectManager* spawner = GSGameObject())
	{
		localPlayer = Cast<UGsGameObjectLocalPlayer>(spawner->FindObject(EGsGameObjectType::LocalPlayer));
	}

	if (nullptr != localPlayer)
	{
		if (FGsGameObjectDataCreature* creatureData = localPlayer->GetCastData<FGsGameObjectDataCreature>())
		{
			if (TSharedPtr<FGsStatBase> base = creatureData->GetStatBase())
			{
				_changeStatHandle = base->OnChangeStatDelegate.AddRaw(this, &FGsSaveBatteryHandler::OnSetLocalPlayerStat);
			}
		}
	}
}

void FGsSaveBatteryHandler::UnBindEvents()
{
	// 캐릭터 상태 이벤트
	UGsGameObjectLocalPlayer* localPlayer = nullptr;
	if (UGsGameObjectManager* spawner = GSGameObject())
	{
		localPlayer = Cast<UGsGameObjectLocalPlayer>(spawner->FindObject(EGsGameObjectType::LocalPlayer));
	}

	if (nullptr != localPlayer)
	{
		if (FGsGameObjectDataCreature* creatureData = localPlayer->GetCastData<FGsGameObjectDataCreature>())
		{
			if (TSharedPtr<FGsStatBase> base = creatureData->GetStatBase())
			{
				base->OnChangeStatDelegate.Remove(_changeStatHandle);
			}
		}
	}
}

void FGsSaveBatteryHandler::InitEnterTime()
{
	const UGsTableSaveBatteryTime* table = Cast<UGsTableSaveBatteryTime>(FGsSchemaSaveBatteryTime::GetStaticTable());
	if (nullptr == table)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[FGsSaveBatteryHandler::InitEnterTime] SaveBatteryTimeData is Null"));
#endif
		return;
	}

	TArray<const FGsSchemaSaveBatteryTime*> allRows;
	if (false == table->GetAllRows(allRows))
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[FGsSaveBatteryHandler::InitEnterTime] SaveBatteryTimeData is Empty"));
#endif
		return;
	}

	_enterTimeList.Empty();
	for (int32 i = 0; i < allRows.Num(); ++i)
	{
		if (nullptr == allRows[i])
			continue;

		_enterTimeList.Emplace(allRows[i]->time);
	}
}

bool FGsSaveBatteryHandler::CanEnterSaveBattery()
{
	// 관전 모드가 진행중인가?
	if (FGsObserverManager* observerManager = GSObserver())
	{
		if (observerManager->GetIsObserveStart())
		{
			return false;
		}
	}

	if (UGsUIManager* uiManager = GUI())
	{
		// 무비가 켜져있는가?
		if (uiManager->IsActiveWidget(TRAY_MOVIE_NAME))
		{
			return false;
		}

		// NPC 대화중인가?
		if (uiManager->IsActiveWidget(NPC_DIALOG_NAME))
		{
			return false;
		}

		// 오프라인 플레이에 진입 중 혹은 진입했는가?
		if (uiManager->IsActiveWidget(OFFLINE_PLAY_NAME))
		{
			return false;
		}
	}
	
	// 시네마틱이 진행중인가?
	if (UGsSequenceManager* sequenceManager = GSequence())
	{
		if (sequenceManager->GetPlayingSequenceResId() > 0)
		{
			return false;
		}
	}

	return true;
}

void FGsSaveBatteryHandler::OnActiveSaveBattery()
{
	if (false == CanEnterSaveBattery())
		return;

	if (!GUI()->IsActiveWidget(TRAY_SAVEBATTERY_NAME))
	{
		if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
		{
			eventProgressManager->OnReqStartEvent(EGsEventProgressType::SAVE_BATTERY, this);
			_requestEvent = true;
			_tick = 2;
		}
	}
}

void FGsSaveBatteryHandler::OnKeyboardInput(const IGsMessageParam* InParam)
{
	GSSaveBattery()->SetIsKeyboardPressed(true);
}

void FGsSaveBatteryHandler::InvalidateAll()
{
	if (_trayUI.IsValid())
	{
		_trayUI->InvalidateAll();
	}
}

void FGsSaveBatteryHandler::InvalidateComsumable()
{
	if (_trayUI.IsValid())
	{
		if (UGsUIHUDConsumable* consumableUI = _trayUI->GetConsumableUI())
		{
			consumableUI->InvalidateAll();
		}

		if (UGsUIHUDSpiritShot* spiritShotUI = _trayUI->GetSpiritShotUI())
		{
			spiritShotUI->InvalidateAll();
		}

		_trayUI->InvalidateReturnScroll();
	}
}

void FGsSaveBatteryHandler::InvalidatePlayerInfo()
{
	if (_trayUI.IsValid())
	{
		if (UGsUIHUDPlayerInfo* playerInfoUI = _trayUI->GetPlayerInfoUI())
		{
			playerInfoUI->InvalidateAll();
		}
	}
}

void FGsSaveBatteryHandler::InvalidateSystemInfo()
{
	if (_trayUI.IsValid())
	{
		if (UGsUIHUDSystemInfo* systemInfoUI = _trayUI->GetSystemInfoUI())
		{
			systemInfoUI->InvalidateAll();
		}
	}
}

void FGsSaveBatteryHandler::InvalidateMode(const IGsMessageParam* InParam)
{
	if (_trayUI.IsValid())
	{
		_trayUI->InvalidateMode();
	}
}

void FGsSaveBatteryHandler::OnChangeLocalPlayerLevel(uint64 InLevel)
{
	InvalidatePlayerInfo();
	OnIncreasePlayerExp(0); // 현재 레벨에 맞는 경험치 % 표현이 되도록 갱신
}

void FGsSaveBatteryHandler::OnChangePlayerExp(uint64 InExp)
{
	if (_trayUI.IsValid())
	{
		if (UGsUIHUDSystemInfo* systemInfo = _trayUI->GetSystemInfoUI())
		{
			systemInfo->InvalidateExp();
		}
	}
}

void FGsSaveBatteryHandler::OnIncreasePlayerExp(uint64 InExp)
{
	if (_trayUI.IsValid())
	{
		_trayUI->OnIncreasePlayerExp(InExp);
	}
}

void FGsSaveBatteryHandler::OnIncreaseGold(uint64 InAmount)
{
	if (_trayUI.IsValid())
	{
		_trayUI->OnIncreaseGold(InAmount);
	}
}

void FGsSaveBatteryHandler::OnAddItemList(FGsItemMessageParamBase& InParam)
{
	if (_trayUI.IsValid())
	{
		if (UGsUIInventorySaveBattery* inventoryUI = _trayUI->GetInventoryUI())
		{
			const FGsAddItemList* param = StaticCast<const FGsAddItemList*>(&InParam);
			if (nullptr != param)
			{
				for (ItemDBId itemDBId : param->_itemDbidList)
				{
					inventoryUI->AddItem(itemDBId);
				}
			}
		}
		_trayUI->InvalidateInventoryCount();
	}
}

void FGsSaveBatteryHandler::OnUpdateItem(FGsItemMessageParamBase& InParam)
{
	if (_trayUI.IsValid())
	{
		if (UGsUIInventorySaveBattery* inventoryUI = _trayUI->GetInventoryUI())
		{
			const FGsUpdateItem* param = StaticCast<const FGsUpdateItem*>(&InParam);
			check(param);

			inventoryUI->UpdateItem(param->_itemDBID);
		}
		_trayUI->InvalidateInventoryCount();
	}
}

void FGsSaveBatteryHandler::OnRemoveItem(FGsItemMessageParamBase& InParam)
{
	if (_trayUI.IsValid())
	{
		if (UGsUIInventorySaveBattery* inventoryUI = _trayUI->GetInventoryUI())
		{
			const FGsRemoveItem* param = StaticCast<const FGsRemoveItem*>(&InParam);
			check(param);

			inventoryUI->RemoveItem(param->_itemDBID, param->_itemTID);
		}
	}
}

void FGsSaveBatteryHandler::OnRemoveItemList(FGsItemMessageParamBase& InParam)
{
	if (_trayUI.IsValid())
	{
		_trayUI->InvalidateInventoryCount();
	}
}

void FGsSaveBatteryHandler::OnUpdateItemWeight(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
		return;

	const FGsUpdateInvenWeightInfo* param = InParam->Cast<const FGsUpdateInvenWeightInfo>();
	if (nullptr == param)
		return;

	if (_trayUI.IsValid())
	{
		_trayUI->OnUpdateItemWeight(param->_weightPercent);
	}
}

void FGsSaveBatteryHandler::OnUpdateWeightPenaltyType(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
		return;

	const FGsUpdateInvenWeightPenaltyType* param = InParam->Cast<const FGsUpdateInvenWeightPenaltyType>();
	if (nullptr == param)
		return;

	_trayUI->OnUpdateWeightPenaltyType(param->_gradeType);
}

void FGsSaveBatteryHandler::OnHitLocal(const IGsMessageParam* InParam)
{
	const int32 ALERT_OFF = 0;
	const int32 ALERT_ON = 1;

	const FGsGameObjectMessageParamTarget* castParam = InParam->Cast<const FGsGameObjectMessageParamTarget>();
	if (nullptr == castParam)
		return;

	UGsGameObjectBase* attacker = castParam->_paramTarget;
	if (attacker->IsObjectType(EGsGameObjectType::Player) == true)
	{
		if (UGsGameUserSettings* userSetting = GGameUserSettings())
		{
			int32 value = userSetting->GetSaveBatterySetting(EGsOptionSaveBattery::IS_PVP_AUTO_OFF);
			if (ALERT_ON == value)
			{
				OnCloseAll();
			}
		}
	}
}

void FGsSaveBatteryHandler::OnLocalPlayerDie(const IGsMessageParam* InParam)
{
	if (_trayUI.IsValid())
	{
		_trayUI->OnLocalPlayerDie();
	}
}

void FGsSaveBatteryHandler::OnContentsUnlock(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
		return;

	if (const FGsPrimitivePairInt32* param = StaticCast<const FGsPrimitivePairInt32*>(InParam))
	{
		if (StaticCast<EGsUnlockContentsType>(param->_first) == EGsUnlockContentsType::OFFLINE_PLAY)
		{
			_trayUI->InvalidateOfflinePlay();
		}
	}
}

void FGsSaveBatteryHandler::OnDungeonTimeSetData(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
		return;

	const FGsUIMsgParamDungeonTime* message = InParam->Cast<const FGsUIMsgParamDungeonTime>();
	if (nullptr == message)
		return;

	_isDungeonTimeLimit = message->_isTimeLimit;
	_isDungeonTimeShowSecond = message->_isShowSencond;
	_dungeonEndTime = message->_time;
}

void FGsSaveBatteryHandler::OnDungeonTimePlay(const IGsMessageParam* InParam)
{
	_isDungeonTimeShow = true;

	if (_isDungeonTimeLimit)
	{
		if (_trayUI.IsValid())
		{
			_trayUI->PlayRemainTime();
		}
	}
}

void FGsSaveBatteryHandler::OnDungeonTimePause(const IGsMessageParam* InParam)
{
	if (_isDungeonTimeLimit)
	{
		if (_trayUI.IsValid())
		{
			_trayUI->PauseRemainTime();
		}
	}
}

void FGsSaveBatteryHandler::OnDungeonTimeStop(const IGsMessageParam* InParam)
{
	_isDungeonTimeShow = false;
	if (_trayUI.IsValid())
	{
		_trayUI->StopRemainTime();
	}
}

void FGsSaveBatteryHandler::OnInvalidateDetectionTargetList(const IGsMessageParam* InParam)
{
	if (_trayUI.IsValid())
	{
		_trayUI->InvalidateDetectionTarget();
	}
}

void FGsSaveBatteryHandler::OnClose(const IGsMessageParam* InParam)
{
	// 명시적으로 종료하면 ReStart를 False한다.
	// 레벨 전환, 자동 부활 등에서 UI Manager를 통해 CloseAllStack 등을 통해 닫으면
	// NativeDestruct만 실행되기 때문에 ReStart가 초기화 되지 않는다.
	if (FGsSaveBatteryManager* saveBatteryManager = GSSaveBattery())
	{
		saveBatteryManager->SetReStart(false);
	}

	if (UGsUIManager* uiManager = GUI())
	{
		if (uiManager->IsActiveWidget(TRAY_SAVEBATTERY_NAME))
		{
			uiManager->CloseByKeyName(TRAY_SAVEBATTERY_NAME, false, true);
		}
		else
		{
			Exit();
		}
	}
}

void FGsSaveBatteryHandler::OnCloseAll(const IGsMessageParam* InParam)
{
	// 명시적으로 종료하면 ReStart를 False한다.
	GSSaveBattery()->SetReStart(false);

	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->CloseAllStack(_trayUI.Get());
	}
}

void FGsSaveBatteryHandler::OnCloseUI(const IGsMessageParam* InParam)
{
	if (GSSaveBattery()->IsReStart())
		return;

	if (UGsUIManager* uiManager = GUI())
	{
		if (uiManager->IsActiveWidget(TRAY_SAVEBATTERY_NAME))
		{
			uiManager->CloseByKeyName(TRAY_SAVEBATTERY_NAME, false, true);
		}
		else
		{
			Exit();
		}
	}
}

void FGsSaveBatteryHandler::InvalidateInventory(const IGsMessageParam* InParam /*= nullptr*/)
{
	const FGsUIMsgParamIntArray* param = StaticCast<const FGsUIMsgParamIntArray*>(InParam);
	if (nullptr == param)
		return;

	if (_trayUI.IsValid())
	{
		if (UGsUIInventorySaveBattery* inventoryUI = _trayUI->GetInventoryUI())
		{
			if (0 == param->_data.Num())
			{
				inventoryUI->InvalidateAll();
			}
			else
			{
				for (int32 index : param->_data)
				{
					inventoryUI->RefreshIconByIndex(index);
				}
			}
		}
	}
}

void FGsSaveBatteryHandler::OnSetLocalPlayerStat(const FGsStatInfo& InStatInfo)
{
	if (_trayUI.IsValid())
	{
		if (UGsUIHUDPlayerInfo* playerInfo = _trayUI->GetPlayerInfoUI())
		{
			playerInfo->SetStat(InStatInfo);
		}
	}
}

void FGsSaveBatteryHandler::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{
	_requestEvent = false;

	if (_clickedBeforeStartEvent)
	{
		_clickedBeforeStartEvent = false;
		GSEventProgress()->ClearAllData();
		return;
	}

	Enter();
}

void FGsSaveBatteryHandler::FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType)
{
	if (EGsEventProgressType::EVENT_ACTION_CINEMATIC == In_nextType)
	{
		if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
		{
			eventProgressManager->SetDirtyNextEventForce(false);
			_isReservedCinematic = true;
		}
	}
	else
	{
		OnCloseAll();
	}
}

void FGsSaveBatteryHandler::RequestOfflinePlay()
{
	_exitType = SaveBatteryExitType::OfflinePlay;

	OnCloseAll();
}

void FGsSaveBatteryHandler::RequestReturn()
{
	_exitType = SaveBatteryExitType::ReturnScroll;

	OnCloseAll();
}

void FGsSaveBatteryHandler::SetEnableWorldRendering(const bool bInEnable)
{
	if (nullptr == GEngine)
		return;

	UGsGameViewPortClient* viewportClient = Cast<UGsGameViewPortClient>(GEngine->GameViewport);
	if (nullptr == viewportClient)

		return;

	viewportClient->bDisableWorldRendering = !bInEnable;
}

void FGsSaveBatteryHandler::SetFrameRateLimit(float InFrameRate)
{
	if (UGsGameUserSettings* userSetting = GGameUserSettings())
	{
		if (UGsDataContainManager* dataManager = GData())
		{
			if (UGsGlobalConstant* globalData = dataManager->GetGlobalData())
			{
				if (nullptr != GEngine)
				{
					_prevFrameRate = GEngine->GetMaxFPS();
					userSetting->SetFrameRateLimit(InFrameRate);
					GEngine->SetMaxFPS(InFrameRate);
				}
			}
		}
	}
}

void FGsSaveBatteryHandler::InvalidateCounterAttack()
{
	if (_trayUI.IsValid())
	{
		if (UGsUIHUDCounterAttackButton* counterAttackButtonUI = _trayUI->GetCounterAttackButtonUI())
		{
			counterAttackButtonUI->UpdateVisible();
		}
	}
}