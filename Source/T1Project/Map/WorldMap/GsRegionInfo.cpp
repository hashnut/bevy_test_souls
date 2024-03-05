#include "GsRegionInfo.h"

#include "Map/WorldMap/GsRegionMonsterInfo.h"

void FGsRegionInfo::Finalize()
{
	if (_arrayPortalInfo.Num() != 0)
	{
		for (auto portalInfoIter : _arrayPortalInfo)
		{
			if (portalInfoIter != nullptr)
			{
				delete portalInfoIter;
			}
		}
		_arrayPortalInfo.Empty();
	}
	// 침공 넘어가서 포털 정보는 위에거 참조만해서 위에서 delete하고 empty만 하자
	if (_arrayInvasionEnterPortalInfo.Num() != 0)
	{
		_arrayInvasionEnterPortalInfo.Empty();
	}

	if (_arrayTeleportInfo.Num() != 0)
	{
		for (auto teleportInfoIter : _arrayTeleportInfo)
		{
			if (teleportInfoIter != nullptr)
			{
				delete teleportInfoIter;
			}
		}
		_arrayTeleportInfo.Empty();
	}


	if (_arrayMonsterInfo.Num() != 0)
	{
		for (auto monsterInfoIter : _arrayMonsterInfo)
		{
			if (monsterInfoIter != nullptr)
			{
				monsterInfoIter->Finalize();
				delete monsterInfoIter;
			}
		}
		_arrayMonsterInfo.Empty();
	}

	if (_arrayRegionMapSanctumInfo.Num() != 0)
	{
		for (auto regionMapSanctumIter : _arrayRegionMapSanctumInfo)
		{
			if (regionMapSanctumIter != nullptr)
			{
				delete regionMapSanctumIter;
			}
		}
		_arrayRegionMapSanctumInfo.Empty();
	}

	if (_mapMonsterSameIdInfo.Num() != 0)
	{
		for (auto monsterSameIdInfo : _mapMonsterSameIdInfo)
		{
			if (nullptr != monsterSameIdInfo.Value)
			{
				monsterSameIdInfo.Value->Finalize();
				delete monsterSameIdInfo.Value;
			}
		}
		_mapMonsterSameIdInfo.Empty();
	}
}