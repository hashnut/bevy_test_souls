#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Shared/SharedInclude/PacketWriter.h"
#include "Party/GsPartySlotInfo.h"
#include "Party/GsPartyDefine.h"
/*
	파티원 정보
*/
class FGsPartyMemberInfo : public IGsPartySlotInfo
{
	// 멤버 변수
private:
	// 게임 id
	int64 _gameId;
	// 리더인지
	bool _isLeader;
	// 이름
	FString _name;
	// 레벨
	int _level;
	// 현재 hp
	int _nowHP;
	// 최대 hp
	int _maxHP;
	// 보호막
	int _nowShield;
	// 현재 mp
	int _nowMP;
	// 최대 mp
	int _maxMP;
	// 파티 슬롯 번호
	int _partySlotId;
	// 무기 타입
	CreatureWeaponType _weaponType;
	// 채널 id
	int _channelId;
	// 맵 id
	int _mapId;
	// 위치
	FVector _pos;
	// mp (0~1)
	float _mpRate;	

	// 갱신 정보(EGsPartyMemberInfoType bit 연산)
	uint32 _dirtyFlag;
	// 갱신 정보(EGsPartyMemberInfoType bit 연산)
	// 팝업 세팅용
	uint32 _dirtyPopupSettingFlag;
	// 갱신 정보(EGsPartyMemberInfoType bit 연산)
	// skill target select
	uint32 _dirtySkillTargetSelectFlag;

	// 미니맵에서 디스폰후 정보 갱신 시점에 다시 아이콘 만들지 플래그
	// 근처에 있다가 다른맵 갈 때 해당맵 도착 전까진 update가 안와서 처리
	bool _isDespawn = false;
	// hud에서 아이템 bg show용(근처에 없으면 bg 출력 on)
	bool _isSpawned = false;
	
	// 2022/06/13 PKT - 파티원들의 준비가 필요한 컨텐츠에 사용될 Flag
	bool _complatedReady;
	// inter server(현재 있는 월드 ID)
	WorldId _worldId = 0;
	// 파티원의 원래 월드 ID
	WorldId _homeWorldId = 0;


	// 생성자
public:
	FGsPartyMemberInfo(int64 In_gameId, bool In_isLeader, const FString& In_name,
		int In_level, int In_nowHP, int In_maxHP, int In_nowShield, int In_nowMP, int In_maxMP,
		int In_partySlotId, CreatureWeaponType In_weaponType, int In_channelId,
		int In_mapId, const FVector& In_pos, bool In_isSpawned, bool In_complatedReady, WorldId In_worldId,
		WorldId In_homeWorldId)
		: _gameId(In_gameId), _isLeader(In_isLeader), _name(In_name),
		_level(In_level), _nowHP(In_nowHP), _maxHP(In_maxHP), _nowShield(In_nowShield),
		_nowMP(In_nowMP), _maxMP(In_maxMP), _partySlotId(In_partySlotId),
		_weaponType(In_weaponType), _channelId(In_channelId), _mapId(In_mapId),
		_pos(In_pos), _isSpawned(In_isSpawned), _complatedReady(In_complatedReady), _worldId(In_worldId),
		_homeWorldId(In_homeWorldId)
	{
		SetMpRate();
		
		_dirtyFlag = 0;
		_dirtyPopupSettingFlag = 0;
		_dirtySkillTargetSelectFlag = 0;

		_isDespawn = false;
	}

	// 가상 함수
public:
	virtual const FString& GetName() override
	{
		return _name;
	}
	virtual bool GetIsMember() override
	{
		return true;
	}
	// 로직 함수
public:
	void LeaderChange(int In_leaderSlot)
	{
		_isLeader = (_partySlotId == In_leaderSlot)? true: false;
	}
	// 갱신 정보 추가
	void AddDirtyInfo(EGsPartyMemberInfoType In_type)
	{
		_dirtyFlag |= (uint32)In_type;
		_dirtyPopupSettingFlag |= (uint32)In_type;
		_dirtySkillTargetSelectFlag |= (uint32)In_type;
	}
	// 갱신 정보 초기화
	void ClearDirtyInfo()
	{
		_dirtyFlag = 0;
	}
	// 갱신 정보 초기화(팝업 세팅용)
	void ClearPopupSettingDirtyInfo()
	{
		_dirtyPopupSettingFlag = 0;
	}
	// 갱신 정보 초기화(skill target select)
	void ClearSkillTargetSelectDirtyInfo()
	{
		_dirtySkillTargetSelectFlag = 0;
	}
	// 변경 되었나
	bool IsChangedInfo(EGsPartyMemberInfoType In_type)
	{
		return (_dirtyFlag & (uint32)In_type);
	}
	// 변경 되었나(팝업 세팅)
	bool IsChangedPopupSettingInfo(EGsPartyMemberInfoType In_type)
	{
		return (_dirtyPopupSettingFlag & (uint32)In_type);
	}
	// 변경 되었나(skill target select)
	bool IsChangedSkillTargetSelectInfo(EGsPartyMemberInfoType In_type)
	{
		return (_dirtySkillTargetSelectFlag & (uint32)In_type);
	}
	// 하나라도 바뀌었는가
	bool IsChangedInfo()
	{
		return (_dirtyFlag != 0) ? true : false;
	}
	// 하나라도 바뀌었는가(팝업 세팅)
	bool IsChangedPopupSettingInfo()
	{
		return (_dirtyPopupSettingFlag != 0) ? true : false;
	}
	// 하나라도 바뀌었는가(skill target select)
	bool IsChangedSkillTargetSelectInfo()
	{
		return (_dirtySkillTargetSelectFlag != 0) ? true : false;
	}
	// get ,set
public:
	int64 GetGameId() { return _gameId; }
	int GetPartySlotId() { return _partySlotId; }
	CreatureWeaponType GetWeaponType() { return _weaponType; }
	bool GetIsLeader() { return _isLeader; }
	int GetLevel() { return _level; }
	int GetChannelId() { return _channelId; }
	FVector GetPos() { return _pos; }
	int GetMapId() { return _mapId; }
	bool GetIsDespawn() { return _isDespawn; }
	bool GetIsSpawned() { return _isSpawned; }
	bool GetComplatedReady() { return _complatedReady; }

	void SetData(int64 In_gameId, bool In_isLeader, const FString& In_name,
		int In_level, int In_nowHP, int In_maxHP, int In_nowShield, int In_nowMP, int In_maxMP,
		int In_partySlotId, CreatureWeaponType In_weaponType, int In_channelId,
		int In_mapId, const FVector& In_pos, bool In_isSpawned, bool In_complatedReady, WorldId In_worldId, 
		WorldId In_homeWorldId)
	{
		_gameId = In_gameId;
		_isLeader = In_isLeader;
		_name = In_name;
		_level = In_level;
		_nowHP = In_nowHP;
		_maxHP = In_maxHP;
		_nowShield = In_nowShield;
		_nowMP = In_nowMP;
		_maxMP = In_maxMP;
		_partySlotId = In_partySlotId;
		_weaponType = In_weaponType;
		_channelId = In_channelId;
		_mapId = In_mapId; 
		_pos = In_pos;

		_isSpawned = In_isSpawned;

		_complatedReady = In_complatedReady;

		SetMpRate();

		_dirtyFlag = 0;
		_dirtyPopupSettingFlag = 0;
		_dirtySkillTargetSelectFlag = 0;

		_isDespawn = false;
		_worldId = In_worldId;
		_homeWorldId = In_homeWorldId;
	}

	void SetMpRate()
	{
		if (_maxMP == 0.0f)
		{
			_mpRate = 0.0f;
		}
		else
		{
			_mpRate = _nowMP / (float)_maxMP;
		}
	}

	int GetNowHp() const
	{
		return _nowHP;
	}

	int GetMaxHp() const
	{
		return _maxHP;
	}

	int GetNowShield() const
	{
		return _nowShield;
	}

	float GetMpRate()
	{
		return _mpRate;
	}
	// 채널 변경
	void SetChannelId(int In_channelId)
	{
		_channelId = In_channelId;
	}
	// 맵 id 변경
	void SetMapId(int In_mapId)
	{
		_mapId = In_mapId;
	}
	// 디스폰 설정
	void SetIsDespawn(bool In_val)
	{
		_isDespawn = In_val;
	}
	void SetWorldId(WorldId In_worldId)
	{
		_worldId = In_worldId;
	}
	WorldId GetWorldId()
	{
		return _worldId;
	}
	WorldId GetHomeWorldId()
	{
		return _homeWorldId;
	}

	// 개별 갱신
public:
	
	// 레벨
	void SetLevel(int In_level)
	{
		bool diffVal = (_level != In_level) ? true : false;
		_level = In_level;
		if (diffVal == true)
		{
			AddDirtyInfo(EGsPartyMemberInfoType::Level);
		}
	}
	// hp
	void SetHp(int In_nowHp, int In_maxHp)
	{
		bool diffVal = ((In_nowHp != _nowHP) || (In_maxHp != _maxHP)) ? true : false;
		_nowHP = In_nowHp;
		_maxHP = In_maxHp;
		if (diffVal == true)
		{
			AddDirtyInfo(EGsPartyMemberInfoType::HP);
		}
	}
	// mp
	void SetMp(int In_nowMp, int In_maxMp)
	{
		bool diffVal = ((In_nowMp != _nowMP) || (In_maxMp != _maxMP)) ? true : false;
		_nowMP = In_nowMp;
		_maxMP = In_maxMp;
		if (diffVal == true)
		{
			SetMpRate();
			AddDirtyInfo(EGsPartyMemberInfoType::MP);
		}
	}
	// Shield
	void SetShield(int In_shield)
	{
		bool diffVal = (In_shield != _nowShield) ? true : false;
		_nowShield = In_shield;
		if (diffVal == true)
		{
			AddDirtyInfo(EGsPartyMemberInfoType::HP_Shield);
		}
	}

	// 무기 교체
	void SetWeaponType(CreatureWeaponType In_type)
	{
		bool diffVal = (_weaponType != In_type) ? true : false;
		_weaponType = In_type;
		if (diffVal == true)
		{
			AddDirtyInfo(EGsPartyMemberInfoType::WeaponType);
		}
	}

	void SetIsSpawned(bool In_isSpawned)
	{
		bool diffVal = (In_isSpawned != _isSpawned) ? true : false;
		_isSpawned = In_isSpawned;
		if (diffVal == true)
		{
			AddDirtyInfo(EGsPartyMemberInfoType::Spawn);
		}
	}

	// 2022/06/13 PKT - 준비 상태
	void SetComplatedReady(bool In_complated)
	{
		bool diffVal = (In_complated != _complatedReady) ? true : false;
		_complatedReady = In_complated;
		if (diffVal == true)
		{
			AddDirtyInfo(EGsPartyMemberInfoType::Ready_State);
		}
	}

	// 컨퓨즈 상태이상(HP/MP가림)
	void UpdateAbnormalityConfuse()
	{
		AddDirtyInfo(EGsPartyMemberInfoType::Confuse);
	}
};
