// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

/**
 * 길드 멤버정보
 */
struct T1PROJECT_API FGsGuildMemberData final
{
public:
	UserDBId _userDBId;
	FString _userName;
	Level _level;
	GuildMemberGradeType _memberGrade;
	int32 _weeklyContribution;
	int32 _accumContribution;
	time_t _lastLoginTimeOriginal;
	time_t _joinedTime = 0;
	bool _attendance; // 출석 여부

public:
	FGsGuildMemberData() = default;
	virtual ~FGsGuildMemberData() = default;

public:
	void Set(PD::GC::PKT_GC_ACK_GUILD_MEMBER_LIST_READ::MemberList& InData);
};

//----------------------------------------------------------------------------------------------------------------------
/**
 * 가입 대기자 정보
 */
struct T1PROJECT_API FGsGuildJoinWaitingUser final
{
public:
	UserDBId _userDBId;
	FString _userName;
	Level _level;

public:
	FGsGuildJoinWaitingUser() = default;
	explicit FGsGuildJoinWaitingUser(PD::GC::PKT_GC_ACK_WAITING_JOIN_GUILD_USER_LIST_READ::UserList& InData);
	virtual ~FGsGuildJoinWaitingUser() = default;
};