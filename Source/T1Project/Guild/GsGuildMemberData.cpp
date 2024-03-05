// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildMemberData.h"


//----------------------------------------------------------------------------------------------------------------------
void FGsGuildMemberData::Set(PD::GC::PKT_GC_ACK_GUILD_MEMBER_LIST_READ::MemberList& InData)
{
	_userDBId = InData.UserDBId();
	_userName = InData.UserName();
	_level = InData.UserLevel();
	_memberGrade = InData.MemberGrade();
	_weeklyContribution = InData.ContributionWeekend();
	_accumContribution = InData.ContributionAccuml();
	_lastLoginTimeOriginal = InData.LastLoginTime();
	_attendance = InData.CheckAttendance();
	_joinedTime = InData.GuildJoinedTime();
}

//----------------------------------------------------------------------------------------------------------------------
FGsGuildJoinWaitingUser::FGsGuildJoinWaitingUser(PD::GC::PKT_GC_ACK_WAITING_JOIN_GUILD_USER_LIST_READ::UserList& InData)
{
	_userDBId = InData.UserDBId();
	_userName = InData.UserName();
	_level = InData.UserLevel();
}
