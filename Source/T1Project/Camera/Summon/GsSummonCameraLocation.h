#pragma once

#include "CoreMinimal.h"

#include "Camera/Summon/Interface/GsSummonCameraWork.h"
#include "Camera/Define/GsCameraGameDefine.h"

/*
	뽑기 카메라 처리(이동)
*/
class AGsActorSummonSceneObject;

class FGsSummonCameraLocation : public IGsSummonCameraWork
{
	// 멤버 변수
private:
	// 끝났는지
	bool _isFinish = true;
	// 진행 누적값
	float _progressRate = 0.0f;

	// 카매라 거리 시작
	float _camDistFrom = 0.0f;
	// 카매라 거리 끝
	float _camDistTo = 0.0f;

	// 현재 스피드
	float _nowSpeed = 0.f;

	// 위치 목표
	FVector _locationTo = FVector::ZeroVector;
	// 위치 시작점
	FVector _locationFrom = FVector::ZeroVector;

	// 바라볼 대상(최종)
	FVector _lookTargetTo = FVector::ZeroVector;
	// 바라볼 대상(시작)
	FVector _lookTargetFrom = FVector::ZeroVector;

	AGsActorSummonSceneObject* _owner;
	// 뷰포트  사이즈
	FVector2D _viewSize = FVector2D::ZeroVector;
	// blur 테스트 환경과 다른 fov 에대한 곱해야할 값
	float _fovFactor = 0.0f;
public:
	// 보간 끝 콜백
	TFunction<void(EGsCameraWorkType)> _callbackFinishLerp;

	// 가상 함수
public:
	// 초기화
	virtual void Initialize() override;
	// 정리
	virtual void Finalize()  override{}
	// 갱신
	virtual void Update(float In_deltaTime) override;

	virtual void SetOwner(AGsActorSummonSceneObject* In_owner) override
	{
		_owner = In_owner;
	}
	virtual void OnFinishLerp() override;

	// 로직 함수
public:
	// 거리에 의한 속도 구하기
	float MakeDistSpeed();

	// get, set
public:
	void SetData(float In_distTo, float In_distFrom,
		const FVector& In_lookTargetTo,
		const FVector& In_lookTargetFrom,
		const FVector& In_moveTo, const FVector& In_moveFrom);
	void SetNowSpeed(float In_speed) { _nowSpeed = In_speed; }
	
};