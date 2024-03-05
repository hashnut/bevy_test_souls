#pragma once

#include "Scan/GsScanMemberInfo.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"

/*
*  스캔 몬스터 정보
*/

class FGsScanMemberInfoMonster : public FGsScanMemberInfo
{

	// 생성자
public:
	FGsScanMemberInfoMonster(int In_orderNumber,
		TWeakObjectPtr<UGsGameObjectBase> In_targetObject,
		TWeakObjectPtr<UGsGameObjectBase> In_localObject) :
		FGsScanMemberInfo(In_orderNumber, In_targetObject, In_localObject)
	{
		UpdateName();
	}

	// get
public:
	// npc rank 가져오기
	CreatureRankType GetNPCRank();
	// 퀘스트 타입 가져오기
	bool GetQuestType(OUT QuestType& Out_type);

	bool IsTreasureMonster();
};