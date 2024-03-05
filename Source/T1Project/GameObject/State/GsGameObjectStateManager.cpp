// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameObjectStateManager.h"
#include "ActorEx/GsCharacterBase.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Define/GsGameObjectDefine.h"

#include "Message/GsMessageGameObject.h"

#include "T1Project.h"
#include "UTIL/GsText.h"

#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif

EGsStateBase FGsGameObjectStateManager::GetCurrentState() const
{
	return nullptr != CurrentState() ? static_cast<EGsStateBase>(CurrentState()->GetStateID()) : EGsStateBase::StateBaseMax;
}

bool FGsGameObjectStateManager::IsState(uint8 StateID) const
{
	return (nullptr != _current && _current->GetStateID() == StateID);
}

bool FGsGameObjectStateManager::IsState(EGsStateBase StateType) const
{
	return (nullptr != _current && _current->GetStateID() == static_cast<uint8>(StateType));
}

bool FGsGameObjectStateManager::IsCompleteIdleState() const
{
	return _listChangeState.Num() <= 0 && IsState(EGsStateBase::Idle);
}

#ifdef TEST_STATE
// 다음 상태 가져오기
EGsStateBase FGsGameObjectStateManager::GetNextState()
{
	EGsStateBase nextState = EGsStateBase::None;

	if (_listChangeState.Num() == 0)
	{
		return nextState;
	}

	nextState = static_cast<EGsStateBase>(_listChangeState.Last()._state->GetStateID());
	return nextState;
}
#endif

void FGsGameObjectStateManager::Initialize(FName bpStateName, UGsGameObjectBase* Owner, IGsStateBase* StartState)
{
	_bpStateName = bpStateName;
	_owner = Owner;
	_prev = nullptr;

	_messageParam._paramOwner = _owner;

	ChangeState(StartState);
	Update(Owner, 0.0f);
	check((_current || _next));
}

void FGsGameObjectStateManager::InitializeActor()
{
#if WITH_EDITOR
	if (_owner)
	{
		if (AGsCharacterBase* characterActor = _owner->GetCharacter())
		{
			_owner->DebugAddStateLog(TEXT(""));
			_isDebugCallstackLog = characterActor->_debugCreature._isFsmCallstackLog;
		}
	}
#endif
}

void FGsGameObjectStateManager::Finalize()
{
	ClearDelayState();

	_owner			= nullptr;
	_prev			= nullptr;
}

void FGsGameObjectStateManager::Update(UGsGameObjectBase* Owner, float Delta)
{
	// 상태 전환 실행
	if (_listChangeState.Num() > 0)
	{
		_next = _listChangeState.Last()._state;
		// 무시된 상태에 전달
		for (int i = 0; i < _listChangeState.Num() - 1; ++i)
		{
			_listChangeState[i]._state->IgnoreState(Owner, _next);

#if WITH_EDITOR
			if (Owner->IsDebugStateLog())
			{
				FString log = FString::Printf(TEXT(" (전환 무시 : %s 전환 무시 판정 (현재 상태 : %s)"),
					*_listChangeState[i]._state->GetStateName().ToString(),
					*_current->GetStateName().ToString());

				_owner->DebugAddStateLog(log);
			}
#endif
		}
	}

	if (_next != nullptr)
	{
		// 현재 상태 설정후 버퍼 Clear
		ClearListChangeState();

		// 타이머가 작동 중이라면 Clear
		ClearDelayState();

		// 선행 처리되야 하는게 있는가
		if (false == _next->Ready(Owner))
		{
			// [B1] | ejrrb10 | 선행 처리 준비가 안되었다면, 현재 상태에 맞게 Update 호출해 주어야 하지 않을까?
			//if (_current)
			//{
			//	_current->Update(Owner, Delta);
			//}
			return;
		}

		_prev = _current;
		_current = _next;
		_next = nullptr;

		if (_prev != _current)
		{
			if (_prev)
			{
				_prev->Exit(_owner);

				// 이벤트 전송
				_messageParam._type = static_cast<EGsStateBase>(_prev->GetStateID());
				_owner->GetMessage().SendMessage(MessageGameObject::StateExit, &_messageParam);
			}

			if (_current)
			{
				_current->Enter(_owner);

				// 이벤트 전송
				_messageParam._type = static_cast<EGsStateBase>(_current->GetStateID());
				_owner->GetMessage().SendMessage(MessageGameObject::StateEnter, &_messageParam);

#if WITH_EDITOR
				if (Owner->IsDebugStateLog())
				{
					FString log = FString::Printf(TEXT(" (전환 성공 : %s (Enter) (이전 상태 : %s (Exit))"),
						*_current->GetStateName().ToString(), (_prev) ? *_prev->GetStateName().ToString() : TEXT("None"));

					_owner->DebugAddStateLog(log);
				}
#endif

//#if WITH_EDITOR
//				// ejrrb10 | 멀티 플레이 환경에서, 상태 전환 시 본인/타 플레이어 스크린 위에 디버그 메시지 띄우기
//				// 실시간 상태 변화를 파악하는 것에 유용하게 활용될 수 있다!
//				if (_prev)
//				{
//					FString log = FString::Printf(TEXT("!! %s Exit -> %s Enter"),
//						*_prev->GetStateName().ToString(), *_current->GetStateName().ToString());
//
//					FDateTime currentTime = FDateTime::UtcNow();
//					FString timeString = FDateTime::UtcNow().ToString(TEXT("%H:%M:%S.%f"));
//					log.Append(TEXT(" / Time : "));
//					log.Append(timeString);
//
//					if (_owner->IsA(UGsGameObjectRemotePlayer::StaticClass()))
//					{
//						GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, *FString::Printf(TEXT("%s"), *log));
//					}
//
//					if (_owner->IsA(UGsGameObjectLocalPlayer::StaticClass()))
//					{
//						GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Emerald, *FString::Printf(TEXT("%s"), *log));
//					}
//				}
//				else
//				{
//					FString log = FString::Printf(TEXT("!! %s Enter"),
//						*_current->GetStateName().ToString());
//
//					if (_owner->IsA(UGsGameObjectRemotePlayer::StaticClass()))
//					{
//						GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, *FString::Printf(TEXT("%s"), *log));
//					}
//
//					if (_owner->IsA(UGsGameObjectLocalPlayer::StaticClass()))
//					{
//						GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Emerald, *FString::Printf(TEXT("%s"), *log));
//					}
//				}
//
//				if (_owner->IsDebugStateLog())
//				{
//					if (_prev)
//					{
//						FString log = FString::Printf(TEXT("!! %s Exit -> %s Enter"),
//							*_prev->GetStateName().ToString(), *_current->GetStateName().ToString());
//						_owner->DebugAddStateLog(log);
//					}
//					else
//					{
//						FString log = FString::Printf(TEXT("!! %s Enter"),
//							*_current->GetStateName().ToString());
//						_owner->DebugAddStateLog(log);
//					}
//				}
//#endif
			}
		}
		else
		{
			if (_current)
			{
				_current->ReEnter(_owner);

				// 이벤트 전송
				_messageParam._type = static_cast<EGsStateBase>(_current->GetStateID());
				_owner->GetMessage().SendMessage(MessageGameObject::StateReEnter, &_messageParam);

#if WITH_EDITOR
				if (_owner->IsDebugStateLog())
				{
					FString log = FString::Printf(TEXT("!! %s ReEnter(재진입)"),
						*_current->GetStateName().ToString());
					_owner->DebugAddStateLog(log);
				}
#endif
			}

		}
	}

	if (_current)
	{
		_current->Update(Owner, Delta);
	}

	if (_timerHandle.IsValid())
	{
		_timerHandle._timer -= Delta;
		if (_timerHandle._timer < 0.f)
		{
			ProcessEvent(_timerHandle._state);
			_timerHandle._isExcute = true;
			ClearDelayState();
		}
	}
}

// Static Function
void FGsGameObjectStateManager::ProcessEvent(UGsGameObjectBase* Owner, EGsStateBase StateType)
{
	if (FGsGameObjectStateManager* fsm = Owner->GetBaseFSM())
	{
		fsm->ProcessEvent(StateType);
	}
}

void FGsGameObjectStateManager::ProcessEvent(uint8 StateID)
{
	if (nullptr == _current)
	{
		if (_next && _next->CanTransition(_owner, StateID))
		{
			_next = _next->GetState(StateID);
		}
		return;
	}

	IGsStateBase* changeState = nullptr;
	// 대기 스택 목록이 있을경우 가장 마지막에 해당 되는 State와 판정 비교
	if (_listChangeState.Num() > 0)
	{
		if (_listChangeState.Last()._state->CanTransition(_owner, StateID))
		{
			changeState = _listChangeState.Last()._state->GetState(StateID);
		}		
#if WITH_EDITOR
		else
		{
			if (_owner->IsDebugStateLog())
			{
				FString log = FString::Printf(TEXT("  (변환 대기 : %s 에서 %s로 Transition은 실패"),
					*_listChangeState.Last()._state->GetStateName().ToString(),
					*_listChangeState.Last()._state->GetState(StateID)->GetStateName().ToString());
				_owner->DebugAddStateLog(log);
			}
		}
#endif
	}
	else if (_current->CanTransition(_owner, StateID))
	{
		changeState = _current->GetState(StateID);
	}
#if WITH_EDITOR
	else
	{
		if (_owner->IsDebugStateLog())
		{
			FString log = FString::Printf(TEXT("  (전환 실패 : %s 에서 %s로 Transition은 실패"),
				*_current->GetStateName().ToString(),
				*_current->GetState(StateID)->GetStateName().ToString());
			_owner->DebugAddStateLog(log);
		}
	}
#endif

	// 대기열 추가
	if (changeState)
	{
		_listChangeState.Emplace(FChangeStateInfo(changeState, _isDebugCallstackLog));
	}
	else
	{
		// 이벤트 전송
		_messageParam._type = static_cast<EGsStateBase>(StateID);
		_owner->GetMessage().SendMessage(MessageGameObject::StateFailed, &_messageParam);

 #if WITH_EDITOR
 		if (_owner->IsDebugStateLog())
 		{
 			// 같은 스테이트 변경 Log 생략
 			if (IGsStateBase* state = _current->GetState(StateID))
 			{
 				if (false == state->IsSameState(StateID))
 				{
 					FDateTime time = FDateTime::Now();
 					FString log = FString::Printf(TEXT("  (전환 실패 : %s로 전환 불가능한 상황 (현재 상태 : %s)"), *time.ToString(),
 						*state->GetStateName().ToString(), *_current->GetStateName().ToString());
					_owner->DebugAddStateLog(log);
 					// PrintStateCallStack();
 				}
 			}
 		}
 #endif
	}
}

void FGsGameObjectStateManager::DelayProcessEvent(float Time, uint8 CastStateType, uint8 StateType)
{
	ClearDelayState();

	_timerHandle._castState = CastStateType;
	_timerHandle._state = StateType;
	_timerHandle._timer = Time;
}

void FGsGameObjectStateManager::PrevProcessEvent()
{
	if (_prev)
	{
		ProcessEvent(_prev->GetStateID());
	}
}

void FGsGameObjectStateManager::DelayPrevProcessEvent(float Time, EGsStateBase inCastStateType)
{
	if (_prev)
	{
		DelayProcessEvent(Time, static_cast<uint8>(inCastStateType), _prev->GetStateID());
	}
}

bool FGsGameObjectStateManager::ChangeState(IGsStateBase* State)
{
	check(State);

	_listChangeState.Emplace(FChangeStateInfo(State, _isDebugCallstackLog));

	return true;
}

void FGsGameObjectStateManager::ClearDelayState()
{
	if (_timerHandle.IsValid())
	{
// #if WITH_EDITOR
// 		// 아직 실행되지 않은 상태가 있는데 삭제된 경우
// 		if (false == _timerHandle._isExcute)
// 		{
// 			if (_current)
// 			{
// 				if (AGsCharacterBase* characterActor = _owner->GetCharacter())
// 				{
// 					IGsStateBase* state = _current->GetState(_timerHandle._state);
// 					IGsStateBase* castState = _current->GetState(_timerHandle._castState);
// 					FString log = FString::Printf(TEXT("DelayState IgnoreState %s CastState %s  Current State %s"),
// 						*state->GetStateName().ToString(), *castState->GetStateName().ToString(), *_current->GetStateName().ToString());
// 					_owner->GetCharacter()->_debugCreature.AddLog(log);
// 				}
// 			}
// 		}
// #endif

		_timerHandle._castState = 0;
		_timerHandle._state = 0;
		_timerHandle._isExcute = false;
	}
}

void FGsGameObjectStateManager::ClearListChangeState()
{
#if WITH_EDITOR
	if (_isDebugCallstackLog && _listChangeState.Num() > 1)
	{		
		for (int i = 0; i < _listChangeState.Num(); ++i)
		{
			GSLOG(Warning, TEXT("-------------------- State Transition Try CallStack (%d / %d) ---------------------"),
				i + 1, _listChangeState.Num());
			if (nullptr == _listChangeState[i]._callstack)
			{
				continue;
			}

			_listChangeState[i]._callstack->DumpStackTraces(0, *GLog);
			_listChangeState[i]._callstack->ResetTracking();
			delete _listChangeState[i]._callstack;
		}
	}
#endif

	_listChangeState.Empty();
}

// 실제 리액션과 이름이 헷갈리수 있어서
// 행동 제약 상태인지
bool  FGsGameObjectStateManager::IsHoldAction() const
{
	if (nullptr == _current)
	{
		return false;
	}
	EGsStateBase currState = static_cast<EGsStateBase>(_current->GetStateID());

	bool resVal = false;
	switch (currState)
	{
	case EGsStateBase::Reaction:
		resVal = true;
		break;
	default:
		resVal = false;
	}


	return resVal;
}

bool FGsGameObjectStateManager::IsDelayEvent() const
{
	return _timerHandle.IsValid();
}