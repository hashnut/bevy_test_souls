// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Classes/GsManager.h"
#include "GameObject/Define/GsGameObjectDefine.h"

/**
 * 내 플레이어의 쿨타임 제어 클래스
 * Group으로 묶여진 경우는 tId정보를 따로 처리 하지 않는다.
 * 즉 Group > 0 : tId = 0, Group == 0 : tId
 * @See : const DefGroupKey groupKey(GroupId, GroupId > 0 ? 0 : Id);
 * 추후 InGame 진입전 쿨타임 정보는 FGsGameDataManager 클래스를 통해 갱신 처리 할것 (ex 분/시 단위의 버프)
 */
class T1PROJECT_API FGsCoolTimeManager : public IGsManager
{
public:
	// key : GroupId, tId
	using DefGroupKey = TPair<uint64, int>;

private:
	// 추후 관련 정보가 많이 필요하게 되면 struct로 수정 (ex TotalTime)
	TMap<DefGroupKey, float> _mapGroupCooltime[(int)EGsCoolTime::Max];

	// Remove 캐시
	TArray<DefGroupKey> _listCacheRemove;

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

	virtual void Update(float inTick) override;

public:
	// 이 함수 호출은 네트워크 패킷 응답에서만 처리되어야 할것
	void SetCoolTime(EGsCoolTime Type, uint64 GroupId, int Id, float RemainTime, bool InIsNotify = false);

private:
	DefGroupKey GetKey(uint64 GroupId, int Id) const;

public:
	// 타입별 그룹값 얻기 (현재 미정)
	int GetGroupId(EGsCoolTime Type, int Id) const;

	float GetCoolTime(EGsCoolTime Type, uint64 GroupId, int Id) const;
	float GetCoolTime(EGsCoolTime Type, int id) const;
	bool IsCoolTime(EGsCoolTime Type, int Id) const;
	bool IsCoolTime(EGsCoolTime Type, uint64 GroupID, int Id) const;
};

#define GCoolTime() UGsScopeHolder::GetGameManagerFType<FGsCoolTimeManager>(UGsScopeGame::EManagerType::CoolTime)
