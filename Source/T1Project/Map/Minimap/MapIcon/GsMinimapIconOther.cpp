#include "GsMinimapIconOther.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"

#include "Management/ScopeGame/GsMinimapManager.h"

#include "Engine/World.h"
#include "Engine/Public/TimerManager.h"



// 초기화
void UGsMinimapIconOther::Initialize(UGsGameObjectBase* In_targetObj, 
	UGsUIIconBase* In_iconWidget,
	EGsMapIconType In_iconType, float In_updateTime, UGsMinimapSceneImage* In_sceneImage, int In_depth, int64 In_gameId)
{
	// 부모 처리
	Super::Initialize(In_targetObj, In_iconWidget, In_iconType, In_updateTime, In_sceneImage, In_depth, In_gameId);
	UWorld* world = GSMinimap()->GetWorld();

	if (world == nullptr)
	{
		return;
	}

	world->GetTimerManager().SetTimer(_timerHandle,
		FTimerDelegate::CreateUObject(this, &UGsMinimapIconOther::UpdateTimmer), In_updateTime, true);

	SetIconHidden(false);
}

// 클리어
void UGsMinimapIconOther::Finalize()
{

	if(UWorld* world = GSMinimap()->GetWorld())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
		_timerHandle.Invalidate();
	}
	
	Super::Finalize();
}

void UGsMinimapIconOther::Tick(float In_delta)
{
	if (_targetObj == nullptr)
	{
		return;
	}

	FVector nowPos = _targetObj->GetLocation();

	// 위치가 바뀌었다
	if (nowPos != _oldObjPos)
	{
		_oldObjPos = nowPos;
		_targetObjPosDirty = true;
	}
	
}

// 타이머에의한 업데이트
void UGsMinimapIconOther::UpdateTimmer()
{
	// 내가 움직이던 로컬이 움직이던
	if ( true == _targetObjPosDirty  || true ==  _localPosDirty )
	{
		_targetObjPosDirty = false;
		_localPosDirty = false;
		// 갱신
		UpdatePos();
	}
}