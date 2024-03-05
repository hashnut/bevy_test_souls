#pragma once
#include "CoreMinimal.h"

#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconObject.h"

#include "SlateCore/Public/Layout/Geometry.h"

#include "GsRegionMapIconTreasureMonster.generated.h"

/*
	지역맵 local
*/

class UGsGameObjectLocalPlayer;
UCLASS()
class T1PROJECT_API UGsRegionMapIconTreasureMonster : public UGsRegionMapIconObject
{
	GENERATED_BODY()

protected:
	//UPROPERTY()
	//UGsGameObjectLocalPlayer* _ownObject = nullptr;

	uint64 _seperateId{ 0 };

public:
	virtual void Finalize() override;
	virtual void Tick(float In_delta) override;
	virtual FVector2D GetPos() override;
	// 로직함수

	//void SetOwnObject(UGsGameObjectLocalPlayer* In_ownPlayer); //local만
	void SetMonsterSeperateID(uint64 seperateId) { _seperateId = seperateId; }
};