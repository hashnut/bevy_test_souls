#include "GSRegionMapIconParty.h"

#include "Party/GsPartyMemberInfo.h"

// 타이머에의한 업데이트
void UGsRegionMapIconParty::UpdateTimmer()
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
void UGsRegionMapIconParty::Tick(float In_delta)
{
	if (_memberInfo == nullptr)
	{
		return;
	}

	FVector nowPos = _memberInfo->GetPos();

	// 위치가 바뀌었다
	if (nowPos != _oldObjPos)
	{
		_oldObjPos = nowPos;
		_targetObjPosDirty = true;
	}
}

FVector2D UGsRegionMapIconParty::GetPos()
{
	if (_memberInfo == nullptr)
	{
		return FVector2D::ZeroVector;
	}

	FVector pos = _memberInfo->GetPos();
	return FVector2D(pos);
}