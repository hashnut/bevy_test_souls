// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/Damage/GsDamageHandlerBase.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

#include "Animation/AnimMontage.h"
#include "Animation/GsAnimInstanceState.h"
#include "Animation/AnimNotify/AnimNotifyState_HomingProjectile.h"
#include "ActorComponentEx/GsHommingProjectileComponent.h"
#include "ActorEx/GsCharacterBase.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotify.h"
#include "DataSchema/Skill/Client/GsSchemaClientSkillLevelInfo.h"
#include "DataSchema/Skill/Client/GsSchemaClientSkillLevelNotifyInfo.h"

#include "GameObject/Data/GsGameObjectData.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "T1Project.h"

#include "Skill/GsSkill.h"

#include "UTIL/GsTableUtil.h"

//-------------------------------------------------------------------------------------------------
// FGsDamageNotifyInfo
//-------------------------------------------------------------------------------------------------

int FGsDamageNotifyInfo::AllocCount = 0;
int FGsHealNotifyInfo::AllocCount = 0;

FGsDamageNotifyInfo::FGsDamageNotifyInfo()
{
	_packetBuffer = (uint8 *)new PD::SC::PKT_SC_SKILL_DAMAGED_READ;
	++AllocCount;
}

FGsDamageNotifyInfo::~FGsDamageNotifyInfo()
{
	SAFE_DELETE(_packetBuffer);
	--AllocCount;
}

bool FGsDamageNotifyInfo::operator==(const FGsDamageNotifyInfo& __Other) const
{
	if (false == FMath::IsNearlyZero(_offset - __Other._offset))
		return false;

	const PD::SC::PKT_SC_SKILL_DAMAGED_READ* R_packet = reinterpret_cast<const PD::SC::PKT_SC_SKILL_DAMAGED_READ*>(__Other.GetData());
	const PD::SC::PKT_SC_SKILL_DAMAGED_READ* l_packet = reinterpret_cast<const PD::SC::PKT_SC_SKILL_DAMAGED_READ*>(GetData());

	if (l_packet->mCasterId != R_packet->mCasterId)
		return false;
	if (l_packet->mTargetId != R_packet->mTargetId)
		return false;
	if (l_packet->mSkillId != R_packet->mSkillId)
		return false;
	if (l_packet->mSkillLevel != R_packet->mSkillLevel)
		return false;
	if (l_packet->mSkillNotifyId != R_packet->mSkillNotifyId)
		return false;
	if (l_packet->mMediumId != R_packet->mMediumId)
		return false;

	return true;
}

FGsHealNotifyInfo::FGsHealNotifyInfo()
{
	_packetBuffer = (uint8*)new PD::SC::PKT_SC_SKILL_HEALED_READ;
	++AllocCount;
}

FGsHealNotifyInfo::~FGsHealNotifyInfo()
{
	SAFE_DELETE(_packetBuffer);
	--AllocCount;
}

bool FGsHealNotifyInfo::operator==(const FGsHealNotifyInfo& __Other) const
{
	if (false == FMath::IsNearlyZero(_offset - __Other._offset))
		return false;

	const PD::SC::PKT_SC_SKILL_HEALED_READ* R_packet = reinterpret_cast<const PD::SC::PKT_SC_SKILL_HEALED_READ*>(__Other.GetData());
	const PD::SC::PKT_SC_SKILL_HEALED_READ* l_packet = reinterpret_cast<const PD::SC::PKT_SC_SKILL_HEALED_READ*>(GetData());

	if (l_packet->mCasterId != R_packet->mCasterId)
		return false;
	if (l_packet->mTargetId != R_packet->mTargetId)
		return false;
	if (l_packet->mSkillId != R_packet->mSkillId)
		return false;
	if (l_packet->mSkillLevel != R_packet->mSkillLevel)
		return false;
	if (l_packet->mHealByType != R_packet->mHealByType)
		return false;
	if (l_packet->mHpDeltaValue != R_packet->mHpDeltaValue)
		return false;
	if (l_packet->mSkillNotifyId != R_packet->mSkillNotifyId)
		return false;

	return true;
}

//-------------------------------------------------------------------------------------------------
// FGsDamageRunner
//-------------------------------------------------------------------------------------------------

TGsPoolPreSizeAlloc<FGsDamageNotifyInfo>* FGsDamageRunner::PoolNotify = nullptr;

FGsDamageRunner::FGsDamageRunner()
{
}

void FGsDamageRunner::Clear()
{
	if (PoolNotify)
	{
		for (int i = 0; i < _listDamageInfo.Num(); ++i)
		{
			PoolNotify->Release(_listDamageInfo[i]);
		}
	}
	_listDamageInfo.Empty();
}

bool FGsDamageRunner::IsEmpty() const
{
	return _listDamageInfo.Num() == 0;
}

TArray<FGsDamageNotifyInfo*>& FGsDamageRunner::GetDamageNotifyList()
{
	return _listDamageInfo;
}

void FGsDamageRunner::Pop(float inDeltaTime, OUT TArray<FGsDamageNotifyInfo*>& OutList)
{
	for (int i = 0; i < _listDamageInfo.Num(); ++i)
	{
		_listDamageInfo[i]->_offset -= inDeltaTime;
		if (_listDamageInfo[i]->_offset <= 0.0f)
		{
			OutList.Add(_listDamageInfo[i]);
		}
	}

	_listDamageInfo.RemoveAll([&OutList](const FGsDamageNotifyInfo* AssetData)->bool
	{
		for (int i = 0; i < OutList.Num(); ++i)
		{
			if (*OutList[i] == *AssetData)
			{
				return true;
			}
		}
		return false;
	});
}

void FGsDamageRunner::Pop(uint32 NotifyId, OUT TArray<FGsDamageNotifyInfo*>& OutList)
{
	for (int i = 0; i < _listDamageInfo.Num(); ++i)
	{
		if (PD::SC::PKT_SC_SKILL_DAMAGED_READ* packet = reinterpret_cast<PD::SC::PKT_SC_SKILL_DAMAGED_READ*>(
			_listDamageInfo[i]->GetData()))
		{
			if (packet->SkillNotifyId() == NotifyId)
			{
				OutList.Add(_listDamageInfo[i]);
			}
		}
	}

	_listDamageInfo.RemoveAll([&OutList](const FGsDamageNotifyInfo* AssetData)->bool
		{
			for (int i = 0; i < OutList.Num(); ++i)
			{
				if (*OutList[i] == *AssetData)
				{
					return true;
				}
			}
			return false;
		});
}

void FGsDamageRunner::Push(float Offset, PD::SC::PKT_SC_SKILL_DAMAGED_READ* Packet)
{

	FGsDamageNotifyInfo* refInfo = PoolNotify->GetFreeLink();
	refInfo->_offset = Offset;
	refInfo->_stiffenPos = FVector::ZeroVector;
	refInfo->_stiffenDuration = 0.0f;
	FMemory::Memcpy(refInfo->GetData(), Packet, Packet->mPktSize);

	_listDamageInfo.Add(refInfo);
}

bool FGsDamageRunner::SetStiffen(uint32 NotifyId, const FVector& StiffenPos, float fDuration)
{
	FGsDamageNotifyInfo** findNotifyInfo = _listDamageInfo.FindByPredicate([NotifyId](FGsDamageNotifyInfo* el)
		{
			if (PD::SC::PKT_SC_SKILL_DAMAGED_READ* packet = reinterpret_cast<PD::SC::PKT_SC_SKILL_DAMAGED_READ*>(el->GetData()))
			{
				return packet->SkillNotifyId() == NotifyId;
			}
			return false;
		}
	);
	if (findNotifyInfo)
	{
		(*findNotifyInfo)->_stiffenPos = StiffenPos;
		(*findNotifyInfo)->_stiffenDuration = fDuration;
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------
// FGsHealRunner
//-------------------------------------------------------------------------------------------------

TGsPoolPreSizeAlloc<FGsHealNotifyInfo>* FGsHealRunner::PoolHealNotify = nullptr;

FGsHealRunner::FGsHealRunner()
{
}

void FGsHealRunner::Clear()
{
	if (PoolHealNotify)
	{
		for (int i = 0; i < _listHealInfo.Num(); ++i)
		{
			PoolHealNotify->Release(_listHealInfo[i]);
		}
	}
	_listHealInfo.Empty();
}

bool FGsHealRunner::IsEmpty() const
{
	return _listHealInfo.Num() == 0;
}

TArray<FGsHealNotifyInfo*>& FGsHealRunner::GetHealNotifyList()
{
	return _listHealInfo;
}

void FGsHealRunner::Pop(float inDeltaTime, OUT TArray<FGsHealNotifyInfo*>& OutList)
{
	for (int i = 0; i < _listHealInfo.Num(); ++i)
	{
		_listHealInfo[i]->_offset -= inDeltaTime;
		if (_listHealInfo[i]->_offset <= 0.0f)
		{
			OutList.Add(_listHealInfo[i]);
		}
	}

	_listHealInfo.RemoveAll([&OutList](const FGsHealNotifyInfo* AssetData)->bool
		{
			for (int i = 0; i < OutList.Num(); ++i)
			{
				if (*OutList[i] == *AssetData)
				{
					return true;
				}
			}
			return false;
		});
}

void FGsHealRunner::Pop(uint32 NotifyId, OUT TArray<FGsHealNotifyInfo*>& OutList)
{
	for (int i = 0; i < _listHealInfo.Num(); ++i)
	{
		if (PD::SC::PKT_SC_SKILL_HEALED_READ* packet = reinterpret_cast<PD::SC::PKT_SC_SKILL_HEALED_READ*>(
			_listHealInfo[i]->GetData()))
		{
			if (packet->SkillNotifyId() == NotifyId)
			{
				OutList.Add(_listHealInfo[i]);
			}
		}
	}

	_listHealInfo.RemoveAll([&OutList](const FGsHealNotifyInfo* AssetData)->bool
		{
			for (int i = 0; i < OutList.Num(); ++i)
			{
				if (*OutList[i] == *AssetData)
				{
					return true;
				}
			}
			return false;
		});
}

void FGsHealRunner::Push(float Offset, PD::SC::PKT_SC_SKILL_HEALED_READ* Packet)
{
	FGsHealNotifyInfo* refInfo = PoolHealNotify->GetFreeLink();
	refInfo->_offset = Offset;
	FMemory::Memcpy(refInfo->GetData(), Packet, Packet->mPktSize);

	_listHealInfo.Add(refInfo);
}


//-------------------------------------------------------------------------------------------------
// FGsDamageHandlerBase
//-------------------------------------------------------------------------------------------------

bool FGsDamageHandlerBase::IsEmpty() const
{
	return _damageRunner.IsEmpty() && _healRunner.IsEmpty();
}

void FGsDamageHandlerBase::Initialize(UGsGameObjectBase* Owner)
{
	_owner = Owner;
}

void FGsDamageHandlerBase::Update(float Delta)
{
	TArray<FGsDamageNotifyInfo*> outNotify;
	_damageRunner.Pop(Delta, outNotify);
	// 정보가 있으면 처리
	if (outNotify.Num())
	{
		ExcuteDamage(outNotify);
	}

	TArray<FGsHealNotifyInfo*> outHealNotify;
	_healRunner.Pop(Delta, outHealNotify);
	if (outHealNotify.Num())
	{
		ExcuteHeal(outHealNotify);
	}

	if (IsEmpty())
	{
		if (_owner->IsPendingKill())
		{
			_owner->OnDie();
		}
	}	
}

void FGsDamageHandlerBase::Finalize()
{
	_owner = nullptr;
	_damageRunner.Clear();
	_healRunner.Clear();
}

void FGsDamageHandlerBase::FindExcuteDamage(uint32 NotifyId)
{
	TArray<FGsDamageNotifyInfo*> outNotify;
	_damageRunner.Pop(NotifyId, outNotify);
	if (outNotify.Num())
	{
		ExcuteDamage(outNotify);
	}
}

void FGsDamageHandlerBase::ExcuteDamage(TArray<FGsDamageNotifyInfo*>& ListInfo)
{
	for (FGsDamageNotifyInfo* el : ListInfo)
	{
		if (PD::SC::PKT_SC_SKILL_DAMAGED_READ* packet = reinterpret_cast<PD::SC::PKT_SC_SKILL_DAMAGED_READ*>(el->GetData()))
		{
			UGsGameObjectBase* hitObject = GSGameObject()->FindObject(EGsGameObjectType::Creature, packet->mTargetId);
			if (nullptr == hitObject)
			{
				return;
			}

			UGsGameObjectCreature* castObject = hitObject->CastGameObject<UGsGameObjectCreature>();
			castObject->NetSkillDamagedAck(packet);
			// 경직 처리
			if (el->_stiffenPos != FVector::ZeroVector)
			{
				castObject->OnStiffen(el->_stiffenPos, el->_stiffenDuration);
			}
		}

		FGsDamageRunner::PoolNotify->Release(el);
	}
}

void FGsDamageHandlerBase::ExcuteHeal(TArray<FGsHealNotifyInfo*>& ListInfo)
{
	for (FGsHealNotifyInfo* el : ListInfo)
	{
		if (PD::SC::PKT_SC_SKILL_HEALED_READ* packet = reinterpret_cast<PD::SC::PKT_SC_SKILL_HEALED_READ*>(el->GetData()))
		{
			UGsGameObjectBase* hitObject = GSGameObject()->FindObject(EGsGameObjectType::Creature, packet->mTargetId);
			if (nullptr == hitObject)
			{
				return;
			}

			UGsGameObjectCreature* castObject = hitObject->CastGameObject<UGsGameObjectCreature>();
			castObject->NetSkillHealAck(packet);
		}

		FGsHealRunner::PoolHealNotify->Release(el);
	}
}

void FGsDamageHandlerBase::Clear()
{
	// 현재 대기중인 모든것들을 한번에 적용한다.
	ExcuteDamage(_damageRunner.GetDamageNotifyList());
	ExcuteHeal(_healRunner.GetHealNotifyList());
	_damageRunner.Clear();
	_healRunner.Clear();
}

void FGsDamageHandlerBase::Add(PD::SC::PKT_SC_SKILL_DAMAGED_READ* Packet)
{
	const FGsSchemaSkillSet* skillData =
		UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(Packet->SkillId());
	if (nullptr == skillData)
	{
		return;
	}

	float offset = Packet->NotifyTickOffset();
	float totalRunTime = skillData->totalRuntime;

	// 캐스팅 스킬 반영
	if (skillData->category == SkillCategory::CAST)
	{
		// EndSection Type 찾음
		const FGsSchemaSkillNotify* findNotify = skillData->notifyInfoList.FindByPredicate([](const FGsSchemaSkillNotify& el)
			{
				const FGsSchemaSkillNotifySet* skillNotifySet = el.skillNotifyId.GetRow();
				return skillNotifySet && skillNotifySet->type == SkillNotifyType::END_SECTION;
			});

		if (findNotify)
		{
			offset -= findNotify->offset;
		}
	}

	// 공속 반영 스킬
	if (skillData->applyAttackSpeed)
	{
		// 캐스터가 플레이어이고, 존재하면 캐스터의 공속정보를 반영
		UGsGameObjectBase* playerObject = GSGameObject()->FindObject(EGsGameObjectType::Player, Packet->CasterId());
		if (playerObject)
		{
			FGsSkillHandlerBase* skillHandler = playerObject->GetSkillHandler();
			float attackSpeedRate = skillData->isNormal ? skillHandler->GetAttackSpeedRate() : skillHandler->GetCastSpeedRate();
			float offsetRate = offset / totalRunTime;
			float attackSpeedScale = 1.f / attackSpeedRate;
			totalRunTime *= attackSpeedScale;
			offset = totalRunTime * offsetRate;
		}
	}	

	_damageRunner.Push(offset, Packet);
	
	// 선 연출 효과 적용 (주체자가 나일 경우에만 적용)
	if (Packet->MediumId() == 0)
	{
		HitEffect(skillData, Packet->CasterId(), Packet->TargetId(), Packet->HitResultType());
	}
}

void FGsDamageHandlerBase::Add(PD::SC::PKT_SC_SKILL_HEALED_READ* Packet)
{
	_healRunner.Push(Packet->NotifyTickOffset(), Packet);
}

bool FGsDamageHandlerBase::AddStiffen(uint32 NotifyId, const FVector& StiffenPos, float fDuration)
{
	// 현재 버퍼에서 Notify로 찾음
	return _damageRunner.SetStiffen(NotifyId, StiffenPos, fDuration);
}

void FGsDamageHandlerBase::HitEffect(const FGsSchemaSkillSet* SkillData, int64 Caster, int64 Target, HitResultType ResultType)
{
	UGsGameObjectBase* casterObject = GSGameObject()->FindObject(EGsGameObjectType::Creature, Caster);
	if (nullptr == casterObject)
	{
		return;
	}

	AGsCharacterBase* casterActor = casterObject->GetCharacter();
	if (nullptr == casterActor)
	{
		return;
	}

	UGsGameObjectBase* targetObject = GSGameObject()->FindObject(EGsGameObjectType::Creature, Target);
	if (targetObject && targetObject->IsAble())
	{
		AGsCharacterBase* targetActor = Cast<AGsCharacterBase>(targetObject->GetActor());

		// 공격 대상의 유도체 관련 정보를 저장
		// 선행 작업으로 StartSkill 패킷을 받을때 관련 정보를 Clear
		// @see : NetSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet)
		if (UGsHommingProjectileComponent* hommingComponent = casterActor->GetHommingProjectile())
		{
			hommingComponent->SetHommingProjectileInfo(ResultType, targetActor);
		}
	}
}



