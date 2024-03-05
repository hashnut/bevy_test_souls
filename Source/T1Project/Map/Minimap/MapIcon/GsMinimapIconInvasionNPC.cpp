#include "GsMinimapIconInvasionNPC.h"

#include "Management/ScopeGame/GsMinimapManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"

#include "Map/Worldmap/GsRegionInvadeIconInfo.h"
#include "Map/Minimap/SceneImage/GsMinimapSceneImage.h"

#include "UI/UIContent/Common/Icon/GsUIIconBase.h"

#include "Engine/World.h"
#include "Engine/Public/TimerManager.h"

void UGsMinimapIconInvasionNPC::Initialize(FGsRegionInvadeIconInfo* In_iconInfo,
	UGsUIIconBase* In_iconWidget,
	EGsMapIconType In_iconType,
	float In_updateTime,
	UGsMinimapSceneImage* In_sceneImage,
	int In_depth, int64 In_gameId)
{
	if (In_iconInfo == nullptr)
	{
		return;
	}


	_invadeNPCType = In_iconInfo->_NPCType;

	_icon = In_iconWidget;
	_iconType = In_iconType;
	_sceneImage = In_sceneImage;
	_iconDepth = In_depth;
	_gameId = In_gameId;

	UWorld* world = GSMinimap()->GetWorld();

	if (world == nullptr)
	{
		return;
	}

	world->GetTimerManager().SetTimer(_timerHandle,
		FTimerDelegate::CreateUObject(this, &UGsMinimapIconInvasionNPC::UpdateTimmer), In_updateTime, true);
}

// find pos
FVector2D UGsMinimapIconInvasionNPC::GetPos()
{
	FGsRegionInvadeIconInfo* findInfo = GSWorldMap()->GetRegionInvadeInfoByNPCType(_invadeNPCType);

	if (findInfo == nullptr)
	{
		return FVector2D::ZeroVector;
	}

	FVector pos = findInfo->_pos;
	return FVector2D(pos);
}

void UGsMinimapIconInvasionNPC::Tick(float In_delta)
{
	FGsRegionInvadeIconInfo* findInfo = GSWorldMap()->GetRegionInvadeInfoByNPCType(_invadeNPCType);

	if (findInfo == nullptr)
	{
		return;
	}

	FVector nowPos = findInfo->_pos;

	// 위치가 바뀌었다
	if (nowPos != _oldObjPos)
	{
		_oldObjPos = nowPos;
		_targetObjPosDirty = true;
	}
}

void UGsMinimapIconInvasionNPC::UpdateTimmer()
{
	if (true == _localPosDirty ||
		true == _targetObjPosDirty)
	{
		_localPosDirty = false;
		_targetObjPosDirty = false;

		UpdatePos();
	}
}

void UGsMinimapIconInvasionNPC::Finalize()
{

	if (UWorld* world = GSMinimap()->GetWorld())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
		_timerHandle.Invalidate();
	}

	Super::Finalize();
}
