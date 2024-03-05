#pragma once
#include "CoreMinimal.h"
#include "Camera/Define/GsCameraGameDefine.h"

#include "GameObject/Camera/Interface/GsCameraWork.h"

class UGsGameObjectLocalPlayer;

class FGsCameraRotationWarpStart : public IGsCameraWork
{
private:
	// local 플레이어
	UGsGameObjectLocalPlayer* _local;

	// virtual func
public:
	virtual void Initialize() override {};
	virtual void Finalize() override {};
	virtual void Update(float In_deltaTime)override {};


	// virtual set func
public:
	virtual void SetWorkLocalPlayer(UGsGameObjectLocalPlayer* In_local)override { _local = In_local; }

	// logic func
public:
	
	void SetRotation(const FRotator& In_rot);
};