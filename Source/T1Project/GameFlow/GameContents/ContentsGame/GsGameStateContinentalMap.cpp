#include "GsGameStateContinentalMap.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsSpaceCrackManager.h"

#include "Map/WorldMap/GsContinentalSpaceCrackMapInfo.h"
#include "Map/WorldMap/GsRegionGroupInfoBase.h"

#include "Runtime/DataCenter/Public/DataSchema/Common/GsSchemaMinMaxInteger.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/SpaceCrack/GsSchemaSpaceCrackData.h"

#include "UI/UIContent/Window/GsUIWindowContinentalMap.h"

#include "UTIL/GsTableUtil.h"

#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/WorldMap/GsSchemaContinentalMapData.h"


FGsGameStateContinentalMap::FGsGameStateContinentalMap() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsContinentalMap)
{
}


void FGsGameStateContinentalMap::Enter()
{
	Super::Enter();

	if (UGsUIManager* uiManager = GUI())
	{
		auto widget = uiManager->OpenAndGetWidget(TEXT("WindowContinentalMap"));
		if (widget.IsValid())
		{
			_windowUI = Cast<UGsUIWindowContinentalMap>(widget);
		}
	}

	// hide navigation bar(22/03/10)
	// https://jira.com2us.com/wiki/pages/viewpage.action?pageId=266919064
	CloseNavigationBar();
}
void FGsGameStateContinentalMap::Exit()
{
	// 주의: Exit 시 Window 닫기를 하지 않음(UI Stack과 State연계로직 때문. OnBack, CloseAll등의 로직에 의해 닫음) 
	_windowUI = nullptr;

	Super::Exit();
}
void FGsGameStateContinentalMap::MakeContinentalSpaceCrackListSortInfo(int In_continentalMapId)
{
	TArray<FGsContinentalSpaceCrackMapInfo*> arraySpaceCrackSortInfo;

	FGsRegionGroupInfoBase* findInfo = GSWorldMap()->GetSpaceCrackGroupData(In_continentalMapId);
	if (findInfo == nullptr)
	{
		return;
	}
	bool isActiveExist = false;
	bool isDeactiveExist = false;

	for (int mapId : findInfo->_arrayRegionId)
	{
		const FGsSchemaMapData* mapTable = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(mapId);
		if (mapTable == nullptr)
		{
			return;
		}

		FGsContinentalSpaceCrackMapInfo* sortInfo = new FGsContinentalSpaceCrackMapInfo();

		sortInfo->_mapId = mapId;
		sortInfo->_isSpawned =  GSSpaceCrack()->IsSpaceCrackSpawn(mapId);
		if (sortInfo->_isSpawned == true)
		{
			isActiveExist = true;
		}
		else
		{
			isDeactiveExist = true;
		}

		sortInfo->_mapName = mapTable->MapStringId;

		const FGsSchemaSpaceCrackData*  crackTable = GSWorldMap()->FindSpaceCrackData(mapId);
		if (crackTable != nullptr)
		{
			sortInfo->_minTimeMinute = crackTable->activationTime.Min;
			sortInfo->_maxTimeMinute = crackTable->activationTime.Max;
		}


		arraySpaceCrackSortInfo.Add(sortInfo);
	}

	// if at least active 1, deactive 1, make line deco info
	if (isActiveExist == true &&
		isDeactiveExist == true)
	{
		FGsContinentalSpaceCrackMapInfo* sortInfo = new FGsContinentalSpaceCrackMapInfo();
		sortInfo->_isDecoLine = true;
		arraySpaceCrackSortInfo.Add(sortInfo);
	}


	arraySpaceCrackSortInfo.Sort([](FGsContinentalSpaceCrackMapInfo& lhs, FGsContinentalSpaceCrackMapInfo& rhs)
		{
			// 1. deco line exist
			// spawned & deco: spawned front
			// despawned & deco: deco front			
			if (lhs._isDecoLine == true)
			{
				return !rhs._isSpawned;
			}				
			else if (rhs._isDecoLine == true)
			{
				return lhs._isSpawned;
			}

			int lhsSpawned = (int)lhs._isSpawned;
			int rhsSpawned = (int)rhs._isSpawned;


			// 2. spawned map go to front
			if (lhsSpawned != rhsSpawned)
			{				
				return lhsSpawned > rhsSpawned;
			}
			// 3. map id(lower id go to front)
			return lhs._mapId < rhs._mapId;
		});


	GSWorldMap()->SetArraySpaceCrackSortInfo(arraySpaceCrackSortInfo);
}