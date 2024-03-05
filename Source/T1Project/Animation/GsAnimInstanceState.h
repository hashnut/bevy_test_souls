// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimNode_StateMachine.h"
#include "Animation/AnimNotify/AnimNotify_PlayParticleManaged.h"

#include "Shared/Client/SharedEnums/SharedMoveEnum.h"
#include "DataSchema/GsSchemaEnums.h"

#include "GsAnimationDefine.h"
#include "GameObject/Define/GsGameObjectDefine.h"

#include "GsAnimInstanceState.generated.h"

// 쓰레드 동기화 구조체
// 애니메이션 BP의 이벤트 그래프 작업을 할경우 이곳에서 대체 로직을 작성한다.
// BP 스테이트 머신이 여러개 생성되게 될경우는 따로 작업이 필요
// @see : FAnimNode_StateMachine* GetFirstMachine()
USTRUCT(BlueprintType)
struct FGsAnimInstanceStateProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()


friend class UGsAnimInstanceState;
// BP 연동 Animation Fast Path 관련 변수
// 여기에 선언된 값들중 참(true)은 유일해야 한다. 
public:	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool IsInitilaize = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool IsSpawn = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool IsIdle = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool IsBasicMoving = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool IsMoving = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool IsDie = false; // 삭제 예정
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool IsRoar = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool IsRevive = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool IsFreeze = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool IsSocial = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool IsHit = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool IsRunAway = false; 

	// 부가 사용 정보
public:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	int RandomIndex = 0;

	// Idle 세부 상태 플레그
	// 이둘은 상호 베타적이어야 한다. ( 배틀 True 거나 대화 True거나)
	// 전투 상태
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsAnimInstanceState")
	bool IsBattleMode = false;
	// 대화 상태
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsAnimInstanceState")
	bool IsSocialMode = false;
	// 이상 상태
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsAnimInstanceState")
	bool IsAbnormalityMode = false;	
	// Lod Max 단계 체크
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsAnimInstanceState")
	bool IsMaxLodIndex = false;


// 동기화용 변수
public:	
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsAnimInstanceState")
	bool IsActiveMontage = false;
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsAnimInstanceState")
	EGsStateBase StateType = EGsStateBase::None;


	//FAnimNode_StateMachine* _firstMachine = nullptr;

public:
	FGsAnimInstanceStateProxy() : FAnimInstanceProxy() {}
	FGsAnimInstanceStateProxy(UAnimInstance* Instance) : FAnimInstanceProxy(Instance) {}

public:
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	virtual void Update(float DeltaSeconds) override;
	virtual void PostUpdate(UAnimInstance* InAnimInstance) const override;
	virtual void PostEvaluate(UAnimInstance* InAnimInstance) override;

	virtual void CacheBones() override;

private:
	void ChnageStateToBPAnimationFastPath(EGsStateBase State);
	void InitializeAnimationFastPath();

private:
	FAnimNode_StateMachine* GetFirstMachine();
};

/**
 * 애님 블루프린트 동기화용 클래스
 * 애님 블루프린트의 상태머신은 EGsStateBase 설정값 체크로만 변환(transition)을 일으켜야한다.
 */
UCLASS()
class T1PROJECT_API UGsAnimInstanceState : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	DECLARE_DELEGATE(FGsRecalcSizeDelegate);

private:
	// 내부 제어용 파티클 구조체
	// UAnimNotify_PlayParticleManaged 형태의 notify로 구성된 파티클 수집
	struct AnimNotifyParticle
	{
		TArray<TWeakObjectPtr<class UParticleSystemComponent>> _listParticle;
		void Add(TWeakObjectPtr<class UParticleSystemComponent> In_Particle)
		{
			_listParticle.Emplace(In_Particle);
		}
		void RemoveAll(class UParticleSystemComponent* In_Particle)
		{
			_listParticle.RemoveAll([In_Particle](TWeakObjectPtr<class UParticleSystemComponent> el)
			{
				return In_Particle == el.Get();
			});

			for (TWeakObjectPtr<class UParticleSystemComponent>& el : _listParticle)
			{
				//GSLOG(Warning, TEXT("[Debug Particle] not destroy List %s"), *(el.Get()->Template->GetName()));
			}
		}
		bool IsEmpty()
		{
			return _listParticle.Num() == 0;
		}
	};

// 부가 BP용 접근 변수 모음
public:	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	float MoveSpeed = 0.f;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	float BlendSpaceAngle = 0.f;

	// 서로 베타적인 관계 타입
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool IsBattleMode = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool IsSocialMode = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool IsAbnormalityMode = false;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool IsActiveMontage = false;	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool IsPoseAnimation = false;
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsAnimInstanceState")
	bool IsPhysicsAnimation = true;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool IsBlendSpaceAngleStart = false;

	// 몽타주 특정 슬롯 플레이 정보
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	int SlotMontagePlay = 0;

	// 모션 블렌딩 설정 값
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState", Meta=(Tooltip="Montage BlendOut 설정값"))
	float MontageToSequenceBlendTime = 0.f;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState", Meta=(Tooltip="Montage BlendIn 설정값"))
	float SequenceToMontageBlendTime = 0.f;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	EGsStateBase CurrentStateType = EGsStateBase::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsAnimInstanceState", Meta = (Tooltip = "BlendSpaceAngle 초당 도달 각도 (180 = 1초동안 180Angle에 도달)"))
	float BlendSpaceAngleSpeed = 90.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsAnimInstanceState", Meta = (Tooltip = "BlendSpaceAngle 복귀시 초당 도달 각도"))
	float BlendSpaceAngleReturnSpeed = 90.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsAnimInstanceState", Meta = (Tooltip = "BlendSpaceAngle 최소 각도 처리시 초당 도달 각도"))
	float BlendSpaceAngleMinSpeed = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsAnimInstanceState")
	FName StateMachineName = TEXT("Locomotion");

	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	FName _cacheSnapshotName = TEXT("cacheSnapshot");	


	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsAnimInstanceState")
	bool IsMovingAttack = false;


	EGsAnimStateIdleType IdleType;

	// 애니메이션 Index 설정
	// 외부 BPAnimation에서  참조
	int AnimationBlendIndex = 0;

	// 루트모션 사용 몽타주 저장용
	bool IsRootmotionAnimation = false;

protected:
	// 변환뒤 지속 시간 체크
	float Timer, UpperTimer;

	// 이동 블랜드 스페이스에 고정된 값을 전달하기 위한 참고 변수들
	int _walkSpeed = 200;
	int _runSpeed = 500;
	int _vehicleSpeed = 1500;
	
	// 내부 관리용 파티클 버퍼
	TMap<EPlayParticleManagedType, AnimNotifyParticle> _mapAnimNotifyParticle;

public:
	UGsAnimInstanceState();
	virtual ~UGsAnimInstanceState();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//virtual void OnMontageInstanceStopped(FAnimMontageInstance & StoppedMontageInstance) override;

public:
	void InitSkeletalmeshComponent();
	void InitMoveSpeedValue(int walkSpeed, int runSpeed, int vehicleSpeed = 1500);

public:
	// util함수
	// 붙이려는 메시의 캡슐 컴퍼넌트 바닥과 메시의 위치 조정을 위해 차이값을 계산
	// 기준이 되는 CapsuleComponent의 바닥과 스켈레탈 메시의 바닥에 해당하는 본을 기준으로 바닥 위치 동기화 (높이 Offset)
	// SrcCapsuleCompoent : 계산의 기준이 되는 CapsuleComponent
	// DestMeshComponent, DestBoneName : 이동 시켜려는 스켈레탈 메시와 가장 밑바닥이라고 생각하는 Bone또는 소켓 이름
	static float GetCapsuleRelativeHeight(class UCapsuleComponent* SrcCapsuleCompoent,
		USkeletalMeshComponent* DestMeshComponent, const FName& DestBoneName);

public:
	// FSMManager 애님 블루프린트 FSM 파라미터 동기화
	// FSMManager에서 직접 참조하고 싶지만.. 아직 잘모르겠음..
	// 문제점1. AMyCharacter 객체 참조를 하려해도 에디터상 문제가 생김(CDO)
	// 문제점2. 애님 블루프린트 상태 파라미터 검사시 쓰레드 세이프 하지 않음(AMyCharacter 형변환 노드 사용시)
	void ChangeState(uint8 State, int Index = 0, bool IsRandom = true);
	
	void SetIdleType(EGsAnimStateIdleType Type);
	void SetBattleIdleType(bool IsBattle);

	void SetMoveSpeed(MotionType motionType);
	// 스피드값 직접 변경
	void SetMoveSpeed(float Speed);
	// 회전 블렌드 스페이스 단계 설정 
	void SetBlendSpaceAngle(float Angle);	
	void StopBlendSpaceAngle();

	// 물리(카와이) 애니메이션 적용
	void SetPhysicsAnimation(bool IsPhysics);
	// 루트모션 활성 플레그 설정
	void SetRootmotionAnimation(bool IsEnable);

	// 슬롯 몽타주 플레이 지정
	void SetSlotMontagePlay(EGsAnimSlotType AnimSlotType = EGsAnimSlotType::DefaultSlot);
	
	void StopMontagePlay();

	// 에니메이션 Pose
	void Pose();
	void Resume();

public:
	// 해당 에니메이션 데이터 길이 얻기
	float GetStateAnimationLength(FName FsmName, FName StateName);
	// 현재 스테이트 머신 상태 초기화 ( 쓰레드 문제가 생길수 있으니 확인 필요... )
	void ResetCurrentState(FName FsmName);

	// 애님 노티파이 파티클 관리
	void AddAnimNotifyParticle(EPlayParticleManagedType In_Type, TWeakObjectPtr<class UParticleSystemComponent> In_Particle);
	void RemoveAnimNotifyParticle(EPlayParticleManagedType In_Type);

	// 몽타주 JumptoSection 오버라이드
	// MissingAnimNotifyExcute : 한 몽타주 에서 활성화 되지 못하고 넘어간 AnimNotify들을 재생시킨다.
	float PlayAnimMontageJumpToSection(UAnimMontage* AnimMontage, float InPlayRate, FName SectionName, bool MissingAnimNotifyExcute = false);	

	// 해당 에니메이션 AnimNotify 강제 삭제
	void AnimNotifyDestroy(FName FsmName, FName StateName);
	void AnimNotifyDestroy(UAnimSequenceBase* AnimSeqBase);
	void AnimNotifyQueueDestroy();
	void ActiveMontageAnimNotifyDestroy();	

private:
	void AnimNotifyDestroy(UAnimNotify* Notify);

private:
	UFUNCTION()
	void CallbakMontageStarted(UAnimMontage* Montage);

	UFUNCTION()
	void CallbakStateChangedExit(const FAnimNode_StateMachine& Machine, int32 PrevStateIndex, int32 NextStateIndex);

	UFUNCTION()
	void CallbackAnimNotifyParticleDestroy(class UParticleSystemComponent* In_PSystem);

	// Custom AnimProxy
private:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState", meta = (AllowPrivateAccess = "true"))
	FGsAnimInstanceStateProxy _proxy;

	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}

	friend struct FGsAnimInstanceStateProxy;

public:
	// 애님 플루프린트 FSM동기화 참조 함수
	UFUNCTION(BlueprintPure, Category = "GsAnimInstanceState", meta = (BlueprintThreadSafe))
	bool IsState(EGsStateBase State) const;

	UFUNCTION(BlueprintPure, Category = "GsAnimInstanceState", meta = (BlueprintThreadSafe))
	bool IsStates(const TArray<EGsStateBase>& States) const;

	// 캐릭터의 캡슐 콜리더 사이즈 변경을 본 크기에 맞춰 변경
	// 현재 해당 함수 호출은 AnimState BP의 이벤트 그래프에서 호출
	UFUNCTION(BlueprintCallable, Category = "GsAnimInstanceState", meta = (BlueprintThreadSafe))
	void OnInitialize(FName TopBoneName, FName BottomBonName);

public:
	float GetCurrentBlendSpaceAngle() const;
};
