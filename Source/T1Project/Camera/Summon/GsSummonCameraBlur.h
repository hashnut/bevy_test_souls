#pragma once
#include "CoreMinimal.h"
#include "Camera/Summon/Interface/GsSummonCameraWork.h"
#include "Camera/Define/GsCameraGameDefine.h"
/*
	뽑기 카메라 처리 블러
*/
class AGsActorSummonSceneObject;
class FGsSummonCameraBlur : public IGsSummonCameraWork
{
	// 멤버 변수
private:
	// 끝났는지
	bool _isFinish = true;
	// 진행 누적값
	float _progressRate = 0.0f;

	AGsActorSummonSceneObject* _owner;
	// 블러 속도
	float _summonCamBlurSpeed = 0.0f;
public:
	// 보간 끝 콜백
	TFunction<void(EGsCameraWorkType)> _callbackFinishLerp;
	// 가상 함수
public:
	// 초기화
	virtual void Initialize() override;
	// 정리
	virtual void Finalize()  override {}
	// 갱신
	virtual void Update(float In_deltaTime) override;

	virtual void SetOwner(AGsActorSummonSceneObject* In_owner) override
	{
		_owner = In_owner;
	}
	virtual void OnFinishLerp() override;
	// 로직 함수
public:

	void Start() 
	{
		_isFinish = false;
		_progressRate = 0.0f;
	}
};