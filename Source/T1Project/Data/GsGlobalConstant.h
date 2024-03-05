// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Classes/Materials/MaterialInstance.h"
#include "Engine/Classes/Engine/EngineTypes.h"
#include "Runtime/Engine/Public/AI/Navigation/NavQueryFilter.h"
#include "Runtime/NavigationSystem/Public/NavFilters/NavigationQueryFilter.h"

#include "GameObject/ObjectClass/Data/GsHitEffectData.h"
#include "Gameobject/ObjectClass/Data/GsShapeData.h"
#include "ActorComponentEx/GsCameraPostProcessComponent.h"
#include "ActorEx/GsCharacterBase.h"
#include "DataSchema/GsSchemaEnums.h"

#include "Summon/GsSummonCardSlotInfo.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "Map/WorldMap/GsSpaceCrackPortalIconInfo.h"
#include "DataSchema/Sound/GsSchemaSoundResData.h"

#include "GsGlobalConstant.generated.h"

/**
 * 전역에서 쓰이는 설정데이터를 입력받고 처리하기 위한 데이터 에셋
 */
UCLASS(BlueprintType, Category = "GlobalConstant")
class T1PROJECT_API UGsGlobalConstant : public UDataAsset
{
	GENERATED_BODY()
public:
	UGsGlobalConstant();
public:
	
	UPROPERTY(EditAnywhere, Category = "GlobalConstant")
	float GlobalCoolTime;

	// 수동 타겟 검색시 우선 검색 반영 영역
	// 대상 선정 옵션에 상관 없이 해당 영역에 검색된 IsEnemy 개체 우선 타겟팅
	UPROPERTY(EditAnywhere, Category = "TargetConstant", Meta = (Tooltip = "수동 타겟 검색시 우선 검색 반영 영역"))
	float ManualControlTargetSelectionDistance = 300.f;

	// 해당 시간 경과후 유저 타겟대상일 경우 자동 클리어 (Sec)
	UPROPERTY(EditAnywhere, Category = "TargetConstant", Meta = (Tooltip = "수동 타겟 검색시 우선 검색 반영 영역"))
	float AutoUserTargetClear = 5.f;

	// AutoMove 도착 거리에 대한 비율 설정
	UPROPERTY(EditAnywhere, Category = "TargetConstant")
	float LocalPlayerMoveToTargetGoalScale = 0.4f;
	UPROPERTY(EditAnywhere, Category = "TargetConstant")
	float LocalPlayerMoveToTargetGoalScale_Camp = 0.9f;
	UPROPERTY(EditAnywhere, Category = "TargetConstant")
	float ForceDiconnetTime = 600.f;

	// 내 플레이어 Run/Walk 전환 조이스틱 스케일 비율
	UPROPERTY(EditAnywhere, Category = "GlobalConstant")
	float LocalPlayerRunStateChangeRate = 0.5f;

	// 타겟마커 머티리얼 인터페이스
	UPROPERTY(EditAnywhere, Category = "GlobalConstant")
	UMaterialInterface* TargetMarkerDecal_first;
	UPROPERTY(EditAnywhere, Category = "GlobalConstant")
	UMaterialInterface* TargetMarkerDecal_second;

	UPROPERTY(EditAnywhere, Category = "GlobalConstant")
	UMaterialInterface* _interactionMoveTargetMarkerDecal;

	// 섀도우데칼 머티리얼 인터페이스
	UPROPERTY(EditAnywhere, Category = "GlobalConstant")
	UMaterialInterface* ShadowDecalMI;

	// 라돈 Boss Die 시, WorldSlowMotion Time 값
	UPROPERTY(EditAnywhere, Category = "GlobalConstant")
	float WorldSlowMotionTimeValue = 0.5;

	// LocalPlayer 체력 PVP 점등 지속시간 (현재 애니메이션 타임은 2초임)
	UPROPERTY(EditAnywhere, Category = "GlobalConstant")
	float PvpHitEffectDuration = 4.0f;

	// 전역 히트이펙트 처리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GlobalConstant")
	UGsHitEffectData* HitEffectData;
	// 전역 히트 사운드 처리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GlobalConstant")
	FGsSchemaSoundResDataRow HitSoundData;

	// ai possess 전환 대기 시간
	UPROPERTY(EditAnywhere, Category = "GlobalConstant")
	float WaitTimeAIPossess = 0.5f;

	// 지속형 스킬(BUFF) 딜레이 시간
	UPROPERTY(EditAnywhere, Category = "GlobalConstant")
	float BuffSkillDelayTime = 0.3f;

	// 스킬 사용 요청 응답 지연시 요청 관련 플레그 제거 시간
	UPROPERTY(EditAnywhere, Category = "GlobalConstant")
	float SkillRequestRestoreTime = 5.f;

	// LocalPlayer 체력경고 점등 기준값 (max HP 기준. %율 ex: value 값 미만이면 점등)
	UPROPERTY(EditAnywhere, Category = "GlobalConstant")
	int32 ClientCullingDistance = 1800;

	// LocalPlayer 조준 스킬 예시선 정보
	UPROPERTY(EditAnywhere, Category = "GlobalConstant")
	FAimSkillData AimSkillInfo;
	// 캡슐 재계산 대기시간
	UPROPERTY(EditAnywhere, Category = "GlobalConstant")
	float RecalcCapsuleWaitTime = 0.1f;

public:
	// (카메라)스프링 암 거리 보간 속도(모바일)
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float CamArmLengthSpeed = 3.0f;
	// (카메라)스프링 암 거리 보간 속도(PC)
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float CamArmLengthSpeedPC = 3.0f;
	// (카메라)스프링 암 input 거리 보간 속도(모바일)
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float CamInputArmLengthSpeedMobile = 3.0f;
	// (카메라)스프링 암 input 거리 보간 속도(PC)
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float CamInputArmLengthSpeedPC = 3.0f;

	// (카메라)회전 크기
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float CamAddRotVal = 0.3f;
	// (카메라)회전 보간 속도	
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float CamAutoRotIntpSpeed = 1.5f;

	// (카메라)모드 변환 회전 보간 속도
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float CamModeChangeRotLerpSpeed = 1.5f;

	// (카메라)대사 연출 회전 보간 속도
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float CamDialogRotLerpSpeed = 1.0f;

	// (카메라)대사 연출 이동 보간 속도
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float CamDialogLocLerpSpeed = 20.0f;

	// (카메라)대사 연출 속도 곱하는값 
	// 기획팀에선 이값만 곱하면 전체적으로 속도조절이 됨
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float CamDialogLerpSpeedMul = 1.0f;

	// (cam) dialog total min speed
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float CamDialogMinSpeed = 0.5f;
	// (카메라)따라가기 회전 보간 속도
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float CamFollowRotLerpSpeed = 0.3f;
	// (camera) shoulder target follow rot speed
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float CamShoulderTargetRotLerpSpeed = 0.3f;

	// (카메라)그리팅 카메라 줌
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float CamGreetingZoom = 100.0f;

	// (카메라)그리팅 카메라 회전 더할값
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	FRotator CamAddGreetingRotator = FRotator::ZeroRotator;

	// (카메라)그리팅 카메라 socket offset 더할 값
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	FVector CamAddGreetingSocketOffset = FVector::ZeroVector;

	// 캡슐 기본 half 사이즈(비율 계산용)
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float _camGreetingBaseCapsuleHalfHeight = 40.0f;

	// 캡슐 높이 곱할 값 사이즈(비율 계산용)
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float _camGreetingMulCapsuleHeight = 40.0f;

	// (카메라)그리팅 카메라 줌 강도
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float CamZoomPower = 1;

	// (카메라)그리팅 카메라 줌 강도(PC)
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float CamZoomPowerPC = 3;
	// (카메라)기본 모드
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	EGsCameraMode _camBaseMode = EGsCameraMode::BackView;
	// (카메라) 워프 줌 거리
	UPROPERTY(EditAnywhere, Category= "CameraConstant")
	float _cameraWarpZoom = 100.0f;
	// (카메라) 줌 완료 diff
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float _cameraZoomFinishDiff = 10.0f;
	// (카메라) fixed quarter degree 값(yaw 방향)(north)
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float _cameraFixedQuarterModeYawDegreeNorth = 0.0f;
	// (카메라) fixed quarter degree 값(yaw 방향)(east)
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float _cameraFixedQuarterModeYawDegreeEast = 90.0f;
	// (카메라) fixed quarter degree 값(yaw 방향)(south)
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float _cameraFixedQuarterModeYawDegreeSouth = 180.0f;
	// (카메라) fixed quarter degree 값(yaw 방향)(west)
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float _cameraFixedQuarterModeYawDegreeWest = 270.0f;

	// (카메라) 카메라 이동시킬 기준 본 이름
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	FName _cameraMoveTargetBoneName;
	// (카메라) 숄더뷰 follow 쿨타임
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float _cameraShoulderFollowCoolTime = 1.0f;
	// (카메라) zoom min lerp 속도
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float _cameraZoomMinLerpSpeed = 1.0f;
	// (카메라) dof 스케일 최대값
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float _cameraDepthOfFieldScaleMax = 2.0f;
	// 모든 모드의 최소값 체크(요거 밑으로 내려가면 자름)
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float _cameraModeZoomMin = 150.0f;
	// 그리팅 fov
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float _cameraGreetingFOV = 50.0f;
	// 그리팅 dof focal region
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float _cameraGreetingDepthOfFieldFocalRegionMobile = 200.0f;
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float _cameraGreetingDepthOfFieldFocalDistance = 200.0f;
	// 숄더뷰 타겟 추가 yaw 값
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float _cameraShoulderTargetAddYaw = -20.0f;

	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float _cameraZoomDepthofFieldFstop = 1.0f;

	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	bool _isUseDialogLerpZoom = true;

	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	int _directGreetingNextTick = 3;

public:
	UPROPERTY(EditAnywhere, Category = "WarpCameraConstant")
	float _warpCameraZoomValue = 800.f;
	UPROPERTY(EditAnywhere, Category = "WarpCameraConstant")
	FRotator _warpCameraControllerRotatorOffset = FRotator(-20.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, Category = "WarpCameraConstant")
	float _warpAnimationCutRate = 0.9f;
public:
	// (입력) 터치 이동 값(모바일)
	UPROPERTY(EditAnywhere, Category = "Input")
	float _touchMoveFactor = 0.25f;
	// (입력) 터치 이동 값(PC)
	UPROPERTY(EditAnywhere, Category = "Input")
	float _touchMoveFactorPC = 1.0f;

public:
	// (바라보기) 회전 속도
	UPROPERTY(EditAnywhere, Category = "CameraLookAt")
	float LookAtRotSpeed = 3.0f;
	// (바라보기) yaw 최소 값
	UPROPERTY(EditAnywhere, Category = "CameraLookAt")
	float LookAtYawMin = -60.0f;
	// (바라보기) yaw 최대 값
	UPROPERTY(EditAnywhere, Category = "CameraLookAt")
	float LookAtYawMax = 60.0f;
	// (lookat) finish finish diff val
	UPROPERTY(EditAnywhere, Category = "CameraLookAt")
	float LookAtFinishDiffVal = 0.001f;

public:
	// (로비줌) 줌 강도(PC)
	UPROPERTY(EditAnywhere, Category = "CameraLobbyZoom")
	float LobbyCamZoomPowerPC = 50;
	// (로비줌) 줌 강도(모바일)
	UPROPERTY(EditAnywhere, Category = "CameraLobbyZoom")
	float LobbyCamZoomPowerMobile = 10;
	// (로비줌) 캐릭터 선택창 초기화 줌 비율
	UPROPERTY(EditAnywhere, Category = "CameraLobbyZoom")
	float SelectInitZoomRate = 0.7f;
	// (로비 카메라) 머리 위치 다른값 판정값
	UPROPERTY(EditAnywhere, Category = "CameraLobbyZoom")
	float cameraLobbyDiffHeadPos = 0.1f;

public:
	// (로비줌) pitch 회전 min
	UPROPERTY(EditAnywhere, Category = "CameraLobbyRot")
	float PitchRotMin = -25;
	// (로비줌) pitch 회전 max
	UPROPERTY(EditAnywhere, Category = "CameraLobbyRot")
	float PitchRotMax = 10;
	// (로비줌) pitch 회전 max
	UPROPERTY(EditAnywhere, Category = "CameraLobbyRot")
	float PitchRotSpeed = 3.0f;

	UPROPERTY(EditAnywhere, Category = "CameraLobby")
	float LobbyCamMoveFactorMobile = 0.5f;
	UPROPERTY(EditAnywhere, Category = "CameraLobby")
	float LobbyCamMoveFactorPC = 0.5f;
public:
	// (로비) 월드 입장 시 카메라 회전 연출 시간	
	UPROPERTY(EditAnywhere, Category = "CameraLobby")
	float LobbyGameEnterCamMoveTimeRot = 2.0f;

	// (로비) 월드 입장 시 카메라 위치 연출 시간	
	UPROPERTY(EditAnywhere, Category = "CameraLobby")
	float LobbyGameEnterCamMoveTimePos = 1.0f;

	// (로비) 월드 입장 시 카메라 FOV 연출 시간	
	UPROPERTY(EditAnywhere, Category = "CameraLobby")
	float LobbyGameEnterCamMoveTimeFOV = 2.0f;

	// (로비) 월드 입장 시 카메라 FOV
	UPROPERTY(EditAnywhere, Category = "CameraLobby")
	float LobbyGameEnterCamFOV = 40.0f;

	// (로비) 월드 입장 시 카메라 회전 값
	UPROPERTY(EditAnywhere, Category = "CameraLobby")
	FRotator LobbyGameEnterCamRot = FRotator(349.8f, 155.0f, 0.0f);

	// (로비) 월드 입장 시 카메라 위치 값
	UPROPERTY(EditAnywhere, Category = "CameraLobby")
	FVector LobbyGameEnterCamPos = FVector(1030.0f, 0.0f, 0.0f);

public:
	UPROPERTY(EditAnywhere, Category = "LobbyCharacter")
	FName Lobby_Pos_CustomizeCharacter;

	UPROPERTY(EditAnywhere, Category = "LobbyCharacter")
	FName Lobby_Pos_CLobbyCharacter;

	UPROPERTY(EditAnywhere, Category = "LobbyCharacter")
	FString LobbyPrePlayableMovieName;

	UPROPERTY(EditAnywhere, Category = "LobbyCharacter")
	FString LobbyPostPlayableMovieName;

	UPROPERTY(EditAnywhere, Category = "LobbyCharacter")
	FString OpeningMovieName;

	UPROPERTY(EditAnywhere, Category = "LobbyCharacter")
	int LobbyCharacterPresetSequenceId = 0;

	UPROPERTY(EditAnywhere, Category = "LobbyCharacter")
	FName LobbyCharacterPreset1_Tag;

	UPROPERTY(EditAnywhere, Category = "LobbyCharacter")
	FName LobbyCharacterPreset2_Tag;

	UPROPERTY(EditAnywhere, Category = "LobbyCharacter")
	FName LobbyCharacterPreset3_Tag;

	UPROPERTY(EditAnywhere, Category = "LobbyCharacter")
	FName LobbyCharacterPreset4_Tag;

	UPROPERTY(EditAnywhere, Category = "LobbyCharacter")
	float LobbyCharacterPreset1_PosZ = 0;

	UPROPERTY(EditAnywhere, Category = "LobbyCharacter")
	float LobbyCharacterPreset2_PosZ = 0;

	UPROPERTY(EditAnywhere, Category = "LobbyCharacter")
	float LobbyCharacterPreset3_PosZ = 0;

	UPROPERTY(EditAnywhere, Category = "LobbyCharacter")
	float LobbyCharacterPreset4_PosZ = 0;

	UPROPERTY(EditAnywhere, Category = "LobbyLevel")
	bool IsLobbyLevel = false;

public:
	// (인풋)pich 회전시 추가 곱
	UPROPERTY(EditAnywhere, Category = "InputConstant")
	float InputPitchRotMul = 1;

	UPROPERTY(EditAnywhere, Category = "Ending")
	float EndingSlowMotionSecond = 5.5f;

	UPROPERTY(EditAnywhere, Category = "Ending")
	float EndingFadeOutSecond = 1.0f;

	UPROPERTY(EditAnywhere, Category = "InputConstant")
	float _touchMoveNavigationDepth = 1000.0f;

	//Player camera post process effect
public:
	UPROPERTY(EditDefaultsOnly, Category = "UGsCameraPostProcessComponent")
	TArray<FPostProcessMaterialData> _playerCameraPostProcessMaterials;

	UPROPERTY(EditDefaultsOnly, Category = "UGsCameraPostProcessComponent")
	TMap<EGsPlayerPostProcess, FPostProcessCurveData> _PlayerCameraPostProcessMap;

	UPROPERTY(EditAnywhere, Category = "UGsCameraPostProcessComponent")
	FCameraMotionBlurData _motionBlur;

	// 미니맵 
public:
	// 보여지는 반경
	UPROPERTY(EditAnywhere, Category = "Minimap")
	float _viewGameObjectRadius = 1000.0f;

	// 미니맵: 추가될 자동 이동 경로 간격(path 노드들 사이에 메꿀 간격)
	UPROPERTY(EditAnywhere, Category = "Minimap")
	float _minimapAddAutoPathDist = 10.0f;

	// 미니맵: 자동 이동 경로 삭제될 간격(로컬이 근처에 도달하면 삭제할 간격)
	UPROPERTY(EditAnywhere, Category = "Minimap")
	float _minimapAutoPathRemoveDist = 500.0f;

	// 파티클
public:
	// 파티클 스케일을 위한 기준 사이즈 설정 (x : radius, y : height)
	UPROPERTY(EditAnywhere, Category = "Effect")
	FVector2D _defaultParticleCapsuleSize = FVector2D(34.f, 88.f);

	// 포션 파티클
	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* _potionParticle;
	// 디버프 파티클
	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* _debuffParticle;
	// 워프 파티클
	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* _warpParticle;

	// 디버프 이펙트 붙일 소켓 이름
	UPROPERTY(EditAnywhere, Category = "Effect")
	FName _debuffAttachPointName;

	// 디버프 이펙트 오프셋
	UPROPERTY(EditAnywhere, Category = "Effect")
	FVector _debuffOffset;

	// 스킬 습득 파티클(index = SkillGrade -1. 0은 에러값이므로 -1처리)
	UPROPERTY(EditAnywhere, Category = "Effect")
	TArray<UParticleSystem*> _learnSkillParticle;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem*		_equipChangeParticle;

	// 몬스터 애드 이펙트
	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem*		_attackCognitionEffect;
	UPROPERTY(EditAnywhere, Category = "Effect")
	FName					_attackCognitionEffectSocketName;
	UPROPERTY(EditAnywhere, Category = "Effect")
	FVector					_attackCognitionOffsetLocation;
	UPROPERTY(EditAnywhere, Category = "Effect")
	float					_attackCognitionRemove = 1.f;

public:
	UPROPERTY(EditAnywhere, Category = "ClientSpawn")
	float _clientSpawnDistance = 3000.0f;
	UPROPERTY(EditAnywhere, Category = "ClientSpawn")
	float _clientUpdateRate = 0.25f;

	// DirectMoveUpdate(강제 위치 이동) 이용시 같은 위치로 판단할 거리 값
	UPROPERTY(EditAnywhere, Category = "GameObject")
	float _directMoveSameDistance = 100.f;

	// 타유저가 보내온 좌표와 클라이언트에서 보유한 좌표차 (허용범위)
	UPROPERTY(EditAnywhere, Category = "GameObject")
	float _allowDistance = 500.f;

	// 인터랙션
public:
	// 버튼 위치 오프셋(퀘스트 타겟 마크도 같이 쓴다)
	UPROPERTY(EditAnywhere, Category = "Interaction")
	FVector _interactionButtonPosOffset;
	// 인터랙션 위젯의 offset(y값)
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float _interactionWidgetOffset = 50.0f;
	// 가로세로비율 보간으로 처리될 가중치(영향력 적게 처리용)
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float _interactionWidgetAspectRatioWeight = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float _interactionPosToTargetMoveAcceptanceDistance = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float _rateNpcInteractionShiftPos = 0.8f;
public:
	// 상태이상 아이콘 깜빡이는 기준 초(n초 이하 부터 깜빡거림)
	UPROPERTY(EditAnywhere, Category = "Abnormality")
	float _abnormalityIconFlickeringSecond = 10.0f;

	// 이동
public:
	// 위치 이동 도달 range
	UPROPERTY(EditAnywhere, Category = "Move")
	float _posMoveAcceptanceRadius = 100.0f;
	// 테스트 위치 array 값
	UPROPERTY(EditAnywhere, Category = "Move")
	TArray<FVector> _arrTestPos;
	// if find failed at navi goal
	// retry other possible pos in radius
	UPROPERTY(EditAnywhere, Category = "Move")
	float _findNearPosRandomRadius = 1000.0f;
	UPROPERTY(EditAnywhere, Category = "Move")
	FVector _findNearPosExtend;
	UPROPERTY(EditAnywhere, Category = "Move")
	float _distanceToQuestGoalArea = 200.0f;
	// 퀘스트
public:
	// 메인퀘스트 타겟 마크
	UPROPERTY(EditAnywhere, Category = "Quest")
	FSoftObjectPath _questTargetMark;

	// 반복퀘스트 타겟 마크
	UPROPERTY(EditAnywhere, Category = "Quest")
	FSoftObjectPath _repeatQuestTargetMark;

	// 최소 거리안은 표식안함
	UPROPERTY(EditAnywhere, Category = "Quest")
	float _drawQuestDistanceLimit = 50;

	// 대화 연출
public: 
	// 대화 연출 hit type test
	UPROPERTY(EditAnywhere, Category = "CameraDialog")
	int _testCamDialogCollsion;

	// (카메라)모드 변환 회전 보간 속도(대화 연출)
	UPROPERTY(EditAnywhere, Category = "CameraDialog")
	float _camDialogRotLerpSpeed = 3.0f;

	// 회전 rot 같음처리 비교값
	UPROPERTY(EditAnywhere, Category = "CameraDialog")
	float _rotEqualVal = 0.4f;

	// 카메라 대사 페어리 스폰 회전 값
	UPROPERTY(EditAnywhere, Category = "CameraDialog")
	float _camDialogFairySpawnRotate = 90.0f;

	// 카메라 대사 페어리 스폰 반경
	UPROPERTY(EditAnywhere, Category = "CameraDialog")
	float _camDialogFairySpawnRange = 100.0f;

	UPROPERTY(EditAnywhere, Category = "CameraDialog")
	float _camDialogTargetNPCPositionDistance = 700.0f;

	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	float _dialogCutLoopCameraMoveSpeed = 1.0f;
	UPROPERTY(EditAnywhere, Category = "CameraConstant")
	FRotator _dialogCutLoopCameraMoveOffset;
public:
	// 채널 리스트 요청 쿨타임(초)
	UPROPERTY(EditAnywhere, Category = "Channel")
	int _requestChannelListCooltime = 5;

	// 채널 이동 쿨타임(초)
	UPROPERTY(EditAnywhere, Category = "Channel")
	int _changeChannelCooltime = 180;

public:
	UPROPERTY(EditDefaultsOnly, Category = "DropSystem", meta = (ToolTip = "아이템 드랍 연출을 위한 포물선 데이터"))
	UCurveFloat* _dropObjectSpawnCurve;
	UPROPERTY(EditDefaultsOnly, Category = "DropSystem", meta = (ToolTip = "아이템 드랍 연출시 걸리는 시간"))
	float _dropObjectSpawnDuration = 0.5f;
	UPROPERTY(EditDefaultsOnly, Category = "DropSystem", meta = (ToolTip = "아이템 드랍 연출시 최대 높이"))
	float _dropObjectSpawnHeight = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category = "DropSystem", meta = (ToolTip = "아이템 루팅시 걸리는 시간"))
	float _dropObjectLootingDuration = 0.3f;
	UPROPERTY(EditDefaultsOnly, Category = "DropSystem", meta = (ToolTip = "아이템 루팅 거리"))
	float _dropObjectLootingRangeFactor = 100.0f;

public:
	// 서버 월드 타임 싱크 패킷 전송 주기(초)
	UPROPERTY(EditAnywhere, Category = "WorldTime")
	float _syncWorldTimeDuration = 600.f;

	// 위치 즐겨 찾기
public:
	// 즐겨찾기 갯수 최대갯수
	UPROPERTY(EditAnywhere, Category = "Bookmark")
	int _bookmarkListItemMaxCount = 50;
	// 즐겨찾기 이름 최대 길이
	UPROPERTY(EditAnywhere, Category = "Bookmark")
	int _bookmarkNameLenMax = 24;
	// 즐겨찾기 이름 최소 길이
	UPROPERTY(EditAnywhere, Category = "Bookmark")
	int _bookmarkNameLenMin = 2;
	// 공유 팝업 대기 시간
	UPROPERTY(EditAnywhere, Category = "Bookmark")
	float _bookmarkSharePopupWaitTime = 10.0f;
	// 공유 쿨타임
	UPROPERTY(EditAnywhere, Category = "Bookmark")
	float _bookmarkShareCoolTime = 60.0f;

public:
	// 하드캡
	UPROPERTY(EditAnywhere, Category = "Hardcap")
	float _hardcapAttackSpeedRateMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Hardcap")
	float _hardcapAttackSpeedRateMax = 1.5f;

	// MoveSpeedRate
	UPROPERTY(EditAnywhere, Category = "Hardcap")
	float _hardcapMoveSpeedRateMin = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Hardcap")
	float _hardcapMoveSpeedRateMax = 2.0f;

	// CastingSpeedRata
	UPROPERTY(EditAnywhere, Category = "Hardcap")
	float _hardcapCastingSpeedRateMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Hardcap")
	float _hardcapCastingSpeedRateMax = 1.5f;

public:
	UPROPERTY(EditAnywhere, Category = "Movie")
	float _moviePlayTime = 3.5f;

	// 파티
public:
	// 초대 이름 최대 길이
	UPROPERTY(EditAnywhere, Category = "Party")
	int _partyInviteekNameLenMax = 12;
	// 초대 이름 최소 길이
	UPROPERTY(EditAnywhere, Category = "Party")
	int _partyInviteedNameLenMin = 2;
	// 파티 멤버 최대수
	UPROPERTY(EditAnywhere, Category = "Party")
	int _partyMemberMax = 4;
	// 파티 초대 대기 시간(초)
	UPROPERTY(EditAnywhere, Category = "Party")
	float _partyInviteWaitSecond = 15.f;
	// 아이콘 이동시킬 사이즈
	UPROPERTY(EditAnywhere, Category = "Party")
	float _partyIconShiftSize = 30.f;
	// 방향 widget path
	UPROPERTY(EditAnywhere, Category = "Party")
	FSoftObjectPath _arrowWidgetPath;
	// 60초내 초대횟수 최대치
	UPROPERTY(EditAnywhere, Category = "Party")
	int _maxCountInvite  = 3;
	// 초대 제한 체크 타임(60초)
	UPROPERTY(EditAnywhere, Category = "Party")
	float _inviteLimitCheckTime = 60.0f;
	// 초대 제한 시간(15초)
	UPROPERTY(EditAnywhere, Category = "Party")
	float _inviteBanTime = 15.0f;


	//Prop
public:
	UPROPERTY(EditAnywhere, Category = "Prop")
	UCurveFloat* _propDissolveCurve;
	UPROPERTY(EditAnywhere, Category = "Prop")
	FSoftObjectPath _propTouchPath;

	//fairy window zoom 
public:
	UPROPERTY(EditAnywhere, Category = "CameraFairyZoom")
	float _fairyZoomSpeed = 0.2f; //최대 이동 거리를 움직이는 sec

	UPROPERTY(EditAnywhere, Category = "CameraFairyZoom")
	float _fairyZoomDistance = 50.f; //최대 이동 거리

	UPROPERTY(EditAnywhere, Category = "CameraFairyZoom")
	float _fairyTouchZoomFactor = 10.f; //two touch 이동 간격 조절값

	UPROPERTY(EditAnywhere, Category = "CameraFairyZoom")
	float _fairyRotateMoveFactor = 0.5f; //페어리 회전시 터치 이동거리 조절값(값이 작아질 수록 움직임이 작아짐)

	UPROPERTY(EditAnywhere, Category = "CameraFairyZoom")
	float _fairyRotateMoveLimit = 3.0f;	// 페어리 회전시 터치 이동거리 제한값(min, max가 됨)

	UPROPERTY(EditAnywhere, Category = "CameraFairyZoom")
	FVector _fairyEnchantOffset = FVector(50.f, 0.f, -20.f);

	// 자동전투 옵션
public:
	// 되돌아가기 거리 1
	UPROPERTY(EditAnywhere, Category = "AutoOption")
	float _autoReturnDist1 = 1000.f;
	// 되돌아가기 거리 2
	UPROPERTY(EditAnywhere, Category = "AutoOption")
	float _autoReturnDist2 = 2000.f;
	// 되돌아가기 거리 3
	UPROPERTY(EditAnywhere, Category = "AutoOption")
	float _autoReturnDist3 = 3000.f;
	// 되돌아가기 거리 4
	UPROPERTY(EditAnywhere, Category = "AutoOption")
	float _autoReturnDist4 = 5000.f;
	
	// 서버 패킷 받아서 저장하는 변수
	float _autoReturnNoLimit = 10000.f;

	// 환경설정
public:
	// 현실 시간 몇 분 주기로 날씨가 변경될 것인가 
	UPROPERTY(EditAnywhere, Category = "Environment")
	int32 _weatherChangeCycleMinute = 30;

public:
	UPROPERTY(EditAnywhere, Category = "Environment")
	int32 _maxPenaltyCount = 10;

	//Revive
public:
	UPROPERTY(EditAnywhere, Category = "Show")
	float _revivePopupShowDelayTime = 3.0f;

	// 천마석(정령탄)
public:
	UPROPERTY(EditAnywhere, Category = "SpiritShot")
	FSoftObjectPath _spiritShotHitEffect;
	UPROPERTY(EditAnywhere, Category = "SpiritShot")	
	FSoftObjectPath _spiritShotUseEffect;
	UPROPERTY(EditAnywhere, Category = "SpiritShot")
	FGsSchemaSoundResDataRow _spiritShotHitSound;
	UPROPERTY(EditAnywhere, Category = "SpiritShot")
	FGsSchemaSoundResDataRow _spiritShotUseSound;

	// 터치 이동
public:
	// 터치 이동시 릴리즈 차이값(이값 밑으론 이동처리)
	UPROPERTY(EditAnywhere, Category = "TouchMove")
	float _touchMoveReleaseDiff = 5.0f;
	// 터치 이동시 땅에 남는 액터 경로
	UPROPERTY(EditAnywhere, Category = "TouchMove")
	FSoftObjectPath _touchMovePosObjectPath;

	// 뽑기
public:
	// 뽑기 방 bp 경로
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _summonSceneObjectPath;
	// scene: summon fairy
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _summonFairySceneObjectPath;
	// scene: summon costume
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _summonCostumeSceneObjectPath;
	// scene: compose
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _composeSceneObjectPath;
	// scene: high get
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _highGetSceneObjectPath;
	// scene: 고등급 등장 전(테미르 꽝)
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _highPreSceneObjectPath;

	// 소환석 bp 경로(일반)
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _summonStoneNormalPath;
	// 소환석 bp 경로(고등급)
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _summonStoneHighPath;
	// 코스튬 소환전 bp 경로(일반)
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _summonVeilCostumeNormalPath;
	// 코스튬 소환전 bp 경로(고등급)
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _summonVeilCostumeHighPath;
	// 소환석 스폰 대기 시간
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _spawnSummonStoneWaitSecond = 1.0f;
	// 소환 unveil object 스폰 대기 시간
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _spawnSummonUnveilObjectWaitSecond = 1.0f;
	// 클릭 위젯 오프셋
	UPROPERTY(EditAnywhere, Category = "Summon")
	FVector _clickWigetOffset;
	// 테스트용 소환할 id들(페어리)
	UPROPERTY(EditAnywhere, Category = "Summon")
	TArray<uint32> _testArraySummonFairyIds;
	// 테스트용 소환할 id들(페어리)_1
	UPROPERTY(EditAnywhere, Category = "Summon")
	uint32 _testArraySummonFairyId_Single;
	// 테스트용 소환할 id들(코스튬)
	UPROPERTY(EditAnywhere, Category = "Summon")
	TArray<uint32> _testArraySummonCostumeIds;
	// 테스트용 소환할 id들(코스튬)_1
	UPROPERTY(EditAnywhere, Category = "Summon")
	uint32 _testArraySummonCostumeId_Single;
	// 처음 시작 카메라 연출 시퀀스 id
	UPROPERTY(EditAnywhere, Category = "Summon")
	int _startSummonCameraSequenceId = 0;
	// 일반 등급 페어리 스폰 이펙트
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _effectNormalGradeSummonSpawnFairy;
	// 고급 등급 페어리 스폰 이펙트
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _effectHighGradeSummonSpawnFairy;
	// 일반 등급 코스튬 스폰 이펙트
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _effectNormalGradeSummonSpawnCostume;
	// 고급 등급 코스튬 뽑기전 이펙트
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _effectHighGradeSummonSpawnCostume;
	// normal start sequence costume
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _effectNormalStartSequenceSummonCostume;
	// high (1 more rare) start sequence costume
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _effectHighStartSequenceSummonCostume;

	// 일반 시작 시퀀스 코스튬 0번
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _effectNormalStartSequenceSummonCostume_0;
	// high (1 more rare) start sequence costume 0번
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _effectHighStartSequenceSummonCostume_0;

	// 일반 시작 시퀀스 코스튬 1번
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _effectNormalStartSequenceSummonCostume_1;
	// high (1 more rare) start sequence costume 1번
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _effectHighStartSequenceSummonCostume_1;

	// normal start sequence fairy
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _effectNormalStartSequenceSummonFairy;
	// high (1 more rare) start sequence fairy
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _effectHighStartSequenceSummonFairy;

	// 일반 페어리 날라가는 빛 이펙트
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _effectNormalMoveLightSequenceSummonFairy;
	// 고등급(1개라도 레어 있는거) 페어리 날라가는 빛 이펙트
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _effectHighMoveLightSequenceSummonFairy;

	// 고등급 획득 연출 전 테미르의 카드(코스튬)
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _highPreCardCostume;
	// 고등급 획득 연출 전 테미르의 카드(페어리)
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _highPreCardFairy;

	// 테미르 쾅에서 등급 결정 이펙트 출력(에픽)
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _effectHighPreGradeEpic;
	// 테미르 쾅에서 등급 결정 이펙트 출력(유니크)
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _effectHighPreGradeUnique;
	// 테미르 쾅에서 등급 결정 이펙트 출력(레전드)
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _effectHighPreGradeLegend;


	// 소환석 등급 이펙트 오프셋
	UPROPERTY(EditAnywhere, Category = "Summon")
	FVector _summonStoneGradeEffectOffset;
	// 큐브 등급 이펙트 오프셋
	UPROPERTY(EditAnywhere, Category = "Summon")
	FVector _summonCubeGradeEffectOffset {0,0,30};
	// 고등급 카메라 속도
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonCameraHighGradeSpeed = 1.0f;
	// 고등급 카메라 속도 mul offset
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonCameraSpeedOffset = 1.0f;
	// 줌 바라볼 대상 오프셋
	UPROPERTY(EditAnywhere, Category = "Summon")
	FVector _summonLookTargetOffset;
	// 줌 값
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonArmLength;
	// 드래그 곱할 값
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragMulVal = 1.0f;
	// 드래그 색션 이름
	UPROPERTY(EditAnywhere, Category = "Summon")
	FName _summonMontageDragSectionName;	
	// 소환석 터치 해제 곱할 값
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonStoneTouchReleaseMulVal = 1.0f;
	// 드래그 릴리즈 성공 판단값
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonStoneDragReleaseSuccessCheckVal = 0.7f;
	// 고등급 오픈 카메라 연출 시퀀스 id(페어리:레어)
	UPROPERTY(EditAnywhere, Category = "Summon")
	int _summonHighGradeOpenCameraFairyRareSequenceId = 0;
	// 고등급 오픈 카메라 연출 시퀀스 id(페어리:에픽 이상)
	UPROPERTY(EditAnywhere, Category = "Summon")
	int _summonHighGradeOpenCameraFairyEpicOverSequenceId = 0;
	// 고등급 오픈 카메라 연출 시퀀스 id(코스튬: 레어)
	UPROPERTY(EditAnywhere, Category = "Summon")
	int _summonHighGradeOpenCameraCostumeRareSequenceId = 0;
	// 고등급 오픈 카메라 연출 시퀀스 id(코스튬: 에픽 이상)
	UPROPERTY(EditAnywhere, Category = "Summon")
	int _summonHighGradeOpenCameraCostumeEpicOverSequenceId = 0;
	// cinematic id: summon fairy(11)
	UPROPERTY(EditAnywhere, Category = "Summon")
	int _summonFairySequenceId = 0;
	// cinematic id: summon fairy(1)
	UPROPERTY(EditAnywhere, Category = "Summon")
	int _summonFairySequenceId_Single = 0;
	// cinematic id: summon costume(11)
	UPROPERTY(EditAnywhere, Category = "Summon")
	int _summonCostumeSequenceId = 0;
	// cinematic id: summon cosutme(1)
	UPROPERTY(EditAnywhere, Category = "Summon")
	int _summonCostumeSequenceId_Single = 0;
	// cinematic id: compose start
	UPROPERTY(EditAnywhere, Category = "Summon")
	int _summonComposeStartSequenceId = 0;
	// cinematic id array: compose result(success)
	UPROPERTY(EditAnywhere, Category = "Summon")
	TArray<int> _summonComposeResultSuccessSequenceId;
	// cinematic id array: compose result(fail)
	UPROPERTY(EditAnywhere, Category = "Summon")
	TArray<int> _summonComposeResultFailSequenceId;
	// cinematic id array: fairy high grade get(epic/unique/legend)
	UPROPERTY(EditAnywhere, Category = "Summon")
	TArray<int> _summonFairyHighGetSequenceId;
	// cinematic id array: costume high grade get(epic/unique/legend)
	UPROPERTY(EditAnywhere, Category = "Summon")
	TArray<int> _summonCostumeHighGetSequenceId;
	// 고등급 획득전 테미르 쾅 시네마틱(레어)
	UPROPERTY(EditAnywhere, Category = "Summon")
	int _summonHighPreSequenceId_Rare = 0;
	// 고등급 획득전 테미르 쾅 시네마틱(에픽 이상)
	UPROPERTY(EditAnywhere, Category = "Summon")
	int _summonHighPreSequenceId_OverEpic = 0;

	// 반복 구매인가
	UPROPERTY(EditAnywhere, Category = "Summon")
	bool _isSummonRetry = false;
	// 재화 타입
	UPROPERTY(EditAnywhere, Category = "Summon")
	CurrencyType _summonCurrencyType = CurrencyType::DIA;
	// 재화 비용
	UPROPERTY(EditAnywhere, Category = "Summon")
	int _summonCurrencyCost = 2000;
	// 카메라 블러 속도
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonCamBlurSpeed = 1.0f;
	// 테스트 되어있는 blur fov 크기
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonTestedBlurFOV = 80.0f;
	// fov 관련 더 곱할 값
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonFOVMulFactor = 0.8f;
	// 고등급 컷신 z값 추가 값
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonHighGradeCutsceneAddZPos = 3000000.f;
	// 소환방 추가 높이 값
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonRoomAddZPos = 2000000.f;
	// 소환 연출 보여줄지
	UPROPERTY(EditAnywhere, Category = "Summon")
	bool _summonIsShow = true;
	// 소환 도입부 연출 보여줄지
	UPROPERTY(EditAnywhere, Category = "Summon")
	bool _summonIsSkip = true;
	// 페어리 소환석 스케일
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonFairyStoneScale = 1.0f;
	// 소환 페어리  스케일
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonFairyScale = 1.0f;	
	// 코스튬 뽑기전 오브젝트 스케일
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonVeilCostumeScale = 1.0f;
	// 소환 코스튬  스케일
	UPROPERTY(EditAnywhere, Category = "Summon")	
	float _summonCostumeScale = 1.0f;
	// 페어리 소환석 offset
	UPROPERTY(EditAnywhere, Category = "Summon")
	FVector _summonFairyStoneOffset;
	// 소환 페어리 offset
	UPROPERTY(EditAnywhere, Category = "Summon")
	FVector _summonFairyOffset;
	// 코스튬 뽑기전 오브젝트 offset
	UPROPERTY(EditAnywhere, Category = "Summon")
	FVector _summonVeilCostumeOffset;
	// 소환 코스튬 offset
	UPROPERTY(EditAnywhere, Category = "Summon")
	FVector _summonCostumeOffset;
	// 소환 대기 최대치(코스튬)
	UPROPERTY(EditAnywhere, Category = "Summon")
	int _summonCostumeWaitMaxCount = 30;
	// 소환 대기 최대치(페어리)
	UPROPERTY(EditAnywhere, Category = "Summon")
	int _summonFairyWaitMaxCount = 30;
	// veil object 소환시 나오는 나뭇잎 이펙트
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _summonEffectLeafStartVeilShow;
	// 드래그 모프 타겟 이름
	UPROPERTY(EditAnywhere, Category = "Summon")
	FName _summonDragMorphTargetName;
	// 드래그 모프 타겟 이름(코스튬)
	UPROPERTY(EditAnywhere, Category = "Summon")
	FName _summonDragMorphTargetNameCostume;
	// 소환석 드래그 param: Size Vector y low min
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragParamSizeVectorYLowMin = 25.0f;
	// 소환석 드래그 param: Size Vector y low max
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragParamSizeVectorYLowMax = 50.0f;
	// 소환석 드래그 param: Size Vector y min
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragParamSizeVectorYMin = 50.0f;
	// 소환석 드래그 param: Size Vector y max
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragParamSizeVectorYMax = 100.0f;
	// 소환석 드래그 param: Velocity_val Scalar min
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragParamVelocityValScalarMin = 20.0f;
	// 소환석 드래그 param: Velocity_val Scalar max
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragParamVelocityValScalarMax = 40.0f;
	// 소환석 드래그 param: Velocity_val Scalar low min
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragParamVelocityValScalarLowMin = 10.0f;
	// 소환석 드래그 param: Velocity_val Scalar low max
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragParamVelocityValScalarLowMax = 20.0f;
	// 소환석 드래그 param: Spawn Scalar min
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragParamSpawnScalarMin = 30.0f;
	// 소환석 드래그 param: Spawn Scalar max
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragParamSpawnScalarMax = 60.0f;
	// 소환석 드래그 param: LaySize Scalar min
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragParamLaySizeScalarMin = 500.0f;
	// 소환석 드래그 param: LaySize Scalar max
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragParamLaySizeScalarMax = 1200.0f;
	// 소환석 드래그 param: ElecRaidus Scalar min
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragParamElecRadiusScalarMin = 10.0f;
	// 소환석 드래그 param: ElecRadius Scalar max
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragParamElecRadiusScalarMax = 150.0f;
	// 소환석 드래그 param: ElecSpawn Scalar min
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragParamElecSpawnScalarMin = 3.0f;
	// 소환석 드래그 param: ElecSpawn Scalar max
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragParamElecSpawnScalarMax = 6.0;
	// 소환석 드래그시 모프 타겟 max 값
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragMorphTargetMaxRate = 0.25f;
	// 소환석 드래그시 모프 타겟 속도
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragMorphTargetSpeed = 1.0f;
	// 소환 이펙트 색상 변경 시간
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonEffectColorChangeSecond = 2.0f;
	// 고등급 시퀀스에서 변신전 기본 코스튬 id
	UPROPERTY(EditAnywhere, Category = "Summon")
	int _summonHighGradeSequenceBaseCostumeId;
	// 카드 애니 딜레이
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonCardOpenAniDelay = 0.1f;
	// 드래그 카메라 노출 최대값
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragCameraExposureMax = 10.0f;
	// drag rot min val
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragRotMinVal = 0.0f;
	// drag rot max val
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonDragRotMaxVal = 45.0f;
	// fairy not open bp(rare or higher grade)
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _summonVeilFairy;
	// costume not open bp(rare or higher grade)
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _summonVeilCostume;
	// compose celine
	UPROPERTY(EditAnywhere, Category = "Summon")
	FSoftObjectPath _summonComposeCeline;
	UPROPERTY(EditAnywhere, Category = "Summon")
	TArray<FGsSummonCardSlotInfo> _cardSlotInfo;
	// compose rate: 1th success
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonComposeSuccess_1thRate = 0.5f;
	// compose rate: 1th fail
	UPROPERTY(EditAnywhere, Category = "Summon")
	float _summonComposeFail_1thRate = 0.9f;
	// 페어리 시퀀스 이펙트 사운드 매직 이하 등급 사운드
	UPROPERTY(EditAnywhere, Category = "Summon")
	FString _summonFairySequenceEffectSoundNormal;
	// 페어리 시퀀스 이펙트 사운드 레어 이상 등급 사운드
	UPROPERTY(EditAnywhere, Category = "Summon")
	FString _summonFairySequenceEffectSoundHigh;
	// 합성 BGM 사운드
	UPROPERTY(EditAnywhere, Category = "Summon")
	FString _summonComposeBGMSound;
	// 테미르 시퀀스 연출 나올지(8/9 에는 미포함, 이후에 들어갈듯)
	UPROPERTY(EditAnywhere, Category = "Summon")
	bool _isShowTemirSequence = false;
	// 월드맵
public:
	// 대륙맵 이동 속도
	UPROPERTY(EditAnywhere, Category = "WorldMap")
	float _continentalMapMoveSpeed = 1.0f;
	// pick zone color min
	UPROPERTY(EditAnywhere, Category = "WorldMap")
	FLinearColor _regionMapPickZoneColorMin;
	// pick zone color max
	UPROPERTY(EditAnywhere, Category = "WorldMap")
	FLinearColor _regionMapPickZoneColorMax;
	// 대륙맵: 지역맵 아이템 크기
	// 지역맵에서 대륙맵 올라갈때 위치 찾을때 사용(화면밖에 있는 아이템은 GetCachedGeometry 가 비어있음)
	// 기존 좌상 앵커일때는 사이즈가 FVector2D canvasSize(canvasPanelSlot->LayoutData.Offsets.Right, canvasPanelSlot->LayoutData.Offsets.Bottom);
	// 기준으로 가면 되었는데
	// 해상도에 따라 찌그러지는 이슈가 있어서(원하는 위치 안가지는)
	// 앵커를 전체로 잡았을때 위에 값이 우측에서 여기까지, 바닥에서 여기까지로 바뀌어서
	// 아이템 사이즈를 별도로 뺌(안그러면 캔버스 사이즈를 찾아와야되는데 어딘지 모름...)
	UPROPERTY(EditAnywhere, Category = "WorldMap")
	FVector2D _continentalMapRegionItemSize;
	// 성소 지도로 이동시 도착 띄워놓을 거리
	UPROPERTY(EditAnywhere, Category = "WorldMap")
	float  _sanctumeRegionMapMoveAcceptanceRange = 300.0f;
	// 지역맵 터치 이동 체크 알파값(200 이상으로)
	// 0 이상이면 땅이라고 체크하니까 14 정도 찌꺼기 같은 값이 잡힐때가 있음
	// https://jira.com2us.com/jira/browse/C2URWQ-7732
	UPROPERTY(EditAnywhere, Category = "WorldMap")
	int  _regionMapTouchMoveCheckAlpha = 250;
	// 메일
public:
	// 메일 타이틀 말줄임표 시작 위치
	UPROPERTY(EditAnywhere, Category = "Mail")
	int _mailTitleEllipsisPos = 70;
	UPROPERTY(EditAnywhere, Category = "Mail")
	int _mailContentsEllipsisPos = 80;

public:
	// 페어리 씬 bp 경로
	UPROPERTY(EditAnywhere, Category = "Fairy")
	FSoftObjectPath _fairySceneObjectPath;

	UPROPERTY(EditAnywhere, Category = "Fairy")
	FSoftObjectPath _fairySpawnEffectPath;

public:
	// 코스춤 씬 bp 경로
	UPROPERTY(EditAnywhere, Category = "Costume")
	FSoftObjectPath _costumeSceneObjectPath;

	// npc hide show
public:
	// 나올때, 사라질때 나올 이펙트
	UPROPERTY(EditAnywhere, Category = "npcHideShow")
	FSoftObjectPath _npcShowHideEffect;

public:
	// 길찾기시 코너에서 얼마만큼 떨어저 있을지 세팅
	UPROPERTY(EditAnywhere, Category = "Nav")
	float _offsetFromCornersDistance = 150.0f;	

	// npcDialog 효과
public:
	// 글자 나오는 속도
	UPROPERTY(EditAnywhere, Category = "NpcDialog")
	float _cutStringVelocity = 0.03f;

	// 추가로 머무는 시간
	UPROPERTY(EditAnywhere, Category = "NpcDialog")
	float _cutAddStayTime = 1.0f;

	// 대화 richText Style
	UPROPERTY(EditAnywhere, Category = "NpcDialog")
	FSoftObjectPath _textStyleTalk;

	// 대화 스팟이동때 나오는 페이드인 연출 시간
	UPROPERTY(EditAnywhere, Category = "NpcDialog")
	float _cutSpotWarpFadeInTime = 0.1f;

	UPROPERTY(EditAnywhere, Category = "NpcDialog")
	float _speedNarration = 10.f;

	UPROPERTY(EditAnywhere, Category = "NpcDialog")
	float _updateTimeNarration = 1.f;

public:
	UPROPERTY(EditAnywhere, Category = "EquipSound")
	TSoftObjectPtr<USoundBase> _equipSound;

	// quest auto move
public:
	// npc 없을때 대기 시간(retry)
	UPROPERTY(EditAnywhere, Category = "QuestAutoMove")
	float _questAutoMoveNPCWaitRetryTime = 0.5f;

	// 시공의 틈새 지도
public:
	UPROPERTY(EditAnywhere, Category = "QuestAutoMove")
	TArray<FGsTestSpaceCrackPortalInfo> _arrayTestSpaceCrackPortalInfo;

	// 목적지에 추가되는 acceptance radius(무조건 도착 처리용)
	// 캠프랑 히트되었다면
	// 충돌체 안으로 들어온 이벤트(수호탑 내에 퀘스트 목표가 있을 때 진행 불가 이슈)
	// https://jira.com2us.com/jira/browse/C2URWQ-6363
	UPROPERTY(EditAnywhere, Category = "QuestAutoMove")
	float _autoMoveCampHitAddAcceptanceRadius = 1000.0f;

	// Field Fairy Scale
public:
	UPROPERTY(EditAnywhere, Category = "FieldFairy")
	float _fairyScale{ 0.3f };

	UPROPERTY(EditAnywhere, Category = "FieldFairy")
	FVector _fairyAttachOffset{ FVector::ZeroVector };

	UPROPERTY(EditAnywhere, Category = "EquipSound")
	TSoftObjectPtr<USoundBase> _fairyFieldSpawnSound;

public:
	// 코스춤 씬 bp 경로
	UPROPERTY(EditAnywhere, Category = "Community")
	FSoftObjectPath _communityGirlSceneObjPath;

public:
	// 서브 다이얼로그 actor 스폰위치
	UPROPERTY(EditAnywhere, Category = "SubDialog")
	FVector _subActorSpawnPosOffset = FVector::ZeroVector;

	// Field boss
public:
	// 필드 보스 레디 시간(분, 이거보다 작아야 함)
	UPROPERTY(EditAnywhere, Category = "FieldBoss")
	int _fieldBossReadyMinutes = 30;
	UPROPERTY(EditAnywhere, Category = "FieldBoss")
	FSoftObjectPath _bossScene;
	UPROPERTY(EditAnywhere, Category = "FieldBoss")
	float _minBossZoomDistance = 250;
	UPROPERTY(EditAnywhere, Category = "FieldBoss")
	float _maxBossZoomDistance = 500;	

	// 워프
public:
	// 워프시 트레이 페이드 이펙트 색상
	UPROPERTY(EditAnywhere, Category = "Warp")
	FLinearColor _warpTrayFadeEffectColor {FLinearColor::Black};

	//SpaceCrack
public:
	// 필드 보스 레디 시간(분, 이거보다 작아야 함)
	UPROPERTY(EditAnywhere, Category = "SpaceCrack")
	int _spaceCrackBossSoundResId = 0;	
	// 필드 보스 레디 시간(분, 이거보다 작아야 함)
	UPROPERTY(EditAnywhere, Category = "SpaceCrack")
	int _spaceCrackNormalSoundResId = 0;	

	//Camp
public:
	// 기사단 수호탑에 사용될 NavModifier BP class Path
	UPROPERTY(EditAnywhere, Category = "GuildCamp")
	FSoftObjectPath _navModifierBPclassPath;
	// 기사단 수호탑에 건설 모션 Data
	UPROPERTY(EditAnywhere, Category = "GuildCamp")
	FInteractionMotionWeaponData _buildCampHammerMeshData;

public:
	/** 기사단 SNS 링크 최대 갯수 */
	UPROPERTY(EditAnywhere, Category = "Guild", Meta=(ClampMin=0, ClampMax=9))
	int32 _guildSnsUrlMaxCount = 2;
	/** 기사단 SNS 링크 최대 글자수 */
	UPROPERTY(EditAnywhere, Category = "Guild", Meta=(ClampMin=0, ClampMax=9))
	int32 _guildSnsUrlMaxLength = 50;
	/** 기사단 SNS 링크 미지정인 경우 실행할 액션 선택 */
	UPROPERTY(EditAnywhere, Category = "Guild")
	EGsGuildSnsShortcutAction _guildSnsLinkUnknownAction = EGsGuildSnsShortcutAction::CopyToClipboard;
	
	// 스캔
public:
	// 스캔 거리
	UPROPERTY(EditAnywhere, Category = "Scan")
	float _scanDist = 3000.0f;
	// 스캔 멤버 갯수
	UPROPERTY(EditAnywhere, Category = "Scan")
	int _scanMemberCount = 10;
	// 스캔 타이머(거리용)
	UPROPERTY(EditAnywhere, Category = "Scan")
	float _scanTimmerSecond = 1.0f;
	// 스캔 타이머 초 1
	UPROPERTY(EditAnywhere, Category = "Scan")
	float	_scanTimmerSecond_1 = 3;
	// 스캔 타이머 초 2
	UPROPERTY(EditAnywhere, Category = "Scan")
	float	_scanTimmerSecond_2 = 5;
	// 스캔 타이머 초 3
	UPROPERTY(EditAnywhere, Category = "Scan")
	float	_scanTimmerSecond_3 = 10;
	// 스캔 클릭 쿨타임
	UPROPERTY(EditAnywhere, Category = "Scan")
	float	_scanClickCoolTime = 3.0f;
	// 스캔 공격 상태 유지 시간(10초)
	UPROPERTY(EditAnywhere, Category = "Scan")
	float	_scanAttackStateWaitTime = 10.0f;
	// add offset drag widget
	UPROPERTY(EditAnywhere, Category = "Scan")
	FVector2D	_scanDragAddOffset;
	UPROPERTY(EditAnywhere, Category = "Scan")
	float _squaredDragDiffVal = 0.1f;
public:
	// 절전 모드 해제 길이 비율 (기본 : 가로 길이 10%)
	UPROPERTY(EditAnywhere, Category = "SaveBattery")
	float	_saveBatteryCloseDistanceRate = 10.0f;
	UPROPERTY(EditAnywhere, Category = "SaveBattery")
	float _saveBatteryMaxFPS = 30.0f;

public:
	UPROPERTY(EditAnywhere, Category = "SphereTree")
	int		_MaxSphereCount = 5000;

	UPROPERTY(EditAnywhere, Category = "SphereTree")
	float	_RootNodeMaxSize  = 12800.0f;

	UPROPERTY(EditAnywhere, Category = "SphereTree")
	float	_LeapNodeMaxSize = 2560.0f;

	UPROPERTY(EditAnywhere, Category = "SphereTree")
	float	_NodeGravyValue	= 320.0f;

public:
	UPROPERTY(EditAnywhere, Category = "SphereTree")
	int		_HitPoolCount = 30000;

public:
	// 2022/01/13 PKT - TreeView Fold Animation Time
	UPROPERTY(EditAnywhere, Category = "EnchantScroll")
	int	_treeViewFoldAnimationTime = 10;

public:		
	UPROPERTY(EditAnywhere, Category = "SequencPlayer")
	TArray<FName> _hideTagList;

	// 플레이어블 이벤트
public:
	// 플레이어블 이벤트 로컬 클래스 bp 경로
	UPROPERTY(EditAnywhere, Category = "PlayableEvent")
	FSoftObjectPath _playableEventLocalBPPath;
	// 플레이어블 이벤트 로컬 스킬 0
	UPROPERTY(EditAnywhere, Category = "PlayableEvent")
	TArray<int> _playableEventLocalSkill;
	// 카메라 회전 가능 값(기준점에서 -15 ~ +15)
	UPROPERTY(EditAnywhere, Category = "PlayableEvent")
	float _playableEventCameraRotYawPossible = 15.0f;
	// 플레이어블 이벤트 지역명
	UPROPERTY(EditAnywhere, Category = "PlayableEvent")
	FText _playableEventZoneName;
	// 스킬 글로벌 쿨타임
	UPROPERTY(EditAnywhere, Category = "PlayableEvent")
	float _playableEventSkillGlobalCoolTime = 1.0f;
	// 테스트로 플레이어블 이벤트 들어갈수 있게 처리
	UPROPERTY(EditAnywhere, Category = "PlayableEvent")
	bool _testPlayableEventEnter = false;
	// level path
	UPROPERTY(EditAnywhere, Category = "PlayableEvent")
	FSoftObjectPath _playableEventlevelPath;
	UPROPERTY(EditAnywhere, Category = "PlayableEvent")
	float _playableEventFadeInTime = 3.0f;
	UPROPERTY(EditAnywhere, Category = "PlayableEvent")
	FSoftObjectPath _playableEventMonsterAiControllerClassPath;
	UPROPERTY(EditAnywhere, Category = "PlayableEvent")
	float _playableEventRespawnSecond = 2.0f;
	UPROPERTY(EditAnywhere, Category = "PlayableEvent")
	float _playableEventFinishDelaySecond = 2.0f;
	UPROPERTY(EditAnywhere, Category = "PlayableEvent")
	float _playableEventMonsterDespawnSecond = 1.0f;
	UPROPERTY(EditAnywhere, Category = "PlayableEvent")
	float _playableEventStartCameraBlendingSpeed = 0.5f;
	UPROPERTY(EditAnywhere, Category = "PlayableEvent")
	float _playableEventEndCameraBlendingSpeed = 1.0f;
	UPROPERTY(EditAnywhere, Category = "PlayableEvent")
	bool _isPlayPlayable = true;
	UPROPERTY(EditAnywhere, Category = "PlayableEvent")
	int _monsterDeathHitCount = 3;
	UPROPERTY(EditAnywhere, Category = "PlayableEvent")
	UParticleSystem* _playableEventMonsterSpawnPath;

public:
	UPROPERTY(EditAnywhere, Category = "HitMotion")
	UCurveFloat* _hitMotionBlendTime;

	UPROPERTY(EditAnywhere, Category = "HitMotion")
	UCurveFloat* _hitAmplitudeTime;

	UPROPERTY(EditAnywhere, Category = "HitMotion")
	float _hitAmplitudeMultiplier = 50.0f;

public:
	UPROPERTY(EditAnywhere, Category = "SmoothRotation")
	FGsSmoothRotationInfo _smoothRotationInfo;

public:
	// Dialog
	UPROPERTY(EditAnywhere, Category = "DialogDOF")
	float _dialogDepthOfFieldScale = 2.0f;

public:
	// 유저 프로필 Scene 경로
	UPROPERTY(EditAnywhere, Category = "UserProfile")
	FSoftObjectPath _userProfileScenePath;

public:
	UPROPERTY(EditAnywhere, Category = "InvasionWarp")
	FSoftObjectPath _portalNamePlatePath;

	UPROPERTY(EditAnywhere, Category = "InvasionWarp")
	FSoftObjectPath _portaInvasionTouchPath;

public:
	UPROPERTY(EditAnywhere, Category = "EliteDungeon")
	FSoftObjectPath _eliteDungeonPortalPlatePath;
	//Debug
public:	
	UPROPERTY(EditAnywhere, Category = "Debug")
	float _npcInfoWidgetHeight = 400.0f;

	// URO(Update Rate Optimization)
public:
	//USkeletalMeshComponent
	/** if TRUE, Owner will determine how often animation will be updated and evaluated. See AnimUpdateRateTick()
	* This allows to skip frames for performance. (For example based on visibility and size on screen). */
	UPROPERTY(EditAnywhere, Category = "URO")
	bool bEnableUpdateRateOptimizations = true;
	/** Enable on screen debugging of update rate optimization.
	 * Red = Skipping 0 frames, Green = skipping 1 frame, Blue = skipping 2 frames, black = skipping more than 2 frames.
	 * @todo: turn this into a console command. */
	UPROPERTY(EditAnywhere, Category = "URO")
	bool bDisplayDebugUpdateRateOptimizations = false;

	//FAnimUpdateRateParameters
	/** Rate of animation evaluation when non rendered (off screen and dedicated servers).
	* a value of 4 means evaluated 1 frame, then 3 frames skipped */
	UPROPERTY(EditAnywhere, Category = "URO")
	int32 BaseNonRenderedAnimUpdateRate = 4;

	/** Array of MaxDistanceFactor to use for AnimUpdateRate when mesh is visible (rendered).
	 * MaxDistanceFactor is size on screen, as used by LODs
	 * Example:
	 *		BaseVisibleDistanceFactorThesholds.Add(0.4f)
	 *		BaseVisibleDistanceFactorThesholds.Add(0.2f)
	 * means:
	 *		0 frame skip, MaxDistanceFactor > 0.4f
	 *		1 frame skip, MaxDistanceFactor > 0.2f
	 *		2 frame skip, MaxDistanceFactor > 0.0f
	 */
	UPROPERTY(EditAnywhere, Category = "URO")
	TArray<float> AnimUpdateRateVisibleMaxDistanceFactor;

	/** Max Evaluation Rate allowed for interpolation to be enabled. Beyond, interpolation will be turned off. */
	UPROPERTY(EditAnywhere, Category = "URO")
	int32 MaxEvalRateForInterpolation = 4;

	/** The bucket to use when deciding which counter to use to calculate shift values */
	UPROPERTY(EditAnywhere, Category = "URO")
	EUpdateRateShiftBucket UroShiftBucket = EUpdateRateShiftBucket::ShiftBucket0;

	/** Whether or not to use the defined LOD/Frameskip map instead of separate distance factor thresholds */
	// 1. first check this value(LOD)
	// 2. if this false, check BaseVisibleDistanceFactorThesholds(AnimUpdateRateVisibleMaxDistanceFactor, size on screen)
	UPROPERTY(EditAnywhere, Category = "URO")
	bool bUseLODMapForURO = true;

	/** Map of LOD levels to frame skip amounts. if bShouldUseLodMap is set these values will be used for
	* the frameskip amounts and the distance factor thresholds will be ignored. The flag and these values
	* should be configured using the customization callback when parameters are created for a component.
	*
	* Note that this is # of frames to skip, so if you have 20, that means every 21th frame, it will update, and evaluate.
	*/
	UPROPERTY(EditAnywhere, Category = "URO")
	TMap<int32, int32> LODToFrameSkipMap;

public:
	UPROPERTY(EditAnywhere, Category = "AutoEmotion")
	float MinTimeAutoEmotionReact = 5.0f;

	UPROPERTY(EditAnywhere, Category = "AutoEmotion")
	float MaxTimeAutoEmotionReact = 7.0f;

	UPROPERTY(EditAnywhere, Category = "AutoEmotion")
	float OffsetTimeStartAutoEmotionReact = 10.f;

public:
	UPROPERTY(EditAnywhere, Category = "Crashltyic")
		bool isCrashltyicTestValid = true;
	UPROPERTY(EditAnywhere, Category = "Crashltyic")
		FString CrashltyicParam = TEXT("CrashCheck");

public:
	UPROPERTY(EditAnywhere, Category = "GUI")
	float DragTriggerDistance = 50.f;

	// npc function slot
public:
	UPROPERTY(EditAnywhere, Category = "NPCFunctionSlot")
	int _npcFunctionSlotMaxCount = 7;

	// invasion enemy
public:
	UPROPERTY(EditAnywhere, Category = "Invasion")
	FSoftObjectPath _invasionEnemyIcon;
	// Alert VibrateAlert
public:
	UPROPERTY(EditAnywhere, Category = "Alert")
	FSoftObjectPath _forceFeedbackEffect;

	UPROPERTY(EditAnywhere, Category = "Alert")
	FSoftObjectPath _alertRingSound;

	UPROPERTY(EditAnywhere, Category = "Alert")
	FSoftObjectPath _alertPvpAttackSound;

	UPROPERTY(EditAnywhere, Category = "Alert")
	FSoftObjectPath _alertLowHpSound;

	// auto loot
public:
	UPROPERTY(EditAnywhere, Category = "Auto")
	float _autoItemPickUpDelaySecond = 20.0f;

	// 루팅 서버 응답 대기 클리어 시간
	UPROPERTY(EditAnywhere, Category = "Auto")
	float _lootServerResponseWaitClearSecond = 2.0f;

	// counter attack
public:
	UPROPERTY(EditAnywhere, Category = "Auto")
	float _counterAttackButtonDeactivationSecond = 5.0f;

public:
	UPROPERTY(EditAnywhere, Category = "Auto")
	int _aiTaskRetryCount = 3;
	UPROPERTY(EditAnywhere, Category = "Auto")
	float _aiTaskRetryDelayTime = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Auto")
	float _aiIdleMakeJobDelayTime = 0.3f;
	UPROPERTY(EditAnywhere, Category = "Auto")
	float _aiAutoMoveFailBlockTime = 3.0f;
	UPROPERTY(EditAnywhere, Category = "Auto")
	float _aiWaitRemakeListTime = 1.0f;

	// temporary combat option value
public:
	UPROPERTY(EditAnywhere, Category = "Combat")
	bool _touchTerrainKeepTarget = true;
	UPROPERTY(EditAnywhere, Category = "Combat")
	bool _skillSelectTargetUIShowedAutoUse = false;

	// auto move
public:
	UPROPERTY(EditAnywhere, Category = "Automove")
	TSubclassOf<UNavigationQueryFilter> _autoMoveFilterClass;

	// 자동 이동 워프 반지름 rate 값(원래 반지름보다 더들어가게 처리용)
	UPROPERTY(EditAnywhere, Category = "Automove")
	float _autoMoveWarpAcceptanceRadiusRate = 0.4f;

	UPROPERTY(EditAnywhere, Category = "Automove")
	int _autoMoveIdleCheckCount = 10;

	// 몬스터 드랍 이동 acceptance range 추가값
	UPROPERTY(EditAnywhere, Category = "Automove")
	float _addMonsterDropMoveAcceptanceRange = 100.0f;

public:
	UPROPERTY(EditAnywhere, Category = "Option")
	FIntPoint _windowResolutionMode1;
	UPROPERTY(EditAnywhere, Category = "Option")
	FIntPoint _windowResolutionMode2;
	UPROPERTY(EditAnywhere, Category = "Option")
	FIntPoint _windowResolutionMode3;

public:
	UPROPERTY(EditAnywhere, Category = "CapsuleModify")
	float _FEMAILMinOffset = 0.0f;
	UPROPERTY(EditAnywhere, Category = "CapsuleModify")
	float _FEMAILMaxOffset = 0.0f;

	UPROPERTY(EditAnywhere, Category = "CapsuleModify")
	float _MAILMinOffset = 0.0f;
	UPROPERTY(EditAnywhere, Category = "CapsuleModify")
	float _MAILMaxOffset = 0.0f;

	//카메라 위치세팅용 거리	BoneDistance : BIP001 - Spine2 
public:
	UPROPERTY(EditAnywhere, Category = "CameraInitData")
	float _FEMAILMinBip001Spine = 0.0f;
	UPROPERTY(EditAnywhere, Category = "CameraInitData")
	float _FEMAILMaxBip001Spine = 0.0f;

	UPROPERTY(EditAnywhere, Category = "CameraInitData")
	float _MAILMinBip001Spine = 0.0f;
	UPROPERTY(EditAnywhere, Category = "CameraInitData")
	float _MAILMaxBip001Spine = 0.0f;

	// BM Shop
public:
	UPROPERTY(EditAnywhere, Category = "UseIAPV4", Meta = (Tooltip = "Hive IAPv4 구글 시트 로그 출력 여부"))
	bool _isActiveIAPV4SheetsLog = false;
	UPROPERTY(EditInstanceOnly, Category = "BMShop", Meta = (Tooltip = "[BMShop] 현금 구매 팝업에 청약 철회 출력 여부"))
	bool _showContractNoticeOnPurchasePopup = true;
	UPROPERTY(EditInstanceOnly, Category = "BMShop", Meta = (Tooltip = "[BMShop] 현금 구매 자동 연속 구매 시, 최대 가능한 횟수."))
	uint32 _sequenceCashPurchaseHardcap = 10;
	UPROPERTY(EditInstanceOnly, Category = "BMShop", Meta = (Tooltip = "[BMShop] 대문자로. 기본 통화 코드(ISO 4217)"))
	EGsBMShopCashCurrency _defaultCurrencyCode = EGsBMShopCashCurrency::KRW;
	UPROPERTY(EditInstanceOnly, Category = "BMShop", Meta = (Tooltip = "[BMShop] Windows 플랫폼에서의 복구 버튼 쿨타임(초)"))
	float _shopRestoreCooltimeSecondsOnWindows = 5.f;

	// room contents
public:
	UPROPERTY(EditAnywhere, Category = "RoomContents")
	float _roomContentsPitchMin = -25.0f;
	UPROPERTY(EditAnywhere, Category = "RoomContents")
	float _roomContentsPitchMax = 10.0f;

	UPROPERTY(EditAnywhere, Category = "RoomContents")
	float _roomContentsInitPitch = 10.0f;

	UPROPERTY(EditAnywhere, Category = "RoomContents")
	float _roomContentsZoomAddValPC = 0.3f;
	UPROPERTY(EditAnywhere, Category = "RoomContents")
	float _roomContentsZoomMulValMobile = 1.0f;
	// fairy room
public:
	UPROPERTY(EditAnywhere, Category = "FairyRoom")
	float _fairyHalfSize = 88.0f;

	// 아이템 이름 툴팁 상하 좌우 패딩 값
public:
	UPROPERTY(EditAnywhere, Category = "ItemNameToolTip")
	FVector2D _nameToolTipPadding = FVector2D(5.f, 5.f);

	// quest kill move
public:
	UPROPERTY(EditAnywhere, Category = "QuestKillCenterMove")
	float _questKillCenterCheckSqVal = 400.0f;

public:
	// 해당 몬스터가 맵에서 출현하는 장소를 찍어주는 마커의 갯수
	UPROPERTY(EditAnywhere, Category = "MonsterMapSpawnIcon")
	int _monsterMapSpawnIconShowCount = 3;

	// 아이콘 사이의 최소 간격
	public:
	UPROPERTY(EditAnywhere, Category = "MonsterMapSpawnIcon")
	float MonsterSpawnDistCheckConfig = 4000.f;

	// 프리셋 컨텐츠 출력 제어 옵션
public:
	UPROPERTY(EditAnywhere, Category = "Preset")
	bool _showPreset = true;

	// 무기 평화상태시 Curve이동/회전 속도
public:
	UPROPERTY(EditAnywhere, Category = "WeaponPeaceTM")
	float WeaponCurveLocationSpeed = 10.0f;

	UPROPERTY(EditAnywhere, Category = "WeaponPeaceTM")
	float WeaponCurveRotationSpeed = 10.0f;

	//operating message ticker color
public:
	UPROPERTY(EditAnywhere, Category = "OperatingMessage")
	FLinearColor _bgColorImmediate;
	UPROPERTY(EditAnywhere, Category = "OperatingMessage")
	FLinearColor _bgColorGeneral;
	UPROPERTY(EditAnywhere, Category = "OperatingMessage")
	FLinearColor _bgColorEvent;
	UPROPERTY(EditAnywhere, Category = "OperatingMessage")
	FLinearColor _bgColorUrgent;
	UPROPERTY(EditAnywhere, Category = "OperatingMessage")
	FLinearColor _bgColorSpecial;

	// 2023/5/26 PKT - 몬스터 도감 관련 Sound
public:
	// 2023/5/26 PKT - 몬스터 상세 팝업에서 도감 몬스터의 레벨업 시 사운드 1회 출력
	UPROPERTY(EditAnywhere, Category = "MonsterCollectionRes")
	FGsSchemaSoundResDataRow _effectSoundMonsterCollectionLevelUp;

	// 2023/5/26 PKT - 석판 해금 시 사운드 1회 출력
	UPROPERTY(EditAnywhere, Category = "MonsterCollectionRes")
	FGsSchemaSoundResDataRow _effectSoundMonsterCollectionSlateUnlock;
	
	// 2023/5/26 PKT - 노드 해금 시 사운드 1회 출력
	UPROPERTY(EditAnywhere, Category = "MonsterCollectionRes")
	FGsSchemaSoundResDataRow _effectSoundMonsterCollectionSlotActive;	

	// 2023/5/26 PKT - 전체 레벨 달성, 아이템 일괄 사용 팝업 결과 출력 시 사운드 1회 출력
	UPROPERTY(EditAnywhere, Category = "MonsterCollectionRes")
	FGsSchemaSoundResDataRow _effectSoundMonsterCollectionMultiLevelUp;

public:
	/** 봉인윈도우의 각인 버튼의 잠금 시간(초) */
	UPROPERTY(EditAnywhere, Category = "Seal")
	float _sealWindowTryButtonLockSecs = 0.5f;

	/** 천마석 윈도우의 잠식 버튼의 잠금 시간(초) */
	UPROPERTY(EditAnywhere, Category = "Seal")
	float _spiritShotWindowTryButtonLockSecs = 0.5f;

public:
	UPROPERTY(EditAnywhere, Category = "PreventGameOverIndulgence")
	float _gameRaitingIconViewSec = 5.f;

public:
	// 맵 로딩 대기 카운트
	UPROPERTY(EditAnywhere, Category = "PreventGameOverIndulgence")
	int _waitCountLoadTerrain = 20;

	// 스탯 변화 보여주기
public:	
	UPROPERTY(EditAnywhere, Category = "StatChangeNotify")
	int _statNotifyShowCount = 3;
	UPROPERTY(EditAnywhere, Category = "StatChangeNotify")
	float _statNotifyShowTime = 2;
	UPROPERTY(EditAnywhere, Category = "StatChangeNotify")
	float _statNotifyProduceTime = 0.5;

	// 도감 > 몬스터 일람에서 지식이 얼마나 쌓여야 도감 포인트로 변환할지 레드닷을 띄워주는 기준값
public:	
	UPROPERTY(EditAnywhere, Category = "StatChangeNotify")
	int _codexKnowledgeThresholdForRedDot = 200;

	// AI
public:
	// 대기 액션 시간(인터랙션 실패후 대기 시간)
	// https://jira.com2us.com/jira/browse/C2URWQ-4432
	UPROPERTY(EditAnywhere, Category = "AI")
	float _maxAIWaitSedond = 1.0f;

public:
	UPROPERTY(EditAnywhere, Category = "BattleArenaRank")
	float _battleArenaRankSendDelayTimeS = 0.55f;
	UPROPERTY(EditAnywhere, Category = "BattleArenaRank")
	int32 _battleArenaRankMaxCount = 30;
	UPROPERTY(EditAnywhere, Category = "BattleArenaRank")
	int32 _battleArenaLimitUpdateTimeS = 2;
	UPROPERTY(EditAnywhere, Category = "BattleArenaRank")
	int32 _battleArenaPendingLifeTimeS = 3;
};
