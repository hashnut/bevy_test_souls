// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStateUserProfile.h"
#include "ActorEx/GsActorUserProfileSceneObject.h"
#include "Data/GsDataContainManager.h"
#include "GameFramework/PlayerController.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsUserProfileManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "UI/UIContent/Window/GsUIWindowUserProfile.h"


FGsGameStateUserProfile::FGsGameStateUserProfile() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsUserProfile)
{
}

FGsGameStateUserProfile::~FGsGameStateUserProfile()
{
}

void FGsGameStateUserProfile::Enter()
{
	FGsContentsGameBase::Enter();

	RegisterMessages();

	if (GSUserProfile()->IsExistProfile())
	{
		HideJoystick();
		SpawnScene();
		OpenWindowUI();
		SetNavigationBar(CurrencyType::NONE);
	}
	else
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);
	}
}

void FGsGameStateUserProfile::Exit()
{
	RemoveMessages();
	ShowJoystick();
	DespawnScene();
	CloseWindowUI();
	 
	if (nullptr != GSUserProfile())
	{
		GSUserProfile()->Clear();
	}

	if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
	{
		eventProgressManager->OnReqFinishEvent(EGsEventProgressType::USER_PROFILE);
	}

	FGsContentsGameBase::Exit();
}

void FGsGameStateUserProfile::RegisterMessages()
{
	FGsMessageHolder* msg = GMessage();

	MUserProfile& msgUserProfile = msg->GetUserProfile();
	_userProfileDelegates.Emplace(msgUserProfile.AddRaw(MessageContentUserProfile::INVALIDATE_ALL,
		this, &FGsGameStateUserProfile::OnInvalidateAll));
	_userProfileDelegates.Emplace(msgUserProfile.AddRaw(MessageContentUserProfile::INVALIDATE_COMMENT,
		this, &FGsGameStateUserProfile::OnInvalidateComment));
	_userProfileDelegates.Emplace(msgUserProfile.AddRaw(MessageContentUserProfile::INVALIDATE_INTRODUCTION,
		this, &FGsGameStateUserProfile::OnInvalidateIntroduction));
	_userProfileDelegates.Emplace(msgUserProfile.AddRaw(MessageContentUserProfile::INVALIDATE_KEYWORD,
		this, &FGsGameStateUserProfile::OnInvalidateKeyword));
	_userProfileDelegates.Emplace(msgUserProfile.AddRaw(MessageContentUserProfile::INVALIDATE_NICKNAME_HISTORY,
		this, &FGsGameStateUserProfile::OnInvalidateNickNameHistory));
	_userProfileDelegates.Emplace(msgUserProfile.AddRaw(MessageContentUserProfile::OPEN_KEYWORD_VOTE,
		this, &FGsGameStateUserProfile::OnOpenKeywordVote));
	_userProfileDelegates.Emplace(msgUserProfile.AddRaw(MessageContentUserProfile::ERROR_INVALID_USER,
		this, &FGsGameStateUserProfile::OnErrorInvalidUser));

	MItem& msgItem = msg->GetItem();
	_itemDelegates.Emplace(msgItem.AddRaw(MessageItem::UPDATEITEM,
		this, &FGsGameStateUserProfile::OnInvalidateEquipItem));

	MUserBasicInfo& msgUserBasicInfo = msg->GetUserBasicInfo();
	_userInfoDelegates.Emplace(msgUserBasicInfo.AddRaw(MessageUserInfo::LOOK_INFO,
		this, &FGsGameStateUserProfile::OnLookInfoChanged));

	MUI& msgUI = msg->GetUI();
	_uiDelegates.Emplace(msgUI.AddRaw(MessageUI::OPTION_SAVE_TO_SERVER_COMPLETED,
		this, &FGsGameStateUserProfile::OnOptionSaveCompleted));

	MSystem& msgSystem = msg->GetSystem();
	_systemDelegates.Emplace(msgSystem.AddRaw(MessageSystem::GAME_SERVER_RECONNECTION_SUCCESS,
		this, &FGsGameStateUserProfile::OnGameServerReconnectionSuccess));

	MGameObject& msgGameObject = msg->GetGameObject();
	_gameObjectDelegates.Emplace(msgGameObject.AddRaw(MessageGameObject::UPDATE_USER_PRESET_EQUIP,
		this, &FGsGameStateUserProfile::OnUserPresetEquipUpdate));
}

void FGsGameStateUserProfile::RemoveMessages()
{
	FGsMessageHolder* msg = GMessage();

	MUserProfile& msgUserProfile = msg->GetUserProfile();
	for (TPair<MessageContentUserProfile, FDelegateHandle>& userProfileDelegate : _userProfileDelegates)
	{
		msgUserProfile.Remove(userProfileDelegate);
	}
	_userProfileDelegates.Empty();

	MItem& msgItem = msg->GetItem();
	for (TPair<MessageItem, FDelegateHandle>& itemDelegate : _itemDelegates)
	{
		msgItem.Remove(itemDelegate);
	}
	_itemDelegates.Empty();

	MUserBasicInfo& msgUserBasicInfo = msg->GetUserBasicInfo();
	for (TPair<MessageUserInfo, FDelegateHandle>& userInfoDelegate : _userInfoDelegates)
	{
		msgUserBasicInfo.Remove(userInfoDelegate);
	}
	_userInfoDelegates.Empty();

	MUI& msgUI = msg->GetUI();
	for (TPair<MessageUI, FDelegateHandle>& uiDelegate : _uiDelegates)
	{
		msgUI.Remove(uiDelegate);
	}
	_uiDelegates.Empty();

	MSystem& msgSystem = msg->GetSystem();
	for (MsgSystemHandle& systemDelegate : _systemDelegates)
	{
		msgSystem.Remove(systemDelegate);
	}
	_systemDelegates.Empty();

	MGameObject& msgGameObject = msg->GetGameObject();
	for (MsgGameObjHandle& gameObjectDelegate : _gameObjectDelegates)
	{
		msgGameObject.Remove(gameObjectDelegate);
	}
	_gameObjectDelegates.Empty();
}

void FGsGameStateUserProfile::OnInvalidateAll()
{
	if (_windowUI.IsValid())
	{
		_windowUI->InvalidateAll();
	}

	if (_actorScene.IsValid())
	{
		_actorScene->SetCharacter();
		_actorScene->SetFairy();
	}
}

void FGsGameStateUserProfile::OnInvalidateComment()
{
	if (_windowUI.IsValid())
	{
		_windowUI->InvalidateComment();
	}
}

void FGsGameStateUserProfile::OnInvalidateIntroduction()
{
	if (_windowUI.IsValid())
	{
		_windowUI->InvalidateIntroduction();
	}
}

void FGsGameStateUserProfile::OnInvalidateKeyword()
{
	if (_windowUI.IsValid())
	{
		_windowUI->InvalidateKeyword();
	}
}

void FGsGameStateUserProfile::OnInvalidateNickNameHistory()
{
	if (_windowUI.IsValid())
	{
		_windowUI->InvalidateNickNameHistory();
	}
}

void FGsGameStateUserProfile::OnInvalidateEquipItem(FGsItemMessageParamBase& InParam)
{
	if (_windowUI.IsValid())
	{
		_windowUI->InvalidateEquipItem();
		_windowUI->CloseStatTooltip();
	}
}

void FGsGameStateUserProfile::OnLookInfoChanged(uint64 InGameId)
{
	if (GSUserProfile()->IsMyProfile())
	{
		if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
		{
			if (player->GetGameId() == InGameId)
			{
				if (_actorScene.IsValid())
				{
					_actorScene->SetCharacter();
				}
			}
		}
	}
}

void FGsGameStateUserProfile::OnOpenKeywordVote()
{
	if (_windowUI.IsValid())
	{
		_windowUI->OpenKeywordVote();
	}
}

void FGsGameStateUserProfile::OnOptionSaveCompleted(const IGsMessageParam*)
{
	if (_windowUI.IsValid())
	{
		_windowUI->OnOptionSaveCompleted();
	}
}

void FGsGameStateUserProfile::OnGameServerReconnectionSuccess()
{
	if (_windowUI.IsValid())
	{
		_windowUI.Get()->Close();
	}
}

void FGsGameStateUserProfile::OnUserPresetEquipUpdate(const struct IGsMessageParam* inParam)
{
	if (_windowUI.IsValid())
	{
		_windowUI->InvalidateEquipItem();
		_windowUI->CloseStatTooltip();
	}
}

void FGsGameStateUserProfile::SpawnScene()
{
	UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (player)
	{
		GLevel()->HoldStreamingLevelByLocation(player->GetLocation());
	}

	UWorld* world = GEngine->GameViewport->GetWorld();
	FString sceneObjectPath = GData()->GetGlobalData()->_userProfileScenePath.ToString();
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
		if (AGsActorUserProfileSceneObject* actor = Cast<AGsActorUserProfileSceneObject>(world->SpawnActor(loadObjectC, &tr, spawnParam)))
		{
			_actorScene = actor;
			actor->InitAction();
		}
	}

	FGsPrimitiveInt32 param(static_cast<int32>(EGsEnvEvent::FAIRY_ROOM));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_START_EVENT, &param);
}

void FGsGameStateUserProfile::DespawnScene()
{
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

	FGsPrimitiveInt32 param(static_cast<int32>(EGsEnvEvent::FAIRY_ROOM));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_END_EVENT, &param);
}

void FGsGameStateUserProfile::OpenWindowUI()
{
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->SetHideFlags(EGsUIHideFlags::STATE_SCENE_WINDOW);

		TWeakObjectPtr<UGsUIWidgetBase> windowProfile = uiManager->OpenAndGetWidget(TEXT("WindowUserProfile"));
		if (windowProfile.IsValid())
		{
			_windowUI = Cast<UGsUIWindowUserProfile>(windowProfile);
		}
	}
}

void FGsGameStateUserProfile::CloseWindowUI()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->ClearHideFlags(EGsUIHideFlags::STATE_SCENE_WINDOW);
	}
}

void FGsGameStateUserProfile::OnErrorInvalidUser()
{
	GSUserProfile()->ErrorInvalidUser();

	if (_windowUI.IsValid())
	{
		_windowUI->SetTargetUserExist(false);
	}
}

void FGsGameStateUserProfile::ShowJoystick()
{
	if (GSGameObject())
	{
		UWorld* world = GSGameObject()->GetWorld();
		if (world == nullptr)
			return;

		if (APlayerController* playerController = world->GetFirstPlayerController())
		{
			playerController->CreateTouchInterface();
		}
	}
}

void FGsGameStateUserProfile::HideJoystick()
{
	if (GSGameObject())
	{
		UWorld* world = GSGameObject()->GetWorld();
		if (world == nullptr)
			return;

		if (APlayerController* playerController = world->GetFirstPlayerController())
		{
			playerController->ActivateTouchInterface(nullptr);
		}
	}
}
