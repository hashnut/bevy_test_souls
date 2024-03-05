#pragma once

#include "Classes/GsState.h"
#include "Classes/GsStateMng.h"
#include "Classes/Container/GsSharedMap.h"
#include "Classes/GsManager.h"
#include "Camera/Define/EGsDialogCameraViewType.h"

#include "DataSchema/GsSchemaEnums.h"
#include "Message/GsMessageSystem.h"

#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGame.h"

#include "Camera/Define/GsCameraGameDefine.h"



//--------------------------------------------------------------
// 카메라 모드 관리자
//--------------------------------------------------------------

struct FGsSchemaCameraModeData;
struct FGsSchemaCameraDialogViewData;

class UGsGameObjectBase;
class UGsGameObjectLocalPlayer;
class FGsInputEventMsgBase;
class FGsCameraDialogHandler;

class FGsCameraModeManager :
	public IGsManager

{
	// 멤버 변수들
private:
	// 현재 모드
	EGsCameraMode _currentMode;
	// 저장용 모드
	EGsCameraMode _backupMode = EGsCameraMode::Max;
	// 그리팅 뷰인가
	// 최초 한번만쓰는거라서 정적 멤버
	// 계속 나올꺼면 비정적으로 가야함
	// local 안에 있을땐 static 값이었지만
	// 매니징 단에 들어와서는 유지되므로 non static으로 간다
	bool _isGreetingView = true;
	// 캐릭터 초기화 했는지?
	bool _isInitCharacter = false;
	// pitch 회전, 줌값을 저장해놓았는지?
	bool _isBackUpTransform = false;
	// 저장용 quat
	FQuat _backupQuat = FQuat::Identity;

	// 카메라 백업 줌 값
	TMap<EGsCameraMode, float> _dicCamBackupZoom;

	// 카메라 백업 pitch 값
	TMap<EGsCameraMode, float> _dicCamBackupPitch;

	// camera backup zoom observer
	TMap<EGsCameraMode, float> _dicObserverCamBackupZoom;

	// camera backup pitch observer
	TMap<EGsCameraMode, float> _dicObserverCamBackupPitch;

	// 카메라 저장 회전
	FRotator _backupCamRotation = FRotator::ZeroRotator;
	// 카메라 저장 위치
	FVector _backupCamLocation = FVector::ZeroVector;

	FRotator _backupBackViewDiffRot = FRotator::ZeroRotator;
	// 대사전 바라보던 곳 저장
	FVector _backupBeforeDialogLookPos = FVector::ZeroVector;
	// 대사 카메라 바라보던곳 저장
	FVector _backupDialogLookPos = FVector::ZeroVector;
	// 대사 카메라 거리 저장
	float _bakcupDialogCamDist = 0.0f;

	// 로컬 플레이어
	UGsGameObjectLocalPlayer* _localPlayer = nullptr;
	// 테스트 hit 대사 그릴지
	bool _isDrawTestHitDialog = false;

	// 카메라 다이얼로그 연출중인가
	bool _isCameraDialogPlay = false;
	// cam dialog mode + last lerp
	bool _isCameraDialogPlayAll = false;

	// use quest call back
	EGsCamearDialogEndCallBackType _cameraDialogEndCallbackType = EGsCamearDialogEndCallBackType::None;
	// 대사 카메라 뷰 타입
	EGsDialogCameraViewType _dialogCameraViewType = EGsDialogCameraViewType::DialogBackLocalSingleNpc;
	// 현재 대화 뷰 데이터
	const FGsSchemaCameraDialogViewData* _currDialogViewData = nullptr;

	// 머리 본 이름
	FName _headBoneName;
	
	// 대화 보간 끝인가
	bool _isDialogLerpEnd = false;
	// 컷 페이드 사용하는가
	bool _isUseCutFadeOut = false;

	// 유저 세팅에 저장한게 있는지
	bool _isBackupUserSetting = false;
	// 저장 가능한 모드
	TArray<EGsCameraMode> _arraySavePossibleMode;
	// 카메라 타겟본 높이 저장값
	float _savedCameraTargetBoneHeight = 0.0f;
	// 카메라 숄더뷰 따라가기 쿨타임(드래그 회전시) 틱
	float _cameraShoulderFollowCoolTimeTick = 0.0f;

	// current fixed quarter yaw type
	EGsCameraFixedQuarterYawType _fixedQuarterYawType = EGsCameraFixedQuarterYawType::North;

	bool _isZoomFirstCameraMove = false;

	// 생성자 소멸자
public:
	virtual ~FGsCameraModeManager();

	
	// 가상 함수
public:
	// 초기화(처음 한번만)
	virtual void Initialize() override;
	// 갱신
	virtual void Update(float In_deltaTime) override;

	virtual void Finalize() override;

	// static 함수
public:
	// 모드에 따른 카메라 데이터 가져오기
	static const FGsSchemaCameraModeData* GetCamModeData(EGsCameraMode In_mode);

public:

	// hit pos 구하기(DialogBackLocalSingleNpc, DialogBackNpc)
	bool GetDialogHitPos(UGsGameObjectBase* In_camParent,
		const FVector& In_farPos,
		const FVector& In_nearPos,
		const FVector& In_lookTargetPos,
		FVector& Out_pos);

	// side hit pos 구하기(DialogSide)
	bool GetDialogHitPosSide(UGsGameObjectBase* In_camParent,
		const FVector& In_targetPos,
		const FVector& In_localPos,
		const FVector& In_lookTargetPos,
		FVector& Out_pos);

	// greeting hit pos 구하기(DialogGreetingLocal, DialogGreetingNPC)
	bool GetDialogHitPosGreeting(UGsGameObjectBase* In_camParent,
		const FVector& In_lookTargetPos,
		const FVector& In_forwardVector,
		FVector& Out_pos);

	// greeting object hit pos 구하기(DialogGreetingObject)
	bool GetDialogHitPosGreetingObject(UGsGameObjectBase* In_camParent,
		const FVector& In_targetPos,
		const FRotator& In_targetRot,
		const FVector& In_lookTargetPos,
		FVector& Out_pos);

	// 대사 회전과 위치 가져오기
	bool GetDialogRotAndPos(UGsGameObjectBase* In_camParent,
		UGsGameObjectBase* In_targetObj,
		const FGsSchemaCameraModeData* In_camData,
		const FVector& In_targetPos, 
		const FVector& In_localPos, 
		FVector& Out_pos,
		FRotator& Out_rot,
		FVector& Out_lookTargetPos, 
		float& Out_distCam,
		bool& Out_isHit);

	AActor* GetDialogTargetAcotr(UGsGameObjectBase* In_targetObj);

	// 로직 함수들
public:
		
	// 캐릭터 세팅(초기화와 시점이 다를수 있음...)
	// 2. 백업데이터를 지울지(시공의 틈새에선 안지운다)
	void SetCharacter(UGsGameObjectLocalPlayer* In_char, bool In_isClearBackup);
	// 캐릭터 삭제될때(상태들 가지고 있던 캐릭터 null 처리 해줘야함)
	void RemoveCharacter();
	// 백업 데이터 클리어
	void ClearBackupData();
	// 되돌릴 tr 저장(줌, pitch 회전(위,아래))
	void SaveBackUpTransform();
	// 줌, 회전, diff rot to(back view), 모드
	void SaveBackUpData();
	// 모드에 다른 디폴트 값으로 저장
	void SaveModeBackupData(EGsCameraMode In_mode);

	// 저장 모드로 변경
	void ChangeToBackupMode();

	// 대사 모드 hit 테스트용 그리기
	void DrawTestHitDialogMode();
	// 대사 모드 hit 테스트용 그리기 액티브
	void DrawTestActive(bool In_val);
	// 기본 zoom pitch 사용
	void SaveUseDefaultZoomAndPitch();

	// get bone height value from game data at initialization time 
	// (data generated when lobby character is selected)
	void SaveCameraTargetBoneHeight();
	// load zoom and pitch values to use in observer mode
	void LoadObserverCamModeZoomAndPitch();

	// get, set 함수
public:

	// 현재 모드 카메라 데이터 가져오기
	const FGsSchemaCameraModeData* GetCurrCamModeData();

	// 그리팅 뷰인가
	bool			GetIsGreetingView();
	void			SetIsGreetingView(bool In_val) { _isGreetingView = In_val; }
	EGsCameraMode	GetCurrentMode()			{ return _currentMode; }
	void			SetCurrentMode(EGsCameraMode In_mode) { _currentMode = In_mode; }

	EGsCameraMode	GetBackupMode()				{ return _backupMode; }

	bool			GetIsBackUpTransform()		{ return _isBackUpTransform; }
	bool			GetBackupCamZoom(EGsCameraMode In_mode, float& Out_Zoom);
	void			SetBackupCamZoom(float In_zoom);
	// 저장된 pitch 값을 구한다
	bool			GetBackupCamPitch(EGsCameraMode In_mode, float& Out_pitch);
	// 현재 모드의 pitch를 저장한다
	void			SetBackupCamPitch();

	const FQuat&	GetBackupQuat()				{ return _backupQuat; }

	void			SetBackupQuat(const FQuat& In_quat) { _backupQuat = In_quat; }

	bool			GetIsBackupUserSetting() { return _isBackupUserSetting; }
	void			SetIsBackupUserSetting(bool In_val) { _isBackupUserSetting = In_val; }
	// 맵전환, 대사연출 저장(backuptransform)
	// user setting 저장
	// 둘중하나라도 있으면
	bool			GetIsBackUpData()
	{
		return (_isBackUpTransform || _isBackupUserSetting);
	}

	const FVector& GetBackupCamLocation() { return _backupCamLocation;}
	const FRotator& GetBackupCamRotation() { return _backupCamRotation; }

	void			SetIsCameraDialogPlay(bool In_val) { _isCameraDialogPlay = In_val; }
	bool			GetIsCameraDialogPlay() { return _isCameraDialogPlay; }

	void			SetIsCameraDialogPlayAll(bool In_val) { _isCameraDialogPlayAll = In_val; }
	bool			GetIsCameraDialogPlayAll() { return _isCameraDialogPlayAll; }


	void			SetCameraDialogEndCallbackType(EGsCamearDialogEndCallBackType In_val)
	{
		_cameraDialogEndCallbackType = In_val;
	}
	EGsCamearDialogEndCallBackType			GetCameraDialogEndCallbackType()
	{
		return _cameraDialogEndCallbackType;
	}
	// 대사 카메라 뷰 타입 세팅
	void			SetDialogCameraViewData(const FGsSchemaCameraDialogViewData* In_data);
	// 대사 카메라 뷰 타입 가져오기
	EGsDialogCameraViewType GetDialogCameraViewType()
	{
		return _dialogCameraViewType;
	}

	// 저장
	// 1. 이전 바라보는곳(카메라 연출 시작 바라보는곳 ,나중에 돌아가야되는 바라보는 곳 )
	// 2. 현재 모드 뷰의 바라보는곳
	// 3. 현재 모드 뷰의 바라보는 곳과 카메라 거리
	void			SetBackupDialog(const FVector& In_beforeDialogLookPos,
		const FVector& In_dialogLookPos,
		float In_dialogCamDist);
	const FVector&	GetBackupBeforeDialogLookPos()
	{
		return _backupBeforeDialogLookPos;
	}
	// 현재 뷰 모드의 바라보는 곳 저장
	void SetBackupDialogLookPos(const FVector& In_dialogLookPos)
	{
		_backupDialogLookPos = In_dialogLookPos;
	}
	// 현재 뷰 모드의 바라보는 곳 가져오기
	const FVector& GetBackupDialogLookPos()
	{		
		return _backupDialogLookPos;
	}
	// 현재 뷰 모드의 바라보는 곳과 카메라 거리 저장
	void SetBackupDialogCamDist(float In_dialogCamDist)
	{
		_bakcupDialogCamDist = In_dialogCamDist;
	}
	// 현재 뷰 모드의 바라보는 곳과 카메라 거리 가져오기
	float			GetBakcupDialogCamDist()
	{
		return _bakcupDialogCamDist;
	}
	void SetIsDialogLerpEnd(bool In_isDialogLerpEnd)
	{
		_isDialogLerpEnd = In_isDialogLerpEnd;
	}
	// 히트 체크
	bool GetHitPos(UGsGameObjectBase* In_local,
		const FVector& In_startPos, const FVector& In_endPos,
		FVector& Out_hitPos);

	// 현재 view 데이터 접근
	const FGsSchemaCameraDialogViewData* GetCurrentViewData()
	{
		return _currDialogViewData;
	}

	bool GetIsUseCutFadeOut()
	{
		return _isUseCutFadeOut;
	}

	void SetIsUseCutFadeOut(bool In_isUseCutFadeOut)
	{
		// B1: not use camera move(himichani)
		// https://jira.com2us.com/jira/browse/CHR-17798
		//_isUseCutFadeOut = In_isUseCutFadeOut;
		_isUseCutFadeOut = true;
	}
	// 현재 저장 가능한지
	bool GetIsSavePossible();

	void SetSavedCameraTargetBoneHeight(float In_val)
	{
		_savedCameraTargetBoneHeight = In_val;
	}

	float GetSavedCameraTargetBoneHeight()
	{
		return _savedCameraTargetBoneHeight;
	}
	// backup pitch 구하기
	// 1. backup 값
	// 2. 없으면 기본 모드값
	// 3. min, max 검증 까지 함
	float GetModeBackupCheckedPitch(EGsCameraMode In_mode);
	// mode pitch 구하기
	// 1. 기본 모드값
	// 2. min, max 검증 까지 함
	float GetModeCheckedPitch(EGsCameraMode In_mode);

	float GetCameraShoulderFollowCoolTimeTick()
	{
		return _cameraShoulderFollowCoolTimeTick;
	}

	EGsCameraFixedQuarterYawType GetFixedQuarterYawType();
	void SetFixedQuarterYawType(EGsCameraFixedQuarterYawType In_type)
	{
		_fixedQuarterYawType = In_type;
	}

	UGsGameObjectBase* FindDialogFairyObj();

	bool GetIsZoomFirstCameraMove()
	{
		return _isZoomFirstCameraMove;
	}

	void SetIsZoomFirstCameraMove(bool In_val)
	{
		_isZoomFirstCameraMove = In_val;
	}
};

#define GSCameraMode() UGsScopeHolder::GetGameManagerFType<FGsCameraModeManager>(UGsScopeGame::EManagerType::CameraMode)