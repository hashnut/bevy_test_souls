// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSanctumData.h"
#include "DataSchema/Sanctum/GsSchemaSanctumData.h"
#include "DataSchema/Sanctum/GsSchemaSanctumNexusData.h"
#include "Guild/GsGuildHelper.h"
#include "UTIL/GsTableUtil.h"


FGsSanctumData::~FGsSanctumData()
{
	_tableData = nullptr;
}

PassivityId FGsSanctumData::GetNexusPassivityIdStatic(SanctumAreaId InSanctumId, int32 InCampLevel)
{
	const FGsSchemaSanctumData* sanctumData = 
		UGsTableUtil::FindRowById<UGsTableSanctumData, FGsSchemaSanctumData>(InSanctumId);
	if (sanctumData)
	{
		if (const FGsSchemaSanctumNexusData* nexusData = sanctumData->sanctumNexusId.GetRow())
		{
			int32 levelIndex = InCampLevel - 1;

			// 인덱스 값이라 1을 뺀다
			if (nexusData->constructionBuffList.IsValidIndex(levelIndex))
			{
				if (const FGsSchemaPassivitySet* passivitySet = nexusData->constructionBuffList[levelIndex].GetRow())
				{
					return passivitySet->id;
				}
			}
		}
	}

	return INVALID_PASSIVITY_ID;
}

void FGsSanctumData::SetTableData(const FGsSchemaSanctumData* InData)
{
	if (nullptr == InData)
	{
		_sanctumId = INVALID_SANCTUM_AREA_ID;
		_tableData = nullptr;

		return;
	}

	_sanctumId = InData->sanctumAreaId;

	if (const FGsSchemaMapData* mapData = InData->sanctumMapId.GetRow())
	{
		_mapId = mapData->id;
	}
	else
	{
		_mapId = INVALID_MAP_ID;
	}

	_tableData = InData;
}

void FGsSanctumData::SetGuildData(PD::GC::PKT_GC_ACK_MAP_SANCTUM_INFO_READ::OccupancyList& InData)
{
	_guildDBId = InData.GuildDBId();
	_guildName = InData.GuildName();
	_guildEmblemId = InData.GuildEmblemId();
	_guildNexusLevel = InData.GuildLevel(); // 수호탑 레벨이다
}

void FGsSanctumData::Clear()
{
	_guildDBId = INVALID_GUILD_DB_ID;
	_guildName.Empty();
	_guildEmblemId = INVALID_GUILD_EMBLEM_ID;
	_guildNexusLevel = 0;
}

void FGsSanctumData::PrepareBattle()
{
	Clear();
}

FText FGsSanctumData::GetMapName() const
{
	if (_tableData)
	{
		if (const FGsSchemaMapData* mapData = _tableData->sanctumMapId.GetRow())
		{
			return mapData->MapStringId;
		}
	}

	return FText::GetEmpty();
}

FText FGsSanctumData::GetSanctumName() const
{
	return _tableData ? _tableData->nameText : FText::GetEmpty();
}

int32 FGsSanctumData::GetRequiredNexusLevel() const
{
	return _tableData ? _tableData->needNexusLevel : 0;
}

PassivityId FGsSanctumData::GetSanctumPassivityId() const
{
	if (_tableData)
	{
		if (const FGsSchemaPassivitySet* passivitySet = _tableData->rewardPassivityId.GetRow())
		{
			return passivitySet->id;
		}
	}
	
	return INVALID_PASSIVITY_ID;
}

PassivityId FGsSanctumData::GetNexusPassivityId(int32 InCampLevel) const
{
	if (_tableData)
	{
		if (const FGsSchemaSanctumNexusData* nexusData = _tableData->sanctumNexusId.GetRow())
		{
			int32 levelIndex = InCampLevel - 1;

			// 인덱스 값이라 1을 뺀다
			if (nexusData->constructionBuffList.IsValidIndex(levelIndex))
			{
				if (const FGsSchemaPassivitySet* passivitySet = nexusData->constructionBuffList[levelIndex].GetRow())
				{
					return passivitySet->id;
				}
			}
		}
	}

	return INVALID_PASSIVITY_ID;
}

bool FGsSanctumData::IsOccupied() const
{
	return (INVALID_GUILD_DB_ID != _guildDBId) ? true : false;
}
