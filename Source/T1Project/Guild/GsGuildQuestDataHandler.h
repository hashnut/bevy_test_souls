// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Classes/GsPool.h"
#include "Guild/GsGuildQuestData.h"

namespace PD
{
	namespace GC
	{
		struct PKT_GC_ACK_GUILD_QUEST_INFO_READ;		
	}

	namespace SC
	{
		struct PKT_SC_ACK_QUEST_GUILD_REFRESH_READ;
	}
}

/**
 * 기사단 퀘스트 데이터 관리
 */
class T1PROJECT_API FGsGuildQuestDataHandler final
{
private:
	time_t _zealPointResetTime = 0;
	time_t _questResetTime = 0;
	int32 _refreshStep = 0; // 주의: 서버 초기값이 1이라고 함
	time_t _lastQuestResetTime = 0;
	int32 _completeCount = 0;

	TArray<FGsGuildQuestData*> _questList;
	TGsPoolDefault<FGsGuildQuestData> _poolQuestData;	

public:
	FGsGuildQuestDataHandler() = default;
	~FGsGuildQuestDataHandler();

public:
	void Clear();

	void SetLastQuestResetTime(time_t InTime);
	void SetQuestInfo(PD::GC::PKT_GC_ACK_GUILD_QUEST_INFO_READ* InPacket);
	void SetQuestRefresh(PD::SC::PKT_SC_ACK_QUEST_GUILD_REFRESH_READ* InPacket);
	
	void SetGuildQuestState(StoryId InStoryId, QuestId InQuestId, QuestState InState);
	void SetCompleteCount(int32 InCount) { _completeCount = InCount; }

public:
	FGsGuildQuestData* GetQuestData(StoryId InStoryId);
	int32 GetRefreshStep() const;
	time_t GetZealPointResetTime() const { return _zealPointResetTime; }
	time_t GetQuestResetTime() const { return _questResetTime; }
	time_t GetLastQuestResetTime() const { return _lastQuestResetTime; }
	TArray<FGsGuildQuestData*>& GetQuestList() { return _questList; }

	// 수락 갯수 체크(퀘스트 수락 막기 위함)
	int32 GetAcceptCount() const;
	// 완료한 갯수 체크(UI 표시에 사용)
	int32 GetCompleteCount() const { return _completeCount; }
};
