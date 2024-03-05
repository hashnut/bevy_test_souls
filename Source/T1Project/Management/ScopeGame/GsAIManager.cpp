#include "GsAIManager.h"

#include "AI/GsAIStateManager.h"
#include "AI/Data/GsAIDefine.h"
#include "AI/Data/GsAITaskInfo.h"
#include "AI/Data/GsMannerModeInfo.h"
#include "AI/ContentsCondition/GsAIContentsConditionDefault.h"
#include "AI/ContentsCondition/GsAIContentsConditionAttOrder.h"
#include "AI/ContentsCondition/GsAIContentsConditionAutoReturn.h"
#include "AI/ContentsCondition/GsAIContentsConditionAutoOff.h"
#include "AI/Quest/GsAIQuestHandler.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGameClientVersion.h"

#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Autoplay/GsSchemaAITaskData.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "AI/Data/EGsAIContentsType.h"

#include "ControllerEx/GsPlayerController.h"

#include "Input/GsInputBindingLocalPlayer.h"

#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateAuto.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"
#include "GameObject/Movement/GsAutoMoveInfo.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayerBase.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Define/GsGameObjectDefine.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "Message/GsMessageContentHud.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsInteractionMessageParam.h"
#include "Message/MessageParam/GsAIMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/MessageParam/GsStageMessageParam.h"

#include "Input/GsInputEventMsgBase.h"

#include "Option/GsGameUserSettings.h"

#include "T1Project.h"
#include "DrawDebugHelpers.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "UTIL/GsMove.h"
#include "UTIL/GsGameObjectUtil.h"

#include "UI/UIContent/Helper/GsUIHelper.h"


FGsAIManager::~FGsAIManager()
{
	
}

// 초기화(처음 한번만)
void FGsAIManager::Initialize()
{
	// 상태 관리자 생성
	_stateManager = new FGsAIStateManager();
	_stateManager->Initialize(this);

	_contentsConditionDefault = new FGsAIContentsConditionDefault();
	_contentsConditionDefault->Initialize(this);

	_contentsConditionAttOrder = new FGsAIContentsConditionAttOrder();
	_contentsConditionAttOrder->Initialize(this);

	_contentsConditionAutoReturn = new FGsAIContentsConditionAutoReturn();
	_contentsConditionAutoReturn->Initialize(this);

	_contentsConditionAutoOff = new FGsAIContentsConditionAutoOff();
	_contentsConditionAutoOff->Initialize(this);

	// 데이터 로드
	LoadAIData();
	// 조건 함수 연결
	RegisterConditionFunc();
	// 메시지 연결
	BindMessage();

	_isAIOn = false;

	// 퀘스트 관련 처리 생성
	_questHandler = new FGsAIQuestHandler();
	_questHandler->Initialize();

	// 백업 데이터 불러오기
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		int findIntMannerMode = 
			gameUserSettings->GetCombatSetting(EGsOptionCombat::MANNER_MODE);

		// 이넘 영역 밖이면 디폴트로 off		
		_mannerMode =
			(findIntMannerMode < (int)EGsAIBattleOptionMannerMode::ON ||
				findIntMannerMode >(int)EGsAIBattleOptionMannerMode::OFF) ?
			EGsAIBattleOptionMannerMode::OFF : (EGsAIBattleOptionMannerMode)findIntMannerMode;

		int findIntNonAutoMoveKeepAuto =
			gameUserSettings->GetCombatSetting(EGsOptionCombat::NONAUTO_MOVE_KEEP_AUTO);

		_isNonAutoMoveKeepAuto = (bool)findIntNonAutoMoveKeepAuto;
	}
}


// 초기화
void FGsAIManager::SetCharacter(UGsGameObjectLocalPlayer* In_player)
{
	// 저장
	_localPlayer = In_player;

	if (nullptr != _stateManager)
	{
		_stateManager->SetCharacter(_localPlayer);
	}

	if (_contentsConditionDefault != nullptr)
	{
		_contentsConditionDefault->SetCharacter(_localPlayer);
	}

	if (_contentsConditionAttOrder != nullptr)
	{
		_contentsConditionAttOrder->SetCharacter(_localPlayer);
	}
	if (_contentsConditionAutoReturn != nullptr)
	{
		_contentsConditionAutoReturn->SetCharacter(_localPlayer);
	}
	if (_contentsConditionAutoOff != nullptr)
	{
		_contentsConditionAutoOff->SetCharacter(_localPlayer);
	}
	// ai on 인지로 체크
	EGsAIContentsType nowType = (_isAIOn == true) ? EGsAIContentsType::Field : EGsAIContentsType::Auto_off;
	ChangeContentsType(nowType);

	_isInitCharacter = true;

	if (_questHandler != nullptr)
	{
		_questHandler->SetCharacter(_localPlayer);
	}
}
// 캐릭터 삭제 될때
void FGsAIManager::RemoveCharacter()
{
	_isInitCharacter = false;
	if (_stateManager != nullptr)
	{
		_stateManager->RemoveCharacter();
	}

	if (_contentsConditionDefault != nullptr)
	{
		_contentsConditionDefault->RemoveCharacter();
	}
	if (_contentsConditionAttOrder != nullptr)
	{
		_contentsConditionAttOrder->RemoveCharacter();
	}
	if(_contentsConditionAutoReturn != nullptr)
	{
		_contentsConditionAutoReturn->RemoveCharacter();
	}
	if (_contentsConditionAutoOff != nullptr)
	{
		_contentsConditionAutoOff->RemoveCharacter();
	}
	if (_questHandler != nullptr)
	{
		_questHandler->RemoveCharacter();
	}

	_localPlayer = nullptr;
}
// 해제
void FGsAIManager::Finalize()
{
	// 데이터가 뭔가 있다면
	if (_mapTaskResolver.Num() > 0)
	{
		for (auto iter : _mapTaskResolver)
		{
			if (nullptr != iter.Value)
			{
				iter.Value->Finalize();
				delete iter.Value;
			}
		}
		_mapTaskResolver.Empty();
	}

	if (nullptr != _stateManager)
	{
		_stateManager->Finalize();
		delete _stateManager;
		_stateManager = nullptr;
	}

	if (nullptr != _contentsConditionDefault)
	{
		_contentsConditionDefault->Finalize();
		delete _contentsConditionDefault;
		_contentsConditionDefault = nullptr;
	}
	if (nullptr != _contentsConditionAttOrder)
	{
		_contentsConditionAttOrder->Finalize();
		delete _contentsConditionAttOrder;
		_contentsConditionAttOrder = nullptr;
	}
	if (nullptr != _contentsConditionAutoReturn)
	{
		_contentsConditionAutoReturn->Finalize();
		delete _contentsConditionAutoReturn;
		_contentsConditionAutoReturn = nullptr;
	}

	if (nullptr != _contentsConditionAutoOff)
	{
		_contentsConditionAutoOff->Finalize();
		delete _contentsConditionAutoOff;
		_contentsConditionAutoOff = nullptr;
	}

	// 바인딩 된 메시지 해제
	FGsMessageHolder* msg = GMessage();
	
	if (_actionDelegates.Num() != 0)
	{
		for (auto iter : _actionDelegates)
		{
			msg->GetInput().Remove(iter);
		}
		_actionDelegates.Empty();
	}
	if (_aiDelegates.Num() != 0)
	{
		for (auto iter : _aiDelegates)
		{
			msg->GetGameObject().Remove(iter);
		}
		_aiDelegates.Empty();
	}

	if (_systemDelegates.Num() != 0)
	{
		for (auto iter : _systemDelegates)
		{
			msg->GetSystem().Remove(iter);
		}
		_systemDelegates.Empty();
	}

	if (_systemParamDelegates.Num() != 0)
	{
		for (auto iter : _systemParamDelegates)
		{
			msg->GetSystemParam().Remove(iter);
		}
		_systemParamDelegates.Empty();
	}
	
	if (nullptr != _questHandler)
	{
		_questHandler->Finalize();
		delete _questHandler;
		_questHandler = nullptr;
	}

	MStageParam& msgStage = msg->GetStageParam();
	for (MsgStageHandle& msgStageHandle : _msgStageHandleList)
	{
		msgStage.Remove(msgStageHandle);
	}
	_msgStageHandleList.Empty();


	// 매너 체크 타겟 클리어
	ClearMannerCheckTarget();
}
// 컨텐츠 타입 변경
void FGsAIManager::ChangeContentsType(EGsAIContentsType In_type)
{
	_currentContentsType = In_type;
	const FGsAITaskResolver* findData = _mapTaskResolver.FindRef(In_type);

	if (nullptr != _stateManager)
	{
		_stateManager->ChangeContentsType(findData);
	}
}

// 갱신
void FGsAIManager::Update(float In_delta)
{
	// 캐릭터 세팅 전이다
	// 워프 중이다
	if (false == _isInitCharacter ||
		true == _isStartLoadToPo)
	{
		return;
	}

	if (_questHandler != nullptr)
	{
		_questHandler->Update(In_delta);
	}

	// 다음맵 이동 관련 처리
	if (_isDirtyNextMapMove == true)
	{
		TryDelayedNextMapMove();
	}


	// ai play를 할수 있는가
	if (false == CanAIPlay())
	{
		return;
	}

	// 입력이 있는가
	if (true == IsThereInput())
	{
		return;
	}

	// 상태 갱신
	if (nullptr != _stateManager)
	{
		_stateManager->Update(In_delta);
	}

	if (_contentsConditionDefault != nullptr)
	{
		_contentsConditionDefault->Update();
	}
}

// ai 데이터 로드
void FGsAIManager::LoadAIData()
{
	const UGsTable* tbl = FGsSchemaAITaskData::GetStaticTable();

	if (nullptr == tbl)
	{
		return;
	}

	TArray<const FGsSchemaAITaskData*> rows;
	tbl->GetAllRows<FGsSchemaAITaskData>(rows);

	if (rows.Num() == 0)
	{
		return;
	}
	
	for (auto taskIter : rows)
	{
		FGsAITask* task = new FGsAITask();

		for (auto conditionIter : taskIter->ReserveConditions)
		{
			FGsAICondition* condition = new FGsAICondition();
			condition->_conditionType = (EGsAIConditionType)conditionIter.ConditionType;
			condition->_boolVal = conditionIter.FunctionCompareData;

			task->_reserveConditions.Add(condition);
		}

		for (auto conditionIter : taskIter->ActionConditions)
		{
			FGsAICondition* condition = new FGsAICondition();
			condition->_conditionType = (EGsAIConditionType)conditionIter.ConditionType;
			condition->_boolVal = conditionIter.FunctionCompareData;

			task->_actionConditions.Add(condition);
		}

		for (auto conditionIter : taskIter->ClearConditions)
		{
			FGsAICondition* condition = new FGsAICondition();
			condition->_conditionType = (EGsAIConditionType)conditionIter.ConditionType;
			condition->_boolVal = conditionIter.FunctionCompareData;

			task->_clearConditions.Add(condition);
		}


		FGsAIAction* action = new FGsAIAction();
		action->_type = (EGsAIActionType)taskIter->Action.ActionType;

		task->_action = action;
		task->id = UGsTableUtil::ConvertRow<int32>(taskIter);
		task->_annotation = taskIter->Annotation;

		task->_reserveCount = taskIter->ReserveCount;


		task->_isHighPriority = taskIter->IsHighPriority;

		FGsAITaskResolver** findData =
			_mapTaskResolver.Find(taskIter->ContentsType);

		// 기존에 없으면
		if (findData == nullptr)
		{
			FGsAITaskResolver* newData = new FGsAITaskResolver();
			newData->_tasks.Add(task);
			_mapTaskResolver.Add(taskIter->ContentsType, newData);
		}
		// 기존에 있다면
		else
		{
			(*findData)->_tasks.Add(task);
		}
	}
}

// 컨디션 함수 연결
void FGsAIManager::RegisterConditionFunc()
{	
	if ( _mapTaskResolver.Num() == 0)
	{
		return;
	}

	for (auto iter : _mapTaskResolver)
	{

		for (int i = 0; i < iter.Value->_tasks.Num(); ++i)
		{
			FGsAITask* iterCon = iter.Value->_tasks[i];
			iterCon->id = i;

			for (auto iterCheck : iterCon->_reserveConditions)
			{
				switch (iterCheck->_conditionType)
				{
				case EGsAIConditionType::TARGET_EXIST:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsTargetExist(); };
					break;
				case EGsAIConditionType::HAS_AUTO_RETURN_MODE:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionAutoReturn->GetIsAutoReturnMode(); };
					break;
				case EGsAIConditionType::IS_TOO_FAR_FROM_AUTO_START_POS:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionAutoReturn->IsTooFarFromAutoStartPos(); };
					break;
				case EGsAIConditionType::IS_IN_SEARCH_RANGE_ANYONE_ENEMY:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsInSearchRangeAnyoneEnemy(); };
					break;
				case EGsAIConditionType::IS_IN_SEARCH_RANGE_ITEM:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsInSearchRangeItem(); };
					break;
				case EGsAIConditionType::IS_IN_SEARCH_RANGE_PROP:
					iterCheck->_func =[this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsSearchRangePropObject(); };
					break;
				case EGsAIConditionType::IS_QUEST_LAST_DROP_ITEM:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->GetIsMyMissingItemExist(); };
					break;
				case EGsAIConditionType::IS_QUEST_AUTO_PLAY:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsQuestAutoPlay(); };
					break;
				case EGsAIConditionType::IS_COUNTER_ATTACK_ON:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsCounterAttackOptionOn(); };
					break;
				case EGsAIConditionType::IS_COUNTER_ATTACK_TARGET_EXIST:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsCounterAttackTargetExist(); };
					break;
				case EGsAIConditionType::IS_AUTO_LOOT_COOL_TIME:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsAutoLootCooltime(); };
					break;
				case EGsAIConditionType::IS_QUEST_AUTO_LOOT:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsQuestAutoLoot(); };
					break;
				case EGsAIConditionType::IS_RESERVE_LOOTING:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsReserveLooting(); };
					break;
				case EGsAIConditionType::IS_AUTO_RETURN_MOVE:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsAutoReturnMove(); };
					break;
				case EGsAIConditionType::IS_GAMEOBJ_FSM_AUTO_MOVE:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsGameObjFSMAutoMove(); };
					break;
				case EGsAIConditionType::IS_QUEST_CENTER_MOVE:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsQuestCenterMove(); };
					break;
				case EGsAIConditionType::IS_STAY_LONG_TIME:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsStayLongTime(); };
					break;
				case EGsAIConditionType::IS_FIRST_JOB_RETRY:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsFirstJobRetry(); };
					break;
				case EGsAIConditionType::IS_SERVER_MOVE_LOCK:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsServerMoveLock(); };
					break;
				case EGsAIConditionType::IS_AUTO_RETURN_POSSIBLE_MAP:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionAutoReturn->GetIsMapUseAutoReturn(); };
					break;
				case EGsAIConditionType::IS_RESERVED_ACTION_AUTO_MOVE_STOP_WITH_CRS:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsReservedActionAutoMoveStopWithClearReserveSkill(); };
					break;
				}
			}

			for (auto iterCheck : iterCon->_actionConditions)
			{
				switch (iterCheck->_conditionType)
				{
				case EGsAIConditionType::IS_GAMEOBJ_FSM_IDLE:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsGameObjectFSMIdle(); };
					break;
				case EGsAIConditionType::IS_GAMEOBJ_SKILL_USEABLE_STATE:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsGameObjectSkillUseableState(); };
					break;
				case EGsAIConditionType::IS_LOOT_SERVER_RESPONSE_WAIT:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsLootServerResponseWait(); };
					break;
				case EGsAIConditionType::IS_DIALOG_PLAY:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsDialogPlay(); };
					break;
				case EGsAIConditionType::IS_INPUT_BLOCK:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsInputBlock(); };
					break;
				case EGsAIConditionType::IS_QUEST_AUTO_LOOT:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsQuestAutoLoot(); };
					break;
				}
			}

			for (auto iterCheck : iterCon->_clearConditions)
			{
				switch (iterCheck->_conditionType)
				{
				case EGsAIConditionType::IS_LOOT_IMPOSSIBLE:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsLootImpossible(); };
					break;
				case EGsAIConditionType::IS_ATTACKABLE:
					iterCheck->_func = [this, iterCheck]()
					{ return iterCheck->_boolVal == _contentsConditionDefault->IsAttackable(); };
					break;
				}
			}

		}
	}
}


// 스킬 버튼 누름
void FGsAIManager::OnPressSkill(const FGsInputEventMsgBase& /*In_msg*/)
{
	// ai 진행 상태와 상관없이 갱신
	_isSkillBtnPressed = true;

	if (nullptr != _contentsConditionDefault)
	{
		_contentsConditionDefault->OnPressSkill();
	}
}
// 스킬 버튼 땜
void FGsAIManager::OnReleaseSkill(const FGsInputEventMsgBase& /*In_msg*/)
{
	// ai 진행 상태와 상관없이 갱신
	_isSkillBtnPressed = false;

	if (false == CanAIPlay())
	{
		return;
	}
	// 복귀 위치 저장
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_MODE_SAVE_AUTO_POSITION, nullptr);
	if (nullptr != _stateManager)
	{
		// 릴리즈 시점에 wait으로 변경...
		_stateManager->ClearState();
	}
}

// 조이스틱 땜
void FGsAIManager::OnReleaseJoystick(const FGsInputEventMsgBase& /*In_msg*/)
{
	// ai 진행 상태와 상관없이 갱신
	_isJoystickPressed = false;

	if (false == CanAIPlay())
	{
		return;
	}

	// 복귀 위치 저장
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_MODE_SAVE_AUTO_POSITION, nullptr);
	if (nullptr != _stateManager)
	{
		// 릴리즈 시점에 wait으로 변경...
		_stateManager->ClearState();
	}
}
// 터치 무브 이동 시작
void FGsAIManager::StartTouchMove()
{
	// 수동이동시 auto 유지 false면 auto 끈다 
	if (_isNonAutoMoveKeepAuto == false)
	{
		SetActiveAI(false);
	}
}
// 조이스틱 누름
void FGsAIManager::OnPressJoystick(const FGsInputEventMsgBase& /*In_msg*/)
{
	// ai 진행 상태와 상관없이 갱신
	_isJoystickPressed = true;

	// 수동이동시 auto 유지 false면 auto 끈다 
	if (_isNonAutoMoveKeepAuto == false)
	{
		SetActiveAI(false);
	}

	if (nullptr != _contentsConditionDefault)
	{
		_contentsConditionDefault->OnPressJoystick();
	}
}

// 메시지 바인딩 처리
void FGsAIManager::BindMessage()
{
	FGsMessageHolder* msg = GMessage();
		
	// input쪽
	_actionDelegates.Emplace(
		msg->GetInput().AddRaw(MessageInput::PressSkill, this, &FGsAIManager::OnPressSkill)
	);
	_actionDelegates.Emplace(
		msg->GetInput().AddRaw(MessageInput::ReleaseSkill, this, &FGsAIManager::OnReleaseSkill)
	);
	_actionDelegates.Emplace(
		msg->GetInput().AddRaw(MessageInput::ReleaseJoystick, this, &FGsAIManager::OnReleaseJoystick)
	);
	_actionDelegates.Emplace(
		msg->GetInput().AddRaw(MessageInput::PressJoystick, this, &FGsAIManager::OnPressJoystick)
	);

	// ai 쪽
	_aiDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::AI_MODE_ACTIVE_TOGGLE, this, &FGsAIManager::OnActiveToggleAI)
	);
	_aiDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::AI_MODE_CONTENTS_CHANGE, this, &FGsAIManager::OnContentsChange)
	);

	_aiDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_DIE, this, &FGsAIManager::OnLocalPlayerDie)
	);

	_aiDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_TARGET_CLEAR, this, &FGsAIManager::OnLocalTargetClear)
	);

	_aiDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::AI_STOP_CHECK_TOGGLE, this, &FGsAIManager::OnStopCheckToggle)
		);

	_aiDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::INTERACTION_CONTENTS_BY_NPC, this, &FGsAIManager::OnInteractionContents)
	);

	_aiDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::AI_QUEST_AUTO_MOVE_START, this,
			&FGsAIManager::OnQuestAutoMoveStart)
	);

	_aiDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this,
			&FGsAIManager::OnLocalPlayerSpawnComplete)
	);

	_aiDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_REVIVE, this,
			&FGsAIManager::OnLocalPlayerSpawnComplete)
	);

	_aiDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_FADE_IN_END, this,
			&FGsAIManager::OnLocalSpawnFadeInEnd)
	);
	_aiDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::AUTO_MOVE_FAILED, this,
			&FGsAIManager::OnAutoMoveFailed)
	);
	_aiDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::INTERACTION_BUTTON_CLICK, this,
			&FGsAIManager::OnInteractionButtonClick)
	);

	_aiDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::AUTO_MOVE_CANT_FIND_NEXTMAP_WARP, this,
			&FGsAIManager::OnAutoMoveCantFindNextMapWarp)
	);

	_aiDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_TARGET_CHANGED, this,
			&FGsAIManager::OnLocalTargetChanged)
	);
	_systemParamDelegates.Emplace(
		msg->GetSystemParam().AddRaw(MessageSystem::LOAD_TOPO_START, this,
			&FGsAIManager::OnLoadTopoStart)
	);

	_systemParamDelegates.Emplace(
		msg->GetSystemParam().AddRaw(MessageSystem::SEQUENCE_PLAYER_START, this,
			&FGsAIManager::OnSequencePlayerStart)
	);

	_systemParamDelegates.Emplace(
		msg->GetSystemParam().AddRaw(MessageSystem::SEQUENCE_PLAYER_END, this,
			&FGsAIManager::OnSequencePlayerEnd)
	);

	_systemDelegates.Emplace(
		msg->GetSystem().AddRaw(MessageSystem::NET_DISCONECTED, this,
			&FGsAIManager::OnNetDisconected)
	);


	//stage message
	MStageParam& msgStage = msg->GetStageParam();
	_msgStageHandleList.Emplace(msgStage.AddRaw(MessageStage::ENTER_TOWN_CALLBACK,
		this, &FGsAIManager::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddRaw(MessageStage::LEAVE_TOWN_CALLBACK,
		this, &FGsAIManager::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddRaw(MessageStage::ENTER_SAFETY_CALLBACK,
		this, &FGsAIManager::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddRaw(MessageStage::LEAVE_SAFETY_CALLBACK,
		this, &FGsAIManager::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddRaw(MessageStage::ENTER_ARENA_READY_CALLBACK,
		this, &FGsAIManager::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddRaw(MessageStage::LEAVE_ARENA_READY_CALLBACK,
		this, &FGsAIManager::OnChangeSafetyMode));
}

// 조작이 있는가(수동 조작)
bool FGsAIManager::IsThereInput() const 
{
	if (true == _isSkillBtnPressed || true == _isJoystickPressed)
	{
		return true;
	}
	return false;
}

// ai 킬지 말지
void FGsAIManager::SetAIOnOff(bool In_isOn)
{
	_isAIOn = In_isOn;

	// 새로운 컨텐츠 타입
	EGsAIContentsType newContentType = EGsAIContentsType::Auto_off;
	// 자동진행 시작
	if (true == _isAIOn)
	{
		// 필드 타입
		newContentType = EGsAIContentsType::Field;

		// 제자리 돌아가기 저장
		GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_MODE_SAVE_AUTO_POSITION, nullptr);
	}

	// 컨텐츠 타입 변경
	ChangeContentsType(newContentType);	
}
// ai on off 토글
void FGsAIManager::ToggleAIOnOff()
{
	// 바꿈
	SetAIOnOff(!_isAIOn);
}
// ai 멈출지
void FGsAIManager::SetAIStop(bool In_val)
{
	_isAIStop = In_val;

	// stop 끝낼때
	if (false == _isAIStop)
	{
		if (nullptr != _stateManager)
		{
			// 릴리즈 시점에 wait으로 변경...
			_stateManager->ClearState();
		}
	}
}
// 로그 출력할지 세팅
void FGsAIManager::SetIsShowLog(bool In_val)
{ 
	_isShowLog = In_val; 
	if (nullptr != _stateManager)
	{
		_stateManager->SetIsShowLog(In_val);
	}
}
// ai on, off toggle
void FGsAIManager::OnActiveToggleAI(const IGsMessageParam*)
{
	// 자동 공격이 잠겨있을 경우 비활성화
	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::AUTO_ATTACK, true))
	{
		SetActiveAI(false);
		return;
	}

	// 현재랑 반대로 변경
	ToggleAIOnOff();
	// 변경된거 전파
	FGsPrimitiveInt32 param(static_cast<int32>(_isAIOn));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_MODE_ACTIVE, &param);

	// 땅터치 자동 이동중 auto 키거나 피격시 남은 거리 표시 이슈 
	// 오토 키거나 끌때 skill 에서 auto move stop 시키는데
	// 예약된 데이터도 삭제하자
	// https://jira.com2us.com/jira/browse/C2URWQ-6526
	if (true == IsBranch(EGsGameClientBranchType::AUTO_MOVE_REMAIN_LENGTH_SHOW))
	{
		if (_localPlayer != nullptr)
		{
			_localPlayer->ClearReserveDataByReserveWork(nullptr);
		}
	}
}
// ai on, off 강제 세팅
void FGsAIManager::SetActiveAI(bool In_isActive)
{
	// 같으면 처리할 필요 없음
	if (_isAIOn == In_isActive)
	{
		return;
	}
	// 바꿈
	SetAIOnOff(In_isActive);
	// 변경된거 전파
	FGsPrimitiveInt32 param(static_cast<int32>(_isAIOn));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_MODE_ACTIVE, &param);
}
// 컨텐츠 변경 casting: EGsAIContentsType
void FGsAIManager::OnContentsChange(const IGsMessageParam* In_contentsType)
{
	const FGsPrimitiveInt32* param = In_contentsType->Cast<const FGsPrimitiveInt32>();
	EGsAIContentsType type = static_cast<EGsAIContentsType>(param->_data);
	ChangeContentsType(type);
}

// ai를 플레이 해도 되는지
bool FGsAIManager::CanAIPlay() const
{ 
	// 스톱 상태면 false
	// if safety zone entered, can't play ai
	if ((_isAIStop == true && _isUseStop == true) 
		||_isSafetyZone == true
		)
	{
		return false;
	}

	return true;
}
// 로컬 사망
// 내가 죽으면 auto 끄자(바로 npc 공격해서 불편하다는 건의 사항옴)
void FGsAIManager::OnLocalPlayerDie(const IGsMessageParam*)
{
	SetActiveAI(false);
}

// 로컬 타겟 클리어
void FGsAIManager::OnLocalTargetClear(const IGsMessageParam*)
{
	// ai play를 할수 있는가
	if (false == CanAIPlay())
	{
		return;
	}
	// 자동 이동중이면 멈춤
	if (_stateManager == nullptr)
	{
		return;
	}	

	if (nullptr != _stateManager)
	{
		// 릴리즈 시점에 wait으로 변경...
		_stateManager->ClearState();
	}

}

// ai stop 플래그 쓸지 말지 toggle
void FGsAIManager::OnStopCheckToggle(const IGsMessageParam* In_isActive)
{
	_isUseStop = !_isUseStop;
}


void FGsAIManager::OnInteractionContents(const IGsMessageParam* In_data)
{
	// npc list 이동이면 처리
	if (_isNpcListMoveStart == false)
	{
		return;
	}

	_isNpcListMoveStart = false;


	const FGsInteractionMessageParamContents* param = In_data->Cast<const FGsInteractionMessageParamContents>();
	FGsMovementHandlerLocalPlayer* movement = _localPlayer->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (nullptr == movement)
	{
		return;
	}

	// 현재 npclist  이동중이면
	if (movement->GetStateAutoMove().GetMovementAutoContentsType() == EGsMovementAutoContentsType::NpcListMove)
	{
		movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::None);

		// update slot ui
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_NPCFUNCTION);
	}
}

// 퀘스트 자동 이동 시작
void FGsAIManager::OnQuestAutoMoveStart(const IGsMessageParam* In_data)
{
	// 모든타입은 ai 를 끈다
	// 일단끔
	SetActiveAI(false);
	

	// 퀘스트 핸들러에서는 메시지로 안하는 이유는
	// ai를 끌때 스킬핸들러에서 ai move stop으로 상태 전이되는데
	// 이동 처리후 stop이 되어서 멈춘다
	// ai manager가  받아서 멤버에게 전달로 변경
	if (nullptr != _questHandler)
	{
		_questHandler->OnQuestAutoMoveStart(In_data);
	}
}

// 로컬 스폰 끝
void FGsAIManager::OnLocalPlayerSpawnComplete(const IGsMessageParam*)
{
	if (nullptr == _localPlayer)
	{
		return;
	}
	
	if (_isDirtyNextMapMove == false && 
		_nextMapMoveInfo._isNextMoveOn == true)
	{		
		MoveToPosAuto(_nextMapMoveInfo._moveContentsType, _nextMapMoveInfo._moveMapId,
			_nextMapMoveInfo._movePos, true, _nextMapMoveInfo._useAcceptanceRadius);
	}
	
	// flag clear
	_nextMapMoveInfo._sendWarp = false;

	
	if (_contentsConditionAutoReturn == nullptr)
	{
		return;
	}
	// auto 켜져있고
	// 제자리 돌아가기 on 이면 위치 저장
	if (_isAIOn == true &&
		_contentsConditionAutoReturn->GetIsAutoReturnMode() == true)
	{
		_contentsConditionAutoReturn->SaveAutoStartPos();
	}

	// 매너 체크 타겟 클리어
	ClearMannerCheckTarget();

	if (_contentsConditionDefault != nullptr)
	{
		_contentsConditionDefault->OnLoadPlayerSpawnComplete();
	}

	// 클리어 데이터
	_isSkillBtnPressed = false;
	_isJoystickPressed = false;
}

// stat load topo 시작
void FGsAIManager::OnLoadTopoStart(const IGsMessageParam* inParam)
{
	_isStartLoadToPo = true;
}

// 특정 테리토리영역 진입
void FGsAIManager::OnChangeSafetyMode(const IGsMessageParam* InParam)
{
	const FGsStageFenceMsgParam* param = static_cast<const FGsStageFenceMsgParam*>(InParam);
	if (nullptr == param)
	{
		return;
	}
	_isSafetyZone = param->_isSafety;
}

// 자동 이동(다음맵 진행)
void FGsAIManager::MoveToPosAuto(EGsMovementAutoContentsType In_type, int In_mapId,
	const FVector& In_pos, bool In_isAutoOff,
	float In_useAcceptanceRadius)
{

	if (In_isAutoOff == true)
	{
		// 모든타입은 ai 를 끈다
		// 일단끔
		SetActiveAI(false);
	}

	if (_localPlayer == nullptr)
	{
		return;
	}

	int currentMapId = GLevel()->GetCurrentLevelId();
	bool isUseWarp = false;
	if (In_mapId != currentMapId)
	{
		isUseWarp = true;
	}
	else
	{
		AGsCharacterLocalPlayer* localChar = _localPlayer->GetLocalCharacter();
		if (localChar == nullptr)
		{
			return;
		}
		AGsPlayerController* controller = localChar->GetPlayerController();
		if (controller == nullptr)
		{
			return;
		}

		FVector localPos = localChar->GetActorLocation();

		float acceptanceRadius = FGsMove::GetMovePosRange();
		// -값 들어가면 이동 실패 되므로 최소값으로 세팅
		acceptanceRadius = FMath::Max(acceptanceRadius, 0.1f);

		//네비 메쉬로 이동 가능한가
		bool isMovePossible = FGsMove::IsMovePossible(controller,
			localChar,
			In_pos, localPos,
			acceptanceRadius);

		if (isMovePossible == false)
		{
			FGsSchemaSpotInfo spotInfo;
			int warpId = 0;
			if (true == GSClientSpawn()->TryGetCurrentMapWarpInfo(currentMapId, localPos, In_pos,
				warpId, spotInfo))
			{
				isUseWarp = true;
			}
		}
	}


	if (isUseWarp == true)
	{
		_nextMapMoveInfo._isNextMoveOn = true;
		_nextMapMoveInfo._moveContentsType = In_type;
		_nextMapMoveInfo._moveMapId = In_mapId;
		_nextMapMoveInfo._movePos = In_pos;
		_nextMapMoveInfo._useAcceptanceRadius = In_useAcceptanceRadius;
	}
	else
	{
		_nextMapMoveInfo._isNextMoveOn = false;
	}

	// flag clear
	_nextMapMoveInfo._sendWarp = false;

	FGsMovementHandlerLocalPlayer* movement = _localPlayer->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (nullptr == movement)
	{
		return;
	}

	TArray<FVector> arrPos;
	arrPos.Add(In_pos);

	movement->SetMovementAutoContentsType(In_type);
	movement->ChangeAutoMoveState().StartMapIdPositions(_localPlayer, In_mapId,
		arrPos,		
		FGsAutoMoveCompletedDelegate::CreateRaw(this, &FGsAIManager::CallbackMoveToPosEnd),
		FGsAutoMoveCompletedOneDelegate::CreateRaw(this, &FGsAIManager::CallbackMoveToPosFailed),
		false,
		In_useAcceptanceRadius
	);

	// 제자리 돌아가기는 quest active 안끈다
	if (In_type != EGsMovementAutoContentsType::AutoReturnMove)
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_ACTIVE_OFF, nullptr);
	}
}
// 자동 이동(현재맵 진행)
void FGsAIManager::MoveToPosAutoCurrentMap(EGsMovementAutoContentsType In_type, const FVector& In_pos, bool In_isAutoOff)
{
	int currentMapId = GLevel()->GetCurrentLevelId();
	MoveToPosAuto(In_type, currentMapId, In_pos, In_isAutoOff);
}
void FGsAIManager::CallbackMoveToPosEnd()
{
	if (_localPlayer == nullptr)
	{
		return;
	}

	_localPlayer->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);	


	FGsMovementHandlerLocalPlayer* movement = _localPlayer->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (movement != nullptr)
	{
		EGsMovementAutoContentsType nowAutoContentsType =
			movement->GetStateAutoMove().GetMovementAutoContentsType();

		if (nowAutoContentsType == EGsMovementAutoContentsType::AutoReturnMove)
		{
			// clear 
			FGsAIReserveParam aiReserveClearParam(EGsAIActionType::MOVE_TO_AUTO_START_POS);
			GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_CLEAR_RESERVE_JOB, &aiReserveClearParam);
		}
		
		movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::None);
	}


	if (_nextMapMoveInfo._isNextMoveOn == false)
	{
		return;
	}

	//1. 현재맵이랑 이동 맵이 같으면	
	int currentMapId = GLevel()->GetCurrentLevelId();
	if (currentMapId == _nextMapMoveInfo._moveMapId)
	{
		//2. 최종 목적지 근처로 왔다면
		float acceptanceRadius = FGsMove::GetMovePosRange();
		// -값 들어가면 이동 실패 되므로 최소값으로 세팅
		acceptanceRadius = FMath::Max(acceptanceRadius, 0.1f);

		// 먼가 입력이 있었으면 그걸쓴다
		if (_nextMapMoveInfo._useAcceptanceRadius != 0.0f)
		{
			acceptanceRadius = _nextMapMoveInfo._useAcceptanceRadius;
		}

		bool isReachGoalPos = movement->GetStateAutoMove().HasReached(_nextMapMoveInfo._movePos, acceptanceRadius);
		if (isReachGoalPos == true)
		{
			_nextMapMoveInfo._isNextMoveOn = false;
			// flag clear
			_nextMapMoveInfo._sendWarp = false;
		}
	}
}
void FGsAIManager::CallbackMoveToPosFailed(const FPathFollowingResult& Result)
{
	if (_localPlayer == nullptr)
	{
		return;
	}

	_localPlayer->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);

	// 워프 앞에서 서버에서 멈추게하는 패킷(SC_MOVE_IMMEDIATE_STOP) 받았을때
	// 다음맵 이동 예약값 안날리게 보호 한 값때문에 안날림(_sendWarp == true)	
	// send warp -> server -> MoveImmediateStopAck -> fail called
	if (_nextMapMoveInfo._sendWarp == false)
	{
		_nextMapMoveInfo._isNextMoveOn = false;
	}
}
void FGsAIManager::SetNextMapWarpStart()
{
	// 워프 앞에서 서버에서 멈추게하는 패킷(SC_MOVE_IMMEDIATE_STOP) 받았을때
	// 다음맵 이동 예약값 안날리게 보호 한거
	// next map warp server send
	if (_nextMapMoveInfo._isNextMoveOn == true)
	{
		_nextMapMoveInfo._sendWarp = true;
	}
}

// 로컬 스폰 페이드 인 끝(퀘스트 액티브 시점과 맞춘다)
void FGsAIManager::OnLocalSpawnFadeInEnd(const IGsMessageParam*)
{
	_isStartLoadToPo = false;
}

// 자동 이동 실패
void FGsAIManager::OnAutoMoveFailed(const IGsMessageParam* InParam)
{
	if (InParam == nullptr)
	{
		return;
	}
	const FGsMoveFailedMessageParam* failedParam = InParam->Cast<const FGsMoveFailedMessageParam>();
	if (failedParam == nullptr)
	{
		return;
	}

	NpcListMoveFailed(failedParam->_moveFailedReason, failedParam->_npcTblId);


}

void FGsAIManager::NpcListMoveFailed(EGsMoveFailedReason In_reason, int In_npcTblId)
{
	// npc list 이동이면 처리
	if (_isNpcListMoveStart == false)
	{
		return;
	}

	_isNpcListMoveStart = false;
	

	if (In_reason == EGsMoveFailedReason::InteractionNPCNotExist)
	{
	// 인터랙션 npc 타겟 없을때 나오는 팝업 에디터에서만 나오게 처리
	// https://jira.com2us.com/jira/browse/C2URWQ-2208
#if WITH_EDITOR
		const FString failTickerString =
			FString::Printf(TEXT("find fail from npc tbl id: %d"), In_npcTblId);

		FText failTickerText = FText::FromString(failTickerString);
		FGsUIHelper::TrayMessageTicker(failTickerText);
		FGsUIHelper::PopupOK(failTickerText);
#endif


		// 상태 idle로 돌리기
		FGsGameObjectStateManager::ProcessEvent(_localPlayer, EGsStateBase::Idle);
	}

	// update slot ui
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_NPCFUNCTION);
}
// 인터랙션 버튼 클릭(ai 끄자)
void FGsAIManager::OnInteractionButtonClick(const IGsMessageParam* In_param)
{
	SetActiveAI(false);
}

// 스킬 시작 SEND
void FGsAIManager::SendStartSkill(int64 In_targetId)
{
	// 옵션 체크(꺼져 있으면 처리안함)
	if (_mannerMode == EGsAIBattleOptionMannerMode::OFF)
	{
		return;
	}

	// 스킬 시작 송신
	_isSkillStartSend = true;
	// 수신 초기화
	_isSkillStartRecv = false;
	_mannerCheckTargetId = In_targetId;
}

// 스킬 시작 RECV remote
void FGsAIManager::RecvStartSkillRemote(int64 In_targetId, int64 In_remoteId)
{
	// 옵션 체크(꺼져 있으면 처리안함)
	if (_mannerMode == EGsAIBattleOptionMannerMode::OFF)
	{
		return;
	}

	// 타겟이 다르면 추가
	if (_mannerCheckTargetId != In_targetId)
	{
		AddMannerCheckTarget(In_targetId, In_remoteId);
		return;
	}

	// 타겟이 같고
	// 로컬이 스킬 send 이미 수신했으면 끝
	if (_isSkillStartRecv == true)
	{
		return;
	}

	// 로컬보다 먼저 remote가 수신 받음
	AddMannerCheckTarget(In_targetId, In_remoteId);
}
// 스킬 시작 RECV local
void FGsAIManager::RecvStartSkillLocal(int64 In_targetId)
{
	// 옵션 체크(꺼져 있으면 처리안함)
	if (_mannerMode == EGsAIBattleOptionMannerMode::OFF)
	{
		return;
	}
	
	// 스킬 시작 수신
	_isSkillStartRecv = true;
	// 스킬 시작 송신 clear
	_isSkillStartSend = false;
}

// 매너 체크 타겟 추가
void FGsAIManager::AddMannerCheckTarget(int64 In_targetId, int64 In_remoteId)
{
	// 옵션 체크(대상이 기사단, 파티 일때는 처리안함)
	if (_mannerMode == EGsAIBattleOptionMannerMode::ALLY_EXCEPTION)
	{
		if (UGsGameObjectUtil::GetIsMyCrewRemoteByGameId(In_remoteId) == true)
		{
			return;
		}
	}

	FGsMannerModeInfo* findData = FindMannerModeInfoByMonsterId(In_targetId);
	// 찾는게 없다면 추가
	if (findData == nullptr)
	{
		// remove ai target
		UGsGameObjectBase* target = GSGameObject()->FindObject(EGsGameObjectType::NonPlayerBase, In_targetId);
		if (target == nullptr)
		{
			return;
		}

		// 월드 보스, 파티던전 보스는 매너모드에 속하지 않음
		// https://jira.com2us.com/jira/browse/C2URWQ-6511
		UGsGameObjectNonPlayerBase* npcBase = target->CastGameObject<UGsGameObjectNonPlayerBase>();
		if (nullptr == npcBase)
		{
			return;
		}

		const FGsSchemaNpcData* npcData = npcBase->GetNpcData();
		if (nullptr == npcData)
		{
			return;
		}
		
		// 기사단 던전 보스도 제외
		// https://jira.com2us.com/jira/browse/C2URWQ-7426
		if (NpcFunctionType::WORLD_BOSS  == npcData->npcFunctionType ||
			NpcFunctionType::PARTY_DUNGEON_BOSS == npcData->npcFunctionType ||
			NpcFunctionType::GUILD_BOSS == npcData->npcFunctionType)
		{
			return;
		}

		FGsMannerModeInfo* newData = new FGsMannerModeInfo(In_targetId, In_remoteId);
		_arrayMannerTargetInfo.Add(newData);
		
		FGsAIReserveParam aiReserveClearParam(EGsAIActionType::ATTACK_ANYONE, target);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_CLEAR_RESERVE_JOB, &aiReserveClearParam);
		

		// 1. ai on
		// 2. auto move to target(auto move state, target same)
		// -> auto move stop
		
		if (IsAIOn() == true)
		{
			if (_localPlayer == nullptr ||
				_localPlayer->GetBaseFSM() == nullptr)
			{
				return;
			}
			if (_localPlayer->GetBaseFSM()->IsState(EGsStateBase::AutoMove) == false)
			{
				return;
			}

			FGsTargetHandlerLocalPlayer* targetHandler = _localPlayer->GetCastTarget<FGsTargetHandlerLocalPlayer>();
			if (targetHandler == nullptr)
			{
				return;
			}

			if (targetHandler->GetTarget() != target)
			{
				return;
			}


			// stop auto move
			_localPlayer->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);
		}
	}
}

// 매너 모드 정보 얻기(몬스터 game id로)
FGsMannerModeInfo* FGsAIManager::FindMannerModeInfoByMonsterId(int64 In_gameId)
{
	FGsMannerModeInfo** findData =
		_arrayMannerTargetInfo.FindByPredicate(
			[In_gameId](FGsMannerModeInfo* el)
			{
				return el->GetMonsterGameId() == In_gameId;
			});

	if (findData == nullptr)
	{
		return nullptr;
	}
	return *findData;
}
// 매너 모드 정보 얻기(타유저 game id로)
FGsMannerModeInfo* FGsAIManager::FindMannerModeInfoByOwnerRemoteId(int64 In_gameId)
{
	FGsMannerModeInfo** findData =
		_arrayMannerTargetInfo.FindByPredicate(
			[In_gameId](FGsMannerModeInfo* el)
			{
				return el->GetRemoteGameId() == In_gameId;
			});

	if (findData == nullptr)
	{
		return nullptr;
	}
	return *findData;
}
// 매너 체크 타겟 삭제
void FGsAIManager::RemoveMannerCheckTarget(int64 In_targetId)
{
	if (FGsMannerModeInfo* findData = FindMannerModeInfoByMonsterId(In_targetId))
	{
		delete findData;
		_arrayMannerTargetInfo.Remove(findData);
	}

}
// 매너 타겟 오너 삭제
void FGsAIManager::RemoveMannerTargetOwnerRemote(int64 In_remoteGameId)
{
	if (FGsMannerModeInfo* findData = FindMannerModeInfoByOwnerRemoteId(In_remoteGameId))
	{
		delete findData;
		_arrayMannerTargetInfo.Remove(findData);
	}

}

// 매너 체크 타겟 초기화
void FGsAIManager::ClearMannerCheckTarget()
{
	if (_arrayMannerTargetInfo.Num() != 0)
	{
		for (FGsMannerModeInfo* iter : _arrayMannerTargetInfo)
		{
			if (iter == nullptr)
			{
				continue;
			}
			delete iter;
		}
		_arrayMannerTargetInfo.Empty();
	}
}

// 매너모드 세팅
void FGsAIManager::SetMannerMode(EGsAIBattleOptionMannerMode In_mode)
{
	_mannerMode = In_mode;

	// 기존 정보 클리어
	ClearMannerCheckTarget();
}

// 매너 타겟인가
bool FGsAIManager::GetIsMannerTarget(UGsGameObjectBase* In_object)
{
	if (_mannerMode == EGsAIBattleOptionMannerMode::OFF ||
		In_object == nullptr)
	{
		return false;
	}

	FGsMannerModeInfo* findData = FindMannerModeInfoByMonsterId(In_object->GetGameId());
	return (findData == nullptr) ? false : true;

}
bool FGsAIManager::GetIsLootTargetExistAndPossible()
{
	bool result = (GetContentsConditionDefault()->IsInSearchRangeItem()) ? true : false;

	return result;
}

// auto return dist
float FGsAIManager::GetReturnDist()
{
	FGsAIContentsConditionAutoReturn* autoReturn = GetContentsConditionAutoReturn();
	if (autoReturn == nullptr)
	{
		return 0.0f;
	}
	float returnDist = autoReturn->GetReturnDist();
	return returnDist;
}

// req auto move cool time check
// 1. fsm: auto move
// 2. auto move contents type: loot item move
void FGsAIManager::RequestAutoLootCooltime()
{
	if (_localPlayer == nullptr ||
		_localPlayer->GetBaseFSM() == nullptr)
	{
		return;
	}
	if (_localPlayer->GetBaseFSM()->IsState(EGsStateBase::AutoMove) == false)
	{
		return;
	}
	FGsMovementHandlerLocalPlayer* movement = _localPlayer->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (nullptr == movement)
	{
		return;
	}

	if (movement->GetStateAutoMove().GetMovementAutoContentsType() != EGsMovementAutoContentsType::LootItemMove)
	{
		return;
	}

	if (_contentsConditionDefault == nullptr)
	{
		return;
	}
	_contentsConditionDefault->StartAutoLootCooltime();

	FGsAIReserveParam aiReserveClearParam(EGsAIActionType::LOOT_ITEM);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_CLEAR_RESERVE_ALL_JOB_BY_TYPE, &aiReserveClearParam);

	FGsAIReserveParam aiReserveClearParam_lootItemAutoOff(EGsAIActionType::LOOT_ITEM_AUTO_OFF);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_CLEAR_RESERVE_ALL_JOB_BY_TYPE, &aiReserveClearParam_lootItemAutoOff);

	// stop auto move
	_localPlayer->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);
}

bool FGsAIManager::GetIsAutoLootCoolTime()
{
	return GetContentsConditionDefault()->IsAutoLootCooltime();
}
// 서버 이동 lock 상태인가
bool FGsAIManager::IsServerMoveLock()
{
	return GetContentsConditionDefault()->IsServerMoveLock();
}
void FGsAIManager::OnLocalTargetChanged(const IGsMessageParam* In_param)
{
	if (nullptr == In_param)
	{
		return;
	}

	const FGsGameObjectMessageParamTarget* castParam = In_param->Cast<const FGsGameObjectMessageParamTarget>();
	if (castParam->_paramTarget == nullptr ||
		castParam->_isFirstTarget == false)
	{
		return;
	}


	// set first target complete
	RequestAutoLootCooltime();
}

void FGsAIManager::OnSequencePlayerStart(const IGsMessageParam* inParam)
{
	SetAIStop(true);
}
void FGsAIManager::OnSequencePlayerEnd(const IGsMessageParam* inParam)
{
	SetAIStop(false);
}

// load complete에서 워프 연출있다면 이동대기 끝나고 할처리
void FGsAIManager::SetNextMapMoveWait()
{
	// 맵전환 후 이동이 없다면 return
	if (_nextMapMoveInfo._isNextMoveOn == false)
	{
		return;
	}
	// 이동 요청은 있었는데 서버에서 중단했으므로
	// 이동가능할때 다시 이동하자
	_isDirtyNextMapMove = true;
}

// 지연된 다음맵 이동 시도
void FGsAIManager::TryDelayedNextMapMove()
{
	if (_localPlayer == nullptr)
	{
		return;
	}
	// 이동 제약중이면 return
	if (true == _localPlayer->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT))
	{
		return;
	}

	// dirt 해제
	_isDirtyNextMapMove = false;

	// 이동 시작

	if (_nextMapMoveInfo._isNextMoveOn == true)
	{
		MoveToPosAuto(_nextMapMoveInfo._moveContentsType, _nextMapMoveInfo._moveMapId,
			_nextMapMoveInfo._movePos, true, _nextMapMoveInfo._useAcceptanceRadius);
	}
}

// 자동 이동 다음 워프 없을때 정보 지우는 처리(남아있으면 워프후 다시 시도하는 이슈 수정)
// https://jira.com2us.com/jira/browse/C2URWQ-6497
void FGsAIManager::OnAutoMoveCantFindNextMapWarp(const IGsMessageParam* In_param)
{
	if (_nextMapMoveInfo._isNextMoveOn == true)
	{
		_nextMapMoveInfo._isNextMoveOn = false;
	}
}

// 네트워트 끊김
// 네트워크 끊길때 자동 이동 멈추게 처리
// https://jira.com2us.com/jira/browse/C2URWQ-2208
void FGsAIManager::OnNetDisconected()
{
	if (_localPlayer == nullptr)
	{
		return;
	}

	// 1. 스테이트 auto move 인지
	bool isAutoMove = _localPlayer->GetBaseFSM()->IsState(EGsStateBase::AutoMove);
	// 아니면 처리안함
	if (isAutoMove == false)
	{
		return;
	}

	// 이동 예약 데이터 삭제
	_localPlayer->ClearReserveDataByReserveWork();

	_localPlayer->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);
}

// 디버그 예약 리스트 show
void FGsAIManager::ShowDebugReserveList(bool In_isShow)
{
	_isShowDebugReserveList = In_isShow;

	FGsPrimitiveInt32 param(static_cast<int32>(In_isShow));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::DEBUG_AI_RESERVE_LIST_SHOW, &param);
}

// 디버그용 예약 리스트 갱신 요청
void FGsAIManager::SendDebugReserveListUpdate()
{
	if (_isShowDebugReserveList == false)
	{
		return;
	}

	GMessage()->GetGameObject().SendMessage(MessageGameObject::DEBUG_AI_RESERVE_LIST_UPDATE, nullptr);
}