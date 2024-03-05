#pragma once

#include "CoreMinimal.h"
#include "Camera/Define/GsCameraZoomLobbyDefine.h"
#include "Message/GsMessageContents.h"
#include "GameFlow/GameContents/GsContentsMode.h"
/*
	로비 줌 관리
*/


class AActor;
class AGsCharacterLocalPlayer;
class AGsCharacterPlayer;
class USkeletalMeshComponent;

struct FGsSchemaCustomizeCameraData;

class FGsCameraHandlerLobby
{
	// 멤버 변수
private:	
	// 로비 메시지 델리 게이트(해제용)
	TArray<TPair<MessageContentsLobby, FDelegateHandle>> _delegateLobbyMsg;
	// 소켓 오프셋 머리 위치(본위치)
	FVector _camSoketOffsetHead = FVector::ZeroVector;
	// 소켓 오프셋 시작 값(타입변경 탭에서 사용)
	FVector _camSocketOffsetFrom = FVector::ZeroVector;
	// 소켓 오프셋 종료 값(타입변경 탭에서 사용)
	FVector _camSocketOffsetTo = FVector::ZeroVector;
	
	// 캐릭터 
	AGsCharacterLocalPlayer* _characterLocalPlayer;
	
	// 스프링 암 거리 목표값
	float _armLengthTo = 0.0f;	
	// 스프링 암 거리 시작값
	float _armLengthFrom = 0.0f;
	// 줌 시작인가?
	bool _isZoomStart = false;
	//  변환 진행율(delta time * speed  누적값)
	float _zoomProgressRate = 0.0f;


	// 탭에 의한 줌인가?
	bool _isTabZoom = false;
	// pitch 회전 목표값(미친듯 돌아서 quat으로 변경)
	FQuat _pitchQuatTo = FQuat::Identity;
	// pitch 회전 시작값(미친듯 돌아서 quat으로 변경)
	FQuat _pitchQuatFrom = FQuat::Identity;
	// 회전(pitch) rate 값
	float _progressRotPitchRate = 0.0f;
	// pitch 회전인가?
	bool _isRotPitch = false;

	// 오프셋값이 변경되었는가?
	bool _isDirtyOffset = false;

	// 최근 줌모드
	EGsLobbyZoomMode _currZoomMode = EGsLobbyZoomMode::WholeBody;

	// 게임 입장 시 카메라 이동 연출
	bool _isGameEnterCamMove = false;
	float _gameEnterMoveValue = 0.0f;

	float _curLength = 0.0f;
	float _endLength = 0.0f;


	FRotator _camRot;
	FRotator _camMoveRot;

	float _gameEnterMoveTime;
	float _startFOV;
	// 카메라 로비 머리 위치 다르다는 판정값
	float _cameraLobbyDiffHeadPos = 0.0f;

	FVector _currentHeadPos;

	// 소멸자
public:
	~FGsCameraHandlerLobby();

	// 외부 이벤트
public:
	// 줌값 변경
	void OnZoomChange(float In_val);
	// 줌모드 변경
	void OnChangeZoomMode(EGsLobbyZoomMode In_mode);
	// 머리 줌
	void OnZoomHead();
	// 전신 줌
	void OnZoomWholeBody();
	// 키 변경
	void OnHeightChange();

	// 로직 함수
public:
	// 초기화(캐릭터 begin play에서 호출)
	void Initialize(AGsCharacterLocalPlayer* In_char);
	// clear
	void Finalize();
	// 생성시 줌해놓은게
	// 선택창에서 유지되어서
	// 선택창 들어갈때 줌아웃 처리
	// zoom rate: 1.0f (최대 빠진거)
	// zoom rate: 0.0f (최소 빠진거)
	void InitZoom(float In_zoomRate = 1.0f);
	// 업데이트
	void Update(float In_delta);
	// 업데이트 줌
	bool UpdateZoom(float In_delta);	
	// 업데이트 pitch 회전 
	void UpdateRotPitch(float In_delta);
	// 오프셋 계산(머리중심 <-> 몸통중심 을 줌값을 기준으로 계산한다)
	void CalcOffset(float In_lengthRate);	

	// 머리 위치값 바뀌었는지 체크
	void CheckHeadDiff();
	// 오프셋 갱신
	void UpdateOffset();

	// 줌 시작
	void ZoomStart(float In_zoomFrom, float In_zoomTo);

	// get, set
public:
	// 줌 비율값 구하기
	// 머리(0.0f) - 전신(1.0f)
	float GetZoomRate();
	// 비율로 스프링 암 거리 구하기
	float GetSpringArmLength(float In_rate);

	FVector GetCurrentHeadPos(AGsCharacterPlayer* In_char, bool In_isRelativePos = false);

	void SetCurrentHeadPos(FVector& In_pos)
	{
		_currentHeadPos = In_pos;
	}

	// 게임 입장 시 카메라 이동 연출
public:
	void OnGameEnterMove(AActor* In_actor);
	void UpdateEnterMove(float In_delta);

	// test용
	void OnGameEnterMoveStop() { _isGameEnterCamMove = false; }
};