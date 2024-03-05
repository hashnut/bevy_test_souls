// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsSequenceResTable.h"
#include "Cinematic/GsSchemaSequenceResData.h"
#include "Message/GsMessageSystem.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "GsSequencePlayer.generated.h"

/**
 * 
 */
static const FName SequencerActorTag(TEXT("SequencerActor"));
static const FName HideWhenSequencePlayTag(TEXT("HideWhenSequencePlay"));

class ULevelSequencePlayer;
class ULevelSequence;
class ALevelSequenceActor;
class UActorSequencePlayer;
class ULevelSequence;
class AActor;

struct FGsSequenceMessageParam;

UCLASS()
class T1PROJECT_API UGsSequencePlayer : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	ULevelSequencePlayer*		_sequencePlayer;		// 시퀀서 플레이어	
	UPROPERTY()
	ALevelSequenceActor*		_sequenceActor;			// 시퀀서	
	UPROPERTY()
	ULevelSequence*				_levelSequence;			// Aasset (리소스)	
	UPROPERTY()
	UActorSequencePlayer*		_actorSequencePlayer;	// 액터시퀀서 플레이어	
		

protected:
		// 시퀀스 플레이 전 액터가 보이던 상태
	UPROPERTY()
		TArray<AActor*>				_showActorBeforeSequencePlayArray;
	UPROPERTY()
		TArray<AActor*>				_hideActorBeforeSequencePlayArray;

protected:
	UPROPERTY()
	int32						_sequenceId;			// ID
	UPROPERTY()
	uint64						_audioId = 0;			// audio ID

	// 컷신 로딩시 버벅일경우 미리 저장한다
	// 저장된 기존 level 데이터
	UPROPERTY()
	TArray<ULevelSequence*>		_arrayStoredLevelSequence;
	// 검색용 저장 레벨 데이터
	TMap<int32, ULevelSequence*> _mapStoredLevelSequnce;

	const FGsSchemaSequenceResData* _sequenceResData = nullptr;

	MsgSystemHandleArray	_listSystemParamDelegate;
	MsgSystemHandleArray	_listSystemDelegate;

	FGsSequenceMessageParam _messageParam;

	bool _isUIHide = true;
	bool _isSoundMixChanged = false;
	bool _isClickSkip = false;
	FTimerHandle _timeHandleDimmedOn;

	bool _isDirtySkipOn = false;
public:
	virtual void Initialize();
	virtual void Finalize();

private:
	//액터의 show, hide 설정을 위한 함수
	void UpdateVisible(UWorld* inWorld, bool isFinish = false);	
	// 오직 시퀀스 플레이 할때만 액터 보이도록 설정	
	void ClearShowActorBeforeSequencePlay();	
	void ClearHideActorBeforeSequencePlay();


	void PostSequenceEnd();

	void DimmedOff();

	

public:
	// 스퀸서 애셋을 플레이 시킨다.
	void PlaySequence(const IGsMessageParam* inParam);
	void PlaySequence(ULevelSequence* inLevelSequence);
	void StopSequence();
	void StopSequence(const IGsMessageParam* inParam);
	void PauseSequence(const IGsMessageParam* inParam);
	void PauseNextPlaySequence(const IGsMessageParam* inParam);

	// 테스트
	void ForceStopSequence(const IGsMessageParam* inParam);

	// 시퀀스 플레이어만 종료(메시지 send, 나머지 정리 빼고)
	void StopSequenceOnlyPlayer(const IGsMessageParam* inParam);

	// 시퀀스 플레이어만 종료 (클라도 서버도 메세지를 보내지 않는다.)
	void ErrorStopSequence(const IGsMessageParam* inParam);

	// 액터의 시퀀서를 플레이 시킨다. 
	// 애셋은 따로 존재 하지 않는다. 시퀀스는 액터에 컴포넌트로 존재한다. 
	void PlayActorSequence(AActor* inOwnerActor, const FString& inActorSequenceName);
	void StopActorSequence();

	void Update(float In_delta);
private:
	bool IsGameFlowTypeGame()  const;

public:
	bool IsPlaying() const;		// 시네마틱이 나오고있는 중인가?
	bool IsPaused() const;		// 시네마틱이 일시정지 상태인가?

	void StopSound();			// 시네마틱에서 틀어놓은 사운드가 있으면 중지 후 믹서빼기

public:
	UFUNCTION()
	void OnPlayLevelSequence();
	UFUNCTION()
	void OnFinishLevelSequence();

	UFUNCTION()
	void OnWillEnterBackground();
	UFUNCTION()
	void OnHasEnteredForeground();

	void NetDisconected();			// 네트워크 연결 해제
	void ReconnectStart();			// 재연결 시작
	void ReconnectEnd();			// 재연결 완료


public:	
	ULevelSequence* GetSequenceRes(const FSoftObjectPath inSequenceResPath) const;
	const FGsSchemaSequenceResData* GetSequenceResData(int32 inSequenceId) const;		// 리소스 테이블을 얻어온다.	
	// 저장된거 가져오기
	// 없으면 로드
	ULevelSequence* GetStoredRes(int32 In_sequenceId, const FSoftObjectPath inSequenceResPath);

public:
	// R3: 로딩 지연으로 싱크가 깨지는 문제 때문에, 플레이 후 1프레임에 키 이벤트를 받아 재생
	void StartSound();

public:
	static bool IsSequencePlaying();
};
