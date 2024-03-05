#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GsActorSummonVeilObject.generated.h"

/*
	뽑기 공개전 object
*/

class USkeletalMeshComponent;
class UCapsuleComponent;
class UWidgetComponent;
class UParticleSystemComponent;
class UAnimMontage;
class UPrimitiveComponent;
class UCurveFloat;
class URotatingMovementComponent;

class UGsUIBillboardSummonStone;

UCLASS()
class T1PROJECT_API AGsActorSummonVeilObject : public AActor
{
	GENERATED_BODY()

	// 멤버 변수
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, EditFixedSize, Category = Summon, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, EditFixedSize, Category = Summon, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;
	// 클릭 표시 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* _clickWidget = nullptr;
	// 등급 표시 파티클
	UPROPERTY()
	UParticleSystemComponent* _particleGrade;
	// 드래그 연출 파티클
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, EditFixedSize, Category = Summon, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* _particleDrag;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, EditFixedSize, Category = Summon, meta = (AllowPrivateAccess = "true"))
	URotatingMovementComponent* _rotMovementComponent;

	// 데이터 인덱스
	int _dataIndex = -1;
	// 고등급 드래그 하는가
	bool _isUseHighGradeDrag = false;

	// 색상 변경 파람 있는지
	bool _isExistColorChangeParam = false;
	// 드래그 모프 타겟 최대 rate 값
	float _dragMorphTargetMaxRate = 0.0f;
	// 최근 모프 타겟 값
	float _currentMorphTarget = 0.0f;

	// 드래그 성공 모프 타겟 끝났는지
	bool _isSuccessDragMorphTargetFinish = false;

	float _successDragProgressVal = 0.0f;

	float _startSuccessDragMorphTarget = 0.0f;
	float _endSuccessDragMorphTarget = 0.0f;

	// 드래그 커브
	UCurveFloat* _dragCurve;
	// 드래그 끝 연출 커브
	UCurveFloat* _dragFinishCurve;

	float _successDragSpeed = 0.0f;

	float _rotMinVal = 0.0f;
	float _rotMaxVal = 45.0f;
	// before drag success
	bool _isUseTouch = true;

public:

	// 클릭 콜백
	TFunction<void(int)> _callbackClick;

	// 드래그 터치 시작 콜백
	TFunction<void(AGsActorSummonVeilObject*)> _callbackDragTouchStart;
	// 드래그 터치 끝 콜백
	TFunction<void()> _callbackDragTouchEnd;

	// 생성자
public:
	AGsActorSummonVeilObject();

public:
	// bp에서 상급 veil object 드래그 성공시 출력 처리
	UFUNCTION(BlueprintImplementableEvent, Category = "SummonSceneObject")
	void SuccessDragHighVeilObject();

	// 가상 함수
protected:
	virtual void BeginPlay() override;
	// 스케일 값 접근
	virtual float GetObjectScaleValue() { return 1.0f; }
	// 등급 파티클 경로 가져오기
	virtual const FSoftObjectPath* GetGradeParticlePath() { return nullptr; }

	virtual FName GetMorphTargetName() { return NAME_None; }
	// 가상 함수(public)
public:
	virtual bool IsHighGrade() { return false; }
	// 스폰 위치 오프셋
	virtual FVector GetSpawnPosOffset()
	{
		return FVector::ZeroVector;
	}
	// 등급 이펙트 오프셋
	virtual FVector GetGradeEffectOffset()
	{
		return FVector::ZeroVector;
	}

	// 로직 함수
public:
	// 소환석 클릭
	void OnClickStone();
	// 자체 클릭 이벤트
	// 터치 시작
	UFUNCTION()
	void OnStoneTouchBegin(ETouchIndex::Type In_type, UPrimitiveComponent* In_component);
	//// 터치 끝
	//UFUNCTION()
	//void OnStoneTouchEnd(ETouchIndex::Type In_type, UPrimitiveComponent* In_component);

	void OnStoneTouchBegin();
	void OnStoneTouchEnd();

	void Finalize();

	// 드래그 값 변경
	void OnDragProgress(float In_progressVal, float In_preProgressVal);
	// 몽타주 드래그 색션으로 이동
	void MoveMontageDragSection();

	// 등급 파티클 붙이기
	void AttachGradeParticle();
	// 파티클 해제
	void ReleaseGradeParticle();
	// 드래그 파티클 액티브
	void ActivateDragParticle(bool In_active);
	// 이펙트 칼라 변경
	void ChangeEffectColor();
	// 드래그 성공 모프  시작
	void StartSuccessDragMorphTarget();
	// 드래그 성공 모프 업데이트
	void UpdateSuccessDragMorphTarget(float In_deltaTime);

	// get, set
public:
	// 인덱스 세팅
	void SetDataIndex(int In_index)
	{
		_dataIndex = In_index;
	}	
	int GetDataIndex()
	{
		return _dataIndex;
	}
	
	USkeletalMeshComponent* GetMesh() { return Mesh; }

	void SetGradeParticle(UParticleSystemComponent* In_particle)
	{
		_particleGrade = In_particle;
	}
	
	// 터치 충돌체 사용 할지
	void SetTouchCollisionEnable(bool In_isActive);
	// 드래그 사용하는지 세팅
	void SetIsUseHighGradeDrag(bool In_isUse);
	// 몽타주 접근
	UAnimMontage* GetCurrentMontage();
	// 소환석 위젯(ui) 접근
	UGsUIBillboardSummonStone* GetSummonStoneWidget();
	// hide 처리
	void SetHide(bool In_isHide);

	bool GetIsSuccessDragMorpthTargetFinish()
	{
		return _isSuccessDragMorphTargetFinish;
	}

	// 진행되는 progress val 값
	float GetSuccessDragProgressVal()
	{
		return _successDragProgressVal;
	}
	// 위젯 hide 처리
	void SetHideWidget(bool In_isHide);
};