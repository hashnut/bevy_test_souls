#include "GsRegionMapIconLocal.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"

#include "UI/UIContent/Common/Icon/GsUIIconBase.h"

// 매틱 갱신
void UGsRegionMapIconLocal::Tick(float In_delta)
{
	if (_localPosDirty)
	{
		_localPosDirty = false;
		// 아이콘 위치 갱신
		UpdatePos();
	}

	if (nullptr != _targetObject)
	{
		FRotator rot = _targetObject->GetRotation();
		_localRotDirty = !_oldLocalRot.Equals(rot);
		if (_localRotDirty)
		{
			_oldLocalRot = rot;
		}
	}

	if (_localRotDirty)
	{
		_localRotDirty = false;
		UpdateRotation();
	}
}

// 회전 갱신
void UGsRegionMapIconLocal::UpdateRotation()
{
	if (nullptr == _targetObject)
	{
		return;
	}
	FRotator rot = _targetObject->GetRotation();

	// yaw 회전일껄
	float yawRot = rot.Yaw + 90.0f;
	_icon->SetRenderTransformAngle(yawRot);
}