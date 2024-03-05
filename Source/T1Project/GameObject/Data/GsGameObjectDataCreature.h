// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsGameObjectData.h"
#include "GameObject/Stat/GsStatBase.h"

#include "Util/GsClassUtil.h"

struct FGsUserSkillListInfo;
class UGsGameObjectBase;
class FGsStatInfo;
struct FGsSpawnParam;

namespace PD
{
	namespace SC
	{
		struct PKT_SC_CREATURE_COMBAT_STATUS_READ;
	}
}

/**
 *
 */
class T1PROJECT_API FGsGameObjectDataCreature : 
	public FGsGameObjectData
{
	using Super = FGsGameObjectData;

protected:
	TSharedPtr<FGsStatBase> _creatureStat			= nullptr;

	// Local Player는 UserBasicData에 있는 정보를 사용한다.
	UserStatus _userStatus				= UserStatus::USER_STATE_NORMAL;
	bool _isSkipTerrainCheck = false;

public:
	FGsGameObjectDataCreature(const FGsSpawnParam* SpawnParam);
	virtual ~FGsGameObjectDataCreature() = default;

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

public:
	virtual void SetCreatureCombatStatus(PD::SC::PKT_SC_CREATURE_COMBAT_STATUS_READ& Packet);
	virtual void SetUserStatus(UserStatus inStatus);
	virtual void SetSkipTerrainCheck(bool IsCheck);
	
	//UserStatus GetUserStatus(){return _userStatus;}

public:
	TSharedPtr<FGsStatBase> GetStatBase() const { return _creatureStat; }

	template <typename T = FGsStatBase>
	TSharedPtr<FGsStatBase> CreateCreatureStat(UGsGameObjectBase* InOwner)
	{
		_creatureStat = MakeShareable(new T());
		if (_creatureStat)
		{
			_creatureStat->Initialize(InOwner);
		}
		return _creatureStat;
	}


	template <typename T = FGsStatBase>
	T* GetCastStat() const
	{
		return FGsClassUtil::FCheckStaticCast<T>(_creatureStat);
	}

public:
	void UpdateCreatureStatInfo(TArray<TPair<StatType, int32> >& InStatInfoMap);

public:
	const FGsStatInfo* GetCreatureStatInfo(StatType InType) const;
	bool IsZeroHp() const;

public:
	virtual UserStatus GetUserStatus() const;
	virtual bool IsSkipTerrainCheck() const;
};
