// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/ObjectClass/GsGameObjectPropObject.h"
#include "../UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "GsGameObjectWarp.generated.h"

/**
 * 
 */

UCLASS()
class T1PROJECT_API UGsGameObjectWarp final : public UGsGameObjectPropObject
{
	GENERATED_BODY()

private:	
	int32 _destMapID;
	int64 _destSpotIndex;
	uint32 _warpId;
	WarpType _warpType;
	float _radius;	
	TWeakObjectPtr<UGsUIPopupYesNo> _warpPopup;

public:
	UGsGameObjectWarp();
	virtual ~UGsGameObjectWarp();

	virtual void Initialize(const struct FGsSpawnParam* SpawnParam) override;

protected:
	virtual void InitializeActor(AActor* Owner) override;	

public:
	// 충돌 Delegate
	// 일부 하위 클래스만 필요하다면 하위 클래스에 따로 구현		
	virtual void OnBeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 intoherBodyIndex, bool inbFromSweep, const FHitResult& Hit) override;
	virtual void OnEndOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 intoherBodyIndex) override;

public:	
	virtual EGsGameObjectType GetObjectType() const override;

public:		
	UFUNCTION()
	void OnWarp(bool inSuccess = true);

public:
	void SetWarpInfo(int32 inMapID, int64 inSpotID)
	{
		_destMapID = inMapID;
		_destSpotIndex = inSpotID;
	}
};
