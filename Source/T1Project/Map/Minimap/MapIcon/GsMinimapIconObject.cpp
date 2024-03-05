#include "GsMinimapIconObject.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Data/GsGameObjectData.h"

#include "UI/UIContent/HUD/Minimap/GsUIMinimapIcon.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"

#include "Map/Minimap/SceneImage/GsMinimapSceneImage.h"

#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "UMG/Public/Slate/WidgetTransform.h"

#include "T1Project.h"
#include "UTIL/GsGameObjectUtil.h"

UGsMinimapIconObject::~UGsMinimapIconObject()
{
}
// 클리어
void UGsMinimapIconObject::Finalize()
{
	Super::Finalize();

	if (nullptr != _targetObj)
	{
		_targetObj = nullptr;
	}

}
// 초기화
void UGsMinimapIconObject::Initialize(UGsGameObjectBase* In_targetObj,
	UGsUIIconBase* In_iconWidget,
	EGsMapIconType In_iconType, float In_updateTime, UGsMinimapSceneImage* In_sceneImage,
	int In_depth, int64 In_gameId)
{
	_targetObj = In_targetObj;
	_iconType = In_iconType;
	_icon = In_iconWidget;
	_sceneImage = In_sceneImage;
	_iconDepth = In_depth;
	_gameId = In_gameId;

	SetIconHidden(false);
}
// 위치 구하기
FVector2D UGsMinimapIconObject::GetPos()
{
	return GetActorPos2D();
}


FVector2D UGsMinimapIconObject::GetActorPos2D()
{
	if (nullptr == _targetObj)
	{
		return FVector2D::ZeroVector;
	}

	AActor* targetActor = _targetObj->GetActor();
	if (nullptr == targetActor)
	{
		return FVector2D::ZeroVector;
	}
	FVector pos = targetActor->GetActorLocation();
	return FVector2D(pos);
}
// 적인가
bool UGsMinimapIconObject::IsEnemy()
{
	if (_targetObj == nullptr )
	{
		return false;
	}

	return UGsGameObjectUtil::IsEnemy(_targetObj);
}

// 테이블 아이디 구하기
int UGsMinimapIconObject::GetTableId()
{
	if (_targetObj == nullptr)
	{
		return -1;
	}
	FGsGameObjectData* data = _targetObj->GetData();
	if (data == nullptr)
	{
		return -1;
	}

	return data->GetTableId();
}