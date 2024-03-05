#pragma once

#include "CoreMinimal.h"
#include "Camera/Define/GsCameraGameDefine.h"

#include "GameObject/Camera/Interface/GsCameraWork.h"

/*
	카메라 인게임 회전처리 기본
*/
class AGsCharacterLocalPlayer;
class UGsGameObjectLocalPlayer;
class UGsGameObjectBase;
class FGsCameraModeBase;
class UCurveFloat;

class FGsCameraRotationBase : public IGsCameraWork
{
	// 멤버 변수
protected:
	// 모드 회전 변환중인가
	bool _isModeRotChanging = false;

	// 도착값
	FRotator _rotTo = FRotator::ZeroRotator;
	// 시작값
	FRotator _rotFrom = FRotator::ZeroRotator;
	// 현재 값
	FRotator _rotCurrent = FRotator::ZeroRotator;
	//  변환 진행율(delta time * speed  누적값)
	float _progressRate = 0.0f;

	// local 플레이어
	UGsGameObjectLocalPlayer* _local;
	// 실제 카메라 타겟 obj
	UGsGameObjectBase* _targetObj;

	// 회전 타입(월드값)
	EGsCameraRotationType _rotationType;
	// 이전이 대사모드였나
	bool _isAfterDialog = false;

	// is use lerp finish callback
	bool _isUseFinishCallback = false;

	bool _isFinish = true;
	// 소유자(모드)
	FGsCameraModeBase* _owner;
	// 현재 스피드
	float _nowSpeed = 0.f;

	// 소멸자
public:
	// 메모리 누수 이슈
	virtual ~FGsCameraRotationBase() = default;

	// 가상 함수
public:
	virtual void Initialize() override {};
	virtual void Finalize() override;

	virtual void SetRotation(const FRotator& In_rot) {}
	// 상태 enter 될때 마다 호출
	virtual void InitMode() {}
	virtual void OnFinishLerp() override;
	virtual bool IsRotationPossible() { return (_isFinish == false); }
	// 회전속도 구하기
	virtual float GetRotSpeed();
	// 모드 회전 갱신
	virtual void Update(float In_deltaTime) override;
	
	virtual void SetMode(FGsCameraModeBase* In_mode)override { _owner = In_mode; }

	virtual void SetWorkLocalPlayer(UGsGameObjectLocalPlayer* In_local)override { _local = In_local; }
	virtual void SetWorkTarget(UGsGameObjectBase* In_target) override { _targetObj = In_target; };
	virtual void RemoveLocalPlayer() override;

	// 속도 만들기
	virtual void MakeSpeed();

	virtual void ClearFinishValue() override
	{
		_isFinish = true;
	}

	// 로직 함수
public:
	// 거리에 의한 속도 만들기
	float MakeDistSpeed();

	// 가상 get
public:
	// 현재 사용 커브 접근
	virtual UCurveFloat* GetCurve();

	// get, set
public:
	// 모드 회전 변환중인지
	bool GetIsModeRotChanging() { return _isModeRotChanging; }
	void SetIsModeRotChanging(bool In_val)
	{
		_isModeRotChanging = In_val;
	}
	void SetTargetRot(const FRotator& In_rotTo, const FRotator& In_rotFrom, bool In_isNowMove = true);

	void SetIsFinish(bool In_val) { _isFinish = In_val; }
	bool GetIsRotationFinish() { return _isFinish; }
	
	void SetRotationType(EGsCameraRotationType In_type) { _rotationType = In_type; }
	EGsCameraRotationType GetRotationType() { return _rotationType; }

	void SetIsAfterDialog(bool In_val) { _isAfterDialog = In_val; }
	void SetIsUseFinishCallback(bool In_val) { _isUseFinishCallback = In_val; }

	void SetNowSpeed(float In_speed) { _nowSpeed = In_speed; }

	const FRotator& GetCurrentRotator() { return _rotCurrent; }
	const FRotator& GetGoalRotator() { return _rotTo; }
};