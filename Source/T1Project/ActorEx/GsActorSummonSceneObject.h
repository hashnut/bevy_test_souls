#pragma once
#include "CoreMinimal.h"
#include "ActorEx/GsActorSequence.h"

#include "GameFramework/Actor.h"
#include "Engine/Classes/Engine/EngineBaseTypes.h"
#include "InputCore/Classes/InputCoreTypes.h"

#include "Shared/Client/SharedEnums/SharedFairyEnum.h"

#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Camera/Define/GsCameraGameDefine.h"

#include "Summon/GsSummonDefine.h"

#include "Message/GsMessageUI.h"

#include "GsActorSummonSceneObject.generated.h"
/*
	뽑기 씬이 되는 bp
*/

class IGsSummonUnveilObjectInterface;
class IGsSummonCameraWork;

class AGsActorSummonVeilObject;
class AGsActorSummonFairyStone;
class AGsActorSummonUnveilObject;
class AGsActorSummonFairy;
class AGsActorSummonCostume;

class UGsUITraySummon;

class FGsSummonCameraLocation;
class FGsSummonCameraRotation;
class FGsSummonCameraBlur;
class FGsGameObjectDataLocalPlayer;

class USceneComponent;
class UCameraComponent;
class UCineCameraComponent;
class AActor;
class UCurveFloat;

struct IGsMessageParam;

UCLASS()
class T1PROJECT_API AGsActorSummonSceneObject : public AGsActorSequence
{
	GENERATED_BODY()

public:	
	// 스폰 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<USceneComponent*> _arraySpawnPos;
	// 카메라 컴퍼넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//UCineCameraComponent* _sceneCamera = nullptr;
	UCameraComponent* _sceneCamera = nullptr;
	// 카메라 윗단
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _cameraRotator;
	// 나뭇잎 연출 이펙트 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _leafFxPos;

	// 드래그 진행 값
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float _dragProgressVal;
	// 카메로 노출값 백업
	float _backupCameraExposureVal = 0.0f;
	// 드래그 사운드 id(pair가 안맞을수도 있어서 clear쪽에서 다지우는걸로)
	TSet<uint64> _setDragSoundId;
	// 소환 기본 소리 id
	uint64 _summonBaseSoundId = 0;
private:
	// 스폰된 소환석들
	UPROPERTY()
	TArray<AGsActorSummonVeilObject*> _arraySummonVeilObject;
	// 맵: 인덱스, 오브젝트
	TMap<int, AGsActorSummonVeilObject*> _mapSummonVeilObject;

	// 스폰된 페어리들
	UPROPERTY()
	TArray<AActor*> _arraySummonUnveilObject;
	// 맵: 인덱스, 오브젝트
	TMap<int, IGsSummonUnveilObjectInterface*> _mapSummonUnveilObject;

	// 소환석 소환 시작 시간(한개)
	int64 _startTimeSummonStone;
	// unveil 오브젝트 표시 시작 시간(한개)
	int64 _startTimeSummonUnveilObject;

	// 이펙트 칼라 변경 시간 시작점
	int64 _startTimeEffectColorChange;

	// 소환할 소환석 등급들
	TArray<FairyGrade> _arraySummonStoneGrade;
	// 소환할 코스튬 등급들
	TArray<CostumeGrade> _arraySummonVeilCostumeGrade;
	// 순차적으로 보여줄 오브젝트 인덱스
	TArray<int> _arraySummonOrderUnveilObjectShow;
	
	// 결과 ui 참조
	TWeakObjectPtr<UGsUITraySummon> _traySummon;

	// 카메라 기능들
	TMap<EGsCameraWorkType, IGsSummonCameraWork*> _mapCamWorks;

	// 고등급 드래그 소환석
	AGsActorSummonVeilObject* _highGradeDragStone;

	// 이전 터치 위치
	FVector  _prevTouchLoc;
	// 누르는 시점 터치
	FVector _pressTouchLoc;
	// 오른쪽 방향 드래그 인지 (진행도 상승되는 기준)
	bool _isRightDrag = true;
	// 터치 중인지
	bool _isTouchPress = false;
	// 스톤에서 눌렀는지 체크
	bool _stonePress = false;
	// 드래그 종료 했는지
	bool _isDragComplete = false;

	// 선택 베일 오브젝트 (페어리, 코스튬) 인덱스
	int _selectVeilObjectIndex = 0;
	// 소환 진행 단계
	EGsSummonProgressStepType _summonStep = EGsSummonProgressStepType::None;
	// 카메라 위치 백업
	FVector _backupCameraLoc = FVector::ZeroVector;
	// 카메라 회전 백업
	FRotator _backupCameraRot = FRotator::ZeroRotator;

	// 줌중인 카메워크
	TArray<EGsCameraWorkType> _arrayZoomWork;

	// 시퀀스 플레이 했는지
	bool _isPlaySequence = false;
	// 소환 타입
	EGsSummonType _summonType = EGsSummonType::SUMMON_FAIRY;
	// 로컬 데이터
	FGsGameObjectDataLocalPlayer* _localData;

	// 순차 unveil object show 처리 마지막 콜백
	TFunction<void()> _callbackShowFinishOrderUnveilObject;

	// 드래그 끝에서 카메라 화이트 페이드 아웃 커브
	UCurveFloat* _dragCameraWhiteCurve;
	// 드래그 카메라 노출 값 최대
	float _dragCameraExposureMax = 10.0f;
	// 모든 veil show 막을지(일반, 고등급)- 스폰 연출중
	bool _isLockAllVeilShow = false;

	MsgUIHandleArray	_listUiDelegate;
	bool _isUseFadeEffect = false;
	
	// 생성자
public:
	AGsActorSummonSceneObject();

	//bp call function
public:
	// bp에서 나무등 object show -> hide 처리 위해 호출
	UFUNCTION(BlueprintImplementableEvent, Category = "SummonSceneObject")
	void StartOpenningSequence();
	// bp에서 나무등 object hide -> show 처리 위해 호출
	UFUNCTION(BlueprintImplementableEvent, Category = "SummonSceneObject")
	void EndOpenningSequence();
	// bp에서 상급 veil object 드래그 성공시 출력 처리
	UFUNCTION(BlueprintImplementableEvent, Category = "SummonSceneObject")
	void SuccessDragHighVeilObject();
	// touch start(bp camera shake use)
	UFUNCTION(BlueprintImplementableEvent, Category = "SummonSceneObject")
	void TouchStartHighGradeSummonStone();
	// touch end(bp camera shake use)
	UFUNCTION(BlueprintImplementableEvent, Category = "SummonSceneObject")
	void TouchEndHighGradeSummonStone();

	// 가상함수
public:
	virtual void EndPlay(EEndPlayReason::Type Reaseon) override;
	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

	// 로직 함수
public:
	// 시작(skip이면 바로 스폰, 아니면 카메라 무빙 연출 시작)
	void Initialize();

	// 소멸 처리
	void Finalize();

	// 페어리 보여주기(하나)
	void ShowUnveilObject(int In_dataIndex);
	// unveil object 소환 ani 출력
	void PlayUnveilObjectSpawnAni(int In_dataIndex);
	// 페어리 보여주기(저등급만)
	void ShowFairies();
	// 소환석 스폰
	AGsActorSummonVeilObject* SpawnVeilObject(FString In_path, int In_index);
	// 소환석 디스폰
	void DespawnVeilObject(int In_index);
	// 모든 소환석 디스폰
	void DespawnVeilObjects(bool In_isAllGrade);
	// 페어리 스폰
	IGsSummonUnveilObjectInterface* SpawnUnveilObject(FString In_path, int In_index);
	// 모든 페어리 디스폰
	void DespawnUnveilObjects();
	// 고등급인가(페어리)
	bool IsHighGradeFairy(FairyGrade In_grade)
	{
		return ((int)In_grade >= (int)FairyGrade::RARE) ? true : false;
	}
	// 고등급인가(코스튬)
	bool IsHighGradeCostume(CostumeGrade In_grade)
	{
		return ((int)In_grade >= (int)CostumeGrade::RARE) ? true : false;
	}
	//  모든 소환석 터치 충돌체 active on, off
	// 고등급 연출시 사용(연출 시작때 껐다가 끝나고 다시 킴)
	// 고등급은 빼고
	void SetStoneTouchCollisionActive(bool In_isActive, int In_highGradeIndex = -1);
	// 페어리, 소환석 hide 처리
	void SetHideObjects(bool In_isHide, int In_highGradeIndex = -1);
	// 터치 땠을때 소환석 돌아가는 처리
	void UpdateReleseStone(float In_deltaTime);

	// 시간에 따른 다음 소환석 스폰
	void UpdateNextSummonVeilObject();
	// 시간에 따른 다음 unveil object
	void UpdateNextSummonUnveilObject();

	// 시간에 따른 drag 이펙트 색상 처리
	void UpdateDragEffectColor();

	// 고등급 등장 연출 시작
	// 1. 컷신 시작
	void StartHighGradeOpen();
	// 페어리 오픈됨
	// 1. 보여주기
	// 2. 소환석 디스폰
	// 3. 모두 열었으면 ui 버튼 나가기로 갱신
	void OpenUnveilObject(int In_index, bool In_isShowUnveil = true);

	// 선택된 페어리 결과 보기
	// 1. OpenFairy(위의 함수 호출)
	// 2. 카메라 복구(줌상태에서 원래로)
	// 3. 충돌 복구
	void ShowSelectUnveilObjectResult();
	// 모든 고등급 페어리 결과 보기
	void ShowAllHighGradeUnveilObjectResult();
	// 모든 일반등급 페어리 결과 보기
	void ShowAllNormalGradeUnveilObjectResult();

	// 순차 페어리 보여주는 연출
	void ShowUnveilObjectSingle();

	// 모든 페어리 열렸을때 처리
	void ProcAllFairyOpened();

	// 소환 시작
	void StartSummon(EGsSummonType In_summonType);

	// 페어리, 코스튬 스폰(모두)
	void SummonUnveilObjects();

	// veil 오브젝 스폰(모두)
	void SummonVeilObjects();
	// 나뭇잎 이펙트 만들기
	void MakeLeafEffect();
	// 카메라 노출값 갱신
	void UpdateCameraExposure(float In_progressVal);

	// 소환 사운드 믹서 교체
	void ChangeSoundSummonMixer(bool In_isSummonMode);
	// 드래그 사운드 시작
	void StartDragSound();
	// 드래그 사운드 끝
	void EndDragSound();


	// 페어리
public:
	// 페어리 스폰(하나)
	void SummonFairy(int In_dataIndex);
	// 스톤 스폰
	void SummonFairyStones();
	// 스톤 스폰 하나(no skip)
	void SummonFairyStoneSingle();
	// 페어리 소환석 스폰 처리
	AGsActorSummonVeilObject* SpawnFairyStone(int In_dataIndex, FairyGrade In_grade);


	// 코스튬
public:
	// 코스튬 스폰(하나)
	void SummonCostume(int In_dataIndex);
	void SummonVeilCostumes();
	// veil 코스튬 스폰 하나(no skip)
	void SummonVeilCostumeSingle();
	// veil 코스튬 스폰 처리
	AGsActorSummonVeilObject* SpawnVeilCostume(int In_dataIndex, CostumeGrade In_grade);

	// 이벤트
public:
	// 소환석 클릭
	void OnClickSummonVeilObject(int In_index);
	// 고등급 소환석 터치 시작
	void OnTouchStartHighGradeSummonStone(AGsActorSummonVeilObject* In_stone);
	// 고등급 소환석 터치 끝
	void OnTouchEndHighGradeSummonStone();

	// 무비 끝
	void OnStopMovie(bool In_isOpeningCutscene);

	void OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location);
	void OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location);
	void OnTouchMove(ETouchIndex::Type FingerIndex, FVector Location);
	// 줌 보간끝났을때
	void OnZoomLerpFinish(EGsCameraWorkType In_type);

	// message
public:
	// call tray fade effect animation finished
	void OnFadeEffectAniFinished(const IGsMessageParam*);

	// get, set
public:
	// 인덱스에 의한 위치 구하기
	USceneComponent* GetPosComponent(int In_index);
	// 카메라 이동 처리 가져오기
	FGsSummonCameraLocation* GetCameraLocation();
	// 카메라 회전 처리 가져오기
	FGsSummonCameraRotation* GetCameraRotation();
	// 카메라 블러 처리 가져오기
	FGsSummonCameraBlur* GetCameraBlur();
	// 카메라 처리 타입 가져오기
	IGsSummonCameraWork* GetCameraWork(EGsCameraWorkType In_type);
	// 카메라 컴퍼넌트 접근
	UCameraComponent* GetCameraComponent();
	// 선택된 고등급 소환 페어리(등장 컷신에서 참조)
	AGsActorSummonFairy* GetSelectHighGradeSummonFairy();
	// 선택된 고등급 소환 코스튬
	AGsActorSummonCostume* GetSelectHighGradeSummonCostume();
	// 선택된 고등급 소환 코스튬 id(등장 컷신에서 참조)
	CostumeId GetSelectHighGradeSummonCostumeId();
	// 해제 안한 object 아직 있는가
	bool IsVeilObjectExist()
	{
		return (_arraySummonVeilObject.Num() > 0) ? true : false;
	}
	// 남은 스톤이 모두 고등급인지
	bool IsAllHighGradeStone();

	// open all로 상태 바꾸기(남은 소환석이 모두 고등급이면 show result로)
	void SetBtnTypeOpenAll();

	bool GetIsPlaySquence()
	{
		return _isPlaySequence;
	}

	void SetTraySummon(TWeakObjectPtr<UGsUITraySummon> In_ui)
	{
		_traySummon = In_ui;
	}

	void SetLocalData(FGsGameObjectDataLocalPlayer* In_localData)
	{
		_localData = In_localData;
	}
	// 카메라 노출값 세팅
	void SetCameraExposure(float In_val);	
};