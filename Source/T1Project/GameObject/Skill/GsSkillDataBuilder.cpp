// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillDataBuilder.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"

#include "Skill/GsSkill.h"

#include "Management/GsMessageHolder.h"

#include "Gameobject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"

#include "Checker/GsSkillCheckArray.h"
#include "Checker/GsSkillCheckSyncUserData.h"
#include "Checker/GsSkillCheckTarget.h"
#include "Checker/GsSkillCheckAbnormality.h"
#include "Checker/GsSkillCheckState.h"
#include "Checker/GsSkillCheckSame.h"
#include "Checker/GsSkillCheckCoolTime.h"
#include "Checker/GsSkillCheckCancel.h"
#include "Checker/GsSkillCheckRequireCost.h"
#include "Checker/GsSkillCheckStageMode.h"
#include "Checker/GsSkillCheckSlotEnable.h"
#include "Checker/GsSkillManualTargetRange.h"

#include "GsSkillHandlerLocalPlayer.h"


void FGsUseSkillRequest::Clear()
{
	_skillData		= nullptr;
	_targetGameId	= 0;
	_deltaTime		= 0.f;
}

void FGsCastingSkillRequest::Clear()
{
	_castingSkillId	 = 0;
	_castingTimer	 = FDateTime(0);
	_castingComplete = false;
}

void FGsSkillReserve::Clear()
{
	_skillData = nullptr;
	_attackRange = 0.f;
}

void FGsSkillDataBuilder::Initialize(UGsGameObjectLocalPlayer* Owner, FGsSkillHandlerLocalPlayer* SkillHandler)
{
	// 스킬 Checker 등록
	// 순차 검사이기 때문에 Add순서가 중요하다.
	_skillCheck.Add(new FGsSkillCheckSyncUserData());
	_skillCheck.Add(new FGsSkillCheckSlotEnable());
	_skillCheck.Add(new FGsSkillCheckTarget(Owner, this));
	_skillCheck.Add(new FGsSkillManualTargetRange(Owner, Owner->GetTargetHandler()));
	_skillCheck.Add(new FGsSkillCheckAbnormality());
	_skillCheck.Add(new FGsSkillCheckState());
	_skillCheck.Add(new FGsSkillCheckSame(SkillHandler, Owner->GetBaseFSM()));
	_skillCheck.Add(new FGsSkillCheckCoolTime());
	_skillCheck.Add(new FGsSkillCheckCancel(SkillHandler));
	_skillCheck.Initialize();

	_owner = Owner;
}

void FGsSkillDataBuilder::Finalize()
{
	ClearReserveData();
	ClearRequestData();
	ClearCastingData();
	_checkTarget = nullptr;
	_owner = nullptr;

	// 등록된 스킬 체커 제거
	_skillCheck.RemoveAll();
	_skillCheck.Finalize();
}

void FGsSkillDataBuilder::ClearReserveData()
{
	// 유효한 데이터 제거경우에만 보내기
	if (_reserveData.IsVaild())
	{
		GMessage()->GetSkillRserveActiveState().SendMessage(
			MessageContentHud::SKILL_RESERVE_OFF, _reserveData._skillData->GetSkillId());
	}

	_reserveData.Clear();
}

void FGsSkillDataBuilder::ClearRequestData()
{

	// FGsUseSkillRequest 구조체 정보 초기화
	_requestData.Clear();

	// Delegate연결은 Unbind하지 않는다.
}

void FGsSkillDataBuilder::ClearCastingData()
{
	_requestCastingData.Clear();
}

void FGsSkillDataBuilder::SetRequestData(const FGsSkill* SkillData)
{
	_requestData._skillData = SkillData;
}

void FGsSkillDataBuilder::SetReserveData(const FGsSkill* SkillData, float AttackRange)
{
	ClearReserveData();

	_reserveData._skillData = SkillData;
	_reserveData._attackRange = AttackRange;
	// 이벤트 전송
	GMessage()->GetSkillRserveActiveState().SendMessage(
		MessageContentHud::SKILL_RESERVE_ON, _reserveData._skillData->GetSkillId());
}

void FGsSkillDataBuilder::SetCastingData(int SkillId, bool Complete)
{
	_requestCastingData._castingSkillId = SkillId;
	_requestCastingData._castingComplete = Complete;
	_requestCastingData._castingTimer = (Complete) ? FDateTime(0) : FDateTime::Now();
}

void FGsSkillDataBuilder::SetRequestDataRestoreTime(float ElapsedTime)
{
	_requestData._elapsedTime = ElapsedTime;
}

EGsSkillCheckResultType FGsSkillDataBuilder::BuildValidateData(const FGsSkill* SkillData)
{
	// 스킬 사용 가능 여부 판단
	return _skillCheck.ProcessCheck(SkillData);
}

UGsGameObjectBase* FGsSkillDataBuilder::BuildTargetData(const FGsSkill* SkillData)
{
	_checkTarget = nullptr;

	FGsTargetHandlerLocalPlayer* targetHandler = _owner->GetCastTarget< FGsTargetHandlerLocalPlayer>();
	UGsGameObjectBase* target = targetHandler->GetTarget();
	
	// 강제 지정한 타겟일때는 무조건 그 타겟으로 설정
	UGsGameObjectBase* skillSelectTarget = targetHandler->GetSkillSelectTarget();
	if (skillSelectTarget && SkillData->IsSelectTarget())
	{
		_checkTarget = skillSelectTarget;
	}
	// 논타겟에 타겟을 못찾을경우 나자신을 설정
	else if (SkillData->_tableData->skillTargetMethod == SkillTargetMethod::NON_TARGET && nullptr == target)
	{
		_checkTarget = _owner;
	}
	// 자신 스킬을 먼저 검사
	else if (FGsSkillHandlerLocalPlayer::IsMySelfSkill(SkillData->_tableData))
	{
		_checkTarget = _owner;
	}
	// TargetFilter 대상
	else if (_owner->CanTarget(target))
	{		
		_checkTarget = target;
	}
	else
	{
		// 확인이 필요한 상황
		GSLOG(Warning, TEXT("target is not valid!"));
	}
	
	return _checkTarget;
}

float FGsSkillDataBuilder::BuildApproachRangeData(const FGsSkill* SkillData, bool IsMoveInput, bool IsSkillInput)
{
	if (IsMoveInput && IsSkillInput)
	{
		return 0.f;
	}
	return FGsSkillHandlerBase::GetSkillApproachRange(_owner, SkillData->_tableData);
}

bool FGsSkillDataBuilder::UpdateRequestData(float Delta)
{
	if (_requestData.IsVaild())
	{
		_requestData._deltaTime += Delta;
		return _requestData._deltaTime > _requestData._elapsedTime;
	}
	return false;
}