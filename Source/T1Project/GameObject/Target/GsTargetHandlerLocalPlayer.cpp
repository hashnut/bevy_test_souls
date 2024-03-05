// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTargetHandlerLocalPlayer.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"

#include "Data/GsDataContainManager.h"

#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Target/AutoTarget/GsTargetRule.h"
#include "GameObject/Data/GsGameObjectData.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsQuestManager.h"

#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Message/GsMessageContentHud.h"
#include "Message/MessageParam/GsAIMessageParam.h"

#include "Option/GsServerOption.h"

#include "AI/ContentsCondition/GsAIContentsConditionAutoReturn.h"

#include "Party/GsPartyMemberInfo.h"

#include "UI/UIContent/Billboard/GsUIBillboardCreature.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UTIL/GsGameObjectUtil.h"

#include "Scan/GsScanMemberInfo.h"


void FGsTargetHandlerLocalPlayer::Initialize(UGsGameObjectBase* owner)
{
	Super::Initialize(owner);

	_messageParam._paramOwner = owner;
	if (nullptr == _targetHudInfo)
	{
		_targetHudInfo = new TargetHUD();
	}

	if (nullptr == _targetRule)
	{
		_targetRule = new FGsTargetRule();
		_targetRule->Initialize(owner->CastGameObject<UGsGameObjectLocalPlayer>());
	}

	FGsMessageHolder* msg = GMessage();
	
	InitializeMessageDelegateList(&msg->GetGameObject(), &msg->GetInput());
	AddMessageDelegate(msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_HIT,
		this, &FGsTargetHandlerLocalPlayer::CallbackLoacalHit));

	AddMessageDelegate(msg->GetGameObject().AddRaw(MessageGameObject::COMMON_DIE,
		this, &FGsTargetHandlerLocalPlayer::CallbackCheckDespawnTarget));
	AddMessageDelegate(msg->GetGameObject().AddRaw(MessageGameObject::COMMON_DESPAWN,
		this, &FGsTargetHandlerLocalPlayer::CallbackCheckDespawnTarget));
	AddMessageDelegate(msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_TARGET_CLEAR_FORCE,
		this, &FGsTargetHandlerLocalPlayer::CallbackTargetClear));

	AddMessageDelegate(msg->GetGameObject().AddRaw(MessageGameObject::AI_MODE_ACTIVE,
			this, &FGsTargetHandlerLocalPlayer::CallbackAIModeActive));

	AddMessageDelegate(msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_INPUT_TARGET_CHANGE,
		this, &FGsTargetHandlerLocalPlayer::CallbackTargetChangeInput));
	AddMessageDelegate(msg->GetGameObject().AddRaw(MessageGameObject::UPDATE_SCAN_INFO,
		this, &FGsTargetHandlerLocalPlayer::CallbackUpdateScanInfo));
}

void FGsTargetHandlerLocalPlayer::Finalize()
{
	Super::Finalize();

	ClearTarget(true);
	ClearAttackerTarget();
	ClearManualModeAutoTarget();
	ClearSkillSelectTarget();

	if (_targetHudInfo)
	{
		delete _targetHudInfo;
		_targetHudInfo = nullptr;
	}

	if (_targetRule)
	{
		_targetRule->Finalize();
		delete _targetRule;
		_targetRule = nullptr;
	}

	FinalizeMessageDelegateList();
}

void FGsTargetHandlerLocalPlayer::Update(float Delta)
{
	Super::Update(Delta);

	if (_startTime > 0.f)
	{
		float elapsedtime = GetElapsed();
		if (elapsedtime > 1.f)
		{
			_autoUserTargetClearTime -= elapsedtime;
			UpdateTargetRange();
		}
	}
}

UGsGameObjectBase* FGsTargetHandlerLocalPlayer::OnTarget(FGsTargetRule::EGsTargetRuleType TargetRuleType, bool isRotate,
	SelectTargetFilter selectFilter, IffApplyType IffType)
{
	// 타겟 처리에 대한 예외검사 IffApplyType::ENEMY 가 아니라면 지금은 나의 위치를 기준으로 잡는다
	if (IffType == IffApplyType::ENEMY)
	{
		if (_targetRule)
		{
			if (UGsGameObjectBase* target = _targetRule->OnAutoTargetAction(TargetRuleType))
			{
				SetTarget(target, isRotate, selectFilter);
				return target;
			}
		}
	}

	return _owner;
}

bool FGsTargetHandlerLocalPlayer::SetTarget(UGsGameObjectBase* target, bool isRotate,
	SelectTargetFilter selectFilter)
{
	// 자기 자신은 타겟이 될수 없음
	if (target == _owner)
	{
		GSLOG(Error, TEXT("[Target Debug] target is localPlayer"));
		return false;
	}

	// 유효한 객체인가
	if (target && false == target->IsAble())
	{
		GSLOG(Error, TEXT("[Target Debug] target is not able"));
		return false;
	}
	
	if (target == _target)
	{
		// 타겟 UI는 갱신이 필요하다...
		UpdateTargetUI(target, selectFilter);

		if (selectFilter != SelectTargetFilter::TargetChange)
		{
			return true;
		}
	}

	// 내가 타겟 변경이 불가능할때는 변경할수 없음
	if (selectFilter != FGsTargetHandlerBase::SelectTargetFilter::Abnormality_Taunt &&
		_owner->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_CHANGE_TARGET))
	{
		GSLOG(Error, TEXT("[Target Debug] CreatureActionType::CREATURE_ACTION_TYPE_CHANGE_TARGET!"));
		return false;
	}

	// 자동 사냥 모드
	bool isAutoMode = false;
	FGsAIManager* aiManager = GSAI();
	if (nullptr != aiManager)
	{
		isAutoMode = aiManager->IsAIOn();
	}

	bool isSecondTargetSetting = false;

	// 2차 타겟과 동일한가
	if (target == _secondTarget)
	{
		if (selectFilter != SelectTargetFilter::TargetChange)
		{
			_target = target;
			_secondTarget = nullptr;
		}
	}
	// 새로운 타겟인가
	else
	{
		UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(_owner);
		// 타겟 가능 검사
		if (false == localPlayer->CanTarget(target))
		{
			// 들어오면 안될것도 같은데?
			GSLOG(Error, TEXT("[Target Debug] Can't Select Target object! %s"), *target->GetData()->GetName());
			return false;
		}

		// 2차 타겟 선정
		_selectFilter = selectFilter;

		// 적대 관계 확인		
		bool isEnemy = localPlayer->IsEnemy(target);
		
		// 2차 타겟 선정 대상 처리
		switch (selectFilter)
		{
		case SelectTargetFilter::Auto:
			_target = target;
			break;
		case SelectTargetFilter::ScanList:
			// 아군 = 2차 
			if (false == isEnemy)
			{
				_secondTarget = target;
				isSecondTargetSetting = true;
			}
			else
			{
				_target = target;
			}
			break;
		case SelectTargetFilter::SelectTarget:
			// 셀렉 타겟팅 + 1차 없음 + 적대 + Auto On = 1차 그외는 전부 2차
			if (false == (nullptr == _target && isEnemy && isAutoMode))
			{
				_secondTarget = target;
				isSecondTargetSetting = true;
			}
			else
			{
				_target = target;
			}
			break;
		case SelectTargetFilter::TargetChange:
		{
			if (isAutoMode && isEnemy)
			{
				_target = target;
				_secondTarget = nullptr;
			}
			else
			{
				_secondTarget = target;
				isSecondTargetSetting = true;
			}
			break;
		}
		default:
			_target = target;
			break;
		}
	}	

	// 최종 타겟 설정
	UpdateTargetUI(target, selectFilter);
	UpdateTargetEffect();

	// 클리어 시간 갱신
	if (GSAI())
	{
		if (target == _secondTarget || false == GSAI()->IsAIOn())
		{
			// 2차 타겟을 새롭게 잡은 상황이면 다음 1차타겟이 변경되기 전까지 타이머 처리 없음
			_startTime = -1.f;
		}
		else
		{
			_startTime = FApp::GetCurrentTime();
			_autoUserTargetClearTime = GData()->GetGlobalData()->AutoUserTargetClear;
		}
	}

	// B1: Targeting other than automatic sends a message to the ai reservation handler to rank it first.
	// himichani
	// https://jira.com2us.com/jira/browse/CHR-17016


	// 자동on일때 수동으로 클릭했을때만 ai에 예약 리스트에 넣자
	// https://jira.com2us.com/jira/browse/CHR-24592

	if (
		(selectFilter == FGsTargetHandlerBase::LockOn ||
		selectFilter == FGsTargetHandlerBase::ScanList ||
		selectFilter == FGsTargetHandlerBase::SelectTarget)	&&
		isSecondTargetSetting == false &&
		_target != nullptr &&
		isAutoMode == true)
	{
		FGsAIReserveParam aiReserveAddParam(EGsAIActionType::ATTACK_ANYONE, _target.Get(), 0);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_ADD_RESERVE_JOB, &aiReserveAddParam);
	}

	return true;
}

void FGsTargetHandlerLocalPlayer::UpdateTargetUI(UGsGameObjectBase* InTarget,
	SelectTargetFilter InTargetFilterType)
{
	// 새로운 타겟 정보 갱신
	UGsGameObjectCreature* castTarget = InTarget->CastGameObject<UGsGameObjectCreature>();
	if (castTarget)
	{
		// 1차 타겟이 있고 ManualModeAutoTarget이 변경되었다면 Clear
		if (_target.IsValid() && _target->GetGameId() != _manualModeAutoTarget)
		{
			// 자동 사냥 모드가 아닌 상태면 다시 설정
			if (InTargetFilterType != FGsTargetHandlerBase::Auto)
			{
				_manualModeAutoTarget = _target->GetGameId();
			}
			else
			{
				ClearManualModeAutoTarget();
			}
		}

		_cacheLastTarget = InTarget;

		// https://jira.com2us.com/jira/browse/C2URWQ-4269
		bool isShowTargetUi = false;
		if (InTargetFilterType == SelectTargetFilter::LockOn ||
			InTargetFilterType == SelectTargetFilter::ScanList ||
			InTargetFilterType == SelectTargetFilter::SelectTarget)
		{
			isShowTargetUi = true;
		}
		// 특별 몬스터
// 		else if (InTarget->IsObjectType(EGsGameObjectType::NonPlayer))
// 		{
// 			UGsGameObjectNonPlayer* nonPlayer = InTarget->CastGameObject<UGsGameObjectNonPlayer>();
// 			if (const FGsSchemaNpcData* npcTable = nonPlayer->GetNpcData())
// 			{
// 				isShowTargetUi = npcTable->grade >= CreatureRankType::NAMED;
// 			}
// 		}

		if(isShowTargetUi)
		{
			SetEnemyInfoHudUI(InTarget);
		}

		// 이벤트 전송
		_messageParam._paramTarget = InTarget;
		_messageParam._isFirstTarget = (InTarget == GetTarget());
		_messageParam._targetFilterType = InTargetFilterType;
		GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_TARGET_CHANGED,
			&_messageParam);		
	}
}

void FGsTargetHandlerLocalPlayer::ShowTargetEffect(class UGsGameObjectBase* InTarget, bool InVisible,
	bool InIsFirst)
{
	if (nullptr == InTarget)
	{
		return;
	}

	UGsGameObjectCreature* castTarget = InTarget->CastGameObject<UGsGameObjectCreature>();
	if (nullptr != castTarget)
	{
		bool isEnemy = _owner->CastGameObject<UGsGameObjectLocalPlayer>()->IsEnemy(InTarget);
		castTarget->UpdateByTarget(InVisible, InVisible, InIsFirst, isEnemy);
	}
}

void FGsTargetHandlerLocalPlayer::UpdateTargetEffect()
{
	// 클리어 타겟 처리
	if (nullptr != _prevTarget && _target != _prevTarget)
	{
		ShowTargetEffect(_prevTarget, false, true);
	}

	if (nullptr != _prevSecondTarget && _secondTarget != _prevSecondTarget)
	{
		ShowTargetEffect(_prevSecondTarget, false, true);
	}

	_prevTarget = nullptr;
	_prevSecondTarget = nullptr;
	// 새로운 타겟 UI정보 갱신
	if (_target.IsValid())
	{
		ShowTargetEffect(_target.Get(), true, true);

		_prevTarget = _target.Get();
	}

	if (_secondTarget)
	{
		ShowTargetEffect(_secondTarget, true, false);

		_prevSecondTarget = _secondTarget;
	}
}

void FGsTargetHandlerLocalPlayer::SetAttacker(UGsGameObjectBase* Target)
{
	// 현재 타겟 대상이 아닌경우만 설정
	if (Target && _target != Target)
	{
		ClearAttackerTarget(0);
		if (Target->IsObjectType(EGsGameObjectType::Player))
		{
			_attackerPc = Target->GetGameId();
		}
		else
		{
			_attackerNpc = Target->GetGameId();

			OnCounterAttack(Target);
		}
	}
}

void FGsTargetHandlerLocalPlayer::SetSkillSelectTarget(UGsGameObjectBase* InTarget)
{
	// 현재 타겟 대상이 아닌경우만 설정
	if (InTarget && InTarget != _target)
	{
		_skillSelectTarget = InTarget->GetGameId();
	}
}

void FGsTargetHandlerLocalPlayer::ClearTarget(bool isAllClear)
{
	FGsAIReserveParam aiReserveClearParam(EGsAIActionType::ATTACK_ANYONE, _target.Get());
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_CLEAR_RESERVE_JOB, &aiReserveClearParam);

	//GSLOG(Log, TEXT("FGsTargetLocal::ClearTarget"));
	if (isAllClear)
	{
		// 2차 타겟부터 클리어
		ClearTarget(_secondTarget);
	}
	ClearTarget(_target.Get());
}

void FGsTargetHandlerLocalPlayer::ClearTarget(UGsGameObjectBase* target)
{
	if (nullptr == target)
	{
		return;
	}

	UGsGameObjectBase* clearTarget = nullptr;
	UGsGameObjectBase* updateTarget = nullptr;
	if (_secondTarget == target)
	{
		clearTarget = _secondTarget;
		_secondTarget = nullptr;

		if (_target.Get())
		{
			updateTarget = _target.Get();
		}
	}
	else if (_target == target)
	{
		clearTarget = _target.Get();
		_target = nullptr;
		
		if (_secondTarget)
		{
			// 2차 타겟이 있고 아군이라면 티커 출력
			if (false == _owner->CastGameObject<UGsGameObjectLocalPlayer>()->IsEnemy(_secondTarget))
			{
				FText findText;
				FText::FindText(TEXT("UICommonText"), TEXT("Notice_UserAttack"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
			}

			updateTarget = _secondTarget;
		}
	}
	
	if (clearTarget)
	{
		if (_cacheLastTarget == clearTarget)
		{
			_cacheLastTarget = nullptr;
		}

		if (_widgetTarget == clearTarget)
		{
			_widgetTarget = nullptr;
		}

		// Lockon 정보 클리어
		if (_manualModeAutoTarget == clearTarget->GetGameId())
		{
			ClearManualModeAutoTarget();
		}

		FGsGameObjectMessageParam param(clearTarget);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_TARGET_CLEAR, &param);
		if (nullptr != _targetHudInfo)
		{
			GMessage()->GetEnemyHudInfo().SendMessage(MessageContentHud::DEACTIVE_ENEMYINFO, *_targetHudInfo);
		}

		if (nullptr == _target && nullptr == _secondTarget)
		{
			_startTime = -1.f;
		}
	}

	if (updateTarget)
	{
		UpdateTargetUI(updateTarget, SelectTargetFilter::None);
	}

	// 디스 플레이 갱신
	UpdateTargetEffect();
}

void FGsTargetHandlerLocalPlayer::ClearAttackerTarget(int64 InGameId)
{
	if (InGameId == 0)
	{
		_attackerNpc = 0;
		_attackerPc = 0;
	}
	else if (InGameId == _attackerNpc)
	{
		_attackerNpc = 0;
	}
	else if (InGameId == _attackerPc)
	{
		_attackerPc = 0;
	}
}

void FGsTargetHandlerLocalPlayer::ClearSkillSelectTarget(int64 InGameId)
{
	if (InGameId == 0 || _skillSelectTarget == InGameId)
	{
		_skillSelectTarget = 0;
	}
}

void FGsTargetHandlerLocalPlayer::ClearManualModeAutoTarget()
{
	_manualModeAutoTarget = 0;
}

void FGsTargetHandlerLocalPlayer::OnCounterAttack(UGsGameObjectBase* Target)
{
	if (GServerOption() && GSAI() && GSQuest())
	{
		// 반격 옵션 확인
		// 퀘스트 자동이동중에는 반격 안하게 처리
		// https://jira.com2us.com/jira/browse/CHR-20623
		if (GServerOption()->IsCounterAttackOn() && false == GSAI()->IsAIOn() && nullptr == _target &&
			false == GSQuest()->IsAutoProgressing() )
		{
			// 타겟이 없을때만 공격
			SetTarget(Target, true, FGsTargetHandlerBase::LockOn);

			UGsGameObjectLocalPlayer* local = _owner->CastGameObject<UGsGameObjectLocalPlayer>();
			FGsSkillHandlerLocalPlayer* skillHandler = local->GetCastSkill<FGsSkillHandlerLocalPlayer>();
			const FGsSchemaSkillSet* nomalSkill = skillHandler->GetFirstComboData();
			skillHandler->OnSkill(nomalSkill->id);
		}
	}
}

void FGsTargetHandlerLocalPlayer::UpdateTargetRange()
{
	if (nullptr == _target && nullptr == _secondTarget)
	{
		if (_cacheLastTarget)
		{
			// 검증용
			GSLOG(Error, TEXT("Target Is Not Clear!!"));
			ClearTarget(true);
		}
		return;
	}
	
	// 유저타겟팅 클리어
	// 2차 타겟일 경우에만 해당된다.
	if (_secondTarget)
	{
		if (_secondTarget->IsObjectType(EGsGameObjectType::Player) && _autoUserTargetClearTime < 0.f)
		{
			ClearTarget(_secondTarget);
			return;
		}
	}

	if (nullptr == _cacheLastTarget)
	{
		return;
	}

	// ClearTarget이 호출되기 전에 시간을 갱신
	_startTime = FApp::GetCurrentTime();
}

void FGsTargetHandlerLocalPlayer::CallbackLoacalHit(const IGsMessageParam* inParam)
{
	const FGsGameObjectMessageParamTarget* castParam = inParam->Cast<const FGsGameObjectMessageParamTarget>();
	SetAttacker(castParam->_paramTarget);
}

void FGsTargetHandlerLocalPlayer::CallbackCheckDespawnTarget(const IGsMessageParam* inParam)
{
	// UGsGameObjectBase 에서 대상 소멸시 타겟 Clear에 대한 처리는 진행 하고 있다.
	// 이곳은 _lastAttacker 정보에 대한 Clear 처리
	const FGsGameObjectMessageParam* castParam = inParam->Cast<const FGsGameObjectMessageParam>();
	UGsGameObjectBase* paramObject = castParam->_paramOwner;
	ClearAttackerTarget(paramObject->GetGameId());
	ClearSkillSelectTarget(paramObject->GetGameId());

	if (paramObject == GetManualModeAutoTarget())
	{
		ClearManualModeAutoTarget();
	}
}

void FGsTargetHandlerLocalPlayer::CallbackTargetClear(const IGsMessageParam* inParam)
{
	ClearTarget(true);
	ClearSkillSelectTarget();
}

void FGsTargetHandlerLocalPlayer::CallbackAIModeActive(const IGsMessageParam* Param)
{
	const FGsPrimitiveInt32* castParam = Param->Cast<const FGsPrimitiveInt32>();
	if (castParam->_data != 0)
	{
		ClearManualModeAutoTarget();
	}
}

void FGsTargetHandlerLocalPlayer::CallbackTargetChangeInput(const IGsMessageParam* Param)
{
	UGsGameObjectManager* gameObjectManager = GSGameObject();
	if (nullptr == gameObjectManager)
	{
		return;
	}

	TMap<int64, FGsScanMemberInfo*> mapScanMemberInfo = gameObjectManager->GetScanInfo();
	// Scan 활성화된 상태
	if (mapScanMemberInfo.Num() > 0)
	{
		int64 gameId = 0;
		int32 scanMemberMaxCount = GData()->GetGlobalData()->_scanMemberCount;
		for (int32 i = _targetChangeIndex + 1;i <= scanMemberMaxCount;++i)
		{
			gameId = FindScanMemberGameId(i);
			if (gameId != 0)
				break;
		}
		for (int32 i = 1;i <= _targetChangeIndex;++i)
		{
			if (gameId != 0)
				break;

			gameId = FindScanMemberGameId(i);
		}
		
		// 타겟 변경 요청이 들어왔지만 활성화된 타겟이 존재하지 않을 경우 초기화
		_targetChangeIndex = 0;

		if (gameId != 0)
		{
			FGsScanMemberInfo* nextScanMemberInfo = mapScanMemberInfo[gameId];
			_targetChangeIndex = nextScanMemberInfo->GetOrderNumber();

			SetTarget(nextScanMemberInfo->GetTargetObject().Get(), false, SelectTargetFilter::TargetChange);
		}
	}
	else
	{
		UGsGameObjectBase* secondTarget = _secondTarget;
		UGsGameObjectBase* lastTarget = _target.Get();
		UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(_owner);
		UGsGameObjectBase* nearestCreture = GSGameObject()->FindObjectNearestConditionFunc(FVector2D(_owner->GetActor()->GetActorLocation()), EGsGameObjectType::Creature, [localPlayer, secondTarget, lastTarget](UGsGameObjectBase* InGameObject)
			{
				if (InGameObject->IsZeroHP())
					return false;

				if (false == InGameObject->IsAble())
					return false;

				if (lastTarget == InGameObject)
					return false;

				if (secondTarget == InGameObject)
					return false;

				if (localPlayer == InGameObject)
					return false;

				return localPlayer->IsEnemy(InGameObject);
			});
		
		if (nearestCreture)
		{
			SetTarget(nearestCreture, false, SelectTargetFilter::TargetChange);
		}
	}
}

void FGsTargetHandlerLocalPlayer::CallbackUpdateScanInfo(const IGsMessageParam* Param)
{
	UGsGameObjectManager* gameObjectManager = GSGameObject();
	if (nullptr == gameObjectManager)
		return;

	// 스캔 정보 변경 시 첫번째 순번 or 1차 타겟 순번으로 초기화
	_targetChangeIndex = 0;

	if (nullptr == _target)
		return;

	TMap<int64, FGsScanMemberInfo*> mapScanMemberInfo = gameObjectManager->GetScanInfo();
	if (mapScanMemberInfo.Contains(_target->GetGameId()))
	{
		if (FGsScanMemberInfo* memberInfo = mapScanMemberInfo[_target->GetGameId()])
		{
			_targetChangeIndex = memberInfo->GetOrderNumber();
		}
	}
}

void FGsTargetHandlerLocalPlayer::SetEnemyInfoHudUI(UGsGameObjectBase* InTarget)
{
	if (nullptr == InTarget)
		return;

	if (nullptr == _targetHudInfo)
		return;

	// 활성화 전에 기존 정보 정리
	GMessage()->GetEnemyHudInfo().SendMessage(MessageContentHud::DEACTIVE_ENEMYINFO,
		*_targetHudInfo);

	_widgetTarget = InTarget;

	EGsGameObjectType objectType = InTarget->GetObjectType();
	if (EGsGameObjectType::NonPlayer == objectType
		|| EGsGameObjectType::Sandbag == objectType
		|| EGsGameObjectType::Camp == objectType
		|| EGsGameObjectType::Sanctum == objectType)
	{
		UGsGameObjectNonPlayerBase* nonPlayer = Cast<UGsGameObjectNonPlayerBase>(InTarget);
		if (nullptr == nonPlayer)
		{
			return;
		}

		const FGsSchemaNpcData* data = nonPlayer->GetNpcData();
		if (nullptr == data)
			return;

		const FGsSchemaNpcBalance* balance = nonPlayer->GetNpcBalance();
		if (nullptr == balance)
			return;

		// boss : normal  /  level / name
		_targetHudInfo->ResetData();

		_targetHudInfo->_target = nonPlayer;
		_targetHudInfo->_rankType = data->grade;
		_targetHudInfo->_level = data->fixedLevel;
		_targetHudInfo->_name = nonPlayer->GetNpcName().ToString();
		_targetHudInfo->_lifeTime = nonPlayer->GetEndOfLifeTime();

		GMessage()->GetEnemyHudInfo().SendMessage(MessageContentHud::ACTIVE_ENEMYINFO,
			*_targetHudInfo);
	}
	else if (EGsGameObjectType::RemotePlayer == objectType)
	{
		UGsGameObjectRemotePlayer* remotePlayer = static_cast<UGsGameObjectRemotePlayer*>(InTarget);
		FGsGameObjectData* gameObjectData = remotePlayer->GetData();
		if (nullptr == gameObjectData)
			return;


		// boss : normal  /  level / name
		_targetHudInfo->ResetData();

		_targetHudInfo->_target = remotePlayer;
		_targetHudInfo->_rankType = CreatureRankType::NORMAL;
		_targetHudInfo->_level = gameObjectData->GetLevel();
		_targetHudInfo->_name = gameObjectData->GetName();

		GMessage()->GetEnemyHudInfo().SendMessage(MessageContentHud::ACTIVE_ENEMYINFO,
			*_targetHudInfo);
	}
	else
	{
		GSLOG(Error, TEXT("SetEnemyInfoHudUI not found ObjectType %s (%d)"), *InTarget->GetName(), (int)objectType);
		// check(false);
	}
}

UGsGameObjectBase* FGsTargetHandlerLocalPlayer::GetLastTarget() const
{
	return _cacheLastTarget;
}

UGsGameObjectBase* FGsTargetHandlerLocalPlayer::GetWidgetTarget() const
{
	return _widgetTarget;
}

UGsGameObjectBase* FGsTargetHandlerLocalPlayer::GetSecondTarget() const
{
	return _secondTarget;
}

UGsGameObjectBase* FGsTargetHandlerLocalPlayer::GetManualModeAutoTarget() const
{
#if WITH_EDITOR
	// 치트명령어를 통해 자동 공격설정을 제어
	if (_checkAutoTargetFlag)
	{
		return nullptr;
	}
#endif

	// 1차 타겟과 동일한지 확인
	if (_target.IsValid())
	{
		if (_target->GetGameId() == _manualModeAutoTarget)
		{
			return _target.Get();
		}
	}
	return nullptr;
}

UGsGameObjectBase* FGsTargetHandlerLocalPlayer::GetSkillSelectTarget() const
{
	if (_skillSelectTarget != 0)
	{
		return GSGameObject()->FindObject(EGsGameObjectType::Creature, _skillSelectTarget);
	}
	return nullptr;
}

float FGsTargetHandlerLocalPlayer::GetElapsed() const
{
	return FApp::GetCurrentTime() - _startTime;
}

// 관전 대상 정보 넣기
void FGsTargetHandlerLocalPlayer::SetObserveTarget(UGsGameObjectBase* InTarget)
{
	SetEnemyInfoHudUI(InTarget);
}

// observe target hud clear
void FGsTargetHandlerLocalPlayer::ClearObserveTarget()
{
	if (nullptr != _targetHudInfo)
	{
		if (_targetHudInfo->_target == _widgetTarget)
		{
			_widgetTarget = nullptr;
		}

		GMessage()->GetEnemyHudInfo().SendMessage(MessageContentHud::DEACTIVE_ENEMYINFO, *_targetHudInfo);
	}
}

bool FGsTargetHandlerLocalPlayer::SetPartySkillSelectTarget(int In_partySlotId)
{
	FGsPartyMemberInfo* findInfo = GSParty()->FindPartyMemberInfo(In_partySlotId);

	if (findInfo == nullptr)
	{
		return false;
	}
	UGsGameObjectBase* partyObject =
		GSGameObject()->FindObject(EGsGameObjectType::RemotePlayer, findInfo->GetGameId());

	if (partyObject == nullptr)
	{
		return false;
	}
	_skillSelectTarget = partyObject->GetGameId();
	return true;
}

void FGsTargetHandlerLocalPlayer::SetSelfSkillSelectTarget()
{
	// set target local 
	_skillSelectTarget = _owner->GetGameId();
}

bool FGsTargetHandlerLocalPlayer::SetLastTargetSelectTarget()
{
	if (_cacheLastTarget)
	{
		_skillSelectTarget = _cacheLastTarget->GetGameId();
		return true;
	}
	return false;
}

int64 FGsTargetHandlerLocalPlayer::FindScanMemberGameId(int32 InOrderNumber)
{
	TMap<int64, FGsScanMemberInfo*> mapScanMemberInfo = GSGameObject()->GetScanInfo();
	for (TPair<int64, FGsScanMemberInfo*>& scanMemberInfoPair : mapScanMemberInfo)
	{
		if (FGsScanMemberInfo* scanMemberInfo = scanMemberInfoPair.Value)
		{
			// 딤드된 정보는 무시한다.
			if (scanMemberInfo->GetIsDimmed())
				continue;

			if (InOrderNumber == scanMemberInfo->GetOrderNumber())
			{
				return scanMemberInfoPair.Key;
			}
		}
	}

	return 0;
}
