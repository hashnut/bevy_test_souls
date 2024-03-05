
#include "GsContentsServerHelper.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "../T1Project.h"
#include "DataSchema/InterServer/World/GsSchemaWorldData.h"


EGsContentsServerType FGsContentsServerHelper::GetContetnsSertverAtLocation()
{
	EGsContentsServerType outType = EGsContentsServerType::eOriginal;

	if (true == IsLocatedOnDifferentServerThanMain())
	{
		if (IsLocatedOnInvasionServer())
		{	// 2023/8/29 PKT - 침공전을 위해 다른 서버로 넘어간 상태
			outType = EGsContentsServerType::eInvasionBattle;
		}
		else if (IsLocatedOnInterDungeon())
		{	// 2023/8/29 PKT - 시련의 전당등을 위해 넘어간 상태
			outType = EGsContentsServerType::eInterDungeon;
		}
		else if (IsLocatedOnInterField())
		{	// 2023/8/29 PKT - 인터 필드를 위해 넘어간 상태
			outType = EGsContentsServerType::eInterField;
		}
		else if (IsLocatedOnBattleArenaGuildWar())
		{
			outType = EGsContentsServerType::eBattleArena;
		}

#ifdef WITH_EDITOR
		if (EGsContentsServerType::eOriginal == outType)
		{
			GSLOG(Error, TEXT("Invaldate Contetns Server Type At Location"));
		}		
#endif // WITH_EDITOR
	}

	return outType;
}

// 2023/8/28 PKT - 본섭이 아닌 다른 서버에 와 있는가?( 사유는 모르나 본서버에 위치하고 있지 않을때 )
bool FGsContentsServerHelper::IsLocatedOnDifferentServerThanMain()
{
	if (GGameData())
	{
		// 2023/8/29 PKT - 현재 서버 타입이 Normal이면 일반 서버(본 서버) 이다.
		return EnterWorldServerType::NORMAL != GGameData()->GetLoadWorldType();
	}

#ifdef WITH_EDITOR
	GSLOG(Error, TEXT("GameData Manager does not exist."));
#endif // WITH_EDITOR

	return false;
}

// 2023/8/28 PKT - 침공전 컨텐츠를 위해 다른 서버로 넘어가 있는 상태( Local Player가 침공자 일때만 해당 )
bool FGsContentsServerHelper::IsLocatedOnInvasionServer()
{
	if (GGameData())
	{
		return EnterWorldServerType::INVADE_SERVER == GGameData()->GetLoadWorldType();
	}

#ifdef WITH_EDITOR
	GSLOG(Error, TEXT("GameData Manager does not exist."));
#endif // WITH_EDITOR
	return false;
}

bool FGsContentsServerHelper::IsLocatedOnInterServer()
{
	if (GGameData())
	{
		return EnterWorldServerType::INTER_SERVER == GGameData()->GetLoadWorldType();
	}

#ifdef WITH_EDITOR
	GSLOG(Error, TEXT("GameData Manager does not exist."));
#endif // WITH_EDITOR
	return false;
}

// 2023/8/28 PKT - 시련의 전당 컨텐츠를 위해 다른 서버로 넘어가 있는 상태
bool FGsContentsServerHelper::IsLocatedOnInterDungeon()
{
	if (GGameData() && GLevel())
	{
		// 2023/8/29 PKT - Inter Server 이면서 맵타입이 시련의 전당이라면..
		return true == IsLocatedOnInterServer() && MapType::ELITE_DUNGEON_1 == GLevel()->GetMapType();
	}
#ifdef WITH_EDITOR
	GSLOG(Error, TEXT("GameData Manager or Level Manager does not exist."));
#endif // WITH_EDITOR
	return false;
}

// 2023/8/28 PKT - Battle Arena 상태
bool FGsContentsServerHelper::IsLocatedOnBattleArenaGuildWar()
{
	if (GGameData() && GLevel())
	{
		// 2023/8/29 PKT - Inter Server 이면서 맵타입이 Inter Field 이면 인터필드..
		return true == IsLocatedOnInterServer() && MapType::GUILD_WAR == GLevel()->GetMapType();
	}
#ifdef WITH_EDITOR
	GSLOG(Error, TEXT("GameData Manager or Level Manager does not exist."));
#endif // WITH_EDITOR
	return false;
}

// 2023/8/28 PKT - Inter Field 상태
bool FGsContentsServerHelper::IsLocatedOnInterField()
{
	if (GGameData() && GLevel())
	{
		// 2023/8/29 PKT - Inter Server 이면서 맵타입이 Inter Field 이면 인터필드..
		return true == IsLocatedOnInterServer() && MapType::INTER_FIELD == GLevel()->GetMapType();
	}
#ifdef WITH_EDITOR
	GSLOG(Error, TEXT("GameData Manager or Level Manager does not exist."));
#endif // WITH_EDITOR
	return false;
}

const FGsSchemaWorldData* FGsContentsServerHelper::GetWorldDataWithWorldId(WorldId InWorldId)
{
	const UGsTableWorldData* worldDataTable = Cast<UGsTableWorldData>(FGsSchemaWorldData::GetStaticTable());

	TArray<const FGsSchemaWorldData*> worldDataSet;
	if (worldDataTable && worldDataTable->GetAllRows(worldDataSet))
	{
		for (const auto& data : worldDataSet)
		{
			if (data->worldId == InWorldId)
			{
				return data;
			}
		}
	}
	return nullptr;
}

const FGsSchemaWorldData* FGsContentsServerHelper::GetWorldDataWithWorldId(const FText& InPrefix)
{
	const UGsTableWorldData* worldDataTable = Cast<UGsTableWorldData>(FGsSchemaWorldData::GetStaticTable());

	TArray<const FGsSchemaWorldData*> worldDataSet;
	if (worldDataTable && worldDataTable->GetAllRows(worldDataSet))
	{
		for (const auto& data : worldDataSet)
		{
			if (data->worldPrefixText.EqualTo(InPrefix))
			{
				return data;
			}
		}
	}
	return nullptr;
}

FText FGsContentsServerHelper::GetWorldIPrefixdWithWorldId(WorldId InWorldId)
{
	const FGsSchemaWorldData* worldData = GetWorldDataWithWorldId(InWorldId);

	return worldData ? worldData->worldPrefixText : FText::GetEmpty();
}

WorldId FGsContentsServerHelper::GetWorldIdWithWorldPrefix(const FText& InPrefix)
{
	const FGsSchemaWorldData* worldData = GetWorldDataWithWorldId(InPrefix);

	return worldData ? worldData->worldId : INVALID_WORLD_ID;
}