#include "GsScanMemberInfoMonster.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayerBase.h"

#include "Management/ScopeGlobal/GsQuestManager.h"

#include "GameObject/Quest/GsQuestHandler.h"

#include "Quest/Management/GsQuestTargetManagement.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcBalance.h"

// npc rank 가져오기
CreatureRankType FGsScanMemberInfoMonster::GetNPCRank()
{
	if (_targetObject == nullptr)
	{
		return CreatureRankType::MAX;
	}

	UGsGameObjectNonPlayerBase* npc = _targetObject->CastGameObject<UGsGameObjectNonPlayerBase>();
	if (nullptr == npc)
	{
		return CreatureRankType::MAX;
	}
	
	const FGsSchemaNpcData* npcData = npc->GetNpcData();
	if (nullptr == npcData)
	{
		return CreatureRankType::MAX;
	}

	return npcData->grade;
}

// 퀘스트 타입 가져오기
bool FGsScanMemberInfoMonster::GetQuestType(OUT QuestType& Out_type)
{
	if (_targetObject == nullptr)
	{
		return false;
	}

	UGsGameObjectNonPlayerBase* npc = _targetObject->CastGameObject<UGsGameObjectNonPlayerBase>();
	if (nullptr == npc)
	{
		return false;
	}

	int npcTblId = npc->GetNpcTblId();

	if (FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler())
	{
		if (FGsQuestTargetManagement* questTargetManagement = questHandler->GetQuestTargetManagement())
		{
			QuestType outQuestType;
			if (questTargetManagement->IsQuestMonster(npcTblId, outQuestType) == true)
			{
				Out_type = outQuestType;
				return true;
			}
		}
	}

	return false;
}

bool FGsScanMemberInfoMonster::IsTreasureMonster()
{
	if (_targetObject == nullptr)
	{
		return false;
	}

	UGsGameObjectNonPlayerBase* npc = _targetObject->CastGameObject<UGsGameObjectNonPlayerBase>();
	if (nullptr == npc)
	{
		return false;
	}

	return npc->IsTreasureMonster();
}