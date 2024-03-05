#pragma once

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "GsMultiLevelRankDefine.h"
#include "DataSchema/MultiLevelRank/GsSchemaMultiLevelRank.h"
#include "DataSchema/MultiLevelRank/GsSchemaMultiLevelRankSlot.h"


/************************************************************************/
/* Slot (분기)														    */
/************************************************************************/
struct T1PROJECT_API FGsMultiLevelRankSlotData
{
	// slot data
	const FGsSchemaMultiLevelRankSlot* _slotSchema = nullptr;

	// slot state( slot 진행 여부 )
	MultiLevelRankSlotState _slotState = MultiLevelRankSlotState::LOCK;

	// Linked Data
	FGsMultiLevelRankSlotData* _linkedPrevSlot = nullptr;

	// owner data
	struct FGsMultiLevelRankData* _ownerRank = nullptr;

	// story quest 진행 상태
	RankStoryState _storyState = RankStoryState::Normal;
};


/************************************************************************/
/* Rank(랭크)													        */
/************************************************************************/
struct T1PROJECT_API FGsMultiLevelRankData
{	
	// rank data
	const FGsSchemaMultiLevelRank* _rankSchema = nullptr;

	// slot data
	TArray<FGsMultiLevelRankSlotData> _slotList;

	// 랭크가 위치한 서약 정보
	int32 _pledgeGroupId = 0;
};