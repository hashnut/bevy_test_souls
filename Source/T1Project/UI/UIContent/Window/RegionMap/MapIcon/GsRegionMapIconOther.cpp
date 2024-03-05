#include "GsRegionMapIconOther.h"

#include "Management/ScopeGame/GsWorldMapManager.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"

#include "Engine/World.h"
#include "Engine/Public/TimerManager.h"


// 타이머에의한 업데이트
void UGsRegionMapIconOther::UpdateTimmer()
{
	// 타겟 오브젝트가 움직였을때
	if (true == _targetObjPosDirty)
	{
		_targetObjPosDirty = false;
		// 갱신
		UpdatePos();
	}
}


// 매틱 갱신
void UGsRegionMapIconOther::Tick(float In_delta)
{
	if (_targetObject == nullptr)
	{
		return;
	}

	FVector nowPos = _targetObject->GetLocation();

	// 위치가 바뀌었다
	if (nowPos != _oldObjPos)
	{
		_oldObjPos = nowPos;
		_targetObjPosDirty = true;
	}
}