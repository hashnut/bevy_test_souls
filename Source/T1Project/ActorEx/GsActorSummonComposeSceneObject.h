#pragma once
#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "GsActorSummonComposeSceneObject.generated.h"

class UCameraComponent;
class USceneComponent;

class AGsActorSummonComposeCeline;
class UGsUITraySummonSkip;

// summon compose: scene

UCLASS()
class T1PROJECT_API AGsActorSummonComposeSceneObject : public AActor
{
	GENERATED_BODY()

public:
	// 드래그 진행 값
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float _dragProgressVal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _spawnPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _camStartPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _camEndPos;

private:
	UCameraComponent* _sceneCamera = nullptr;

	// 이전 터치 위치
	FVector  _prevTouchLoc;
	// 누르는 시점 터치
	FVector _pressTouchLoc;
	// 위쪽 방향 드래그 인지 (진행도 상승되는 기준)
	bool _isUpDrag = true;
	// 터치 중인지
	bool _isTouchPress = false;
	// 드래그 종료 했는지
	bool _isDragComplete = false;
	// 드래그 사운드 id(pair가 안맞을수도 있어서 clear쪽에서 다지우는걸로)
	TSet<uint64> _setDragSoundId;
	// 소환 합성 bgm 소리 id
	uint64 _summonComposeBGMSoundId = 0;
	UPROPERTY()
	AGsActorSummonComposeCeline* _composeCeline;

	TWeakObjectPtr<UGsUITraySummonSkip> _summonSkipUI;

	// https://jira.com2us.com/jira/browse/C2URWQ-4085
	bool _isDragInteractionFinish = false;

public:
	AGsActorSummonComposeSceneObject();

	// 가상함수
public:
	virtual void EndPlay(EEndPlayReason::Type Reaseon) override;
	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

	// 
public:
	void OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location);
	void OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location);
	void OnTouchMove(ETouchIndex::Type FingerIndex, FVector Location);

	// logic func
public:
	void Initialize();

	void SpawnCeline();

	void DespawnCeline();

	// 터치 땠을때 소환석 돌아가는 처리
	void UpdateReleseStone(float In_deltaTime);
	// 드래그 사운드 끝
	void EndDragSound();
	// 드래그 사운드 시작
	void StartDragSound();
	void CompleteDrag();

	void Finalize();

	// 소환 사운드 믹서 교체후 기본 음악(bgm) 출력
	void ChangeSoundSummonMixer(bool In_isSummonMode);

	void ChangeProgress(float In_nowProgressRate, float In_oldProgressRate);

	void ChangeProgressUI(float In_progressRate);

	void ChangeProgressCamPos(float In_progressRate);

	// get
public:
	TWeakObjectPtr<UGsUITraySummonSkip> GetUITraySummonSkip();
};