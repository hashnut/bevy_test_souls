#include "GsRegionMapIconTreasureMonster.h"

//#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "Management/ScopeGame/GsWorldMapManager.h"


void UGsRegionMapIconTreasureMonster::Tick(float In_delta)
{
	if (_localPosDirty)
	{
		_localPosDirty = false;
		// 아이콘 위치 갱신
		UpdatePos();
	}
}

FVector2D UGsRegionMapIconTreasureMonster::GetPos()
{	
	auto info = GSWorldMap()->GetTreasureMonsterMapInfo(_seperateId, GSWorldMap()->GetShowRegionMapId());
	if(info.IsNoTreasureMonster()) return FVector2D::ZeroVector;

	return FVector2D(info._position);
}

void UGsRegionMapIconTreasureMonster::Finalize()
{
	_seperateId = 0;
}