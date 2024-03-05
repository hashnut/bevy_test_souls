#pragma once

#include "CoreMinimal.h"
#include "GameObject/Camera/Interface/GsCameraWork.h"

/*
    인게임 FOV 관리
*/
class UGsGameObjectLocalPlayer;
class UGsGameObjectBase;

class FGsCameraFOVGame : public IGsCameraWork
{
private:
    // 모드 fov 목표값
    float _modeFOVTo = 0.0f;
    // 모드 fov 시작값
    float _modeFOVFrom = 0.0f;
    // 모드 fov 진행율(delta time * speed 누작값)
    float _progressModeFOVRate = 0.0f;
	// 모드 fov 변환중인지
	bool _isModeFOVChanging = false;
	// local 플레이어
	UGsGameObjectLocalPlayer* _local;
	// 실제 카메라 타겟 obj
	UGsGameObjectBase* _targetObj;

	// 이전이 대사모드였나
	bool _isAfterDialog = false;

protected:
	// 현재 스피드
	float _nowSpeed = 0.f;

	// 가상 함수
public:
	virtual void Initialize() override {}
	virtual void Finalize() override;
	// 소켓 오프셋 갱신
	virtual	void Update(float In_deltaTime) override;
	virtual void SetWorkLocalPlayer(UGsGameObjectLocalPlayer* In_local)override { _local = In_local; }
	virtual void SetWorkTarget(UGsGameObjectBase* In_target) override { _targetObj = In_target; };
	// 캐릭터 삭제 처리
	virtual void RemoveLocalPlayer() override;
	// 속도 만들기
	virtual void MakeSpeed();
	// 회전속도 구하기
	virtual float GetRotSpeed();
	virtual void ClearFinishValue() override
	{
		_isModeFOVChanging = false;
	}
	// 로직 함수
public:
	// 모드 오프셋 시작
	void StartModeFOV(float In_FOVFrom, float in_FOVTo);

	//get, set
public:
	void SetIsAfterDialog(bool In_val) { _isAfterDialog = In_val; }
	virtual void SetNowSpeed(float In_speed);

	// 바로 세팅
	void SetDirectFOV(float In_FOV);

	bool GetIsFOVChanging()
	{
		return _isModeFOVChanging;
	}

	float GetGoalFOV()
	{
		return _modeFOVTo;
	}
};