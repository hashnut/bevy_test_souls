#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"

#include "GameObject/Stat/GsStatInfo.h"

#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGame.h"

#include "Message/GsMessageUserInfo.h"

#include "Party/GsPartyInviterInfo.h"
#include "Party/GsPartyDefine.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Public/Shared/Shared/SharedInclude/SharedTypes.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"

/*
	파티 관리자
*/

class FGsPartyMemberInfo;
class FGsPartyInviteeInfo;
class FGsPartyInviteBanInfo;

class IGsPartySlotInfo;

struct IGsMessageParam;


class FGsPartyManager : public IGsManager
{
	// 멤버 변수
private:
	// 리더 슬롯 id
	int _leaderSlotId;
	// 내 슬롯 id
	int _mySlotId;
	// 멤버 정보
	TArray<FGsPartyMemberInfo*> _arrPartyMemberInfo;
	// 초대 대상 정보
	TArray<FGsPartyInviteeInfo*> _arrPartyInviteeInfo;
	// 초대자 정보
	FGsPartyInviterInfo _inviterInfo;

	// 파티 드롭 소유권 타입
	PartyDropOwnershipType _partyDropOwnershipType;
	// 맵: 초대 제한 정보
	TMap<FString, FGsPartyInviteBanInfo*> _mapInviteBanInfo;

	// 파티에 속했는가
	bool _isInParty = false;
	// 드로우 메뉴 닫을것인지
	bool _isCloseDrawMenu = false;

	int64 _sharedPartyTargetId = -1;
	
	// 가상 함수
public:
	// 초기화
	virtual void Initialize()override;
	// 정리
	virtual void Finalize()override;
	// 갱신
	virtual void Update(float In_delta) override;

	// 외부 접근 로직 함수
public:
	// 파티 설정창 열기
	void OpenPartySettingUI();
	// ui 멤버 갱신
	void UpdateMemberUI(bool In_isAll, bool In_isDrawMenuInit = true, int In_slotId = -1);
	void UpdateSkillTargetUI(bool In_isAll, int In_slotId = -1);
	// ui HUD side 탭 갱신
	void UpdateHUDSideTab();
	// ui 파티 정보 갱신
	void UpdateHUDPartyInfo();
	// 초대 대상 삭제
	void RemovePartyInviteeInfo(int In_partySlotId);
	// 데이터 클리어
	void ClearData();
	// 멤버 삭제
	void RemovePartyMemberInfo(int In_partySlotId);
	// 리더 변경
	void LeaderChange(int In_leaderSlotId, bool In_forceShowTicker = false);
	// 초대 밴 정보 추가
	void AddInviteBanInfo(const FString& In_name);
	// 초대 밴 정보 삭제
	void DelInviteBanInfo(const FString& In_name);

	// 응답
public:
	// 생성 응답
	void RecvPartyCreate(FGsPartyMemberInfo* In_myInfo, PartyDropOwnershipType In_type);
	// 초대 응답
	void RecvPartyInvite(int In_partySlotId, const FString& In_name, WorldId In_homeWorldId);
	// 초대 받음
	void RecvPartyInvited(const FString& In_name, int In_level, int In_channelId, WorldId In_homeWorldId);
	// 초대 수락
	void RecvPartyJoined(PD::GC::PKT_GC_PARTY_JOINED_READ& In_packet,
		TMap<int64, bool> In_mapSpawnedInfo,
		FGsPartyMemberInfo* In_myInfo);

	// 초대 거절
	void RecvPartyRefuseInvitation();
	// 초대 실패
	void RecvPartyInviteFailed(int In_slotId);
	// 리더 변경
	void RecvChangeLeader(int In_leaerSlotId);
	// 전리품 분배룰 변경
	void RecvChangeDropOwnershipType(PartyDropOwnershipType In_type);
	// 2022/06/13 PKT - 파티원 준비 상태 변경(자신 포함)
	void RecvChangedReadyState(int32 In_partySlotId, bool In_isReady);

	// game object 에서 변경(나, 시야 안 파티원)
public:
	// 스탯(hp, mp) 변경
	void OnChangedStat(int64 In_gameId, const FGsStatInfo& In_statInfo);
	// 무기 변경
	void OnChangedWeapon(int64 In_gameId, CreatureWeaponType In_weaponType);
	// 레벨 변경
	void OnChangedLevel(int64 In_gameId, int In_newLevel);
	// 맵 id와 채널 변경
	void OnChangedMapIdAndChannel(int64 In_gameId, int In_newMapId ,int In_newChannelId);
	// 디스폰 변경
	void OnDespawnChange(int64 In_gameId, bool In_isDespawn);
	// 스폰
	void OnSpawn(int64 In_gameId);
	// 디스폰
	void OnDespawn(int64 In_gameId);

	// get, set
public:
	// 멤버 정보 넣기
	void SetPartyMembmerInfo(int64 In_gameId, const FString& In_name,
		int In_level, int In_nowHP, int In_maxHP, int In_nowShield, int In_nowMP, int In_maxMP,
		int In_partySlotId, CreatureWeaponType In_weaponType, int In_channelId,
		int In_mapId, const FVector& In_pos,
		bool In_isSpawned, WorldId In_worldId, WorldId In_homeWorldId);

	// 슬롯 정보 구하기
	IGsPartySlotInfo* GetPartySlotInfo(int In_partySlotId);
	// 멤버 찾기
	FGsPartyMemberInfo* FindPartyMemberInfo(int In_partySlotId);
	// find party member array index by slot id
	int FindPartyMemberIndexBySlotId(int In_partySlotId);
	// 초대 대상 찾기
	FGsPartyInviteeInfo* FindPartyInviteeInfo(int In_partySlotId);
	// find party invitee array index by slot id
	int FindPartyInviteeIndexBySlotId(int In_partySlotId);

	// 게임 id로 멤버 찾기
	FGsPartyMemberInfo* FindPartyMemberInfoByGameId(int64 In_gameId);

	// 내 파티 정보 가져오기
	FGsPartyMemberInfo* FindMyPartyMemberInfo();
	// 리더 파티 정보 가져오기
	FGsPartyMemberInfo* FindLeaderPartyMemberInfo();
	// 모든 파티 멤버 접근
	const TArray<FGsPartyMemberInfo* > GetAllPartyMemberInfo()
	{
		return _arrPartyMemberInfo;
	}
	// 내슬롯 번호인가
	bool IsMySlotId(int In_slotId)
	{
		return (In_slotId == _mySlotId) ? true : false;
	}
	
	// 난 리더인 것인가
	bool AmILeader()
	{
		return (_mySlotId == _leaderSlotId) ? true : false;
	}
	// 파티 안에 있는가
	bool GetIsInParty() { return _isInParty; }
	// 해당 gameId 파티원인가
	bool IsInPartyGameId(int64 In_gameId);
	// 파티원의 이름인가
	bool IsInPartyName(const FString& In_name);
	// 초대중인 사람 이름인가
	bool IsPartyInviteeName(const FString& In_name);
	// 획득 룰 가져오기
	PartyDropOwnershipType GetDropOwnershipType() { return _partyDropOwnershipType; }

	// 초대자 정보 가져오기
	FGsPartyInviterInfo& GetPartyInviteInfo()
	{
		return _inviterInfo;
	}

	bool GetIsCloseDrawMenu() 
	{
		return _isCloseDrawMenu;
	}
	// 파티 멤버 인원 수
	int GetPartyMemberCount();

	// 초대 밴 대상인지
	// 저대상한테 너무 많이 보냄(60초내에 4번 이상)
	bool IsInviteBanTarget(const FString& In_name);
	// 파티장으로 워프 가능한지
	// 1. (다른맵||다른 채널)
	// 2. not other world id with leader
	EGsPartyWarpToLeaderResult IsPossibleWarpToLeader(bool& Out_diffMap);

	void SetSharedPartyTargetId(int64 In_targetId)
	{
		_sharedPartyTargetId = In_targetId;
	}
	int64 GetSharedPartyTargetId()
	{
		return _sharedPartyTargetId;
	}
};

#define GSParty() UGsScopeHolder::GetGameManagerFType<FGsPartyManager>(UGsScopeGame::EManagerType::Party)