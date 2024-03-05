// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/Define/GsGameObjectDefine.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "GsStateBase.h"

#if WITH_EDITOR
#include "Runtime/Core/Public/Containers/StackTracker.h"
#include "NoExportTypes.h"
#endif

/**
 * GameObject 상태 관리 매니져 클래스
 */
class T1PROJECT_API FGsGameObjectStateManager
{
	// ChangeState() 메서드 접근 허용 클래스(유일)
	friend class IGsStateBase;
	
	// 한 프레임에서 일어나는 상태 변환 정보 기록
	struct FChangeStateInfo
	{
#if WITH_EDITOR
		struct FStackTrackerEx : public FStackTracker
		{
			FStackTrackerEx() : FStackTracker(nullptr, nullptr, true)
			{
				CaptureStackTrace();
			}
		};
		FStackTrackerEx* _callstack;
#endif
		class IGsStateBase* _state;
		FChangeStateInfo(class IGsStateBase* State, bool IsDebugMode = false)
		{
			_state = State;
#if WITH_EDITOR
			if (IsDebugMode)
			{
				_callstack = new FStackTrackerEx();
			}
#endif
		}
	};

	// 자체 타이머 구조체
	struct EventTimer
	{
		uint8 _castState = 0;
		uint8 _state = 0;
		float _timer;
		bool _isExcute = false;							// 실행되지 않고 삭제된 경우를 위한 체크
		bool IsValid() const { return _state != 0; }
	};

public:
	DECLARE_DELEGATE_OneParam(FGsStateChangeFailed, uint8/* State Id*/);
	
	bool				_isDebugCallstackLog = false;

private:
	class UGsGameObjectBase* _owner				= nullptr;
	class IGsStateBase*		 _current			= nullptr;
	class IGsStateBase*		 _prev				= nullptr;
	class IGsStateBase*		 _next				= nullptr;
	EventTimer				_timerHandle;

	// 애님 블루프린트 StateMachine 이름을 명시적으로 설정한다.
	FName				_bpStateName;

	// 한프레임에 상태 변환을 시도하는 목록 정보
	// Update에서 최종 상태로 변환을 시도
	TArray<FChangeStateInfo> _listChangeState;

	// 이벤트 전송용 파라미터 구조체 캐싱
	FGsGameObjectMessageParamState _messageParam;

public:
	void Initialize(FName bpStateName, class UGsGameObjectBase* Owner, class IGsStateBase* StartState);
	void Finalize();
	void Update(class UGsGameObjectBase* Owner, float Delta);

	void InitializeActor();

public:
	// 외부 지원 Static 메서드
	static void ProcessEvent(class UGsGameObjectBase* Owner, EGsStateBase StateType);

public:
	// 스테이트 타입값 기반 상태 제어 처리
	// ChangeState() 호출은 상태 클래스 내에서만 가능하도록 Private 선언
	void ProcessEvent(EGsStateBase StateType)
	{
		ProcessEvent(static_cast<uint8>(StateType));
	}

	// 일정 시간 이후에 상태 전환 메세지를 던진다.
	void DelayProcessEvent(float inTime, EGsStateBase inCastStateType, EGsStateBase inStateType)
	{
		DelayProcessEvent(inTime, static_cast<uint8>(inCastStateType), static_cast<uint8>(inStateType));
	}
	void DelayProcessEvent(float Time, uint8 CastStateType, uint8 StateType);

	// 이전 상태 메세지를 던진다.
	void PrevProcessEvent();
	// 일정 시간 이후 이전 상태 메세지를 던진다.
	// 그전에 상태 전환이 발생되었다면 무시된다.
	void DelayPrevProcessEvent(float Time, EGsStateBase inCastStateType);

	// IGsStateBase 선언에 대한 문제를 고민해본다.
	// 현재는 ProcessEvent 템플릿 함수의 호출용으로 사용
	void ProcessEvent(uint8 StateID);

private:
	template <class tState>
	bool ChangeState()
	{
		auto state = tState::GetInstance();
		return ChangeState(state);
	}
	bool ChangeState(class IGsStateBase* State);

private:
	void ClearDelayState();
	void ClearListChangeState();

public:
	const FName& GetBPStateName() const				{ return _bpStateName; }

	FORCEINLINE class IGsStateBase* CurrentState() const	{ return _current; }
	FORCEINLINE class IGsStateBase* PrevState() const		{ return _prev; }

	// 일부 플랫폼에서 인라인 순환 참조로 빌드 실패하여 인라인 삭제
	EGsStateBase GetCurrentState() const;
public:
	
	bool IsState(uint8 StateID) const;
	bool IsState(EGsStateBase StateType) const;
	// 완전한 유휴 상태 확인 : 내부적으로 상태 전환 예정인 목록이 없고 Idle(유휴) 상태일 경우 체크
	bool IsCompleteIdleState() const;

	// 실제 리액션과 이름이 헷갈리수 있어서
	// 행동 제약 상태인지
	bool IsHoldAction() const;
	// 딜레이 예약이 되어있는지 확인
	bool IsDelayEvent() const;

#ifdef TEST_STATE
	// 다음 상태 가져오기
	EGsStateBase GetNextState();
#endif
};
