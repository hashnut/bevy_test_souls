#pragma once

#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Core/Public/Delegates/Delegate.h"

#include "Event/GsEventActionSequencer.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsEventActionBase.generated.h"

struct FGsSchemaBase;

UCLASS()
class UGsEventActionBase : public UObject
{
	GENERATED_BODY()

	friend class UGsEventActionSequencer;

private:
	bool _isPlaying = false;
	FSimpleDelegate _onFinished;

public:
	virtual void Initialize(const FGsSchemaBase* EventActionData);
	bool IsPlaying() const;
	void Finish();

	/**
	 * 특정 이벤트액션은 재연결 성공 시, 서버로 패킷 전송 안하고 재실행함.
	 */
	virtual bool ShouldReplayOnReconnection() const;

private:
	bool Play();
	bool Rewind();

protected:
	/**
	 * Play 를 시도하고, 정상 실행될 경우, true 반환.
	 * false 반환할 경우, 바로 다음에 Finish() 를 호출함. 단, false 를 반환하더라도 IsPlaying() 은 finished 전이므로 true 를 반환함.
	 */
	virtual bool OnPlay();

	/**
	 * Play() 실행 후, OnPlay() 내부에서 플레이 관련 작업 실행 시 여러 문제가 발생할 수 있으므로,
	 * 일단 OnPlay() 에서는 초기화 작업 후 무조건 true 를 반환하면, 바로 다음 순서로 이 함수를 호출하므로,
	 * 이 함수에서 플레이 관련 처리를 추가하여 처리함.
	 */
	virtual bool OnAfterPlay();

	/**
	 * 시작한 이벤트액션이 완료될 때 처리할 작업들
	 */
	virtual void OnFinish();

	/**
	 * 이 이벤트액션의 시작 직전 상태로 리와인드할 때 필요한 작업을 수행.
	 * 오버라이드 하지 않은 경우, false 반환(아무것도 하지 않음).
	 * 이 값이 true 를 반환하면, EventManager 로 true 를 반환한 이벤트액션정보를 전달하여, 다음 실행할 때 수행할 이벤트액션을 지정할 수 있음.
	 */
	virtual bool OnRewind();

	/**
	 * 앱이 백그라운드로 들어갈 때 이벤트
	 */
	virtual void OnApplicationWillEnterBackground();

	/**
	 * 앱이 포그라운드로 나왔을 때 이벤트
	 */
	virtual void OnApplicationHasEnteredForeground();
};
