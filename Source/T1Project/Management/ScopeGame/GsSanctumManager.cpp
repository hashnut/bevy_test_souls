// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSanctumManager.h"
#include "DataSchema/Sanctum/GsSchemaSanctumConfig.h"
#include "DataSchema/Sanctum/GsSchemaSanctumData.h"
#include "DataSchema/Sanctum/GsSchemaSanctumNexusData.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "UTIL/GsTableUtil.h"
#include "T1Project.h"


FGsSanctumManager::~FGsSanctumManager()
{

}

const FGsSchemaSanctumConfig* FGsSanctumManager::GetTableSanctumConfig()
{
	if (const UGsTable* table = FGsSchemaSanctumConfig::GetStaticTable())
	{
		TArray<const FGsSchemaSanctumConfig*> rows;
		if (table->GetAllRows(rows))
		{
			if (rows.IsValidIndex(0))
			{
				return rows[0];
			}
		}
	}

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[Sanctum] Fail to find FGsSchemaSanctumConfig"));
#endif

	return nullptr;
}

const FGsSchemaSanctumData* FGsSanctumManager::GetTableSanctumData(SanctumAreaId InSanctumId)
{
	if (const FGsSchemaSanctumData* sanctumData =
		UGsTableUtil::FindRowById<UGsTableSanctumData, FGsSchemaSanctumData>(InSanctumId))
	{
		return sanctumData;
	}

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[Sanctum] Fail to find FGsSchemaSanctumData. SanctumAreaId: %d"), InSanctumId);
#endif

	return nullptr;
}

const FGsSchemaSanctumNexusData* FGsSanctumManager::GetTableSanctumNexusDataBySanctumId(SanctumAreaId InSanctumId)
{
	if (const FGsSchemaSanctumData* sanctumData = GetTableSanctumData(InSanctumId))
	{
		if (const FGsSchemaSanctumNexusData* nexusData = sanctumData->sanctumNexusId.GetRow())
		{
			return nexusData;
		}
	}

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[Sanctum] Fail to find FGsSchemaSanctumNexusData. SanctumAreaId: %d"), InSanctumId);
#endif

	return nullptr;
}

const FGsSchemaSanctumNexusData* FGsSanctumManager::GetTableSanctumNexusData(SanctumNexusId InNexusId)
{
	if (const FGsSchemaSanctumNexusData* sanctumData =
		UGsTableUtil::FindRowById<UGsTableSanctumNexusData, FGsSchemaSanctumNexusData>(InNexusId))
	{
		return sanctumData;
	}

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[Sanctum] Fail to find FGsSchemaSanctumNexusData. SanctumNexusId: %d"), InNexusId);
#endif

	return nullptr;
}

void FGsSanctumManager::Initialize()
{

}

void FGsSanctumManager::Finalize()
{
	_sanctumDataMap.Empty();
}

void FGsSanctumManager::InitializeAfterResourceDownload()
{
	// 테이블을 읽어와서 _dataMap를 미리 생성해둔다
	TArray<const FGsSchemaSanctumData*> rowList;
	if (const UGsTable* table = FGsSchemaSanctumData::GetStaticTable())
	{
		table->GetAllRows(rowList);
	}

	for (const FGsSchemaSanctumData* row : rowList)
	{
		TSharedPtr<FGsSanctumData> sanctumData = MakeShared<FGsSanctumData>();
		if (sanctumData.IsValid())
		{
			sanctumData->SetTableData(row);
			_sanctumDataMap.Emplace(row->sanctumAreaId, sanctumData);
		}
	}
}

void FGsSanctumManager::SetMapSanctumInfo(PD::GC::PKT_GC_ACK_MAP_SANCTUM_INFO_READ* InPacket)
{
	// 주의: 없는 정보는 리스트가 비어서 오기 때문에, 해당 맵의 모든 성소를 Clear 시킨 후 새 데이터를 채운다
	for (TPair<SanctumAreaId, TSharedPtr<FGsSanctumData>>& itPair : _sanctumDataMap)
	{
		if (InPacket->MapId() == itPair.Value->GetMapId())
		{
			itPair.Value->Clear();
		}
	}

	for (PD::GC::PKT_GC_ACK_MAP_SANCTUM_INFO_READ::OccupancyListIterator iter = InPacket->GetFirstOccupancyListIterator();
		iter != InPacket->GetLastOccupancyListIterator(); ++iter)
	{
		if (TSharedPtr<FGsSanctumData>* sanctumDataPtr = _sanctumDataMap.Find(iter->SanctumAreaId()))
		{
			TSharedPtr<FGsSanctumData> sanctumData = *sanctumDataPtr;
			if (sanctumData.IsValid())
			{
				sanctumData->SetGuildData(*iter);
			}
		}
	}
}

void FGsSanctumManager::PrepareBattle()
{
	for (TPair<SanctumAreaId, TSharedPtr<FGsSanctumData>>& itPair : _sanctumDataMap)
	{
		itPair.Value->PrepareBattle();
	}
}

TWeakPtr<FGsSanctumData> FGsSanctumManager::GetSanctumData(SanctumAreaId InSanctumId)
{
	if (TSharedPtr<FGsSanctumData>* dataPtr = _sanctumDataMap.Find(InSanctumId))
	{
		return *dataPtr;
	}

	return nullptr;
}
