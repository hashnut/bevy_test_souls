// Fill out your copyright notice in the Description page of Project Settings.

#include "GsSkillNotifyNodeBase.h"
#include "Engine/AssetManager.h"

#include "GameObject/Skill/Data/GsSkillDataBase.h"
#include "T1Project.h"
#include "Util/GsTableUtil.h"

int FGsSkillNotifyNodeBase::GetSkillNotifyId() const
{
	return UGsTableUtil::ConvertRow<int32>(_currNotifyData);
}

void FGsSkillNotifyNodeBase::Initialize()
{
	// 초기화
	_pendingKill = false;
	_scale = 1.f;
	_offset = 0.f;
	_currNotifyData = nullptr;
	_netData = nullptr;
}

void FGsSkillNotifyNodeBase::Initialize(UGsGameObjectBase* Owner, const IGsSkillRunnerData* SkillRunnerData,
	const FGsSchemaSkillNotify* NotifyInfo, const FGsSkillNetData* NetData, float OffsetRate)
{
	Initialize();
	if (NotifyInfo)
	{
		if (const FGsSchemaSkillNotifySet* notifyTable = NotifyInfo->skillNotifyId.GetRow())
		{
			_currNotifyData = notifyTable;
			float rate = NotifyInfo->offset / SkillRunnerData->GetTotalTime() * OffsetRate;
			_offset = (SkillRunnerData->GetTotalTime() * rate);
		}
	}
	_netData = NetData;
}

void FGsSkillNotifyNodeBase::Initialize(UGsGameObjectBase* Owner, const FGsSchemaSkillNotifySet* NotifySetData,
	const struct FGsSkillNetData* NetData)
{
	Initialize();
	_currNotifyData = NotifySetData;
	_netData = NetData;
}

void FGsSkillNotifyNodeBase::Finalize()
{
	_pendingKill = true;
}

void FGsSkillNotifyNodeBase::Action(UGsGameObjectBase* Owner)
{

//	GSLOG(Warning, TEXT("SkillNotifyNode Action %s [%d]"), *GetDebugTypeName(), GetSkillNotifyId());
	_pendingKill = true;
}

void FGsSkillNotifyNodeBase::Cancel(UGsGameObjectBase* Owner)
{
}

bool FGsSkillNotifyNodeBase::Update(UGsGameObjectBase* Owner, float CurrentTime)
{
	if (CurrentTime > _offset)
	{
		Finalize();
		return true;
	}
	return false;
}