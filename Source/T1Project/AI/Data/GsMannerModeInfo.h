#pragma once
#include "CoreMinimal.h"
/*
* 매너 모드 에서 사용하는 몬스터 공격 주도권 정보
* 1. monster game id(공격 대상)
* 2. remote game id(주도권 가지고 있는 타유저)
*/

class FGsMannerModeInfo
{
private:
	// 몬스터 game id
	int64 _monsterGameId = 0;
	// 주인 타유저 game id
	int64 _ownerRemoteGameId = 0;

	// 생성자, 소멸자
public:
	FGsMannerModeInfo(int64 In_monsterGameId, int64 In_remoteGameId) :
		_monsterGameId(In_monsterGameId),_ownerRemoteGameId(In_remoteGameId)
	{
	}

	virtual ~FGsMannerModeInfo() = default;

public:
	int64 GetMonsterGameId()
	{
		return _monsterGameId;
	}
	int64 GetRemoteGameId()
	{
		return _ownerRemoteGameId;
	}
};