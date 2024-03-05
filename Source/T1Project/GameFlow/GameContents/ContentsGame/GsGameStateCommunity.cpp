// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStateCommunity.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Data/GsDataContainManager.h"
#include "DataSchema/GsSchemaEnums.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsCommunityEventManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/MessageParam/GsUIMessageParam.h"

#include "UI/UIContent/Window/GsUIWindowCommunity.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "ActorEx/GsActorCommunityNPCScene.h"

#include "EventProgress/GsEventProgressDefine.h"


FGsGameStateCommunity::FGsGameStateCommunity() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsCommunity)
{
	_dialogHandler = MakeUnique<FGsComunityGirlDialogHandler>();
}

FGsGameStateCommunity::~FGsGameStateCommunity()
{
}

void FGsGameStateCommunity::Enter()
{
	FGsContentsGameBase::Enter();

	UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (player)
	{
		GLevel()->HoldStreamingLevelByLocation(player->GetLocation());
	}

	UWorld* world = GEngine->GameViewport->GetWorld();
	auto sceneObjectPath = GData()->GetGlobalData()->_communityGirlSceneObjPath.ToString();
	sceneObjectPath += TEXT("_C");

	if (UClass* loadObjectC = LoadObject<UClass>(nullptr, *sceneObjectPath))
	{
		FTransform tr = FTransform::Identity;
		auto pos = tr.GetLocation();
		pos.Z = 1000000.f;
		tr.SetLocation(pos);

		FActorSpawnParameters spawnParam = FActorSpawnParameters();
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AGsActorCommunityNPCScene* actor = Cast<AGsActorCommunityNPCScene>(world->SpawnActor(loadObjectC, &tr, spawnParam)))
		{
			_actorScene = actor;
			actor->InitAction();
		}
	}

	FGsPrimitiveInt32 param(static_cast<int32>(EGsEnvEvent::FAIRY_ROOM));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_START_EVENT, &param);

	// 네비게이션 바 재화 세팅 (드롭다운 메뉴 감춤, 선택 슬롯 감춤)
	SetNavigationBar(CurrencyType::NONE);

	_dialogHandler->InitDialogTable();
	// 메세지 등록
	RegisterMessages();

	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->SetHideFlags(EGsUIHideFlags::STATE_SCENE_WINDOW);

		auto widget = uiManager->OpenAndGetWidget(TEXT("WindowCommunity"));
		if (widget.IsValid())
		{
			_windowCommunityUI = Cast<UGsUIWindowCommunity>(widget);

			if (const auto dlg = _dialogHandler->GetDialog(EGsStateBase::Spawn))
			{
				_windowCommunityUI->ChangeDialog(dlg);
			}
		}
	}

	if (FGsCommunityEventManager* communityEventManager = GSCommunityEvent())
	{		
		communityEventManager->OpenCommunityEventUI();
		// 서버로 목록 요청
		communityEventManager->SendReqCommunityList();
	}
}

void FGsGameStateCommunity::Finialize(bool In_isSendEventProgress)
{
	// 주의: Exit 시 Window 닫기를 하지 않음(UI Stack과 State연계로직 때문. OnBack, CloseAll등의 로직에 의해 닫음)
	_windowCommunityUI = nullptr;
	// 메세지 해지
	UnregisterMessages();

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
	}

	// HUD 관련 레드닷 갱신 요청
	if (FGsCommunityEventManager* communityMgr = GSCommunityEvent())
	{
		communityMgr->UpdateRedDot(nullptr);
	}

	if(In_isSendEventProgress == true)
	{ 
		if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
		{
			eventProgressManager->OnReqFinishEvent(EGsEventProgressType::COMMUNITY_ROOM);
		}
	}
}

void FGsGameStateCommunity::Exit()
{
	Finialize(true);

	FGsContentsGameBase::Exit();
}

void FGsGameStateCommunity::Update(float In_deltaTime)
{
	FGsContentsGameBase::Update(In_deltaTime);
}

void FGsGameStateCommunity::Init()
{
	FGsContentsGameBase::Init();
}

void FGsGameStateCommunity::Close()
{
	FGsContentsGameBase::Close();
}

void FGsGameStateCommunity::RegisterMessages()
{
	FGsMessageHolder* msg = GMessage();
	auto& communityMsg = msg->GetCommunity();

	_communityGirlDelegates.Emplace(communityMsg.AddRaw(MessageContentCommunity::ACTIVE_WINDOW_INVALIDTEALL,
		this, &FGsGameStateCommunity::WindowUIInvalidateAll));

	_communityGirlDelegates.Add(communityMsg.AddRaw(MessageContentCommunity::SELECT_TAB,
		this, &FGsGameStateCommunity::ActionChangeGirlTabAction));

	_communityGirlDelegates.Add(communityMsg.AddRaw(MessageContentCommunity::CHANGE_ACTION,
		this, &FGsGameStateCommunity::AnimStateStart));

	_communityGirlDelegates.Add(communityMsg.AddRaw(MessageContentCommunity::FINISH_EVENT_PROGRESS,
		this, &FGsGameStateCommunity::FinishEventProgress));

	_communityGirlDelegates.Add(communityMsg.AddRaw(MessageContentCommunity::UPDATE_EVENT_EXPIRE,
		this, &FGsGameStateCommunity::OnEventExpire));
}

void FGsGameStateCommunity::UnregisterMessages()
{
	FGsMessageHolder* msg = GMessage();

	auto& communityMsg = msg->GetCommunity();
	for (auto handler : _communityGirlDelegates)
	{
		communityMsg.Remove(handler);
	}
	_communityGirlDelegates.Empty();
}

void FGsGameStateCommunity::WindowUIInvalidateAll(const IGsMessageParam* InParam)
{
	if (_windowCommunityUI.IsValid())
	{
		_windowCommunityUI->UpdateCommunityEvent();
	}
}

void FGsGameStateCommunity::ActionChangeGirlTabAction(const IGsMessageParam* InParam)
{
	if (_actorScene.IsValid())
	{
		_actorScene.Get()->ActionSelectTab();
	}
}

void FGsGameStateCommunity::AnimStateStart(const IGsMessageParam* InParam)
{
	if (InParam)
	{
		if (const FGsUIMsgParamInt* param = InParam->Cast<const FGsUIMsgParamInt>())
		{
			if (_windowCommunityUI.IsValid())
			{
				if (const auto dlg = _dialogHandler->GetDialog(static_cast<EGsStateBase>(param->_data)))
				{
					_windowCommunityUI->ChangeDialog(dlg);
				}				
			}
		}
	}
}

// 이벤트 프로그래스 종료
// https://jira.com2us.com/jira/browse/C2URWQ-5168
void FGsGameStateCommunity::FinishEventProgress(const IGsMessageParam* InParam)
{
	GUI()->CloseByKeyName(TEXT("WindowCommunity"));
}

void FGsGameStateCommunity::OnEventExpire(const IGsMessageParam* InParam)
{
	if (_windowCommunityUI.IsValid())
	{
		if (true == GUI()->IsActiveWidget(TEXT("WindowCommunity")))
		{
			_windowCommunityUI->UpdateExpire();
		}
	}
}