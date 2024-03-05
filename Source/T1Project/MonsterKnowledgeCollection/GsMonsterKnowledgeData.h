#pragma once
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsMonsterCollectionBook
{
	uint32 id = INVALID_MONSTER_KNOWLEDGE_ID;
	Level level = 0;
	MonsterKnowledgeExp exp = INVALID_MONSTER_KNOWLEDGE_EXP;
	uint32 rewardPoint = 0;
};

struct FGsMonsterCollectionBookResult
{
	uint32 id = INVALID_MONSTER_KNOWLEDGE_ID;
	uint32 rewardPoint = 0;
	bool isLevelUp = false;
};