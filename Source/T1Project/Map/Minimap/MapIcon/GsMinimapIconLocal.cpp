#include "GsMinimapIconLocal.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"

#include "UI/UIContent/HUD/Minimap/GsUIMinimapIcon.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"

#include "UMG/Public/Slate/WidgetTransform.h"

// 매틱 갱신
void UGsMinimapIconLocal::Tick(float In_delta)
{
	if (_localPosDirty)
	{
		_localPosDirty = false;
		// 아이콘 위치 갱신
		UpdatePos();
	}

	if (nullptr != _targetObj)
	{
		FRotator rot = _targetObj->GetRotation();
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
void UGsMinimapIconLocal::UpdateRotation()
{
	if (nullptr == _targetObj ||
		nullptr == _icon)
	{
		return;
	}
	FRotator rot =  _targetObj->GetRotation();

	// yaw 회전일껄
	float yawRot = rot.Yaw + 90.0f;
	_icon->SetRenderTransformAngle(yawRot);
}

// 아이콘 위치 세팅
void UGsMinimapIconLocal::SetIconPos(FVector2D In_objectPos)
{
	Super::SetIconPos(In_objectPos);
}