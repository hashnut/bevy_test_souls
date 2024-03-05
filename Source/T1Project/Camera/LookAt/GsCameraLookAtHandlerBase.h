#pragma once

#include "CoreMinimal.h"
#include "Message/GsMessageContents.h"
#include "GameObject/GsGameObjectHandler.h"

//--------------------------------
// 카메라 바라보기 관리(기본)
//---------------------------------

class USkeletalMeshComponent;
class APlayerController;
class AGsCharacterPlayer;
struct FGsGameObjectMessageParamBase;
class UGsGameObjectBase;

class FGsCameraLookAtHandlerBase : public IGsGameObjectHandler
{
	// 멤버 변수
private:

	// 플레이어 컨트롤러
	APlayerController* _playerController = nullptr;

	// 바라보기 회전 시작값
	FQuat _lookAtQuatFrom = FQuat::Identity;
	// 바라보기 회전 목표값
	FQuat _lookAtQuatTo = FQuat::Identity;

	FVector _baseBoneX = FVector::ZeroVector;
	FVector _baseBoneY = FVector::ZeroVector;
	FVector _baseBoneZ = FVector::ZeroVector;
	// x축 처럼 쓰는 y축 역방향(오잉)
	FVector _baseBoneRecalcX = FVector::ZeroVector;
	// 액터 회전 값
	FQuat _oldActorQuat = FQuat::Identity;

	// 바라보기 회전중인지
	bool _isLookAtRotChanging = false;
	// 바라보기 진행율(delta time * speed 누적값)
	float _progressLookAtRate = 0.0f;
	// 원래대로 돌아가기 인지?
	bool _isBackToNormal = false;
	// 원래 애니회전랑 다른지?
	bool _isDiffAniRot = false;
	// 본 회전했는지?
	bool _isBoneRotChanged = false;
	


	// 메시지 델리게이트 디버그 라인 그릴지
	TPair<MessageContentCamera, FDelegateHandle> _delegateDebugDrawLookAxisLine;
	// 디버그 라인 그릴지
	bool _isDrawDebugLine = false;
protected:
	// 캐릭터
	AGsCharacterPlayer* _char = nullptr;
	// 머리 본 이름
	FName _headBoneName;
	// look at mode 인가?
	bool _isLookAtMode = true;
	// 기본 본 이름
	FName _baseBoneName;
	// 보간 회전인지
	bool _isLerpRotation = false;
public:
	FGsCameraLookAtHandlerBase() : IGsGameObjectHandler(GameObjectHandlerType::LOOKAT) {}
	virtual ~FGsCameraLookAtHandlerBase() = default;

	// 가상함수
public:
	// 초기화(처음 한번만)
	virtual void Initialize(UGsGameObjectBase* owner) override {};		
	// 해제
	virtual void Finalize() override;
	// 갱신
	void Update(float In_delta) override;
public:
	// 기존 초기화 함수이름변경
	virtual void InitializeActor(class AActor* In_char) override;

	// virtual get, set
public:
	virtual void SetBoneRotation(const FQuat& In_quat);
public:
	// 바라보기 갱신
	void UpdateLookAt(float In_delta);
	// 바라보기 시작
	void StartLookAt(FQuat In_targetQuat);
	// 바라보기 끝
	void EndLookAt();
	// 바라보기 체크 하기(min, max 값 안에 있으면 바라보기, 아니면 원래 회전으로 돌아가기)
	void CheckLookAt();
	// 바라보기 rot 가져오기
	bool GetLookAtRot(FRotator& Out_rot);
	// 바라보기 모드 변경
	void ChangeLookAtMode(bool In_mode);
	// 바라보기 끝 도달
	void ReachLookAtTarget();
	// 처음 모드 세팅할때
	void InitMode(bool In_mode);
	// 본 회전
	void BoneRotation(const FQuat& In_quat);

	// 이벤트
public:
	// 디버그 라인 그릴지
	void OnDebugDrawLookAxisLine();

	// get,set
public:
	USkeletalMeshComponent* GetMesh();
	USkeletalMeshComponent* GetMeshTargetHead();
	void SetIsLerpRotation(bool In_val) { _isLerpRotation = In_val; }
	
};