#pragma once
#include "CoreMinimal.h"

#include "Camera/Summon/Interface/GsSummonCameraWork.h"
#include "Camera/Define/GsCameraGameDefine.h"

/*
	뽑기 카메라 회전
*/

class UCameraComponent;
class AGsActorSummonSceneObject;

class FGsSummonCameraRotation : public IGsSummonCameraWork
{
	// 멤버 변수
private:
	bool _isFinish = true;

	// 현재 스피드
	float _nowSpeed = 0.f;
	//  변환 진행율(delta time * speed  누적값)
	float _progressRate = 0.0f;

	// 도착값
	FRotator _rotTo = FRotator::ZeroRotator;
	// 시작값
	FRotator _rotFrom = FRotator::ZeroRotator;
	// 현재 값
	FRotator _rotCurrent = FRotator::ZeroRotator;

	AGsActorSummonSceneObject* _owner;
public:
	// 보간 끝 콜백
	TFunction<void(EGsCameraWorkType)> _callbackFinishLerp;
	// 가상 함수
public:
	// 초기화
	virtual void Initialize() override {}
	// 정리
	virtual void Finalize()  override {}
	// 갱신
	virtual void Update(float In_deltaTime) override;

	virtual void OnFinishLerp() override;

	virtual void SetOwner(AGsActorSummonSceneObject* In_owner) override
	{
		_owner = In_owner;
	}

	// 로직 함수
public:
	void SetRotation(const FRotator& In_rot);
	void SetNowSpeed(float In_speed) { _nowSpeed = In_speed; }

	const FRotator& GetCurrentRotator() { return _rotCurrent; }

	void SetTargetRot(const FRotator& In_rotTo, const FRotator& In_rotFrom);
	// 거리에 의한 속도 만들기
	float MakeDistSpeed();
};