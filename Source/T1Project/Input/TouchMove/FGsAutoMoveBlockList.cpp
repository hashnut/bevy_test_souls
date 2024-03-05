// Fill out your copyright notice in the Description page of Project Settings.

#include "FGsAutoMoveBlockList.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Dungeon/GsDungeonGroupBase.h"
#include "../Management/ScopeGlobal/GsLevelManager.h"
#include "../Management/GsScopeGlobal.h"
#include "../Management/GsScopeHolder.h"
#include "../Management/ScopeGame/GsDungeonManager.h"

#include "Guild/GsGuildHelper.h"

#include "UTIL/GsGameObjectUtil.h"

#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"

#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "GameObject/Fence/GsFenceHandler.h"


bool FGsAutoMoveBlockList::IsAutoMovePossibility()
{
	if (false == FGsAutoMoveBlockList::IsArenaCheck())
		return false;

	if (false == FGsAutoMoveBlockList::IsMapTypeCheck())
		return false;

	if (false == FGsAutoMoveBlockList::IsContentTypeCheck())
	{
		return false;
	}

	// 기사단 영지에서는 퀘스트 진행 못하게 막음
	// https://jira.com2us.com/jira/browse/C2URWQ-8027
	if (true == FGsGuildHelper::IsInAgit())
	{
		return false;
	}

	// 대화 연출 진입전 서버 send 후 응답 대기중인지
	// true면 대기중이라 이동 못함
	bool isMovePossible = false;
	if (false == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE))
	{
		isMovePossible = true;
	}
	return isMovePossible;
}

//맵 타입 체크
bool FGsAutoMoveBlockList::IsMapTypeCheck()
{
	if (const FGsSchemaMapData* mapData = GLevel()->GetCurrentMapInfo())
	{
		if (MapType::WORLD_BOSS == mapData->mapType)
		{
			return false;
		}
		
		// 2023/3/16 PKT - 스토리 던전 외 던전 지역은 모두 Quest 자동이동을 막는다.
		const auto& dungeonGroup = GSDungeon()->FindGroupDataByMapId(mapData->id);
		if (dungeonGroup && false == dungeonGroup->GetSchemaData()->checkQuest)
		{
			return false;
		}
	}

	return true;
}

//컨텐츠 타입 체크 
bool FGsAutoMoveBlockList::IsContentTypeCheck()
{
	if (const FGsSchemaMapData* mapData = GLevel()->GetCurrentMapInfo())
	{
		if (MapContentsType::SPACE_CRACK == mapData->ContentsType)
		{
			return false;
		}
	}

	return true;
}

bool FGsAutoMoveBlockList::IsArenaCheck()
{
	FGsFenceHandler* fenceHandler = GFence();
	if (nullptr == fenceHandler)
		return false;

	// 대결장 안에 있으면 자동 이동을 막는다.
	if (fenceHandler->IsEnteredTerritoryType(TerritoryType::ARENA))
		return false;

	return true;
}
