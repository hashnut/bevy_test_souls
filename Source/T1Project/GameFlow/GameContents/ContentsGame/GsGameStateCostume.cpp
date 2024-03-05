#include "GsGameStateCostume.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsCostumeMessageParam.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "EventProgress/GsEventProgressDefine.h"

#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

#include "ActorEx/GsActorCostumeSceneObject.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "DataSchema/GsSchemaEnums.h"

#include "UI/UIContent/Window/GsUIWindowCostume.h"

#include "Hud/Summon/GsSummonHandler.h"

#include "Management/ScopeGlobal/GsOptionManager.h"

FGsGameStateCostume::FGsGameStateCostume() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsCostume)
{
}

FGsGameStateCostume::~FGsGameStateCostume()
{
	// 2023/6/13 PKT - 소멸 하는 과정에서 Exit를 호출 하면 내부적으로 Update Reddot에서 크래시 발생 여부 있음.
	//					소멸하는 것이 문제라면 다른 방법을 찾아 보자.
	/*if (_actorScene.IsValid())
	{
		Exit();
	}*/
}

void FGsGameStateCostume::Enter()
{
	FGsContentsGameBase::Enter();	

	// 그래픽 옵션이 낮더라도 해당 창 열때는 최상으로 변경되게 수정
	UGsGameUserSettings* gameUserSettings = GGameUserSettings();
	if (gameUserSettings != nullptr)
	{
		gameUserSettings->ApplyCharacterTopGraphicSettings();
	}

	RegisterMessages();

	// bak1210 
	//지형 꺼짐현상 제거
	UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (player)
	{
		GLevel()->HoldStreamingLevelByLocation(player->GetLocation());
	}

	UWorld* world = GEngine->GameViewport->GetWorld();
	auto sceneObjectPath = GData()->GetGlobalData()->_costumeSceneObjectPath.ToString();
	sceneObjectPath += TEXT("_C");

	if (UClass* loadObjectC = LoadObject<UClass>(nullptr, *sceneObjectPath))
	{
		FTransform tr = FTransform::Identity;
		auto pos = tr.GetLocation();
		if (player)
		{
			pos = player->GetLocation();
		}
		pos.Z = 1000000.f;
		tr.SetLocation(pos);

		FActorSpawnParameters spawnParam = FActorSpawnParameters();
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AGsActorCostumeSceneObject* actor = Cast<AGsActorCostumeSceneObject>(world->SpawnActor(loadObjectC, &tr, spawnParam)))
		{
			_actorScene = actor;
			actor->InitAction();
		}
	}

	_summonSceneHandler = UGsSummonManager::GetSummonHandler();

	// UI 출력
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->SetHideFlags(EGsUIHideFlags::STATE_SCENE_WINDOW);

		TWeakObjectPtr<UGsUIWidgetBase> costumeWindow = uiMgr->OpenAndGetWidget(TEXT("WindowCostume"));
		if (costumeWindow.IsValid())
		{
			_windowUI = Cast<UGsUIWindowCostume>(costumeWindow);

			if (_windowUI.IsValid())
			{
				if (_windowUI->IsStackRecoverFlag() == false)
				{
					UGsUIWindowCostume::UIInitData initParam{ 0, 0 };
					_windowUI.Get()->InitializeData(&initParam);
				}
			}
		}

		SetNavigationBar(CurrencyType::NONE);
	}

	// 페어리 화면으로 들어왔다고 이벤트 전달(이전 환경효과가 페어리 화면에 영향주지 않게 하기 위함)
	FGsPrimitiveInt32 param(static_cast<int32>(EGsEnvEvent::FAIRY_ROOM));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_START_EVENT, &param);
}

void FGsGameStateCostume::Exit()
{
	// 닫길때는 원래대로
	UGsGameUserSettings* gameUserSettings = GGameUserSettings();
	if (gameUserSettings != nullptr)
	{
		gameUserSettings->UndoApplyCharacterTopGraphicSettings();
	}

	UnregisterMessages();

	_windowUI = nullptr;

	if (_summonSceneHandler)
	{
		UGsCostumeManager* costumeManager = GSCostume();

		// 현재 play 중일때만 endsummon
		if (_summonSceneHandler->GetIsSummonPlay() == true)
		{
			_summonSceneHandler->EndSummon(false);
			
		}
		// 조이스틱 날렸는지 별도 체크
		if (_summonSceneHandler->GetIsJoystickClear() == true)
		{
			_summonSceneHandler->RestoreJoystick();
		}
		_summonSceneHandler = nullptr;
	}

	if (_actorScene.IsValid())
	{
		_actorScene.Get()->EndAction();

		if (UWorld* world = _actorScene.Get()->GetWorld())
			world->DestroyActor(_actorScene.Get());

	}
	_actorScene.Reset();

	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->ClearHideFlags(EGsUIHideFlags::STATE_SCENE_WINDOW);
	}

	GLevel()->UnHoldStreamingLevel();

	if (FGsMessageHolder* messageMgr = GMessage())
	{
		// 페어리 화면에서 나간다고 이벤트 전달(이전 환경효과로 복원 위함)
		FGsPrimitiveInt32 param(static_cast<int32>(EGsEnvEvent::FAIRY_ROOM));
		messageMgr->GetGameObject().SendMessage(MessageGameObject::ENV_END_EVENT, &param);

		messageMgr->GetHudRedDot().SendMessage(MessageContentHUDRedDot::COSTUME, false);
	}

	if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
	{
		eventProgressManager->OnReqFinishEvent(EGsEventProgressType::COSTUME_ROOM);
	}


	FGsContentsGameBase::Exit();
}


void FGsGameStateCostume::OnResetUI(UGsUIManager::FOnUIManagerResetUI& resetHandler)
{
	resetHandler.AddLambda([](int resetType) {
		if (resetType == 1) //connection 단절
		{
			GUI()->SetHideFlags(EGsUIHideFlags::STATE_SCENE_WINDOW);
		}
		});
}

void FGsGameStateCostume::RegisterMessages()
{
	auto& costumeMsgManager = GMessage()->GetCostume();

	_msgCostumeHandleList.Emplace(costumeMsgManager.AddRaw(MessageCostume::UPDATED,
		this, &FGsGameStateCostume::OnUpdateCostume));

	_msgCostumeHandleList.Emplace(costumeMsgManager.AddRaw(MessageCostume::COLLECTION_UPDATE,
		this, &FGsGameStateCostume::OnUpdateCostumeCollection));

	_msgCostumeHandleList.Emplace(costumeMsgManager.AddRaw(MessageCostume::CHANGE_SELECTION,
		this, &FGsGameStateCostume::OnSelectedCostume));

	_msgCostumeHandleList.Emplace(costumeMsgManager.AddRaw(MessageCostume::CHANGE_WEARON,
		this, &FGsGameStateCostume::OnWearOnCostume));

	_msgCostumeHandleList.Emplace(costumeMsgManager.AddRaw(MessageCostume::OBTAIN_RESULT,
		this, &FGsGameStateCostume::OnObtainCostumeResult));

	_msgCostumeHandleList.Emplace(costumeMsgManager.AddRaw(MessageCostume::CEILING_RESULT,
		this, &FGsGameStateCostume::OnCeilingCostumeResult));

	_msgCostumeHandleList.Emplace(costumeMsgManager.AddRaw(MessageCostume::SUMMONWAIT_UPDATE,
		this, &FGsGameStateCostume::OnSummonWaiingsUpdate));

	_msgCostumeHandleList.Emplace(costumeMsgManager.AddRaw(MessageCostume::SUMMON_COMPLETE,
		this, &FGsGameStateCostume::OnSummonComplete));

	_msgCostumeHandleList.Emplace(costumeMsgManager.AddRaw(MessageCostume::CHANGE_APPEARANCE,
		this, &FGsGameStateCostume::OnSelectedAppearance));

	_msgCostumeHandleList.Emplace(costumeMsgManager.AddRaw(MessageCostume::FIND_COSTUME,
		this, &FGsGameStateCostume::OnFindCostume));

	_msgRedDotHandle = GMessage()->GetHudRedDot().AddRaw(MessageContentHUDRedDot::COSTUME,
		this, &FGsGameStateCostume::OnUpdateRedDot);

	_msgGameReconnected = GMessage()->GetSystem().AddRaw(MessageSystem::GAME_SERVER_RECONNECTION_SUCCESS,
		this, &FGsGameStateCostume::OnGameServerReconnected);
}

void FGsGameStateCostume::UnregisterMessages()
{
	auto& costumeMsgManager = GMessage()->GetCostume();
	for (auto handler : _msgCostumeHandleList)
	{
		costumeMsgManager.Remove(handler);
	}

	GMessage()->GetHudRedDot().Remove(_msgRedDotHandle);

	GMessage()->GetSystem().Remove(_msgGameReconnected);
}

void FGsGameStateCostume::OnUpdateRedDot(bool bInActive)
{
	if (_windowUI.IsValid())
	{
		_windowUI.Get()->OnUpdateRedDot();
	}
}

void FGsGameStateCostume::OnUpdateCostume(const IGsMessageParam* InParam)
{
	const FGsUIMsgMsgUpdateList* param = InParam->Cast<const FGsUIMsgMsgUpdateList>();
	if (param)
	{
		auto list = param->_dataList;
		_windowUI.Get()->OnUpdateCostume(list);
	}
}

void FGsGameStateCostume::OnUpdateCostumeCollection(const IGsMessageParam* InParam)
{
	const FGsUIMsgMsgUpdateList* param = InParam->Cast<const FGsUIMsgMsgUpdateList>();
	if (param)
	{
		auto list = param->_dataList;
		_windowUI.Get()->OnUpdateCostumeCollection(list);
	}
}

void FGsGameStateCostume::OnSelectedCostume(const IGsMessageParam* InParam)
{
	const FGsCostumeMsgParamWearOn* param = InParam->Cast<const FGsCostumeMsgParamWearOn>();
	if (param)
	{
		auto costumeID = param->_costumeId;
		if (auto actorScene = _actorScene.Get())
		{
			actorScene->SetCostume(costumeID, param->_isHairOn);
		}
	}
}
void FGsGameStateCostume::OnWearOnCostume(const IGsMessageParam* InParam)
{
	if (_windowUI.IsValid())
	{
		_windowUI.Get()->OnChangeDressUp();
	}
}

void FGsGameStateCostume::OnObtainCostumeResult(const IGsMessageParam* InParam)
{
	if (_windowUI.IsValid())
	{
		const FGsCostumeMsgParamObtainList* param = InParam->Cast<const FGsCostumeMsgParamObtainList>();
		if (param)
		{
			auto list = param->_dataList;
			bool isShowSummon = GData()->GetGlobalData()->_summonIsShow;
			if (isShowSummon && _actorScene.IsValid())
			{
				_actorScene->ChangeSubScene(true);
				_actorScene->SetHidden(true);
			}

			_windowUI.Get()->OnObtainCostumeResult(param->_obtainReason, list, isShowSummon);
		}
	}
}

void FGsGameStateCostume::OnSummonWaiingsUpdate(const IGsMessageParam* InParam)
{
	if (_windowUI.IsValid())
	{
		const FGsCostumeMsgParamCostumeSummonWait* param = InParam->Cast<const FGsCostumeMsgParamCostumeSummonWait>();
		if (param)
		{
			if (param->_updateType == FGsCostumeMsgParamCostumeSummonWait::Confirm)
			{
				_windowUI.Get()->OnConfirmSummonWaiting(param->_summonWaitDBId);
			}
			else
			{
				_windowUI.Get()->OnUpdateSummonWaiting(param->_summonWaitDBId);
			}
		}
	}
}

void FGsGameStateCostume::OnCeilingCostumeResult(const IGsMessageParam* InParam)
{
	if (_windowUI.IsValid())
	{
		const FGsMsgCostumeCeilingResult* param = InParam->Cast<const FGsMsgCostumeCeilingResult>();
		if (param)
		{	
			_windowUI.Get()->OnCeilingCostumeResult(param->_success);
		}
	}
}

void FGsGameStateCostume::OnSummonComplete(const IGsMessageParam* InParam)
{
	if (_actorScene.IsValid())
	{
		_actorScene->SetHidden(false);
		_actorScene->ChangeSubScene(false);
	}

	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->ClearHideFlags(uiMgr->GetHideFlagsCurrent());
		uiMgr->SetHideFlags(EGsUIHideFlags::STATE_SCENE_WINDOW);

		_windowUI.Get()->SetVisibility(ESlateVisibility::Visible);
		_windowUI.Get()->OnSummonComplete();
	}

	SetNavigationBar(CurrencyType::NONE);
}

void FGsGameStateCostume::OnSelectedAppearance(const IGsMessageParam* InParam)
{
	const FGsCostumeMsgParamAppearanceOn* param = InParam->Cast<const FGsCostumeMsgParamAppearanceOn>();
	if (param)
	{
		auto costumeID = param->_costumeId;
		if (auto actorScene = _actorScene.Get())
		{
			actorScene->SetAppearance(costumeID, param->_hairChange, param->_bodyChange);
		}
	}
}

void FGsGameStateCostume::OnFindCostume(const IGsMessageParam* InParam)
{
	if (_windowUI.IsValid())
	{
		const FGsCostumeMsgParamUpdateList* param = InParam->Cast<const FGsCostumeMsgParamUpdateList>();
		if (param && param->_dataList.IsValidIndex(0))
		{
			_windowUI.Get()->OnFindCostume(param->_dataList[0]);
		}
	}
}

void FGsGameStateCostume::OnGameServerReconnected()
{
	if (_windowUI.IsValid())
	{
		_windowUI.Get()->ReleaseData();
		_windowUI.Get()->Close();
	}
}
