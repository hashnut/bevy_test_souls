#include "GsAIContentsConditionDefault.h"

#include "AI/Function/GsAIFunc.h"
#include "AI/GsAIStateManager.h"
#include "AI/ContentsCondition/GsAIContentsConditionAttOrder.h"
#include "AI/ContentsCondition/GsAIContentsConditionAutoReturn.h"
#include "AI/Data/GsAIDefine.h"
#include "AI/Data/GsAIReserveJob.h"
#include "AI/Quest/GsAIQuestHandler.h"

#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/ObjectClass/GsGameObjectDropObject.h"
#include "GameObject/ObjectClass/GsGameObjectPropInteraction.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Target/AutoTarget/GsTargetRule.h"
#include "GameObject/Target/AutoTarget/AutoTargetMode/GsTargetModeBase.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Target/AutoTarget/AutoTargetMode/GsAutoTargetMode.h"
#include "GameObject/AI/GsAIReserveHandler.h"
#include "GameObject/Looting/GsLootingHandlerLocalPlayer.h"

#include "Item/GsItemManager.h"

#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/MessageParam/GsInteractionMessageParam.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsCameraModeManager.h"

#include "Input/GsInputBindingLocalPlayer.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "DataCenter/Public/Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"

#include "Option/GsServerOption.h"
#include "Option/GsGameUserSettings.h"
#include "Option/GsOptionFunc.h"

#include "T1Project.h"
#include "DrawDebugHelpers.h"

FGsAIContentsConditionDefault::~FGsAIContentsConditionDefault()
{	
}
// 초기화
void FGsAIContentsConditionDefault::Initialize(FGsAIManager* In_mng)
{	
	_aiManager = In_mng;

	if (FGsMessageHolder* msg = GMessage())
	{
		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::AI_MODE_FIRST_BATTLE, this,
				&FGsAIContentsConditionDefault::OnBattingFirst));
		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::AI_MOVE_REQUEST, this,
				&FGsAIContentsConditionDefault::OnRequestMove));
		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::AI_DRAW_SEARCH_RANGE, this,
				&FGsAIContentsConditionDefault::OnSetDrawSearchRange));
		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::AI_AUTO_MOVE_FAIL, this,
				&FGsAIContentsConditionDefault::OnSetShowAutoMoveFail));
		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::AI_AUTO_LOOT_DROP_ITEM_GRADE, this,
				&FGsAIContentsConditionDefault::OnSetDropItemGrade));
		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::DROPOBJECT_DESPAWN, this,
				&FGsAIContentsConditionDefault::OnDropObjectDespawn));

		_msgSystemNoParamDelegates.Emplace(
			msg->GetSystem().AddRaw(MessageSystem::GAME_SERVER_RECONNECTION_SUCCESS,
				this, &FGsAIContentsConditionDefault::OnGameServerReconnectionSuccess));
	}
	// 2021/10/21 PKT - 일단 기본값 세팅. 서버 옵션 패킷 오면 그때 다시 세팅..
	_autoLootDropItemGrade = EGsAIAutoLootDropItemGradeType::All;


	// 대기 시간 틱값 저장
	_lootServerResponseWaitClearTick = GData()->GetGlobalData()->_lootServerResponseWaitClearSecond * ETimespan::TicksPerSecond;
}
// 해제
void FGsAIContentsConditionDefault::Finalize()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_gameObjectMsg.Num() != 0)
		{
			for (auto iter : _gameObjectMsg)
			{
				msg->GetGameObject().Remove(iter);
			}
			_gameObjectMsg.Empty();
		}

		if (_msgSystemNoParamDelegates.Num() != 0)
		{
			for (auto iter : _msgSystemNoParamDelegates)
			{
				msg->GetSystem().Remove(iter);
			}
			_msgSystemNoParamDelegates.Empty();
		}
	}

	if (_arrayMyMissingItem.Num() != 0)
	{
		_arrayMyMissingItem.Empty();
	}
}

void FGsAIContentsConditionDefault::SetCharacter(UGsGameObjectLocalPlayer* In_player)
{
	_localPlayer = In_player;
}
void FGsAIContentsConditionDefault::RemoveCharacter()
{
	_localPlayer = nullptr;
}
// 위치 도착했는가
bool FGsAIContentsConditionDefault::IsArrivePos()
{
	if (_localPlayer == nullptr)
	{
		GSLOG(Error, TEXT("LocalPlayer is nullptr"));
		return false;
	}

	FGsMovementHandlerLocalPlayer* movement = _localPlayer->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	return movement->GetStateAutoMove().HasReached(_movePos, _moveAcceptanceDist);
}

// 타겟 있는가??
bool FGsAIContentsConditionDefault::IsTargetExist()
{
	if (_localPlayer == nullptr)
	{
		GSLOG(Error, TEXT("LocalPlayer is nullptr"));
		return false;
	}

	FGsTargetHandlerBase* target = _localPlayer->GetTargetHandler();

	if (nullptr == target)
	{
		return false;
	}
	
	UGsGameObjectBase* targetBase = target->GetTarget();
	// 타겟이 없으면 false
	if (nullptr == targetBase)
	{
		return false;
	}
	UGsGameObjectCreature* targetCreature = Cast<UGsGameObjectCreature>(targetBase);
	// 타겟이 사망이면 false
	// 타겟이 공격 불가면 false
	if (nullptr == targetCreature ||
		targetCreature->IsDeadState()||
		targetCreature->IsAble() == false)
	{
		return false;
	}

	return true;
}
// 공격 범위 안쪽 인가?
bool FGsAIContentsConditionDefault::IsInAttackRange()
{
	if (nullptr == _localPlayer)
	{
		return false;
	}

	FGsSkillHandlerLocalPlayer* skillMgr = _localPlayer->GetCastSkill<FGsSkillHandlerLocalPlayer>();
	if (nullptr == skillMgr)
	{
		return false;
	}

	const FGsSchemaSkillSet* skillData = FGsAIFunc::FindNowUseSkillID(_localPlayer);

	if (nullptr == skillData)
	{
		return false;
	}

	return FGsSkillHandlerLocalPlayer::OutSide != skillMgr->IsInsideSkillRange(skillData);
}
// 공격 범위 안쪽 인가 , 타겟(공격)
bool FGsAIContentsConditionDefault::IsInAttackRangeTarget(UGsGameObjectBase* In_target)
{
	if (nullptr == _localPlayer)
	{
		return false;
	}

	FGsSkillHandlerLocalPlayer* skillMgr = static_cast<FGsSkillHandlerLocalPlayer*>(_localPlayer->GetSkillHandler());
	if (nullptr == skillMgr)
	{
		return false;
	}

	const FGsSchemaSkillSet* skillData = FGsAIFunc::FindNowUseSkillID(_localPlayer);

	if (skillData == nullptr)
	{
		return false;
	}
	return FGsSkillHandlerLocalPlayer::OutSide != skillMgr->IsInsideSkillRangeTarget(skillData, In_target);
}
// 선공 on, off
void FGsAIContentsConditionDefault::OnBattingFirst(const IGsMessageParam* In_isActive)
{
	const FGsPrimitiveInt32* param = In_isActive->Cast<const FGsPrimitiveInt32>();
	bool isActive = static_cast<bool>(param->_data);
	SetIsBattingFirst(isActive);
}

// 이동 요청
void FGsAIContentsConditionDefault::OnRequestMove(const IGsMessageParam* In_moveData)
{
	const FGsAIMoveData* castData = In_moveData->Cast<const FGsAIMoveData>();
	_isRequestMove = true;
	SetMovePos(castData->_movePos);
	SetAcceptanceDist(castData->_acceptanceDist);

	check(_aiManager);
	// 스테이트 초기화
	_aiManager->GetAIStateManager()->ClearState();
}
// 검색 범위 그릴지 세팅
void FGsAIContentsConditionDefault::OnSetDrawSearchRange(const IGsMessageParam* In_isActive)
{
	const FGsPrimitiveInt32* param = In_isActive->Cast<const FGsPrimitiveInt32>();
	bool isActive = static_cast<bool>(param->_data);
	_isDrawSearchRange = isActive;
}

// 자동 이동 실패 출력할지 세팅
void FGsAIContentsConditionDefault::OnSetShowAutoMoveFail(const IGsMessageParam* In_isActive)
{
	const FGsPrimitiveInt32* param = In_isActive->Cast<const FGsPrimitiveInt32>();
	bool isActive = static_cast<bool>(param->_data);
	_isShowAutoMoveFail = isActive;
}

void FGsAIContentsConditionDefault::OnSetDropItemGrade(const IGsMessageParam* In_isActive)
{
	if (nullptr == In_isActive)
	{
		GSLOG(Error, TEXT("nullptr == In_isActive"));
		return;
	}
	const FGsPrimitiveInt32* param = In_isActive->Cast<const FGsPrimitiveInt32>();	
	SetAutoLootDropItemGrade(StaticCast<EGsAIAutoLootDropItemGradeType>(param->_data));
}

// 사망이 아닌지 체크 하는 함수 가져오기
TFunction<bool(UGsGameObjectBase * obj)> FGsAIContentsConditionDefault::GetNotDeadCheckFunc()
{
	return [](UGsGameObjectBase* obj)
	{
		if (nullptr == obj)
		{
			return false;
		}
		
		UGsGameObjectCreature* creature = Cast<UGsGameObjectCreature>(obj);
		check(creature);

#ifdef TEST_LOG
		GSLOG(Error, TEXT("name: %s, isDeadState: %d"), 
			*creature->GetName(), creature->IsDeadState());
#endif

		return (creature->IsDeadState() == false  &&
			creature->IsZeroHP() == false);
	};
}

// 게임 오브젝트 fsm 이 idle 인지(공격)
bool FGsAIContentsConditionDefault::IsGameObjectFSMIdle()
{
	if (_localPlayer == nullptr)
	{
		return false;
	}
	return _localPlayer->GetBaseFSM()->IsState(EGsStateBase::Idle);
}
// 채집(Prop) 퀘스트 대상이 있는지
bool FGsAIContentsConditionDefault::IsSearchRangePropObject()
{
	if (UGsGameObjectPropInteraction* propObject = GSGameObject()->FindPropObjectNearest(true))
	{
		float distanceToTarget =
			_localPlayer->GetActor()->GetHorizontalDistanceTo(propObject->GetActor()) -
			propObject->GetData()->GetScaledCapsuleRadius();

		// 시야 범위 안인지
		int searchRange = GData()->GetGlobalData()->_autoReturnDist4;
		// 시야 밖이면 실패
		if (distanceToTarget > searchRange)
		{
			return false;
		}
		return true;
	}
	return false;
}
// 시야내 적이 있는지(공격)
bool FGsAIContentsConditionDefault::IsInSearchRangeAnyoneEnemy()
{

	float searchRange = GData()->GetGlobalData()->_autoReturnDist4;
	if (true == _aiManager->GetIsShowAIAttackCheckRadius() &&
		true == GSAI()->GetContentsConditionAutoReturn()->GetIsAutoReturnMode())
	{

		EGsAIAutoReturnStartPosType currType =
			GSAI()->GetContentsConditionAutoReturn()->GetAutoReturnStartPosType();


		// 중심거리부터 체크 할 길이
		float checkCenterDist = 0.0f;
		bool isDrawCenterRadius = false;
		switch (currType)
		{
		case EGsAIAutoReturnStartPosType::Dist20Meter:
		{
			// 20m
			checkCenterDist = GData()->GetGlobalData()->_autoReturnDist2;
			isDrawCenterRadius = true;
		}
		break;
		case EGsAIAutoReturnStartPosType::Dist30Meter:
		{
			// 30m
			checkCenterDist = GData()->GetGlobalData()->_autoReturnDist3;
			isDrawCenterRadius = true;
		}
		break;
		case EGsAIAutoReturnStartPosType::Dist50Meter:
		{
			// 50m
			checkCenterDist = GData()->GetGlobalData()->_autoReturnDist4;
			isDrawCenterRadius = true;
		}
		break;

		}

		AActor* localActor = _localPlayer->GetActor();
		if (localActor != nullptr)
		{
			UWorld* World = localActor->GetWorld();

			// 저장위치로부터 거리가 있으면
			// 이것도 그림
			if (isDrawCenterRadius != 0.0f)
			{
				// 제자리 타입
				// 2,3(10m, 20m): 기준점은 저장된 장소 위치
				FVector autoStartPos =
					GSAI()->GetContentsConditionAutoReturn()->GetAutoStartPos();

				DrawDebugSphere(World, autoStartPos, checkCenterDist,
					16, FColor::Red, false, 1.0f);
			}

		}
	}
	
	FGsTargetHandlerLocalPlayer* targetHandler = _localPlayer->GetCastTarget<FGsTargetHandlerLocalPlayer>();

	if (targetHandler == nullptr)
	{
		return false;
	}
	FGsTargetRule* targetRule = targetHandler->GetTargetRule();

	if (targetRule == nullptr)
	{
		return false;
	}
	FGsAutoTargetMode* autoMode = targetRule->GetAutoMode();

	if (autoMode == nullptr)
	{
		return false;
	}

	TArray<UGsGameObjectBase*> outList;
	autoMode->SearchTarget(outList);
	if (outList.Num() == 0)
	{
		return false;
	}
	
	return true;
}

// 시야내 아이템이 있는지(루팅)
// 인벤토리 루팅 가능한지
bool FGsAIContentsConditionDefault::IsInSearchRangeItem()
{
	// 거리체크는 내부에서 한다
	UGsGameObjectDropObject* findDrop = GSGameObject()->FindDropObjectNearest(true);
	if (nullptr == findDrop)
	{
		return false;
	}

	UGsItemManager* itemManager = GItem();
	if (itemManager == nullptr)
	{
		return false;
	}
	const  FGsSchemaItemCommon* itemTbl =  findDrop->GetItemTable();
	if (itemTbl == nullptr)
	{
		return false;
	}

	// 인벤토리 무게 다 안차야 됨
	// 1. check stackable item 
	// 2. other check slot
	int32 dropItemWeight = findDrop->GetItemWeight();
	if (itemManager->IsInvenWeightFull(dropItemWeight) == false &&
		itemManager->IsCanItemAdd(itemTbl->id, findDrop->GetItemAmount()))
	{
		return true;
	}

	return false;
}


// ai 매니저가 받은 메시지 전달
// 스킬 버튼 누름
void FGsAIContentsConditionDefault::OnPressSkill()
{
	if (_arrayMyMissingItem.Num()!= 0)
	{
		_arrayMyMissingItem.Empty();
	}
}
// 조이스틱 누름
void FGsAIContentsConditionDefault::OnPressJoystick()
{
	if (_arrayMyMissingItem.Num() != 0)
	{
		_arrayMyMissingItem.Empty();
	}
}

// 드랍 오브젝트 디스폰
void FGsAIContentsConditionDefault::OnDropObjectDespawn(const IGsMessageParam* In_param)
{
	const FGsInteractionMessageParam* castData = In_param->Cast<const FGsInteractionMessageParam>();
	if (castData == nullptr)
	{
		return;
	}
	int64 gameId = castData->_targetGameID;
	if (_arrayMyMissingItem.Contains(gameId))
	{
		_arrayMyMissingItem.Remove(gameId);

		// 마지막껄 먹든 사라지면
		// 다시 퀘스트 자동 이동 시작
		if (_arrayMyMissingItem.Num() == 0)
		{
			GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_AUTOPROGRESS_REQUEST, nullptr);
		}

	}
}


void FGsAIContentsConditionDefault::AddMissingMyDropObjectId(int64 In_gameId)
{
	_arrayMyMissingItem.Add(In_gameId);
}

// skill useable state check
bool FGsAIContentsConditionDefault::IsGameObjectSkillUseableState()
{
	if (nullptr == _localPlayer)
	{
		return false;
	}

	FGsSkillHandlerLocalPlayer* skillMgr = _localPlayer->GetCastSkill<FGsSkillHandlerLocalPlayer>();
	if (nullptr == skillMgr)
	{
		return false;
	}

	// 1. check skill cancel section
	if (true == skillMgr->CanCancel() && false == skillMgr->IsRequestData())
	{
		return true;
	}

	// 2. state idle check
	if (true == IsGameObjectFSMIdle())
	{
		return true;
	}
	return false;
}

// quest auto active check
bool FGsAIContentsConditionDefault::IsQuestAutoPlay()
{
	return GSQuest()->IsAutoProgressing();
}

// counter attack option check
bool FGsAIContentsConditionDefault::IsCounterAttackOptionOn()
{
	bool isCounterAttackOn = GServerOption()->IsCounterAttackOn();
	return isCounterAttackOn;
}
// counter attack target check
bool FGsAIContentsConditionDefault::IsCounterAttackTargetExist()
{
	if (_localPlayer == nullptr)
	{
		GSLOG(Error, TEXT("LocalPlayer is nullptr"));
		return false;
	}

	FGsTargetHandlerLocalPlayer* targetHandler = _localPlayer->GetCastTarget<FGsTargetHandlerLocalPlayer>();

	if (targetHandler == nullptr)
	{
		return false;
	}

	int64 attacker = targetHandler->GetNpcAttacker();

	return (attacker == 0) ? false : true;
}

void FGsAIContentsConditionDefault::Update()
{
	// 오토 쿨타임이면 처리
	if (_isAutoLootCooltime == true)
	{		
		int64 nowTick = FDateTime::UtcNow().GetTicks();
		int64 diffTick = nowTick - _autoLootBlockStartTime;

		static float coolTime = GData()->GetGlobalData()->_autoItemPickUpDelaySecond * ETimespan::TicksPerSecond;

		if (diffTick >= coolTime)
		{
			_isAutoLootCooltime = false;
		}
	}

	// 서버 응답 대기면 시간 체크
	// 일정시간후 해제하자
	// https://jira.com2us.com/jira/browse/CHR-24724
	if (_isLootServerResponseWait == true)
	{
		int64 nowTick = FDateTime::UtcNow().GetTicks();
		int64 diffTick = nowTick - _lootServerResponseWaitStartTime;

		if (diffTick >= _lootServerResponseWaitClearTick)
		{
			_isLootServerResponseWait = false;
		}
	}

}

void FGsAIContentsConditionDefault::StartAutoLootCooltime()
{
	_autoLootBlockStartTime = FDateTime::UtcNow().GetTicks();
	_isAutoLootCooltime = true;
}

// auto loot check
bool FGsAIContentsConditionDefault::IsAutoLootCooltime()
{
	return _isAutoLootCooltime;
}

// ai load player spawn complete call
void FGsAIContentsConditionDefault::OnLoadPlayerSpawnComplete()
{
	if (_arrayMyMissingItem.Num() != 0)
	{
		_arrayMyMissingItem.Empty();
	}
}

// quest auto loot check
bool FGsAIContentsConditionDefault::IsQuestAutoLoot()
{
	QuestObjectiveType questType = QuestObjectiveType::MAX;
	if (false == GSQuest()->GetAutoProgressObjectiveType(questType))
	{
		return false;
	}


	return (questType == QuestObjectiveType::INTERACT_PROP_TOUCH) ? true : false;
}

// server response check
bool FGsAIContentsConditionDefault::IsLootServerResponseWait()
{
	return _isLootServerResponseWait;
}

// 1. first job loot target weight over(add current total weight)
// 2. inven slot check
bool FGsAIContentsConditionDefault::IsLootImpossible()
{
	if (_localPlayer == nullptr)
	{
		return false;
	}

	FGsAIReserveHandler* aiReserveHandler = _localPlayer->GetAIReserve();
	if (aiReserveHandler == nullptr)
	{
		return false;
	}

	FGsAIReserveJob* firstJob = aiReserveHandler->GetFirstJob();
	if (firstJob == nullptr)
	{
		return false;
	}

	// 루팅 액션 타입 추가
	if ((firstJob->_actionType != EGsAIActionType::LOOT_ITEM  && firstJob->_actionType != EGsAIActionType::LOOT_ITEM_AUTO_OFF)
	|| firstJob->_targetObj == nullptr)
	{
		return false;
	}
	UGsGameObjectDropObject* castDropObject = Cast<UGsGameObjectDropObject>(firstJob->_targetObj);
	if (castDropObject == nullptr)
	{
		return false;
	}
	bool isAutoLootWeightPossible = FGsOptionFunc::IsPossibleLootByWeightOption(castDropObject->GetItemWeight());

	UGsItemManager* itemManager = GItem();
	if (itemManager == nullptr)
	{
		return false;
	}

	// check slot
	bool isAutoLootInvenSlotPossible = 
		itemManager->IsCanItemAdd(castDropObject->GetItemId(), castDropObject->GetItemAmount());

	// https://jira.com2us.com/jira/browse/C2URWQ-2788
	// check grade
	bool isAutoLootGradePossible = false;
	const FGsSchemaItemCommon* itemTable = castDropObject->GetItemTable();
	if (itemTable != nullptr)
	{
		isAutoLootGradePossible = FGsOptionFunc::IsPossibleLootByGradeOption(itemTable->type, itemTable->grade);
	}

	// check server lock
	bool isServerLock =
		_localPlayer->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT);

	bool isLootAble = (castDropObject->GetState() == EDropObjectState::Lootable)? true : false;
	
	return !isAutoLootWeightPossible || !isAutoLootInvenSlotPossible ||!isAutoLootGradePossible || 
	isServerLock ||
	!isLootAble;
}
// check dialog play
bool FGsAIContentsConditionDefault::IsDialogPlay()
{
	bool isCamPlay = GSCameraMode()->GetIsCameraDialogPlay();
	return isCamPlay;
}
// check input block
bool FGsAIContentsConditionDefault::IsInputBlock()
{
	if (_localPlayer == nullptr)
	{
		return false;
	}
	
	if (UGsInputBindingLocalPlayer* inputBinder = _localPlayer->GetInputBinder())
	{
		// 인풋이 막혀있으면 누르지 않음
		if (inputBinder->IsInputBlock(EGsKeyMappingType::TOUCH_MOVE))
		{
			return true;
		}
	}
	
	return false;
}

// exist reserve looting data(proceeding looting)
bool FGsAIContentsConditionDefault::IsReserveLooting()
{
	if (_localPlayer == nullptr)
	{
		return false;
	}

	bool isReserveLooting = false;	
	if (FGsLootingHandlerLocalPlayer* lootingHandler = _localPlayer->GetLootingHandler())
	{
		if (lootingHandler->IsReserveLootng())
		{
			isReserveLooting = true;
		}
	}
	
	return isReserveLooting;
}

// check move type: auto return move
bool FGsAIContentsConditionDefault::IsAutoReturnMove()
{
	if (_localPlayer == nullptr)
	{
		return false;
	}

	FGsMovementHandlerLocalPlayer* movement = _localPlayer->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	EGsMovementAutoContentsType nowAutoContentsType = movement->GetStateAutoMove().GetMovementAutoContentsType();

	return (nowAutoContentsType == EGsMovementAutoContentsType::AutoReturnMove) ? true : false;
}

// check auto move
bool FGsAIContentsConditionDefault::IsGameObjFSMAutoMove()
{
	if (_localPlayer == nullptr)
	{
		return false;
	}
	return _localPlayer->GetBaseFSM()->IsState(EGsStateBase::AutoMove);
}
// check quest center move
bool FGsAIContentsConditionDefault::IsQuestCenterMove()
{
	FGsAIManager* aiMgr = GSAI();
	if (aiMgr == nullptr)
	{
		return false;
	}

	bool isCenterMove = aiMgr->GetAIQuestHandler()->IsQuestKillCenterMove();
	return isCenterMove;
}

// check stay long time
bool FGsAIContentsConditionDefault::IsStayLongTime()
{
	if (_localPlayer == nullptr)
	{
		return false;
	}

	FGsAIReserveHandler* aiReserveHandler = _localPlayer->GetAIReserve();
	if (aiReserveHandler == nullptr)
	{
		return false;
	}

	return aiReserveHandler->GetIsDirtyReserveRetry();
}

// check first job == retry
bool FGsAIContentsConditionDefault::IsFirstJobRetry()
{
	if (_localPlayer == nullptr)
	{
		return false;
	}

	FGsAIReserveHandler* aiReserveHandler = _localPlayer->GetAIReserve();
	if (aiReserveHandler == nullptr)
	{
		return false;
	}

	FGsAIReserveJob* reserveJob = aiReserveHandler->GetFirstJob();

	if (nullptr == reserveJob)
	{
		return false;
	}

	return (reserveJob->_actionType == EGsAIActionType::AUTO_MOVE_STOP_WITH_RESERVE_RETRY) ? true : false;
}


// check: server move lock
bool FGsAIContentsConditionDefault::IsServerMoveLock()
{
	// check server lock
	bool isServerLock =
		_localPlayer->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT);
	return isServerLock;
}

// check: target attackable
bool FGsAIContentsConditionDefault::IsAttackable()
{
	if (_localPlayer == nullptr)
	{
		return false;
	}

	FGsAIReserveHandler* aiReserveHandler = _localPlayer->GetAIReserve();
	if (aiReserveHandler == nullptr)
	{
		return false;
	}

	FGsAIReserveJob* reserveJob = aiReserveHandler->GetFirstJob();
	if (nullptr == reserveJob ||
		nullptr == reserveJob->_targetObj)
	{
		return false;
	}

	return	reserveJob->_targetObj->IsAble();
}
// check: 이미 중지 및 스킬 clear 액션이 등록되어있는지
bool FGsAIContentsConditionDefault::IsReservedActionAutoMoveStopWithClearReserveSkill()
{
	if (_localPlayer == nullptr)
	{
		return false;
	}

	FGsAIReserveHandler* aiReserveHandler = _localPlayer->GetAIReserve();
	if (aiReserveHandler == nullptr)
	{
		return false;
	}

	return aiReserveHandler->IsExistAction(EGsAIActionType::AUTO_MOVE_STOP_WITH_CLEAR_RESERVE_SKILL);
}
// https://jira.com2us.com/jira/browse/C2URWQ-2421
// 재연결후 ai에서 아이템 주으러 안가는 이슈
// 재연결 성공시
void FGsAIContentsConditionDefault::OnGameServerReconnectionSuccess()
{
	// clear
	SetIsLootServerResponseWait(false);
}
// 루팅 서버 send 보내고 응답 대기 할지
void FGsAIContentsConditionDefault::SetIsLootServerResponseWait(bool In_val)
{
	_isLootServerResponseWait = In_val;

	// 막는 처리 이슈가 많아서(해제가 안되는 상황: 서버 응답이 없거나 어디서 실패하는지 모를때)
	// 일정 시간후에 풀자
	//https://jira.com2us.com/jira/browse/CHR-24724
	if(_isLootServerResponseWait == true)
	{ 
		_lootServerResponseWaitStartTime = FDateTime::UtcNow().GetTicks();
	}
}