#pragma once

#include "CoreMinimal.h"

#include "GameObject/Camera/Interface/GsCameraWork.h"

#include "Camera/Define/GsCameraGameDefine.h"
/*
	인게임 줌 관리
*/
class UGsGameObjectLocalPlayer;
class FGsCameraModeBase;

class FGsCameraZoomGame : public IGsCameraWork
{
	// 멤버 변수
private:
	// 스프링 암 거리 시작값
	float _armLengthFrom = 0.0f;
	// 스프링 암 거리 목표값
	float _armLengthTo = 0.0f;
	//  변환 진행율(delta time * speed  누적값)
	float _progressRate = 0.0f;
	// 줌 최소값 도달 했는지
	bool _isReachZoomMin = false;
	// 카메라 보간 속도
	float _camArmLengthSpeed = 0.0f;
	// 카메라 input 보간 속도
	float _camInputArmLengthSpeed = 0.0f;
	// 줌 종료 최소값
	float _cameraZoomFinishDiff = 0.0f;
	// 줌 백업 하는지
	bool _isBackupZoom = true;

	FVector _camStartPos = FVector::ZeroVector;
	FVector _camStartRot = FVector::ZeroVector;

protected:
	// (카메라)스프링 암 거리 최대치
	float _camArmLengthMax = 1000.0f;
	// (카메라)스프링 암 거리 최소치
	float _camArmLengthMin = 300.0f;
	// 줌 시작인가?
	bool _isZoomStart = false;
	// 터치에 의한 줌인지
	bool _isTouchZoom = false;
	// min 체크 강제 변경 dirty(보간 끝시점이므로 미리 세팅할때)
	bool _isDirtyMinCheckForcedChange = false;
	// min check in lerp end
	bool _isDirtyMinCheckLerpEnd = false;
	// before dialog
	bool _isAfterDialog = false;
	
	bool _isGreetingOffZoom = false;

	
protected:
	// local 플레이어
	UGsGameObjectLocalPlayer* _local;
	// 실제 카메라 타겟 obj
	UGsGameObjectBase* _targetObj;
	// 타겟 모드
	FGsCameraModeBase* _targetMode;

	// 가상 함수
public:
	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual void Update(float In_deltaTime)override;
	// 줌 종료 콜백
	virtual void OnFinishLerp()override;	
	// 타겟모드 세팅
	virtual void SetMode(FGsCameraModeBase* In_mode) override;
	virtual void SetWorkLocalPlayer(UGsGameObjectLocalPlayer* In_local)override { _local = In_local; }
	virtual void SetWorkTarget(UGsGameObjectBase* In_target) override { _targetObj = In_target; };
	virtual void RemoveLocalPlayer() override;

	virtual void ClearFinishValue() override
	{
		_isZoomStart = false;
	}

	// 로직 함수
public:
	// 줌인
	void ZoomIn(float In_mulVal);
	// 줌아웃
	void ZoomOut(float In_mulVal);	
	void InitMode(float In_camZoomMin, float In_camZoomMax);
	// 줌갱신
	bool UpdateZoom(float In_deltaTime);

	void UpdatePosZoom(float In_deltaTime);
	// 그리팅 뷰 세팅
	void SetGreetingView();	
	// 모드줌 시작
	void StartModeZoom(float In_armLengthTo, EGsCameraChangeZoomType In_type);
	// 줌 시작
	void ZoomStart(float In_zoomFrom, float In_zoomTo);

	void PosZoomStart(float In_distance, const FVector& In_startPos, const FVector& In_startRot, 
		bool In_isNowMove = true);
	// 줌 최소값 도달했는지 체크
	void CheckZoomMin(bool In_isForced = false);

	void ProcZoomMin(bool In_isNowMin);

	// get, set
public:
	// 현재 줌 구하기
	float GetNowZoom();
	// 줌 세팅
	void SetZoom(float In_val);
	// 줌 최소 값인가
	bool IsMinZoom();

	bool IsNeedCheckZoomMin();

	bool IsZoomStart()
	{
		return _isZoomStart;
	}
	void SetIsZoomStart(bool In_val)
	{
		_isZoomStart = In_val;
	}
	float GetGoalZoom()
	{
		return _armLengthTo;
	}
	void SetDirectPos(const FVector& In_moveTo);

	void SetIsAfterDialog(bool In_val)
	{
		_isAfterDialog = In_val;
	}
	bool GetIsAfterDialog()
	{
		return _isAfterDialog;
	}
	void SetIsGreetingOffZoom(bool In_val)
	{
		_isGreetingOffZoom = In_val;
	}
	bool GetIsGreetingOffZoom()
	{
		return _isGreetingOffZoom;
	}
};