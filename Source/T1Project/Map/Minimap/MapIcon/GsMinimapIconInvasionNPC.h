#pragma once

#include "CoreMinimal.h"

#include "Map/Minimap/MapIcon/GsMinimapIconBase.h"
#include "Map/GsMapIconType.h"

#include "Map/WorldMap/GsWorldMapDefine.h"

#include "GsMinimapIconInvasionNPC.generated.h"

// minimap icon controller: invasion npc(guardian, monster, wagon)

class UGsUIIconBase;
class UGsMinimapSceneImage;

class FGsRegionInvadeIconInfo;


UCLASS()
class T1PROJECT_API UGsMinimapIconInvasionNPC : public UGsMinimapIconBase
{
	GENERATED_BODY()
private:
	// 필요할때마다 info 정보 접근해서 쓰자
	EGsRegionInvadeNPCType _invadeNPCType;

	FTimerHandle _timerHandle;
	FVector _oldObjPos = FVector::ZeroVector;

	bool _targetObjPosDirty = false;
public:
	// find pos
	virtual FVector2D GetPos() override;
	virtual void Tick(float In_delta) override;
	virtual void Finalize() override;
public:
	void Initialize(FGsRegionInvadeIconInfo* In_iconInfo, 
		UGsUIIconBase* In_iconWidget,
		EGsMapIconType In_iconType,
		float In_updateTime,
		UGsMinimapSceneImage* In_sceneImage,
		int In_depth, int64 In_gameId);

	void UpdateTimmer();
};