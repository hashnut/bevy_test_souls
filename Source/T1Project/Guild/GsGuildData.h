// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Guild/GsGuildDiplomacyData.h"
#include "Classes/SecureVar/GsSecureVar.h"


//----------------------------------------------------------------------------------------------------------------------
/**
 * 길드 기본정보 (가입 창에서 필요한 정보)
 */
class T1PROJECT_API FGsGuildData
{
public:
	GuildDBId _guildDBId = INVALID_GUILD_DB_ID;
	FString _guildName;
	FString _masterName;
	uint8 _guildLevel = INVALID_GUILD_LEVEL;
	uint8 _maxMemberNum = 0;
	uint8 _memberNum = 0;	
	Level _joinLimitLevel = 0;	
	GuildJoinType _joinType = GuildJoinType::MAX;
	GuildEmblemId _emblemId = 0;
	PlanetWorldId _homePlanetWorldId;

protected:
	FString _introduction; // 금칙어 *로 표시하기 위해
	
public:
	FGsGuildData() = default;
	virtual ~FGsGuildData() {};

public:
	virtual void Empty();

	virtual void SetDetail(const GuildDetail& InData);
	virtual void SetOtherGuildDetail(PD::GC::PKT_GC_ACK_GUILD_DETAIL_READ& InData);

public:
	void Set(PD::GC::PKT_GC_ACK_GUILD_LIST_READ::GuildList& InData);
	void Set(PD::GC::PKT_GC_ACK_REQUESTED_JOIN_GUILD_LIST_READ::GuildList& InData);
	void Set(const FGsGuildData& InData);

public:
	void SetIntroduction(const FString& InStr);

	FString GetIntroduction() const { return _introduction; }
	bool IsExistGuild() const { return (INVALID_GUILD_DB_ID != _guildDBId) ? true : false; }
	// 나중에 테이블 참고로 바뀔까봐 함수로 만들어 둠
	uint8 GetMaxMemberNum() const { return _maxMemberNum; }
	WorldId GetWorldId() const;
};

//----------------------------------------------------------------------------------------------------------------------
/**
 * 다른길드 상세보기 시 보여지는 정보
 */
class T1PROJECT_API FGsGuildDataDetail : public FGsGuildData
{
public:
	using Super = FGsGuildData;

public:
	FGsSecureCurrency _guildAsset;
	int32 _attendanceCount = 0; // 출석인원
	GuildDunGeonLevel _dungeonBestLevel = 0;
	time_t _createdTime = 0;
	uint32 _donationRank = 0;
	uint32 _dungeonClearRank = 0;
	uint16 _campLevel = 0;

public:
	FGsGuildDataDetail() = default;
	virtual ~FGsGuildDataDetail() {};

public:
	virtual void Empty() override;

public: 
	virtual FString GetNotice() const { return TEXT(""); }
	virtual bool IsAttandance() const { return false; }
	virtual bool IsJoinedDay() const { return false; }
	virtual GuildContribution GetContribution() const { return 0; }

public:
	virtual void SetDetail(const GuildDetail& InData) override;
	virtual void SetOtherGuildDetail(PD::GC::PKT_GC_ACK_GUILD_DETAIL_READ& InData) override;
};
//----------------------------------------------------------------------------------------------------------------------
/**
 * 내 길드 정보
 */
class T1PROJECT_API FGsGuildDataMine final : public FGsGuildDataDetail
{
public:
	using Super = FGsGuildDataDetail;

public:	
	GuildMemberGradeType _memberGrade;
	uint32 _authority; // 권한
	bool _attendance; // 내 출석 여부
	uint8 _donationCount; // 기부 횟수
	time_t _joinedTime; // 길드 가입시간(당일 출첵/기부 막는데 필요)
	GuildContribution _contribution = 0;
	bool _isJoinedDay = true; // 일일 리셋이 일어나면 false 처리함

	// 외교정보
	TArray<FGsGuildDiplomacyDataEnemy> _enemyList;
	TArray<FGsGuildDiplomacyDataAlly> _allyList;
	TArray<FGsGuildDiplomacyDataState> _stateList;

	TArray<FString> _SNSLinkList;

public:
	time_t _noticeLastUpdateTime = 0;
	time_t _enemyLastUpdateTime = 0;
	time_t _allyLastUpdateTime = 0;
	time_t _stateLastUpdateTime = 0;
	time_t _sanctumLastUpdateTime = 0;

protected:
	GuildZealPoint _zealPoint;
	FString _notice;

public:
	FGsGuildDataMine() = default;
	virtual ~FGsGuildDataMine() {};

public:
	virtual void Empty() override;
	virtual void SetDetail(const GuildDetail& InData) override;

	virtual FString GetNotice() const override { return _notice; }
	virtual bool IsAttandance() const override { return _attendance; }
	virtual bool IsJoinedDay() const override { return _isJoinedDay; }
	virtual GuildContribution GetContribution() const override { return _contribution; }

public:
	void SetMyGuildInfo(PD::GC::PKT_GC_MY_GUILD_INFO_READ& InData); // 첫 접속시 
	void SetUserGuildInfo(PD::SC::PKT_SC_USER_GUILD_INFO_READ& InData); // 생성/가입 시
	void Set(PD::GC::PKT_GC_ACK_OPEN_GUILD_READ& InData);
	void SetNotice(const FString& InStr, bool bUpdateTime);
	void SetSNSLink(int32 InIndex, const FString& InURL);
	void SetSanctumOccupySuccessTime();
	void ResetDaily();

	// 퀘스트
	void SetZealPoint(GuildZealPoint InPoint) { _zealPoint = InPoint; }
	GuildZealPoint GetZealPoint() const { return _zealPoint; }

	// 외교
	void AddEnemyData(PD::GC::PKT_GC_ACK_GUILD_HOSTILE_ADD_READ& InData);
	void AddEnemyData(PD::GC::PKT_GC_INFORM_GUILD_HOSTILE_ADD_READ& InData);
	void RemoveEnemyData(GuildDBId InGuildDBId);
	void AddAllyData(PD::GC::PKT_GC_INFORM_GUILD_ALLY_READ& InData);
	void AddAllyData(FGsGuildDiplomacyDataAlly& InData);
	void RemoveAllyData(GuildDBId InGuildDBId);
	void AddAllyStateData(FGsGuildDiplomacyDataState& InData, bool bCheckDuplicate = true);
	void RemoveAllyStateData(GuildDBId InGuildDBId);
	
	FGsGuildDiplomacyDataEnemy* GetEnemyData(GuildDBId InGuildDBId);
	const FGsGuildDiplomacyDataEnemy* GetEnemyDataByName(const FString& InGuildName) const;
	bool IsEnemy(GuildDBId InGuildDBId) const;

	FGsGuildDiplomacyDataAlly* GetAllyData(GuildDBId InGuildDBId);	
	const FGsGuildDiplomacyDataAlly* GetAllyDataByName(const FString& InGuildName) const;
	bool IsAlly(GuildDBId InGuildDBId) const;

	FGsGuildDiplomacyDataState* GetAllyStateData(GuildDBId InGuildDBId);
	const FGsGuildDiplomacyDataState* GetStateDataByName(const FString& InGuildName) const;
	bool IsAllyState(GuildDBId InGuildDBId) const;
	
	// 외교중(적대/동맹)인 길드일 경우 T, 아니면 F
	bool IsExistInDiplomacyList(GuildDBId InGuildDBId) const;
};
