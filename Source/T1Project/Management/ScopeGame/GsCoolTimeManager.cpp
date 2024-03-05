// Fill out your copyright notice in the Description page of Project Settings.


#include "GsCoolTimeManager.h"
#include "Runtime/Core/Public/Misc/App.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsCoolTimeMessageParam.h"
#include "T1Project.h"

void FGsCoolTimeManager::Initialize()
{
	for (int i = 0; i < (int)EGsCoolTime::Max; ++i)
	{
		_mapGroupCooltime[i].Empty();
	}
}

void FGsCoolTimeManager::Update(float Delta)
{
	for (int i = 0; i < (int)EGsCoolTime::Max; ++i)
	{
		for (auto& el : _mapGroupCooltime[i])
		{
			el.Value -= FApp::GetDeltaTime();

			// 제거 대상 검출
			if (el.Value <= 0.f)
			{
				_listCacheRemove.Emplace(el.Key);
			}
		}

		if (_listCacheRemove.Num() > 0)
		{
			FGsMessageHolder* msg = GMessage();
			// 쿨다운 완료 알림 (클라 판정 기준)
			for (DefGroupKey& el : _listCacheRemove)
			{
				FGsCoolTimeMessageParam messageParam((EGsCoolTime)i, el.Key, el.Value, -1.f);
				msg->GetGameObject().SendMessage(MessageGameObject::LOCAL_COOLTIME_END, &messageParam);
				_mapGroupCooltime[i].Remove(el);
			}
			_listCacheRemove.Empty();
		}
	}
}

void FGsCoolTimeManager::Finalize()
{
	_listCacheRemove.Empty();
	for (int i = 0; i < (int)EGsCoolTime::Max; ++i)
	{
		_mapGroupCooltime[i].Empty();
	}
}

void FGsCoolTimeManager::SetCoolTime(EGsCoolTime Type, uint64 GroupId, int Id, float RemainTime, bool InIsNotify)
{
	TMap<DefGroupKey, float>& refMap = _mapGroupCooltime[(int)Type];
	DefGroupKey groupKey = GetKey(GroupId, Id);
	if (RemainTime <= 0.f)
	{
		// 서버에서 완료에 대한 패킷을 주지 않으므로 사용할 필요가 없어 보인다.
		FGsCoolTimeMessageParam messageParam(Type, GroupId, Id, RemainTime);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_COOLTIME_END, &messageParam);

		// Map에 존재할경우 제거
		refMap.Remove(groupKey);
		return;
	}

	// 첫 Update에서의 Delta 계산이 제외 되어야 함
	// FGsSkillRunnerBase 에서 처리와 마찬가지로 문제가 될경우 FDateTime 활용으로 동시 수정할것
	float correctionRemainTime = RemainTime + FApp::GetDeltaTime();
	if (float* value = refMap.Find(groupKey))
	{
		*value = correctionRemainTime;
	}
	else
	{
		refMap.Emplace(groupKey, correctionRemainTime);
	}

	if (InIsNotify)
	{
		FGsCoolTimeMessageParam messageParam(Type, GroupId, Id, RemainTime);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_COOLTIME_START, &messageParam);
	}
}

FGsCoolTimeManager::DefGroupKey FGsCoolTimeManager::GetKey(uint64 GroupId, int Id) const
{
	return DefGroupKey(GroupId, GroupId > 0 ? 0 : Id);
}

int FGsCoolTimeManager::GetGroupId(EGsCoolTime Type, int Id) const
{
	// 아직 쿨타임 그룹 참조 정보는 결정된게 없다.
	switch (Type)
	{
	case EGsCoolTime::Skill:
		break;
	case EGsCoolTime::Item:
		break;
	}

	return 0;
}

float FGsCoolTimeManager::GetCoolTime(EGsCoolTime Type, uint64 GroupId, int Id) const
{
	DefGroupKey groupKey = GetKey(GroupId, Id);
	if (const float* value = _mapGroupCooltime[(int)Type].Find(groupKey))
	{
		return *value;
	}
	return -1.f;
}

// GetGroupId() 함수가 아직 미구현 상태(기획논의 필요) 이므로 이 함수는 사용 ㄴㄴ
float FGsCoolTimeManager::GetCoolTime(EGsCoolTime Type, int id) const
{
	uint64 groupId = GetGroupId(Type, id);
	return GetCoolTime(Type, groupId, id);
}

// GetGroupId() 함수가 아직 미구현 상태(기획논의 필요) 이므로 이 함수는 사용 ㄴㄴ
bool FGsCoolTimeManager::IsCoolTime(EGsCoolTime Type, int Id) const
{
	float value = GetCoolTime(Type, GetGroupId(Type, Id), Id);
	return value > 0.f;
}

bool FGsCoolTimeManager::IsCoolTime(EGsCoolTime Type, uint64 GroupID, int Id) const
{
	float value = GetCoolTime(Type,GroupID,Id);
	return value > 0.0f;
}