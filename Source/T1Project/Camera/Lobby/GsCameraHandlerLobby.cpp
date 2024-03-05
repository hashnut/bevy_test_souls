#include "GsCameraHandlerLobby.h"

#include "ActorEx/GsCharacterLobbyPlayer.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "DataSchema/Camera/GsSchemaCustomizeCameraData.h"
#include "DataSchema/Camera/GsSchemaContentsCameraData.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Management/ScopeLobby/GsLobbyCharacterManager.h"

#include "GameFlow/GsGameFlowLobby.h"
#include "GameFlow/Stage/StageLobby/GsStageManagerLobby.h"
#include "GameFlow/Stage/StageLobby/GsStageLobbyBase.h"

#include "Camera/Define/GsCameraGameDefine.h"

#include "UTIL/GsTableUtil.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Components/SkeletalMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "Camera/CameraComponent.h"

#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"

#include "T1Project.h"


FGsCameraHandlerLobby::~FGsCameraHandlerLobby()
{	
}
// clear
void FGsCameraHandlerLobby::Finalize()
{
	FGsMessageHolder* message = GMessage();


	if (_delegateLobbyMsg.Num() != 0)
	{
		for (auto iter: _delegateLobbyMsg)		
		{
			message->GetContentsLobby().Remove(iter);
		}
	}
}
// 초기화
void FGsCameraHandlerLobby::Initialize(AGsCharacterLocalPlayer* In_char)
{
	_cameraLobbyDiffHeadPos = GData()->GetGlobalData()->cameraLobbyDiffHeadPos;

	FGsMessageHolder* message = GMessage();
	
	_delegateLobbyMsg.Emplace(
		message->GetContentsLobby().AddRaw(MessageContentsLobby::ZOOM_HEAD, this, &FGsCameraHandlerLobby::OnZoomHead)
	);
	_delegateLobbyMsg.Emplace(
		message->GetContentsLobby().AddRaw(MessageContentsLobby::ZOOM_WHOLE_BODY, this, &FGsCameraHandlerLobby::OnZoomWholeBody)
	);
	_delegateLobbyMsg.Emplace(
		message->GetContentsLobby().AddRaw(MessageContentsLobby::HEIGHT_CHANGE, this, &FGsCameraHandlerLobby::OnHeightChange)
	);

	_characterLocalPlayer = In_char;	

	//InitZoom();
}
// 생성시 줌해놓은게
// 선택창에서 유지되어서
// 선택창 들어갈때 줌아웃 처리
void FGsCameraHandlerLobby::InitZoom(float In_zoomRate)
{
	// 0.0f ~ 1.0f 사이로
	float zoomRate = FMath::Clamp(In_zoomRate, 0.0f, 1.0f);
	float currLength = GetSpringArmLength(zoomRate);

	// rate 값 샷으로 초기화
	_characterLocalPlayer->GetSpringArm()->TargetArmLength = currLength;

	// 오프셋 계산
	CalcOffset(zoomRate);

	_currZoomMode = EGsLobbyZoomMode::WholeBody;

	// 줌 초기화시 기존 플래그도 초기화 한다
	_isZoomStart = false;
	_isTabZoom = false;

	// 게임 입장 시 카메라 이동 연출
	_isGameEnterCamMove = false;
	_gameEnterMoveValue = 0.0f;
}
// 업데이트
void FGsCameraHandlerLobby::Update(float In_delta)
{
	if (_isGameEnterCamMove)
	{			
		UpdateEnterMove(In_delta);
		return;
	}

	// 목표점에 도달했다면
	if (true == UpdateZoom(In_delta))
	{
		_isZoomStart = false;
		_isTabZoom = false;
	}	

	UpdateRotPitch(In_delta);

	// 머리 위치값이 초기화 시점과 일반상황과 위치가 달라서
	// 달라지면 업데이트에서 다시 계산 처리 필요
	CheckHeadDiff();
	// 오프셋 갱신
	UpdateOffset();
}

// 업데이트 줌
bool FGsCameraHandlerLobby::UpdateZoom(float In_delta)
{
	bool isEnd = false;
	if (true == _isZoomStart)
	{
		float zoomSpeed = GData()->GetGlobalData()->CamArmLengthSpeed;
		// 속도가 0이면 안함
		if (zoomSpeed == 0.0f)
		{
			return true;
		}

		float addRate = In_delta * zoomSpeed;
		_zoomProgressRate = FMath::Clamp<float>(_zoomProgressRate + addRate, 0.f, 1.f);
		UCurveFloat* curve = GData()->GetCamFollowCurve();

		float curveVal = 0.0f;
		if (nullptr != curve)
		{
			curveVal = FMath::Clamp<float>(curve->GetFloatValue(_zoomProgressRate), 0.f, 1.f);
		}
		float newLength = FMath::Lerp(_armLengthFrom, _armLengthTo, curveVal);

		// 차이 절대값
		float  diff = 0.0f;

		// 탭 줌이면 계산
		if (_isTabZoom == true)
		{			
			diff = FMath::Abs(_armLengthTo - newLength);
		}

		// 차이 얼마 안나면 끝
		if (diff <= GData()->GetGlobalData()->_cameraZoomFinishDiff)
		{
			newLength = _armLengthTo;
			isEnd = true;
		}
		_characterLocalPlayer->GetSpringArm()->TargetArmLength = newLength;
			
		// 후처리로 바꾸자(update 말단)
		_isDirtyOffset = true;
	}
	return isEnd;
}
// 업데이트 pitch 회전 
void FGsCameraHandlerLobby::UpdateRotPitch(float In_delta)
{
	// pitch 회전이 아니면 안함
	if (false == _isRotPitch)
	{
		return;
	}
	
	float addRate = In_delta * GData()->GetGlobalData()->PitchRotSpeed;
	_progressRotPitchRate = FMath::Clamp<float>(_progressRotPitchRate + addRate, 0.f, 1.f);
	
	FQuat newPitchQuat =
		FMath::Lerp(_pitchQuatFrom, _pitchQuatTo, _progressRotPitchRate);

	// 차이가 있다
	if (false == newPitchQuat.Equals(_pitchQuatTo, 0.01f))
	{
	}
	// 끝에 도달
	else
	{
		newPitchQuat = _pitchQuatTo;
		_isRotPitch = false;
	}

	_characterLocalPlayer->Controller->SetControlRotation(newPitchQuat.Rotator());

	
}
// 줌값 변경
void FGsCameraHandlerLobby::OnZoomChange(float In_val)
{	
	float currentArmLength =
		_characterLocalPlayer->GetSpringArm()->TargetArmLength;

	UGsLobbyCharacterManager* characterMgr = LSLobbyCharacter();
	if (characterMgr == nullptr)
	{
		return;
	}

	const FGsSchemaCustomizeCameraData* currentData = characterMgr->GetCurrentCustomizeCameraData();
	if (nullptr == currentData)
	{
		return;
	}

	float zoomMin = currentData->SpringArmLengthMin;
	float zoomMax = currentData->SpringArmLengthMax;

	float zoomTo = FMath::Clamp(currentArmLength + In_val,
		zoomMin,
		zoomMax);

	// 탭줌은 아님
	_isTabZoom = false;
	ZoomStart(currentArmLength, zoomTo);
}
// 줌모드 변경
void FGsCameraHandlerLobby::OnChangeZoomMode(EGsLobbyZoomMode In_mode)
{
	UGsLobbyCharacterManager* characterMgr = LSLobbyCharacter();
	if (characterMgr == nullptr)
	{
		return;
	}

	const FGsSchemaCustomizeCameraData* currentData = characterMgr->GetCurrentCustomizeCameraData();
	if (nullptr == currentData)
	{
		return;
	}

	float zoomTo = 0.0f;
	if (In_mode == EGsLobbyZoomMode::Head)
	{
		zoomTo = currentData->SpringArmLengthMin;
	}
	else
	{
		zoomTo = currentData->SpringArmLengthMax;
	}
	_isTabZoom = true;

	float zoomFrom = _characterLocalPlayer->GetSpringArm()->TargetArmLength;
	ZoomStart(zoomFrom, zoomTo);

	// 모드 변경시 pitch 값도 되돌린다
	AController* controller = _characterLocalPlayer->Controller;
	if (nullptr == controller)
	{
		return;
	}
	FRotator rot = controller->GetControlRotation();
	// pitch 회전 시작값저장
	_pitchQuatFrom = rot.Quaternion();
	// 도달값 저장
	rot.Pitch = currentData->InitPitchRotation;
	_pitchQuatTo = rot.Quaternion();
	// 모드저장
	_currZoomMode = In_mode;

	// pitch 회전 rate 초기화
	_progressRotPitchRate = 0.0f;
	// pitch 회전 시작
	_isRotPitch = true;

	FQuat _camRotFrom = FQuat::Identity;
	FQuat _camRotTo = FQuat::Identity;
}
// 머리 줌
void FGsCameraHandlerLobby::OnZoomHead()
{
	OnChangeZoomMode(EGsLobbyZoomMode::Head);
}
// 전신 줌
void FGsCameraHandlerLobby::OnZoomWholeBody()
{
	OnChangeZoomMode(EGsLobbyZoomMode::WholeBody);
}

// 키 변경
void FGsCameraHandlerLobby::OnHeightChange()
{
	// 무조건 재계산
	_isDirtyOffset = true;

	// 액터 위치 재계산
	//if (UGsLobbyCharacterManager* characterMgr = LSLobbyCharacter())
	// {
	//	characterMgr->ReCalcCapsuleSize();
	// }

}
// 오프셋 계산(머리중심 <-> 몸통중심 을 줌값을 기준으로 계산한다)
void FGsCameraHandlerLobby::CalcOffset(float In_lengthRate)
{
	UGsLobbyCharacterManager* characterMgr = LSLobbyCharacter();
	if (characterMgr == nullptr)
	{
		return;
	}


	const FGsSchemaCustomizeCameraData* currentData = characterMgr->GetCurrentCustomizeCameraData();
	if (nullptr == currentData )
	{
		return;
	}
	_camSoketOffsetHead = GetCurrentHeadPos(_characterLocalPlayer, true);

	FVector headPos = _camSoketOffsetHead + currentData->HeadAddPos;
	FVector wholeBodyPos = currentData->WholeBodyAddPos;

	FVector currOffset = FMath::Lerp(headPos, wholeBodyPos, In_lengthRate);

	// 소켓 오프셋 변경하면 카메라만 위로 들려서
	// 바라보는 타겟점은 그대로임-> 회전시 이상함
	// 스프링암 전체 위치를 변경
	_characterLocalPlayer->GetSpringArm()->SetRelativeLocation(currOffset);


	const FGsSchemaContentsCameraData* currentCameraContentsData = characterMgr->GetCurrentCameraContentsData();
	if (nullptr == currentCameraContentsData)
	{
		return;
	}

	FVector currentContentsOffset = currentCameraContentsData->CamearPositionOffset;

	float currShiftPosY = FMath::Lerp(currentData->HeadShiftCameraPosY, currentData->WholeBodyShiftCameraPosY, In_lengthRate);

	currentContentsOffset.Y += currShiftPosY;

	_characterLocalPlayer->GetFollowCamera()->SetRelativeLocation(currentContentsOffset);
}



// 머리 위치값 바뀌었는지 체크
void FGsCameraHandlerLobby::CheckHeadDiff()
{
	UGsLobbyCharacterManager* characterMgr = LSLobbyCharacter();
	if (characterMgr == nullptr)
	{
		return;
	}

	USkeletalMeshComponent* meshPtr = _characterLocalPlayer->GetMesh();
	if (nullptr == meshPtr)
	{
		return;
	}
	
	FVector newHeadPos = GetCurrentHeadPos(_characterLocalPlayer, true);

#ifdef TEST_DEBUG_DRAW_
	FTransform headSocketTm =
		meshPtr->GetSocketTransform(TEXT("FXHeadSocket"));

	FVector headSocketPos = headSocketTm.GetLocation();

	FVector bip001HeadPos = meshPtr->GetBoneLocation(TEXT("Bip001-Head"),
		EBoneSpaces::WorldSpace);



	FVector currentHeadBonePos = characterMgr->GetCurrentHeadBonePos();

	FVector vecSpaceConstant = meshPtr->GetBoneLocation(FName("Root"));
	FVector vecSpaceBot = meshPtr->GetBoneLocation(FName("Bip001-L-Toe0"));

	float offsetZ = vecSpaceConstant.Z - vecSpaceBot.Z;

	FVector addOffset = FVector(headSocketPos.X, headSocketPos.Y, headSocketPos.Z + offsetZ);

	UCapsuleComponent* CapsuleComponent = _characterLocalPlayer->GetCapsuleComponent();
	float halfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();

	FVector capsuleSizePos = FVector(0, 0, halfHeight);

	UWorld* world = characterMgr->GetWorld();
	if (world == nullptr)
	{
		return;
	}


	DrawDebugSphere(world, headSocketPos,
		10.0f,8, FColor::Red, false, 1.0);

	DrawDebugSphere(world, addOffset,
		10.0f, 8, FColor::Cyan, false, 1.0);

	DrawDebugSphere(world, capsuleSizePos,
		10.0f, 8, FColor::Orange, false, 1.0);


	DrawDebugSphere(world, bip001HeadPos,
		10.0f,8, FColor::Green, false, 1.0);

	DrawDebugSphere(world, currentHeadBonePos,
		10.0f, 8, FColor::Magenta, false, 1.0);
#endif


	if (newHeadPos.Equals(_camSoketOffsetHead, _cameraLobbyDiffHeadPos) == false)
	{
		_isDirtyOffset = true;
	}
}

// 줌 비율값 구하기
// 머리(0.0f) - 전신(1.0f)
float FGsCameraHandlerLobby::GetZoomRate()
{
	UGsLobbyCharacterManager* characterMgr = LSLobbyCharacter();
	if (characterMgr == nullptr)
	{
		return 0.0f;
	}

	const FGsSchemaCustomizeCameraData* currentData = characterMgr->GetCurrentCustomizeCameraData();
	if (nullptr == currentData)
	{
		return 0.0f;
	}

	float zoomMin = currentData->SpringArmLengthMin;
	float zoomMax = currentData->SpringArmLengthMax;


	// 최대 길이
	float lengthTotalLength =
		zoomMax - zoomMin;

	// 현재
	float currlength =
		_characterLocalPlayer->GetSpringArm()->TargetArmLength - zoomMin;

	// 비율
	float lengthRate = 1.0f;
	// 나누기 밑에 변이 0이 아니면 계산
	if (FMath::IsNearlyZero(lengthTotalLength) == false)
	{
		lengthRate = currlength / lengthTotalLength;
	}
	return lengthRate;
}

// 비율로 스프링 암 거리 구하기
float FGsCameraHandlerLobby::GetSpringArmLength(float In_rate)
{
	UGsLobbyCharacterManager* characterMgr = LSLobbyCharacter();
	if (characterMgr == nullptr)
	{
		return 0.0f;
	}

	const FGsSchemaCustomizeCameraData* currentData = characterMgr->GetCurrentCustomizeCameraData();
	if (nullptr == currentData)
	{
		return 0.0f;
	}

	float zoomMin = currentData->SpringArmLengthMin;
	float zoomMax = currentData->SpringArmLengthMax;

	// 최대 길이
	float lengthTotalLength =
		zoomMax - zoomMin;

	float resultLength =
		In_rate* lengthTotalLength + zoomMin;

	return resultLength;
}

// 오프셋 갱신
void FGsCameraHandlerLobby::UpdateOffset()
{
	// 바뀐거 없으면 return
	if (_isDirtyOffset == false)
	{
		return;
	}

	// 플래그 초기화
	_isDirtyOffset = false;

	// 오프셋 계산
	float lengthRate = GetZoomRate();
	CalcOffset(lengthRate);

}


// 줌 시작
void FGsCameraHandlerLobby::ZoomStart(float In_zoomFrom, float In_zoomTo)
{
	_armLengthFrom = In_zoomFrom;
	_armLengthTo = In_zoomTo;

	_isZoomStart = true;

	_zoomProgressRate = 0.0f;
}


void FGsCameraHandlerLobby::OnGameEnterMove(AActor* In_actor)
{
	if (nullptr == In_actor)
		return;

	UCameraComponent* camComponent = _characterLocalPlayer->GetFollowCamera();
	if (nullptr == camComponent)
		return;
	
	_startFOV = camComponent->FieldOfView;	
	_camMoveRot = GData()->GetGlobalData()->LobbyGameEnterCamRot;	
	_camRot = _characterLocalPlayer->Controller->GetControlRotation();		

	FVector playerPos = _characterLocalPlayer->GetActorLocation();
	FVector camPos = camComponent->GetComponentLocation();
	FVector camMovePos = GData()->GetGlobalData()->LobbyGameEnterCamPos;

	_curLength = FVector::Dist2D(camPos, playerPos);
	_endLength = FVector::Dist2D(camMovePos, playerPos);

	_gameEnterMoveTime = 0;
	_isGameEnterCamMove = true;
}

void FGsCameraHandlerLobby::UpdateEnterMove(float In_delta)
{
	float moveTimeRot = GData()->GetGlobalData()->LobbyGameEnterCamMoveTimeRot;
	float moveTimePos = GData()->GetGlobalData()->LobbyGameEnterCamMoveTimePos;
	float moveTimeFOV = GData()->GetGlobalData()->LobbyGameEnterCamMoveTimeFOV;
	float endFOV = GData()->GetGlobalData()->LobbyGameEnterCamFOV;

	_gameEnterMoveTime += In_delta;
	float gameEnterMovePosValue = FMath::Clamp<float>(_gameEnterMoveTime / moveTimePos, 0.f, 1.f);

	float lookLength = FMath::Lerp(_curLength, _endLength, gameEnterMovePosValue);
	_characterLocalPlayer->GetSpringArm()->TargetArmLength = lookLength;

	float gameEnterMoveRotValue = FMath::Clamp<float>(_gameEnterMoveTime / moveTimeRot, 0.f, 1.f);

	_characterLocalPlayer->Controller->SetControlRotation(FMath::Lerp(_camRot, _camMoveRot, gameEnterMoveRotValue));

	float gameEnterMoveFOVValue = FMath::Clamp<float>(_gameEnterMoveTime / moveTimeFOV, 0.f, 1.f);
	float curveNewFOV = FMath::Lerp(_startFOV, endFOV, gameEnterMoveFOVValue);
	_characterLocalPlayer->GetFollowCamera()->SetFieldOfView(curveNewFOV);
}


FVector FGsCameraHandlerLobby::GetCurrentHeadPos(AGsCharacterPlayer* In_char, bool In_isRelativePos)
{
	if (In_char)
	{
		FTransform actorTr = In_char->GetActorTransform();
		if (USkeletalMeshComponent* skeletalComponent = In_char->GetMesh())
		{

			FVector findPos = FVector::ZeroVector;
#ifdef OLD_GET_HEADPOS		
			FGsGameFlowLobby* lobbyFlow = GMode()->GetLobbyFlow();
			if (lobbyFlow == nullptr)
			{
				return FVector::ZeroVector;
			}
			FGsStageManagerLobby* stageManagerLobby = lobbyFlow->GetStageManagerLobby();
			if (stageManagerLobby == nullptr)
			{
				return FVector::ZeroVector;
			}

			//FGsStageLobbyBase* lobbyBase = stageManagerLobby->GetCurrentState().Get();
			//if (lobbyBase == nullptr)
			//{
			//	return FVector::ZeroVector;
			//}

			//if (true == stageManagerLobby->GetCurrentState().IsValid())
			//{
			//	findPos = stageManagerLobby->GetCurrentState()->GetCurrentHeadPos(skeletalComponent);
			//}

			TSharedPtr<FGsStageLobbyBase> nowStage = stageManagerLobby->Find(stageManagerLobby->GetCurrentStageMode());
			if (true == nowStage.IsValid())
			{
				findPos = nowStage->GetCurrentHeadPos(skeletalComponent);
			}
#else
			findPos = _currentHeadPos;
#endif
			if (In_isRelativePos == true)
			{
				findPos = actorTr.InverseTransformPosition(findPos);
			}

			return findPos;

		}
	}

	return FVector::ZeroVector;
}