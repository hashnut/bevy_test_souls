#pragma once
#include "CoreMinimal.h"
#include "GameObject/Camera/Work/Rotation/GsCameraRotationBase.h"
#include "Camera/Define/GsCameraGameDefine.h"
/*
	카메라 인게임 백뷰 회전 처리
*/

class	AGsCharacterLocalPlayer;
class	UGsGameObjectLocalPlayer;
class	FGsCameraModeBackView;
class	UCurveFloat;

struct	FGsSchemaCameraModeData;

class FGsCameraRotationBackView : public FGsCameraRotationBase
{
	using Super = FGsCameraRotationBase;

	// 멤버 변수들
private:
	// 최근 x input
	float _currentXInput;
	// 최근 y input
	float _currentYInput;

	// 최근 회전 입력값(보간중)
	float _currentAutoRotInput;
	// 목표 인풋값
	float _targetAutoRotInput;
	// 제한 각도
	// 오른쪽
	const float _rightMin = 5.0f;
	const float _rightMax = 175.0f;

	// 왼쪽
	const float _leftMin = -175.0f;
	const float _leftMax = -5.0f;

	// 따라가기 모드
	EGsCameraInitType _followMode = EGsCameraInitType::None;
	// 타겟 숄더뷰 추가 yaw
	float _addShoulderTargetYaw = 0.0f;

	// 가상 함수들
public:
	// 실제 대상 회전 세팅 처리
	virtual void SetRotation(const FRotator& In_rot) override;
	// 회전 가능한가
	virtual bool IsRotationPossible() override;
	virtual void Update(float In_deltaTime)override;
	// 회전속도 구하기
	virtual float GetRotSpeed() override;
	// 속도 만들기
	virtual void MakeSpeed()override;
	virtual void OnFinishLerp() override;
	virtual void Initialize() override;

	// 가상 get
public:
	// 현재 사용 커브 접근
	virtual UCurveFloat* GetCurve() override;

	// 로직 함수들
public:
	// 자동회전 갱신
	void UpdateAutoRot(float In_deltaTime);	
	
	// 따라가기 시작
	void StartFollow(
		EGsCameraInitType In_initType,
		FRotator In_rot,
		bool In_isDirect = false);

	// event
public:	
	void OnTouchPress();
	// joystick or keyboard dir input
	void OnMoveDirectionInput();


	// get, set
public:
	void SetAxisValue(const FVector& In_axisValue);

	FRotator GetDiffRotTo();

	bool GetTouchOn();
	void SetFollowMode(EGsCameraInitType In_mode)
	{
		_followMode = In_mode;
	}
};