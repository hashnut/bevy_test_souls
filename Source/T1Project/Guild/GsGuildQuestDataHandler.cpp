// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildQuestDataHandler.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"


FGsGuildQuestDataHandler::~FGsGuildQuestDataHandler()
{
	Clear();
}

void FGsGuildQuestDataHandler::Clear()
{	
	_zealPointResetTime = 0;
	_questResetTime = 0;
	_refreshStep = 0;
	_lastQuestResetTime = 0;
	_completeCount = 0;

	_questList.Empty();
	_poolQuestData.RemovePool();
}

void FGsGuildQuestDataHandler::SetLastQuestResetTime(time_t InTime)
{
	_lastQuestResetTime = InTime;
}

void FGsGuildQuestDataHandler::SetQuestInfo(PD::GC::PKT_GC_ACK_GUILD_QUEST_INFO_READ* InPacket)
{
	_zealPointResetTime = InPacket->ZealPointResetTime();
	_questResetTime = InPacket->QuestResetTime();
	_refreshStep = InPacket->RefreshNumber();
	_completeCount = InPacket->QuestCompleteCount();

	_questList.Empty();
	_poolQuestData.ReleaseAll();

	for (PD::GC::PKT_GC_ACK_GUILD_QUEST_INFO_READ::QuestListIterator iter = InPacket->GetFirstQuestListIterator();
		iter != InPacket->GetLastQuestListIterator(); ++iter)
	{
		FGsGuildQuestData* questData = _poolQuestData.Claim();
		questData->Clear();
		questData->_storyId = iter->StoryId();
		questData->_questId = iter->QuestId();
		questData->_questState = iter->QuestState();

		_questList.Emplace(questData);
	}
}

void FGsGuildQuestDataHandler::SetQuestRefresh(PD::SC::PKT_SC_ACK_QUEST_GUILD_REFRESH_READ* InPacket)
{
	_refreshStep = InPacket->RefreshNumber();

	_questList.Empty();
	_poolQuestData.ReleaseAll();

	for (PD::SC::PKT_SC_ACK_QUEST_GUILD_REFRESH_READ::QuestListIterator iter = InPacket->GetFirstQuestListIterator();
		iter != InPacket->GetLastQuestListIterator(); ++iter)
	{
		FGsGuildQuestData* questData = _poolQuestData.Claim();
		questData->Clear();
		questData->_storyId = iter->StoryId();
		questData->_questId = iter->QuestId();
		questData->_questState = iter->QuestState();

		_questList.Emplace(questData);
	}
}

void FGsGuildQuestDataHandler::SetGuildQuestState(StoryId InStoryId, QuestId InQuestId, QuestState InState)
{
	if (FGsGuildQuestData* data = GetQuestData(InStoryId))
	{
		data->_questState = InState;
	}
}

FGsGuildQuestData* FGsGuildQuestDataHandler::GetQuestData(StoryId InStoryId)
{
	for (FGsGuildQuestData* iter : _questList)
	{
		if (InStoryId == iter->_storyId)
		{
			return iter;
		}
	}

	return nullptr;
}

int32 FGsGuildQuestDataHandler::GetRefreshStep() const
{
	// 주의: 서버 초기값이 1이라고 한다. 따라서 클라에서 쓸때는 1을 빼서 사용한다.
	return _refreshStep;
}

int32 FGsGuildQuestDataHandler::GetAcceptCount() const
{
	int32 count = 0;
	for (FGsGuildQuestData* iter : _questList)
	{
		if (iter->IsAccepted())
		{
			++count;
		}
	}

	return count;
}
