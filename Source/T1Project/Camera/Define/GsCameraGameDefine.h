#pragma once

// 카메라 초기화 타입
enum class EGsCameraInitType : uint8
{
	None,
	GreetingEnd,
	ModeChange,
	TargetFollow,
};
// 카메라 회전 상태
enum class EGsCameraDialogRotaionState : uint8
{
	None,					// 초기화 하지 않음
	FirstCalcRotion,		// 처음 계산된 회전중
	AddModifyRotaion,		// 추가 수정 회전중
	Finish,					// 종료
};
// 카메라 회전 대상 타입
enum class EGsCameraRotationType : uint8
{
	WorldRotation,				// 월드값
	ControlRotaion,				// 컨트롤 회전값
};
// 카메라 이동 대상 공간 타입
enum class EGsCameraLocationSpaceType : uint8
{
	WorldLocation,				// 월드값
	RelativeLocation,			// 상대적인 값(카메라 컴퍼넌트 기준)
};

// 대화연출 lerp 종료 타입
enum class EGsDialogCameraFinishType : uint8 
{
	Location,
	Rotation,
};
// 카메라 기능 타입
enum class EGsCameraWorkType : uint8
{
	LocationWork,
	RotationWork,
	OffsetWork,
	ZoomWork,
	FOVWork,
	BlurWork,
	DOFWork,
	TargetOffsetWork,
	DialogCutLoopRotationWork,
	WarpStartRotationWork,
};
// 줌 변환 타입
enum class EGsCameraChangeZoomType : uint8 
{
	ModeChange,		// 모드 변환
	WarpStart,		// 워프 시작
	SpawnStart,		// 스폰 시작
	NoLerp,			// 보간없이 세팅
};
// 초기화 모드 타입
enum class EGsCameraInitModeType : uint8
{
	SameMapSpawn,			// 같은맵으로 스폰될때 처리(보간없이 세팅만)
	OtherMapSpawn,			// 다른맵으로 스폰될때 처리(스폰 줌처리만)
	ModeChange,				// 모드 변환
};

// fixed quarter yaw type
enum class EGsCameraFixedQuarterYawType : uint8
{
	North,		
	East,	
	South,
	West,
};

enum class EGsCamearDialogEndCallBackType : uint8
{
	None,
	Quest,
	EventAction,
};