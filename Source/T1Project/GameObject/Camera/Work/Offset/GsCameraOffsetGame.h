#pragma once

#include "CoreMinimal.h"
#include "GameObject/Camera/Interface/GsCameraWork.h"
/*
	인게임 오프셋 관리
*/
class UGsGameObjectLocalPlayer;
class UGsGameObjectBase;

class FGsCameraOffsetGame : public IGsCameraWork
{
private:
	// 모드 소켓 오프셋 목표값
	FVector _modeSocketOffsetTo = FVector::ZeroVector;
	// 모드 소켓 오프셋 시작값
	FVector _modeSocketOffsetFrom = FVector::ZeroVector;
	// 모드 소켓 진행율(delta time * speed  누적값)
	float _progressModeSocketOffsetRate = 0.0f;
	// 모드 소켓 오프셋 변환중인지
	bool _isModeSocketOffsetChanging = false;

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
	virtual void SetWorkLocalPlayer(UGsGameObjectLocalPlayer* In_local) override{ _local = In_local; }
	virtual void SetWorkTarget(UGsGameObjectBase* In_target) override { _targetObj = In_target; };
	// 캐릭터 삭제 처리
	virtual void RemoveLocalPlayer() override;

	virtual void ClearFinishValue() override
	{
		_isModeSocketOffsetChanging = false;
	}

	// 로직 함수
public:
	// 모드 소켓 오프셋 시작
	void StartModeSocketOffset(FVector In_offsetFrom, FVector In_offsetTo);
	
	// set
public:
	void SetDirectSocketOffset(FVector In_offsetTo);
	
};