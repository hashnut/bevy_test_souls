#pragma once
#include "CoreMinimal.h"

#include "GameObject/Camera/Interface/GsCameraWork.h"

/*
*  인게임 dof 관리
*/
class UGsGameObjectBase;
class UGsGameObjectLocalPlayer;
class FGsCameraModeBase;

class UCameraComponent;

class FGsCameraDOFGame : public IGsCameraWork
{
	// 멤버 변수
private:
	// 모바일 스케일 시작값
	float _mobileScaleFrom = 0.0f;
	// 모바일 스케일 목표 값
	float _mobileScaleTo = 0.0f;
	//  변환 진행율(delta time * speed  누적값)
	float _progressRate = 0.0f;
	// 종료했는지
	bool _isFinish = true;
	// 켜지는건지
	bool _isOn = false;

	// local 플레이어
	UGsGameObjectLocalPlayer* _local;
	// 실제 카메라 타겟 obj
	UGsGameObjectBase* _targetObj;
	// 타겟 모드
	FGsCameraModeBase* _targetMode;
	// 보간 속도
	float _cameraZoomMinLerpSpeed = 0.0f;
	// 가상함수
public:
	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual void Update(float In_deltaTime)override;
	virtual void SetWorkLocalPlayer(UGsGameObjectLocalPlayer* In_local)override { _local = In_local; }
	virtual void SetWorkTarget(UGsGameObjectBase* In_target) override { _targetObj = In_target; };
	virtual void RemoveLocalPlayer() override;
	virtual void ClearFinishValue() override 
	{
		_isFinish = true;
	}
	// 로직 함수
public:
	void StartDOF(float In_mobileScaleFrom, float In_mobileScaleTo, bool In_isOn);
	// 바로 세팅 하기
	void SetDirectDOF( float In_mobileFocalRegion ,float In_mobileScale, bool In_isOn,
		float In_focalDistance);

	void UpdateFocalDistance();

	// get, set
public:
	// dof 옵션값 on, off
	void SetDofOption(bool In_isOn);

	float GetFocalDistance();
	UCameraComponent* GetCameraComponent();
};