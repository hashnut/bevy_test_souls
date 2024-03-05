// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillRunnerBase.h"

#include "Data/GsResourceManager.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "DataSchema/Skill/GsSchemaSkillCommon.h"
#include "DataSchema/Skill/GsSchemaSkillReaction.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"

#include "GameObject/Skill/Notify/GsSkillNotifyNodeCollision.h"
#include "GameObject/Skill/Notify/GsSKillNotifyNodeProjectile.h"
#include "GameObject/Skill/Notify/GsSkillNotifyNodeGuideEffect.h"
#include "GameObject/Skill/Notify/GsSkillNotifyNodeBeamParticle.h"

#include "T1Project.h"
#include "Animation/AnimMontage.h"
#include "UTIL/GsTimeUtil.h"

FGsSkillRunnerBase::FGsSkillRunnerBase(TFunction<void(int, bool)> EndRun)
{
	_currentInfo = MakeShareable(new FGsSkillNotifyCache());
	_callbackEndRun = EndRun;
}

FGsSkillRunnerBase::~FGsSkillRunnerBase()
{
	ClearRunner();
}

void FGsSkillRunnerBase::UseSkillNode(int SkillNotifyId)
{
	if (_currentInfo.IsValid())
	{
		// 최소 검증
		TSharedPtr<FGsSkillNotifyNodeBase> NotifyInfo = _currentInfo.Get()->Pop();
		if (SkillNotifyId != NotifyInfo->GetSkillNotifyId())
		{
			GSLOG(Error, TEXT("UseSkillNode Failed! <Use SKillNotifyId %d  Currenut SKillNotifyId %d>")
				, SkillNotifyId, NotifyInfo->GetSkillNotifyId());
		}
		else
		{
			NotifyInfo->Action(_owner);
		}
	}
}

void FGsSkillRunnerBase::StartRunner(UGsGameObjectBase* Owner, const FGsSchemaSkillCommon* Data)
{
	ClearRunner();

	if (Data)
	{
		_commonData.SetData(Data);
		_currData = &_commonData;

		StartRunner(Owner);

		// Common Skill은 Cancel 설정이 따로 없다
		// 현재는 무조건 캔슬 가능 처리
		_cancelTime = 0.0001f;
	}
}

void FGsSkillRunnerBase::StartRunner(UGsGameObjectBase* Owner, const FGsSchemaSkillReaction* Data)
{
	ClearRunner();
}

void FGsSkillRunnerBase::StartRunner(UGsGameObjectBase* Owner, const FGsSchemaSkillSet* Data, float SpeedScale,
	const FGsSkillNetData* NetData)
{
	ClearRunner();

	if (NetData)
	{
		_skillNetData = NetData;
	}

	if (Data)
	{
		_skillData.SetData(Data);
		_currData = &_skillData;
		// applyAttackSpeed 설정값에 따라 공속이 반영여부 처리
		_speedScale = (Data->applyAttackSpeed) ? SpeedScale : 1.f;

		StartRunner(Owner);

		// cancel 타이밍 비율 계산 
		// 현재 스킬 타입(FGsSchemaSkillSet)만  존재
		// _offsetRate 설정은 StartRunner() 함수 내에서 처리 하고 있으므로 순서 유의
		float originRate = Data->cutRemainInfo.delayOffset / Data->totalRuntime;
		_cancelTime = (_totalRuntime * originRate);
	}	
}

void FGsSkillRunnerBase::JumpToRunner(class UGsGameObjectBase* Owner, float JumpOffset)
{
	_offsetTime = JumpOffset * _scaledOffsetRate;

	// 이전 Notify 정보들은 제거 
	if (_currentInfo.Get()->IsValid())
	{
		TArray<TSharedPtr<FGsSkillNotifyNodeBase>> actionNodes;
		_currentInfo.Get()->Pop(actionNodes, _offsetTime);
	}
}

void FGsSkillRunnerBase::StartRunner(UGsGameObjectBase* Owner)
{
	if (IsValid())
	{
		_scaledOffsetRate = 1.f / _speedScale;
		_totalRuntime = (_currData->GetTotalTime() * _scaledOffsetRate);

		_owner = Owner;
		_timer = FDateTime::Now();

// 		TArray<FGsSchemaSkillNotify> mergeNotify;
// 		if (const TArray<FGsSchemaSkillNotify>* notifyList = _currData->GetNotifyList())
// 		{
// 			mergeNotify = *_currData->GetNotifyList();
// 		}
// 
// 		// 클라이언트용 Notify 확인
// 		if (_currData->GetClientNotifyList() && _currData->GetClientNotifyList()->Num() > 0)
// 		{
// 			TArray<FGsSchemaSkillNotify> clientNotifyList(*_currData->GetClientNotifyList());
// 			clientNotifyList.RemoveAll([Owner](const FGsSchemaSkillNotify& el)
// 			{
// 				if (const FGsSchemaSkillNotifySet* notifySet = el.skillNotifyId.GetRow())
// 				{
// 					return notifySet->localPlayerOnly && !Owner->IsObjectType(EGsGameObjectType::LocalPlayer);
// 				}
// 				return true;
// 			});
// 			mergeNotify.Append(clientNotifyList);
// 		}
// 
// 		if (mergeNotify.Num() > 0)
// 		{
// 			SetNotifyNode(mergeNotify);
// 		}

		CreateRuntimeSkillNode();
	}
}

void FGsSkillRunnerBase::ClearRunner()
{
	_currentInfo.Get()->Clear();
	_commonData.Clear();
	_skillData.Clear();

	_owner			= nullptr;
	_currData		= nullptr;
	_skillNetData	= nullptr;
	_totalRuntime   = 0.f;
	_cancelTime		= 0.f;
	 _elapsedTime	= 0.f;
	_offsetTime		= 0.f;
	_speedScale		= 1.f;
	_scaledOffsetRate = 1.f;
	_timer			= FDateTime(0);	
}

bool FGsSkillRunnerBase::Update(float DeltaTime)
{
	if (IsValid())
	{	
		 _elapsedTime = (FDateTime::Now() - _timer).GetTotalSeconds() + _offsetTime;
		// 스킬의 전체시간이 만료되었는지 체크
		return !IsEnd();
	}

	return true;
}

TSharedPtr<FGsSkillNotifyNodeBase> FGsSkillRunnerBase::CreateSkillNode(const FGsSchemaSkillNotifySet* Data)
{
	switch (Data->type)
	{
	case SkillNotifyType::DAMAGE:
		return MakeShareable(new FGsSkillNotifyNodeCollision());
	case SkillNotifyType::FIRE_PROJECTILE:
		return MakeShareable(new FGsSKillNotifyNodeProjectile());
	case SkillNotifyType::SKILLGUIDE_EFFECT:
		return MakeShareable(new FGsSkillNotifyNodeGuideEffect());
	case SkillNotifyType::BEAM_PARTICLE_EFFECT:
		return MakeShareable(new FGsSkillNotifyNodeBeamParticle());
	}
	return nullptr;
}

void FGsSkillRunnerBase::EndRunner()
{
	if (IsValid())
	{
		int32 endSkillId = _currData->GetId();
		bool isEnd = IsEnd();

		ClearRunner();

		// 콜백 호출은 Data Clear이후에 진행 하며 필요 정보는 파라미터로 처리
		// 콜백을 받아 수행하는 곳에서 현재 데이터를 보는 경우가 있기에 순서에 유의
		if (_callbackEndRun != nullptr)
		{
			_callbackEndRun(endSkillId, isEnd);
			//_callbackEndRun = nullptr;
		}
	}
}

void FGsSkillRunnerBase::CancelRunner()
{
	// 남아 있는 SkillNode들에게 Noti
	if (_currentInfo.IsValid())
	{
		//TSharedRef<FGsSkillNotifyCache> notifys = _currentInfo.ToSharedRef();
		for (TSharedPtr<FGsSkillNotifyNodeBase> el : _currentInfo->_listNotify)
		{
			el->Cancel(_owner);
		}
	}
}

void FGsSkillRunnerBase::SetNotifyNode(const TArray<FGsSchemaSkillNotify>& listNotify)
{
	if (_currentInfo.IsValid())
	{
		_currentInfo->Clear();

		for (const FGsSchemaSkillNotify& el : listNotify)
		{
			if (const FGsSchemaSkillNotifySet* notifyData = el.skillNotifyId.GetRow())
			{
				if (TSharedPtr<FGsSkillNotifyNodeBase> findNode = CreateSkillNode(notifyData))
				{
					findNode->Initialize(_owner, _currData, &el, _skillNetData, _scaledOffsetRate);
					_currentInfo->_listNotify.Emplace(findNode);
				}
			}
		}		
	}
}

UAnimMontage* FGsSkillRunnerBase::GetAni() const
{
	if (IsValid())
	{
		// 현재는 스킬 정보를 미리 알수 없기 때문에 Direct 로드로 진행한다
		return GResource()->LoadSync<UAnimMontage>(_currData->GetPath());

	}
	return nullptr;
}

bool FGsSkillRunnerBase::IsValid() const
{
	return  nullptr != _currData;
}

float FGsSkillRunnerBase::GetElapsedTime() const
{
	return _elapsedTime;
	//return (FDateTime::Now() - _timer).GetTotalSeconds() + _offsetTime;
}

float FGsSkillRunnerBase::GetTotalRunTime() const
{
	return _totalRuntime;
}

float FGsSkillRunnerBase::GetCancelTime() const
{
	return _cancelTime;
}

bool FGsSkillRunnerBase::CanMove() const
{
	return IsValid() ? _currData->CanMove() : false;
}

bool FGsSkillRunnerBase::IsEnd()
{
	return IsValid() ? _totalRuntime <= GetElapsedTime() : true;
}


bool FGsSkillRunnerBase::CanCancel()
{
	if (IsValid())
	{
		if (_cancelTime > 0.f && _cancelTime <= GetElapsedTime())
		{
			return true;
		}

		if (_totalRuntime <= GetElapsedTime())
		{
			return true;
		}

		return false;
	}

	return true;
}

const FGsSchemaSkillSet* FGsSkillRunnerBase::GetSkillData() const
{
	if (_skillData.IsValid())
	{
		return _skillData.GetTableData();
	}
	return nullptr;
}

const FGsSkillNetData* FGsSkillRunnerBase::GetNetSkillData() const
{
	return _skillNetData;
}

float FGsSkillRunnerBase::GetSpeedScale() const
{
	return _speedScale;
}