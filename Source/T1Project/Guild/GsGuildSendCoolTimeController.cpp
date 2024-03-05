// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildSendCoolTimeController.h"
#include "Net/GsNetSendServiceGuild.h"
#include "Net/GsNetSendServiceQuest.h"


void FGsGuildSendCoolTimeController::Initialize()
{
	_checkerMap.Empty();
	_checkerMap.Emplace(EGsCoolTimeType::GuildList, FGsIntervalChecker(5.f));
	_checkerMap.Emplace(EGsCoolTimeType::WaitingGuildList, FGsIntervalChecker(10.f));
	_checkerMap.Emplace(EGsCoolTimeType::SearchGuild, FGsIntervalChecker(3.f));
	_checkerMap.Emplace(EGsCoolTimeType::RefreshGuildList, FGsIntervalChecker(10.f));
	_checkerMap.Emplace(EGsCoolTimeType::MyGuildInfo, FGsIntervalChecker(10.f));
	_checkerMap.Emplace(EGsCoolTimeType::MemberList, FGsIntervalChecker(10.f));
	_checkerMap.Emplace(EGsCoolTimeType::WaitingUserList, FGsIntervalChecker(5.f));
	_checkerMap.Emplace(EGsCoolTimeType::AuthorityList, FGsIntervalChecker(5.f));
	_checkerMap.Emplace(EGsCoolTimeType::BuildingList, FGsIntervalChecker(10.f));
	_checkerMap.Emplace(EGsCoolTimeType::JoinType, FGsIntervalChecker(1.f));
	_checkerMap.Emplace(EGsCoolTimeType::EnemyList, FGsIntervalChecker(5.f));
	_checkerMap.Emplace(EGsCoolTimeType::AllyList, FGsIntervalChecker(5.f));
	_checkerMap.Emplace(EGsCoolTimeType::AllyStateList, FGsIntervalChecker(5.f));
	_checkerMap.Emplace(EGsCoolTimeType::QuestInfo, FGsIntervalChecker(5.f));
	_checkerMap.Emplace(EGsCoolTimeType::QuestRefresh, FGsIntervalChecker(0.5f));
	_checkerMap.Emplace(EGsCoolTimeType::AgitDungeonRaid, FGsIntervalChecker(5.f));
	_checkerMap.Emplace(EGsCoolTimeType::AgitDungeonRaidBookmarkList, FGsIntervalChecker(10.f));
}

void FGsGuildSendCoolTimeController::ResetAllCoolTime()
{
	for (TPair<EGsCoolTimeType, FGsIntervalChecker>& itPair : _checkerMap)
	{
		itPair.Value.Reset();
	}
}

void FGsGuildSendCoolTimeController::ResetCoolTime(EGsCoolTimeType InType)
{
	if (FGsIntervalChecker* checker = _checkerMap.Find(InType))
	{
		checker->Reset();
	}
}

bool FGsGuildSendCoolTimeController::IsCoolTimeOver(EGsCoolTimeType InType)
{
	if (FGsIntervalChecker* checker = _checkerMap.Find(InType))
	{
		return checker->IsTimeOver();
	}

	return true;
}

void FGsGuildSendCoolTimeController::UpdateCoolTime(EGsCoolTimeType InType)
{
	if (FGsIntervalChecker* checker = _checkerMap.Find(InType))
	{
		checker->UpdateTime();
	}
}

bool FGsGuildSendCoolTimeController::SendRequestGuildList()
{
	if (IsCoolTimeOver(EGsCoolTimeType::GuildList))
	{
		FGsNetSendServiceGuild::SendReqGuildList();
		UpdateCoolTime(EGsCoolTimeType::GuildList);

		return true;
	}

	return false;
}

bool FGsGuildSendCoolTimeController::SendRequestRequestedJoinGuildList()
{
	if (IsCoolTimeOver(EGsCoolTimeType::WaitingGuildList))
	{
		FGsNetSendServiceGuild::SendReqRequestedJoinGuildList();
		UpdateCoolTime(EGsCoolTimeType::WaitingGuildList);
		return true;
	}

	return false;
}

bool FGsGuildSendCoolTimeController::SendRequestSearchGuild(const FString& InData, bool bSameServerOnly)
{
	if (IsCoolTimeOver(EGsCoolTimeType::SearchGuild))
	{
		FGsNetSendServiceGuild::SendReqSearchGuild(InData, bSameServerOnly);
		UpdateCoolTime(EGsCoolTimeType::SearchGuild);

		// 길드리스트랑 동일한 패킷으로 받으므로, 리스트를 다시 받아야 한다
		ResetCoolTime(EGsCoolTimeType::GuildList);
		return true;
	}

	return false;
}

bool FGsGuildSendCoolTimeController::SendRequestRefreshGuildList()
{
	if (IsCoolTimeOver(EGsCoolTimeType::RefreshGuildList))
	{
		FGsNetSendServiceGuild::SendReqGuildList();
		UpdateCoolTime(EGsCoolTimeType::RefreshGuildList);
		return true;
	}

	return false;
}

bool FGsGuildSendCoolTimeController::SendRequestOpenGuild()
{
	if (IsCoolTimeOver(EGsCoolTimeType::MyGuildInfo))
	{
		FGsNetSendServiceGuild::SendReqOpenGuild();
		UpdateCoolTime(EGsCoolTimeType::MyGuildInfo);
		return true;
	}

	return false;
}

bool FGsGuildSendCoolTimeController::SendRequestMemberList()
{
	if (IsCoolTimeOver(EGsCoolTimeType::MemberList))
	{
		FGsNetSendServiceGuild::SendReqGuildMemberList();
		UpdateCoolTime(EGsCoolTimeType::MemberList);
		return true;
	}

	return false;
}

bool FGsGuildSendCoolTimeController::SendRequestJoinWaitingUserList()
{
	if (IsCoolTimeOver(EGsCoolTimeType::WaitingUserList))
	{
		FGsNetSendServiceGuild::SendReqWaitingJoinGuildUserList();
		UpdateCoolTime(EGsCoolTimeType::WaitingUserList);
		return true;
	}

	return false;
}

bool FGsGuildSendCoolTimeController::SendRequestAuthorityList()
{
	if (IsCoolTimeOver(EGsCoolTimeType::AuthorityList))
	{
		FGsNetSendServiceGuild::SendReqAuthorityList();
		UpdateCoolTime(EGsCoolTimeType::AuthorityList);
		return true;
	}

	return false;
}

bool FGsGuildSendCoolTimeController::SendRequestBuildingList()
{
	if (IsCoolTimeOver(EGsCoolTimeType::BuildingList))
	{
		FGsNetSendServiceGuild::SendReqGuildBuildingList();
		UpdateCoolTime(EGsCoolTimeType::BuildingList);
		return true;
	}

	return false;
}

bool FGsGuildSendCoolTimeController::SendRequestChangeJoinType(GuildJoinType InType)
{
	if (IsCoolTimeOver(EGsCoolTimeType::JoinType))
	{
		FGsNetSendServiceGuild::SendReqChangeJoinType(InType);
		UpdateCoolTime(EGsCoolTimeType::JoinType);
		return true;
	}

	return false;
}

void FGsGuildSendCoolTimeController::SendReqEnemyList()
{
	if (IsCoolTimeOver(EGsCoolTimeType::EnemyList))
	{
		FGsNetSendServiceGuild::SendReqHostileList();
		UpdateCoolTime(EGsCoolTimeType::EnemyList);
	}
}

void FGsGuildSendCoolTimeController::SendReqAllyList()
{
	if (IsCoolTimeOver(EGsCoolTimeType::AllyList))
	{
		FGsNetSendServiceGuild::SendReqAllyList();
		UpdateCoolTime(EGsCoolTimeType::AllyList);
	}
}

void FGsGuildSendCoolTimeController::SendReqAllyStateList()
{
	if (IsCoolTimeOver(EGsCoolTimeType::AllyStateList))
	{
		FGsNetSendServiceGuild::SendReqAllyStateList();
		UpdateCoolTime(EGsCoolTimeType::AllyStateList);
	}
}

bool FGsGuildSendCoolTimeController::SendReqQuestInfo()
{
	if (IsCoolTimeOver(EGsCoolTimeType::QuestInfo))
	{
		FGsNetSendServiceGuild::SendReqGuildQuestInfo();
		UpdateCoolTime(EGsCoolTimeType::QuestInfo);
		return true;
	}

	return false;
}

bool FGsGuildSendCoolTimeController::SendReqQuestRefresh()
{
	if (IsCoolTimeOver(EGsCoolTimeType::QuestRefresh))
	{
		FGsNetSendServiceQuest::SendReqQuestGuildRefresh();
		UpdateCoolTime(EGsCoolTimeType::QuestRefresh);
		return true;
	}

	return false;
}

bool FGsGuildSendCoolTimeController::SendReqAgitDungeonRaid()
{
	if (IsCoolTimeOver(EGsCoolTimeType::AgitDungeonRaid))
	{
		FGsNetSendServiceGuild::SendReqAgitDungeonRaid();
		UpdateCoolTime(EGsCoolTimeType::AgitDungeonRaid);
		return true;
	}

	return false;
}

void FGsGuildSendCoolTimeController::SendReqAgitDungeonRaidBookmarkList()
{
	if (IsCoolTimeOver(EGsCoolTimeType::AgitDungeonRaidBookmarkList))
	{
		FGsNetSendServiceGuild::SendReqAgitDungeonRaidBookmarkList();
		UpdateCoolTime(EGsCoolTimeType::AgitDungeonRaidBookmarkList);
	}
}
