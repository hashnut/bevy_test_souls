#include "GsRegionMapIconInvasionNPC.h"

#include "Map/Worldmap/GsRegionInvadeIconInfo.h"

void UGsRegionMapIconInvasionNPC::UpdateTimmer()
{
	// party pos changed
	if (true == _targetObjPosDirty)
	{
		_targetObjPosDirty = false;
		// 갱신
		UpdatePos();
	}
}

// 매틱 갱신
void UGsRegionMapIconInvasionNPC::Tick(float In_delta)
{
	if (_iconInfo == nullptr)
	{
		return;
	}

	FVector nowPos = _iconInfo->_pos;

	// 위치가 바뀌었다
	if (nowPos != _oldObjPos)
	{
		_oldObjPos = nowPos;
		_targetObjPosDirty = true;
	}
}

FVector2D UGsRegionMapIconInvasionNPC::GetPos()
{
	if (_iconInfo == nullptr)
	{
		return FVector2D::ZeroVector;
	}

	FVector pos = _iconInfo->_pos;
	return FVector2D(pos);
}