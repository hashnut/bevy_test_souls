#pragma once

#include "GameObject/Camera/Mode/GsCameraModeControlBase.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Camera/Define/GsCameraGameDefine.h"
//--------------------------------------
// back view 모드 
//--------------------------------------

// 자동 회전 처리
// 1. 인풋값을 360도로 치환한다
// 2. 키보드입력에서는 8가지 방향만 있다(0,1,-1 값을 x,y로 가짐)
// 2.1 조이스틱이면 360도 다됨
// 3. 제한 값안에서만 회전한다 ex) 10~100, 260~350
// 4. 180도 이상이면 -360도를 빼서 크기값 만듬
// 5. 최대 크기까지 보간으로 처리
// 6. 수동 회전시 처리 안함



class	AGsCharacterLocalPlayer;
class	FGsCameraRotationBackView;

struct	FGsSchemaCameraModeData;

class FGsCameraModeBackView final :public FGsCameraModeControlBase
{
	using Super = FGsCameraModeControlBase;

private:
	// 터치 해제 시작 시간(쿨타임 계산용)
	int64 _touchReleaseStartTime =0;
	// 따라가기 쿨인지(터치 했을때)
	bool _isFollowCoolTime = false;
	// 생성자, 소멸자
public:
	FGsCameraModeBackView();
	FGsCameraModeBackView(EGsCameraMode In_mode) : FGsCameraModeControlBase(In_mode) {}
	virtual ~FGsCameraModeBackView() = default;


	// 각종 가상 함수들
public:
	virtual void Initialize() override;
	virtual void Enter() override;
	virtual void Exit() override;

	// 터치 시작
	virtual void TouchPress() override;
	virtual void SetAxisValue(const FVector& inAxisValue) override;
	// 다음 스텝 진행
	virtual void NextStep(FGsCameraModeHandler* In_handler) override;

	// 엔터에서 
	// 기본데이터 초기화와(InitMode)
	// 입력에 의한 연출(ChangeModeByInput) 분리
	// 그리팅(Greeting) 처리때문에 분리

	
	// 모드 초기화(데이터들)
	virtual void InitMode(
		AGsCharacterLocalPlayer* In_localChar, const FGsSchemaCameraModeData* In_modeData) override;

	// 인풋에 의한 모드 변환
	virtual void ChangeModeNormal(const FGsSchemaCameraModeData* In_modeData) override;

	// 대화 끝 변환
	virtual void ChangeModeDialogEnd() override;

	// 캐릭터가 이동 될때 여기가 호출된다(자동이동, 수동 이동)
	virtual void UpdateMove(bool In_isAuto) override;
	// 자동 공격시 회전 갱신
	virtual void UpdateAttack(bool In_isAuto) override;


	// 그리팅 끝 호출
	virtual void OnGreetingOff(bool In_isDirect = false) override;
	// 업데이트
	virtual void Update(float In_deltaTime) override;

	// 터치 끝
	virtual void TouchRelease(bool In_isSimpleTouch) override;

	virtual void PressJoystick() override;

	// 가상 get, set
public:
	// 모드 회전 구하기
	virtual FRotator GetModeRot(const FGsSchemaCameraModeData* In_modeData) override;
	// 현재 줌min 체크 하는가
	virtual bool GetIsNowCheckZoomMin() override;

	// Mutator 함수들(Logical)
public:

	// 따라가기 시작
	void StartFollow(EGsCameraInitType In_initType, bool In_isDirect = false);

	// follow 갱신
	void UpdateFollow(float In_deltaTime);
	// follow 쿨 갱신
	void UpdateFollowCoolTime(float In_deltaTime);
	

	// get, set
public:
	// 카메라 회전 백뷰 처리 접근
	FGsCameraRotationBackView* GetCameraRotationBackView();

};