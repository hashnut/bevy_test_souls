#pragma once

#include "CoreMinimal.h"
#include "GameObject/Camera/Interface/GsCameraWork.h"
/*
	spring arm target offset lerp
*/
class UGsGameObjectLocalPlayer;
class UGsGameObjectBase;

class FGsCameraTargetOffsetGame : public IGsCameraWork
{
private:
	// target offset goal value
	FVector _modeTargetOffsetTo = FVector::ZeroVector;
	// target offset start value
	FVector _modeTargetOffsetFrom = FVector::ZeroVector;
	// target offset progress (delta time * speed  누적값)
	float _progressModeTargetOffsetRate = 0.0f;
	// target offset value changing
	bool _isModeTargetOffsetChanging = false;

	// 타겟 플레이어
	UGsGameObjectLocalPlayer* _local;
	// 실제 카메라 타겟 obj
	UGsGameObjectBase* _targetObj;

	// 가상 함수
public:
	virtual void Initialize() override {}
	virtual void Finalize() override;
	// 소켓 오프셋 갱신
	virtual	void Update(float In_deltaTime) override;
	virtual void SetWorkLocalPlayer(UGsGameObjectLocalPlayer* In_local) override { _local = In_local; }
	virtual void SetWorkTarget(UGsGameObjectBase* In_target) override { _targetObj = In_target; };
	// 캐릭터 삭제 처리
	virtual void RemoveLocalPlayer() override;

	virtual void ClearFinishValue() override
	{
		_isModeTargetOffsetChanging = false;
	}

	// 로직 함수
public:
	// 모드 target 오프셋 시작
	void StartModeTargetOffset(FVector In_offsetFrom, FVector In_offsetTo);

	// set
public:
	void SetDirectTargetOffset(FVector In_offsetTo);

};