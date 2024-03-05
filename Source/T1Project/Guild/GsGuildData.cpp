// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildData.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "UTIL/GsText.h"
#include "Guild/GsGuildHelper.h"
#include "T1Project.h"
#include "Classes/SecureVar/GsSecureVar.h"


//----------------------------------------------------------------------------------------------------------------------
void FGsGuildData::Empty()
{
	_guildDBId = INVALID_GUILD_DB_ID;
	_guildName.Empty();
	_masterName.Empty();
	_introduction.Empty();
	_guildLevel = INVALID_GUILD_LEVEL;
	_memberNum = 0;
	_maxMemberNum = 0;
	_joinLimitLevel = 0;	
	_joinType = GuildJoinType::MAX;
	_emblemId = INVALID_GUILD_EMBLEM_ID;
	_homePlanetWorldId.value = 0;
}

void FGsGuildData::SetDetail(const GuildDetail& InData)
{
	_guildDBId = InData.mGuildDBId;
	//_guildName
	//_masterName
	//_introduction
	_guildLevel = InData.mGuildLevel;
	//_memberNum
	//_maxMemberNum
	_joinLimitLevel = InData.mLimitedUserLevel;
	_joinType = InData.mGuildJoinType;
	_emblemId = InData.mGuildEmblemId;
	_homePlanetWorldId.value = InData.mCurrentHomePlanetWorldId.value;
}

void FGsGuildData::SetOtherGuildDetail(PD::GC::PKT_GC_ACK_GUILD_DETAIL_READ& InData)
{
	SetDetail(InData.ConstRefGuildDetailInfo());

	_guildName = InData.GuildName();
	_masterName = InData.CaptainName();
	_memberNum = InData.CurUserCnt();
	_maxMemberNum = InData.MaxUserCnt();

	SetIntroduction(InData.GuildDesc());
}

void FGsGuildData::Set(PD::GC::PKT_GC_ACK_GUILD_LIST_READ::GuildList& InData)
{
	_guildDBId = InData.GuildDBId();
	_guildName = InData.GuildName();
	_guildLevel = InData.GuildLevel();
	_memberNum = InData.CurUserCnt();
	_maxMemberNum = InData.MaxUserCnt();
	_masterName = InData.CaptainName();
	_joinLimitLevel = InData.LimitedUserLevel();	
	_joinType = InData.GuildJoinType();
	_emblemId = InData.GuildEmblemId();
	_homePlanetWorldId.value = InData.ConstRefCurrentHomePlanetWorldId().value;

	SetIntroduction(InData.GuildDesc());
}

void FGsGuildData::Set(PD::GC::PKT_GC_ACK_REQUESTED_JOIN_GUILD_LIST_READ::GuildList& InData)
{
	_guildDBId = InData.GuildDBId();
	_guildName = InData.GuildName();
	_guildLevel = InData.GuildLevel();
	_memberNum = InData.CurUserCnt();
	_maxMemberNum = InData.MaxUserCnt();
	_masterName = InData.CaptainName();
	_joinLimitLevel = InData.LimitedUserLevel();
	_joinType = GuildJoinType::APPROVAL;
	//_emblemId = 
	//_homePlanetWorldId.value =

	SetIntroduction(InData.GuildDesc());
}

void FGsGuildData::Set(const FGsGuildData& InData)
{
	_guildDBId = InData._guildDBId;
	_guildName = InData._guildName;
	_masterName = InData._masterName;
	_introduction = InData._introduction;
	_guildLevel = InData._guildLevel;
	_memberNum = InData._memberNum;
	_maxMemberNum = InData._maxMemberNum;
	_joinLimitLevel = InData._joinLimitLevel;
	_joinType = InData._joinType;
	_emblemId = InData._emblemId;
	_homePlanetWorldId.value = InData._homePlanetWorldId.value;
}

void FGsGuildData::SetIntroduction(const FString& InStr)
{
	_introduction = InStr;
	//FGsTextUtil::ChangeBanSentence(InStr, [this](bool success, FString inText, FString outText) {
	//		_introduction = outText;
	//	});
}

WorldId FGsGuildData::GetWorldId() const
{
	if (0 == _homePlanetWorldId.value)
	{
		return INVALID_WORLD_ID;
	}

	return _homePlanetWorldId.st.worldId;
}
//----------------------------------------------------------------------------------------------------------------------
void FGsGuildDataDetail::Empty()
{
	Super::Empty();

	_guildAsset.Set(0);
	_attendanceCount = 0;
	_dungeonBestLevel = 0;
	_createdTime = 0;
	_donationRank = 0;
	_dungeonClearRank = 0;
	_campLevel = 0;
}

void FGsGuildDataDetail::SetDetail(const GuildDetail& InData)
{
	Super::SetDetail(InData);

	_guildAsset.Set(InData.mGuildAsset);
	_dungeonBestLevel = InData.mDungeonBestLevel;
	_createdTime = InData.mCreatedTime;
	_campLevel = InData.mCampLevel;
}

void FGsGuildDataDetail::SetOtherGuildDetail(PD::GC::PKT_GC_ACK_GUILD_DETAIL_READ& InData)
{
	Super::SetOtherGuildDetail(InData);

	_attendanceCount = InData.AttendanceUserNum();
	_donationRank = InData.GuildContributionRank();
	_dungeonClearRank = InData.GuildDungeonClearRank();
}
//----------------------------------------------------------------------------------------------------------------------
void FGsGuildDataMine::Empty()
{
	FGsGuildDataDetail::Empty();

	_notice.Empty();
	_memberGrade = GuildMemberGradeType::UNKNOWN;
	_authority = GUILD_RIGHTS_LEVEL_NONE;
	_attendance = false;
	_donationCount = 0;
	_joinedTime = 0;
	_zealPoint = 0;
	_contribution = 0;

	_isJoinedDay = true;
	_sanctumLastUpdateTime = 0;

	_enemyList.Empty();
	_allyList.Empty();
	_stateList.Empty();

	_SNSLinkList.Empty();
}

void FGsGuildDataMine::SetDetail(const GuildDetail& InData)
{
	Super::SetDetail(InData);

	_zealPoint = InData.mGuildZealPoint;
	_contribution = InData.mGuildContribution;
	
	_noticeLastUpdateTime = InData.mLastNoticeTime;
	_enemyLastUpdateTime = InData.mLastHostileTime;
	_allyLastUpdateTime = InData.mLastAllyTime;
	_stateLastUpdateTime = InData.mLastAllyRequestTime;
}

void FGsGuildDataMine::SetMyGuildInfo(PD::GC::PKT_GC_MY_GUILD_INFO_READ& InData)
{
	const GuildDetail& guildDetail = InData.ConstRefGuildDetailInfo();
	SetDetail(guildDetail);

#if 0
#if WITH_EDITOR
	FDateTime testNoticeTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_noticeLastUpdateTime);
	FDateTime testEnemyTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_enemyLastUpdateTime);
	FDateTime testAllyTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_allyLastUpdateTime);
	FDateTime testStateTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_stateLastUpdateTime);
	FDateTime testSanctumTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(InData.LastSanctumCheckTime());

	GSLOG(Log, TEXT("[REDDOT] MyGuildInfo Notice: %s"), *(testNoticeTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] MyGuildInfo Enemy: %s"), *(testEnemyTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] MyGuildInfo Ally: %s"), *(testAllyTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] MyGuildInfo State: %s"), *(testStateTime.ToString()));
	GSLOG(Log, TEXT("[REDDOT] MyGuildInfo Sanctum: %s"), *(testSanctumTime.ToString()));
#endif
#endif

	_guildName = InData.GuildName();	
	SetIntroduction(InData.GuildDesc());
	SetNotice(InData.GuildNotice(), false);

	_memberGrade = InData.MemberGrade();
	_authority = InData.GuildRights();
	_attendance = InData.CheckAttendance();
	_joinedTime = InData.GuildJoinedTime();
	// 일일리셋 패킷 받으면 false로 바꾸는데, 다시 바뀌지 않도록 true일때만 체크
	if (_isJoinedDay)
	{
		_isJoinedDay = FGsGuildHelper::IsGuildJoinedDay(_joinedTime);
	}
	_sanctumLastUpdateTime = InData.LastSanctumCheckTime();

	_enemyList.Empty();
	for (PD::GC::PKT_GC_MY_GUILD_INFO_READ::HostileGuildListIterator iter = InData.GetFirstHostileGuildListIterator();
		iter != InData.GetLastHostileGuildListIterator(); ++iter)
	{
		FGsGuildDiplomacyDataEnemy enemyData;
		enemyData._guildDBId = iter->GuildId();

		_enemyList.Emplace(enemyData);
	}

	_allyList.Empty();
	TArray<GuildDBId> tempAllyList;
	InData.MakeAllyGuildListVector<TArray<GuildDBId>, GuildDBId>(tempAllyList);
	for (int i = 0; i < tempAllyList.Num(); ++i)
	{
		FGsGuildDiplomacyDataAlly allyData;
		allyData._guildDBId = tempAllyList[i];

		_allyList.Emplace(allyData);
	}

	_stateList.Empty();
	for (PD::GC::PKT_GC_MY_GUILD_INFO_READ::AllyGuildRequestListIterator iter = InData.GetFirstAllyGuildRequestListIterator();
		iter != InData.GetLastAllyGuildRequestListIterator(); ++iter)
	{
		FGsGuildDiplomacyDataState stateData;
		stateData._guildDBId = iter->GuildId();
		stateData._allyType = iter->Type();

		_stateList.Emplace(stateData);
	}
}

void FGsGuildDataMine::SetUserGuildInfo(PD::SC::PKT_SC_USER_GUILD_INFO_READ& InData)
{
	_guildDBId = InData.GuildDBId();
	_guildName = InData.GuildName(); // 가입 티커때문에 지우면 안됨
	_emblemId = InData.GuildEmblemId();

	_authority = InData.GuildRights();
	_attendance = false; // 첫 가입/생성 시 출석 체크 불가
	_guildLevel = InData.GuildLevel();
	_memberGrade = InData.MemberGrade();
}

void FGsGuildDataMine::Set(PD::GC::PKT_GC_ACK_OPEN_GUILD_READ& InData)
{
	SetDetail(InData.ConstRefGuildDetailInfo());

	_guildName = InData.GuildName();
	_memberGrade = InData.MemberGrade();
	_authority = InData.GuildRights();
	_attendance = InData.CheckAttendance();
	_masterName = InData.CaptainName();
	_guildLevel = InData.GuildLevel();
	_maxMemberNum = InData.MaxUserCnt(); // GuildLevel 테이블로 알수있어서 없어도 되긴 함
	_memberNum = InData.CurUserCnt();
	_donationCount = InData.DonateCnt();
	_attendanceCount = InData.AttendanceUserNum();
	_joinedTime = InData.GuildJoinedTime();
	_donationRank = InData.GuildContributionRank();
	_dungeonClearRank = InData.GuildDungeonClearRank();

	SetIntroduction(InData.GuildDesc());
	SetNotice(InData.GuildNotice(), false);

	_SNSLinkList.Empty();
	for (PD::GC::PKT_GC_ACK_OPEN_GUILD_READ::SnsUrlListIterator iter = InData.GetFirstSnsUrlListIterator();
		iter != InData.GetLastSnsUrlListIterator(); ++iter)
	{
		_SNSLinkList.Emplace(iter->SnsUrl());
	}

	// 일일리셋 패킷 받으면 false로 바꾸는데, 다시 바뀌지 않도록 true일때만 체크
	if (_isJoinedDay)
	{
		_isJoinedDay = FGsGuildHelper::IsGuildJoinedDay(_joinedTime);
	}
}

void FGsGuildDataMine::SetNotice(const FString& InStr, bool bUpdateTime)
{
	_notice = InStr;

	if (bUpdateTime)
	{
		_noticeLastUpdateTime = FGsTimeSyncUtil::GetServerNowSecond();
	}	

	//FGsTextUtil::ChangeBanSentence(InStr, [this](bool success, FString inText, FString outText) {
	//		_notice = outText;
	//	});
}

void FGsGuildDataMine::SetSNSLink(int32 InIndex, const FString& InURL)
{
	if (_SNSLinkList.Num() > InIndex)
	{
		_SNSLinkList[InIndex] = InURL;
		return;
	}

	// 비어있는 인덱스는 빈 스트링으로 채운 후 정보넣기
	int32 emptySlotCount = InIndex - _SNSLinkList.Num();
	if (0 < emptySlotCount)
	{
		// 빈 슬롯으로 채움
		for (int32 i = 0; i < emptySlotCount; ++i)
		{
			_SNSLinkList.Emplace(FString());
		}
	}
	_SNSLinkList.Emplace(InURL);
}

void FGsGuildDataMine::SetSanctumOccupySuccessTime()
{
	_sanctumLastUpdateTime = FGsTimeSyncUtil::GetServerNowSecond();
}

void FGsGuildDataMine::ResetDaily()
{
	_attendance = false;
	_donationCount = 0;
	_isJoinedDay = false;
}

void FGsGuildDataMine::AddEnemyData(PD::GC::PKT_GC_ACK_GUILD_HOSTILE_ADD_READ& InData)
{
	if (FGsGuildDiplomacyDataEnemy* enemyData = GetEnemyData(InData.GuildId()))
	{
		enemyData->SetData(InData);
		return;
	}

	FGsGuildDiplomacyDataEnemy newData;
	newData.SetData(InData);

	_enemyList.Emplace(newData);	
}

void FGsGuildDataMine::AddEnemyData(PD::GC::PKT_GC_INFORM_GUILD_HOSTILE_ADD_READ& InData)
{
	if (FGsGuildDiplomacyDataEnemy* enemyData = GetEnemyData(InData.GuildDBId()))
	{
		enemyData->SetNotifyData(InData);
		return;
	}

	FGsGuildDiplomacyDataEnemy newData;
	newData.SetNotifyData(InData);

	_enemyList.Emplace(newData);

	_enemyLastUpdateTime = FGsTimeSyncUtil::GetServerNowSecond();
}

void FGsGuildDataMine::RemoveEnemyData(GuildDBId InGuildDBId)
{
	for (int i = 0; i < _enemyList.Num(); ++i)
	{
		if (InGuildDBId == _enemyList[i]._guildDBId)
		{
			_enemyList.RemoveAt(i);
			return;
		}
	}
}

void FGsGuildDataMine::AddAllyData(PD::GC::PKT_GC_INFORM_GUILD_ALLY_READ& InData)
{
	if (FGsGuildDiplomacyDataAlly* allyData = GetAllyData(InData.GuildId()))
	{
		allyData->SetData(InData);
		return;
	}

	FGsGuildDiplomacyDataAlly newData;
	newData.SetData(InData);

	_allyList.Emplace(newData);

	_allyLastUpdateTime = FGsTimeSyncUtil::GetServerNowSecond();
}

void FGsGuildDataMine::AddAllyData(FGsGuildDiplomacyDataAlly& InData)
{
	if (FGsGuildDiplomacyDataAlly* data = GetAllyData(InData._guildDBId))
	{
		data->SetAllyData(&InData);
		return;
	}

	_allyList.Emplace(InData);
}

void FGsGuildDataMine::RemoveAllyData(GuildDBId InGuildDBId)
{
	for (int i = 0; i < _allyList.Num(); ++i)
	{
		if (InGuildDBId == _allyList[i]._guildDBId)
		{
			_allyList.RemoveAt(i);
			return;
		}
	}
}

void FGsGuildDataMine::AddAllyStateData(FGsGuildDiplomacyDataState& InData, bool bCheckDuplicate)
{
	if (bCheckDuplicate)
	{
		if (FGsGuildDiplomacyDataState* data = GetAllyStateData(InData._guildDBId))
		{
			data->SetStateData(&InData);
			return;
		}
	}

	_stateList.Emplace(InData);

	_stateLastUpdateTime = FGsTimeSyncUtil::GetServerNowSecond();
}

void FGsGuildDataMine::RemoveAllyStateData(GuildDBId InGuildDBId)
{
	for (int i = 0; i < _stateList.Num(); ++i)
	{
		if (InGuildDBId == _stateList[i]._guildDBId)
		{
			_stateList.RemoveAt(i);
			return;
		}
	}
}

FGsGuildDiplomacyDataEnemy* FGsGuildDataMine::GetEnemyData(GuildDBId InGuildDBId)
{
	for (int i = 0; i < _enemyList.Num(); ++i)
	{
		if (InGuildDBId == _enemyList[i]._guildDBId)
		{
			return &_enemyList[i];
		}
	}

	return nullptr;
}

const FGsGuildDiplomacyDataEnemy* FGsGuildDataMine::GetEnemyDataByName(const FString& InGuildName) const
{
	for (int i = 0; i < _enemyList.Num(); ++i)
	{
		if (InGuildName == _enemyList[i]._guildName)
		{
			return &_enemyList[i];
		}
	}

	return nullptr;
}

bool FGsGuildDataMine::IsEnemy(GuildDBId InGuildDBId) const
{
	for (int i = 0; i < _enemyList.Num(); ++i)
	{
		if (InGuildDBId == _enemyList[i]._guildDBId)
		{
			return true;
		}
	}

	return false;
}

FGsGuildDiplomacyDataAlly* FGsGuildDataMine::GetAllyData(GuildDBId InGuildDBId)
{
	for (int i = 0; i < _allyList.Num(); ++i)
	{
		if (InGuildDBId == _allyList[i]._guildDBId)
		{
			return &_allyList[i];
		}
	}

	return nullptr;
}

const FGsGuildDiplomacyDataAlly* FGsGuildDataMine::GetAllyDataByName(const FString& InGuildName) const
{
	for (int i = 0; i < _allyList.Num(); ++i)
	{
		if (InGuildName == _allyList[i]._guildName)
		{
			return &_allyList[i];
		}
	}

	return nullptr;
}

bool FGsGuildDataMine::IsAlly(GuildDBId InGuildDBId) const
{
	for (int i = 0; i < _allyList.Num(); ++i)
	{
		if (InGuildDBId == _allyList[i]._guildDBId)
		{
			return true;
		}
	}

	return false;
}

FGsGuildDiplomacyDataState* FGsGuildDataMine::GetAllyStateData(GuildDBId InGuildDBId)
{
	for (int i = 0; i < _stateList.Num(); ++i)
	{
		if (InGuildDBId == _stateList[i]._guildDBId)
		{
			return &_stateList[i];
		}
	}

	return nullptr;
}

const FGsGuildDiplomacyDataState* FGsGuildDataMine::GetStateDataByName(const FString& InGuildName) const
{
	for (int i = 0; i < _stateList.Num(); ++i)
	{
		if (InGuildName == _stateList[i]._guildName)
		{
			return &_stateList[i];
		}
	}

	return nullptr;
}

bool FGsGuildDataMine::IsAllyState(GuildDBId InGuildDBId) const
{
	for (int i = 0; i < _stateList.Num(); ++i)
	{
		if (InGuildDBId == _stateList[i]._guildDBId)
		{
			return true;
		}
	}

	return false;
}

bool FGsGuildDataMine::IsExistInDiplomacyList(GuildDBId InGuildDBId) const
{
	for (int i = 0; i < _enemyList.Num(); ++i)
	{
		if (InGuildDBId == _enemyList[i]._guildDBId)
		{
			return true;
		}
	}

	for (int i = 0; i < _allyList.Num(); ++i)
	{
		if (InGuildDBId == _allyList[i]._guildDBId)
		{
			return true;
		}
	}

	return false;
}
