#pragma once
#include "CoreMinimal.h"

#include "Camera/Define/GsCameraGameDefine.h"

#include "GameObject/Camera/Interface/GsCameraWork.h"

#include "Engine/Classes/Engine/EngineTypes.h"

/*
	카메라 이동 인게임 처리(대화 연출)

	대화연출에선 배경 충돌체 때문에 원하는곳을 못 바라볼수 있으므로
	스프링 암의 줌이나 오프셋을 쓰는게 아니라
	카메라를 직접 움직인다
*/

class UGsGameObjectLocalPlayer;
class UGsGameObjectBase;
class FGsCameraModeBase;

class FGsCameraLocationGame : public IGsCameraWork
{
	// 멤버 변수
private:

	// 이전이 대사모드였나
	bool _isAfterDialog = false;
	// is use lerp finish callback
	bool _isUseFinishCallback = false;
	
protected:
	// 소유자(모드)
	FGsCameraModeBase* _owner;
	// 로컬 플레이어
	UGsGameObjectLocalPlayer* _local;
	// 실제 카메라 타겟 obj
	UGsGameObjectBase* _targetObj;
	// 진행 누적값
	float _progressRate = 0.0f;
	// 끝났는지
	bool _isFinish = true;
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

	// 카매라 거리 시작
	float _camDistFrom = 0.0f;
	// 카매라 거리 끝
	float _camDistTo = 0.0f;
	// 위치 공간 타입
	EGsCameraLocationSpaceType _locationSpaceType = EGsCameraLocationSpaceType::WorldLocation;

	TEnumAsByte<ECollisionChannel> ProbeChannel;
	float ProbeSize;

	// 가상 함수
public:
	virtual void Initialize() override;
	virtual void Finalize() override;
	// 업데이트
	virtual void Update(float In_deltaTime) override;
	// 목표 위치 도달
	virtual void OnFinishLerp() override;

	virtual void SetMode(FGsCameraModeBase* In_mode) override { _owner = In_mode; }

	virtual void SetWorkLocalPlayer(UGsGameObjectLocalPlayer* In_local)override { _local = In_local; }
	virtual void SetWorkTarget(UGsGameObjectBase* In_target) override { _targetObj = In_target; };
	// 캐릭터 삭제
	virtual void RemoveLocalPlayer() override;
	// 회전속도 구하기
	virtual float GetRotSpeed();
	// 속도 만들기
	virtual void MakeSpeed();

	virtual void ClearFinishValue() override
	{
		_isFinish = true;
	}

	// 로직 함수
public:
	float MakeDistSpeed();
	// 바라보면서 업데이트
	void UpdateLookAt(float In_deltaTime);

	FVector BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime);

	// get, set
public:
	void SetIsFinish(bool In_val) { _isFinish = In_val; }


	void SetIsAfterDialog(bool In_val) { _isAfterDialog = In_val; }
	void SetIsUseFinishCallback(bool In_val) { _isUseFinishCallback = In_val; }

	void SetNowSpeed(float In_speed) { _nowSpeed = In_speed; }

	void SetData(float In_distTo, float In_distFrom,
		const FVector& In_lookTargetTo,
		const FVector& In_lookTargetFrom,
		const FVector& In_moveTo, const FVector& In_moveFrom,
		bool In_isNowMove = true);

	void SetDataNormal(
		const FVector& In_moveTo, const FVector& In_moveFrom);


	// 위치 설정 바로 하기
	void SetDirectPos(const FVector& In_moveTo);

	void SetLocationSpace(EGsCameraLocationSpaceType In_spaceType)
	{
		_locationSpaceType = In_spaceType;
	}
};