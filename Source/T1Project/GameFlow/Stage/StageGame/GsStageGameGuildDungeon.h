// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStageGameBase.h"
#include "Message/GsMessageContentDungeon.h"
#include "Message/GsMessageUI.h"
namespace PD
{
	namespace SC
	{
		struct PKT_SC_INFORM_GUILD_DUNGEON_RANK_READ;
	}
}
//------------------------------------------------------------------------------
// 스테이지 관리할때 필요한 메모리 할당관리자(상속구조도 지원하기 위함)
//------------------------------------------------------------------------------
class UGsUIHUDDungeonTime;
struct IGsMessageParam;
struct FGsContributionData;

struct FGsUserData
{
public:
	int32 _level;
	FString _userName;
};

class T1PROJECT_API FGsStageGameGuildDungeon : public FGsStageGameBase
{
private:
	TWeakObjectPtr<class UGsUIHUDDungeonTime>	_hudDungeonTime;
	MsgGuildDungeonHandleArray _guildDungeonMsg;
	TArray<TSharedPtr<FGsContributionData>> _contributionArray;
	FDateTime _expireTime;

	

public:
	FGsStageGameGuildDungeon();
	virtual ~FGsStageGameGuildDungeon();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;

private:
	void OnExitGuildDungeon(const IGsMessageParam* inParam);
	void OnGuildDungeonClear(const IGsMessageParam* inParam);
	void ClearContribution();
	void OnObserving(const IGsMessageParam* inParam);
	
	
public:
	void SetContributionArray(PD::SC::PKT_SC_INFORM_GUILD_DUNGEON_RANK_READ* inPkt);
	void SetExpireTime(FDateTime& inTime);
	FDateTime GetExpireTime(){return _expireTime;}

	void GetSortedRankingList(OUT TArray<TWeakPtr<FGsContributionData>>& OutList);
};
