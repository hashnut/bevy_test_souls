#include "GsMinimapIconAutoMovePath.h"

#include "Management/ScopeGame/GsMinimapManager.h"

#include "Engine/World.h"
#include "Engine/Public/TimerManager.h"

void UGsMinimapIconAutoMovePath::Initialize(UGsUIIconBase* In_iconWidget,
	EGsMapIconType In_iconType,
	float In_updateTime,
	UGsMinimapSceneImage* In_sceneImage,
	int In_depth, FVector2D In_pos,
	float In_deleteDist)
{
	_icon = In_iconWidget;
	_iconType = In_iconType;
	_sceneImage = In_sceneImage;
	_iconDepth = In_depth;
	_pathPos = In_pos;
	_deleteDist =In_deleteDist;

	_deleteSquaredDist = In_deleteDist * In_deleteDist;

	UWorld* world = GSMinimap()->GetWorld();

	if (world == nullptr)
	{
		return;
	}

	world->GetTimerManager().SetTimer(_timerHandle,
		FTimerDelegate::CreateUObject(this, &UGsMinimapIconAutoMovePath::UpdateTimmer), In_updateTime, true);

	SetIconHidden(false);
}

// 위치 구하기
FVector2D UGsMinimapIconAutoMovePath::GetPos()
{
	return _pathPos;
}


// 타이머에의한 업데이트
void UGsMinimapIconAutoMovePath::UpdateTimmer()
{
	// 로컬이 움직이면
	if (true == _localPosDirty)
	{
		_localPosDirty = false;
		// 갱신
		UpdatePos();
	}
}

// 클리어
void UGsMinimapIconAutoMovePath::Finalize()
{

	if (UWorld* world = GSMinimap()->GetWorld())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
		_timerHandle.Invalidate();
	}

	Super::Finalize();
}

// 아이콘 삭제해야하는가
bool UGsMinimapIconAutoMovePath::IsDeleteIcon(FVector2D In_localPos)
{
	float currentDistSquared = FVector2D::DistSquared(In_localPos, _pathPos);
	return (currentDistSquared <= _deleteSquaredDist)? true: false;
}