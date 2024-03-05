// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildQuestData.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Guild/GsGuildHelper.h"
#include "DataSchema/Quest/Guild/GsSchemaQuestGuildStory.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Quest/Table/GsQuestGuildTableCacheSet.h"


void FGsGuildQuestData::Clear()
{

}

EGsGuildQuestStateType FGsGuildQuestData::GetQuestState() const
{
	switch (_questState)
	{
	case QuestState::ACCEPTED:
	{
		return EGsGuildQuestStateType::ACCEPTED;
	}
	case QuestState::COMPLETED:
	{
		return EGsGuildQuestStateType::COMPLETED;
	}
	case QuestState::REWARDED:	
	{
		return EGsGuildQuestStateType::REWARDED;
	}
	break;
	}

	return EGsGuildQuestStateType::NONE;
}

bool FGsGuildQuestData::IsValidQuest() const
{
	if (QuestState::DELETED == _questState ||
		QuestState::GIVEUP == _questState)
	{
		return false;
	}
	
	return true;
}

bool FGsGuildQuestData::IsAccepted() const
{
	return (QuestState::ACCEPTED == _questState) ? true : false;
}

bool FGsGuildQuestData::IsComplete() const
{
	if (IsValidQuest())
	{
		if (QuestState::COMPLETED == _questState ||
			QuestState::REWARDED == _questState)
		{
			return true;
		}
	}

	return false;
}