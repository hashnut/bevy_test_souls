#include "GsMinimapIconNpc.h"

#include "Management/ScopeGame/GsMinimapManager.h"

#include "Engine/World.h"
#include "Engine/Public/TimerManager.h"

void UGsMinimapIconNpc::Initialize(UGsUIIconBase* In_iconWidget,
	EGsMapIconType In_iconType, 
	float In_updateTime,
	UGsMinimapSceneImage* In_sceneImage,
	int In_depth, int64 In_gameId, FVector2D In_pos,
	int In_tableId)
{
	_icon = In_iconWidget;
	_iconType = In_iconType;
	_sceneImage = In_sceneImage;
	_iconDepth = In_depth;
	_gameId = In_gameId;
	_npcPos = In_pos;
	_tableId = In_tableId;

	UWorld* world = GSMinimap()->GetWorld();

	if (world == nullptr)
	{
		return;
	}

	world->GetTimerManager().SetTimer(_timerHandle,
		FTimerDelegate::CreateUObject(this, &UGsMinimapIconNpc::UpdateTimmer), In_updateTime, true);

	SetIconHidden(false);
}

// 위치 구하기
FVector2D UGsMinimapIconNpc::GetPos()
{
	return _npcPos;
}


// 타이머에의한 업데이트
void UGsMinimapIconNpc::UpdateTimmer()
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
void UGsMinimapIconNpc::Finalize()
{

	if (UWorld* world = GSMinimap()->GetWorld())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
		_timerHandle.Invalidate();
	}

	Super::Finalize();
}