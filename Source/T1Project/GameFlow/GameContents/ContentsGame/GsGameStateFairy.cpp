#include "GsGameStateFairy.h"

#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"

#include "Message/MessageParam/GsFairyMessageParam.h"

#include "ActorEx/GsActorFairyObject.h"
#include "ActorEx/GsActorSummonSceneObject.h"

#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Fairy/GsFairyData.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "Summon/GsSummonDefine.h"

#include "EventProgress/GsEventProgressDefine.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Management/ScopeGlobal/GsOptionManager.h"

FGsGameStateFairy::FGsGameStateFairy() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsFairy)
{
}

FGsGameStateFairy::~FGsGameStateFairy()
{
	// 2023/6/13 PKT - 소멸 하는 과정에서 Exit를 호출 하면 내부적으로 Update Reddot에서 크래시 발생 여부 있음.
	//					소멸하는 것이 문제라면 다른 방법을 찾아 보자.
	/*if (_actorFairy.IsValid())
	{
		Exit();
	}*/
}

void FGsGameStateFairy::Enter()
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
	auto sceneObjectPath = GData()->GetGlobalData()->_fairySceneObjectPath.ToString();
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
		if (AGsActorFairyObject* actor = Cast<AGsActorFairyObject>(world->SpawnActor(loadObjectC, &tr, spawnParam)))
		{
			_actorFairy = actor;
			actor->InitAction();
		}
	}

	
	_summonSceneHandler = UGsSummonManager::GetSummonHandler();
				
	// UI 출력
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->SetHideFlags(EGsUIHideFlags::STATE_SCENE_WINDOW);

		TWeakObjectPtr<UGsUIWidgetBase> fairywidget = uiMgr->OpenAndGetWidget(TEXT("WindowFairy"));
		if (fairywidget.IsValid())
		{
			_windowUI = Cast<UGsUIWindowFairy>(fairywidget);

			if (_windowUI.IsValid())
			{
				_windowUI.Get()->InitializeData(&_reserveData);
				_reserveData._initData = 0;
				_reserveData._tabType = 0;
			}
		}

		SetNavigationBar(CurrencyType::NONE);
	}

	// 페어리 화면으로 들어왔다고 이벤트 전달(이전 환경효과가 페어리 화면에 영향주지 않게 하기 위함)
	FGsPrimitiveInt32 param(static_cast<int32>(EGsEnvEvent::FAIRY_ROOM));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_START_EVENT, &param);
}

void FGsGameStateFairy::Exit()
{
	// 닫길때는 원래대로
	UGsGameUserSettings* gameUserSettings = GGameUserSettings();
	if (gameUserSettings != nullptr)
	{
		gameUserSettings->UndoApplyCharacterTopGraphicSettings();
	}

	UnregisterMessages();

	//if (_windowUI.IsValid() && _windowUI.Get()->_isOpend)
	//{
	//	GUI()->Close(_windowUI.Get());
	//}

	_windowUI = nullptr;

	if (_summonSceneHandler)
	{
		UGsFairyManager* fairyManager = GSFairy();

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
	if (_actorFairy.IsValid())
	{
		_actorFairy.Get()->SetFairy(TEXT("")); 

		_actorFairy.Get()->EndAction();

		if(UWorld* world = _actorFairy.Get()->GetWorld())
			world->DestroyActor(_actorFairy.Get());
	
	}
	_actorFairy.Reset();

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

		messageMgr->GetHudRedDot().SendMessage(MessageContentHUDRedDot::FAIRY, false);
	}


	if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
	{
		eventProgressManager->OnReqFinishEvent(EGsEventProgressType::FAIRY_ROOM);
	}


	FGsContentsGameBase::Exit();
}

void FGsGameStateFairy::OnResetUI(UGsUIManager::FOnUIManagerResetUI& resetHandler)
{
	resetHandler.AddLambda([](int resetType) {
		if (resetType == 1) //connection 단절
		{
			GUI()->SetHideFlags(EGsUIHideFlags::STATE_SCENE_WINDOW);
		}
		});
}

void FGsGameStateFairy::RegisterMessages()
{
	auto& fairyMsgManager = GMessage()->GetFairy();

	_msgFairyHandleList.Emplace(fairyMsgManager.AddRaw(MessageFairy::UPDATED,
		this, &FGsGameStateFairy::OnUpdateFairy));

	_msgFairyHandleList.Emplace(fairyMsgManager.AddRaw(MessageFairy::COLLECTION_UPDATE,
		this, &FGsGameStateFairy::OnUpdateFairyCollection));

	_msgFairyHandleList.Emplace(fairyMsgManager.AddRaw(MessageFairy::CHANGE_SELECTION,
		this, &FGsGameStateFairy::OnSelectedFairy));

	_msgFairyHandleList.Emplace(fairyMsgManager.AddRaw(MessageFairy::CHANGE_EQUIP,
		this, &FGsGameStateFairy::OnSummonedFairy));

	_msgFairyHandleList.Emplace(fairyMsgManager.AddRaw(MessageFairy::ENCHANT_RESULT,
		this, &FGsGameStateFairy::OnEnchantFairyResult));

	_msgFairyHandleList.Emplace(fairyMsgManager.AddRaw(MessageFairy::CEILING_RESULT,
		this, &FGsGameStateFairy::OnCeilingFairyResult));

	_msgFairyHandleList.Emplace(fairyMsgManager.AddRaw(MessageFairy::OBTAIN_RESULT,
		this, &FGsGameStateFairy::OnObtainFairyResult));

	_msgFairyHandleList.Emplace(fairyMsgManager.AddRaw(MessageFairy::SUMMONWAIT_UPDATE,
		this, &FGsGameStateFairy::OnSummonWaiingFairyUpdate));

	_msgFairyHandleList.Emplace(fairyMsgManager.AddRaw(MessageFairy::SUMMON_COMPLETE,
		this, &FGsGameStateFairy::OnSummonComplete));

	_msgFairyHandleList.Emplace(fairyMsgManager.AddRaw(MessageFairy::INITTAB_CHANGE,
		this, &FGsGameStateFairy::OnChangeFairyWindowInit));

	_msgFairyHandleList.Emplace(fairyMsgManager.AddRaw(MessageFairy::FIND_FAIRY,
		this, &FGsGameStateFairy::OnFindFairy));

	_msgFairyHandleList.Emplace(fairyMsgManager.AddRaw(MessageFairy::ENCHANT_READY,
		this, &FGsGameStateFairy::SetFairyEnchantPos));

	_msgFairyHandleList.Emplace(fairyMsgManager.AddRaw(MessageFairy::ENCHANT_CANCEL,
		this, &FGsGameStateFairy::SetFairyNormalPos));

	_msgRedDotHandle = GMessage()->GetHudRedDot().AddRaw(MessageContentHUDRedDot::FAIRY,
		this, &FGsGameStateFairy::OnUpdateRedDot);

	_msgGameReconnected = GMessage()->GetSystem().AddRaw(MessageSystem::GAME_SERVER_RECONNECTION_SUCCESS,
		this, &FGsGameStateFairy::OnGameServerReconnected);

}

void FGsGameStateFairy::UnregisterMessages()
{
	auto& fairyMsgManager = GMessage()->GetFairy();
	for (auto handler : _msgFairyHandleList)
	{
		fairyMsgManager.Remove(handler);
	}

	GMessage()->GetHudRedDot().Remove(_msgRedDotHandle);

	GMessage()->GetSystem().Remove(_msgGameReconnected);
}

void FGsGameStateFairy::OnUpdateRedDot(bool bInActive)
{
	_windowUI.Get()->OnUpdateRedDot();
}

void FGsGameStateFairy::OnUpdateFairy(const IGsMessageParam* InParam)
{
	const FGsFairyMsgParamUpdateList* param = InParam->Cast<const FGsFairyMsgParamUpdateList>();
	if (param)
	{
		auto list = param->_dataList;
		_windowUI.Get()->OnUpdateFairy(list);
	}
}

void FGsGameStateFairy::OnUpdateFairyCollection(const IGsMessageParam* InParam)
{
	const FGsFairyMsgParamCollectionList* param = InParam->Cast<const FGsFairyMsgParamCollectionList>();
	if (param)
	{
		auto list = param->_dataList;
		_windowUI.Get()->OnUpdateFairyCollection(list);
	}
}

void FGsGameStateFairy::OnSelectedFairy(const IGsMessageParam* InParam)
{
	const FGsFairyMsgParamInt* param = InParam->Cast<const FGsFairyMsgParamInt>();
	if (param)
	{
		auto fairyID = param->_data;
		if (auto fairyData = GSFairy()->GetFairyData(fairyID).Pin())
		{
			auto fairyPath = fairyData->GetFairySpawnObjectPath();

			if (auto fairyObj = _actorFairy.Get())
			{
				FString path = fairyPath.ToString();
				path += TEXT("_C");

				fairyObj->SetFairy(path, fairyData->GetFairyBaseEffect());
			}
		}
	}
}

void FGsGameStateFairy::OnSummonedFairy(const IGsMessageParam* InParam)
{
	if (_windowUI.IsValid())
	{
		_windowUI.Get()->OnChangedSummonFairy();
	}
}

void FGsGameStateFairy::OnEnchantFairyResult(const IGsMessageParam* InParam)
{
	if (_windowUI.IsValid())
	{
		const FGsFairyMsgParamEnchantResult* param = InParam->Cast<const FGsFairyMsgParamEnchantResult>();
		if (param)
		{	
			_windowUI.Get()->OnEnchantFairyResult(param->_fairyId, param->_result);
		}
	}
}

void FGsGameStateFairy::OnCeilingFairyResult(const IGsMessageParam* InParam)
{
	if (_windowUI.IsValid())
	{
		const FGsFairyMsgParamEnchantResult* param = InParam->Cast<const FGsFairyMsgParamEnchantResult>();
		if (param)
		{
			_windowUI.Get()->OnCeilingFairyResult(param->_result);
		}
	}
}

void FGsGameStateFairy::OnObtainFairyResult(const IGsMessageParam* InParam)
{
	if (_windowUI.IsValid())
	{
		const FGsFairyMsgParamObtainList* param = InParam->Cast<const FGsFairyMsgParamObtainList>();
		if (param)
		{
			auto list = param->_dataList;
			bool isShowSummon = GData()->GetGlobalData()->_summonIsShow;
			if (isShowSummon && _actorFairy.IsValid())
			{
				_actorFairy->ChangeSubScene(true);
				_actorFairy->SetHidden(true);
			}

			_windowUI.Get()->OnObtainFairyResult(param->_obtainReason, list, isShowSummon);
		}
	}
}

void FGsGameStateFairy::OnSummonWaiingFairyUpdate(const IGsMessageParam* InParam)
{
	if (_windowUI.IsValid())
	{
		const FGsFairyMsgParamFairySummonWait* param = InParam->Cast<const FGsFairyMsgParamFairySummonWait>();
		if (param)
		{
			if (param->_updateType == FGsFairyMsgParamFairySummonWait::Confirm)
			{
				_windowUI.Get()->OnConfirmSummonWaitingFairy(param->_summonWaitDBId);
			}
			else 
			{
				_windowUI.Get()->OnUpdateSummonWaitingFairy(param->_summonWaitDBId);
			}
		}
	}
}

void FGsGameStateFairy::OnSummonComplete(const IGsMessageParam* InParam)
{
	if (_actorFairy.IsValid())
	{
		_actorFairy->SetHidden(false);
		_actorFairy->ChangeSubScene(false);
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

void FGsGameStateFairy::OnChangeFairyWindowInit(const IGsMessageParam* InParam)
{
	const FGsFairyMsgParamChangeTab* param = InParam->Cast<const FGsFairyMsgParamChangeTab>();
	if (param)
	{
		_reserveData._tabType = param->_tab;
		_reserveData._initData = param->_optData;
	}

	if (_windowUI.IsValid())
	{	
		_windowUI.Get()->ChangeInitTab(&_reserveData);
		_reserveData._initData = 0;
		_reserveData._tabType = 0;
	}
}


void FGsGameStateFairy::OnFindFairy(const IGsMessageParam* InParam)
{
	if (_windowUI.IsValid())
	{
		const FGsFairyMsgParamUpdateList* param = InParam->Cast<const FGsFairyMsgParamUpdateList>();
		if (param && param->_dataList.IsValidIndex(0))
		{
			_windowUI.Get()->OnFindFairy(param->_dataList[0]);
		}
	}
}

void FGsGameStateFairy::SetFairyEnchantPos(const IGsMessageParam* InParam)
{
	if (auto fairyObj = _actorFairy.Get())
	{
		FVector enchantPos = GData()->GetGlobalData()->_fairyEnchantOffset; //페어리 회전시 터치 이동거리 조절값(값이 작아질 수록 움직임이 작아짐)

		fairyObj->SetZoomOffset(enchantPos);
	}
}

void FGsGameStateFairy::SetFairyNormalPos(const IGsMessageParam* InParam)
{
	if (auto fairyObj = _actorFairy.Get())
	{
		fairyObj->ResetZoomOffset();
	}
}

void FGsGameStateFairy::OnGameServerReconnected()
{
	if (_windowUI.IsValid())
	{
		_windowUI.Get()->ReleaseData();
		_windowUI.Get()->Close();
	}
}
