#pragma once

#include "Container/GsSharedMap.h"
#include "UTIL/GsText.h"
//----------------------------------------------------------------
// 상태 관리자
// TGsMapAllocator 활용하면 여러가지 관리 객체를 구성할 수있다 
// ex) 검색 GSGameModeManager
//----------------------------------------------------------------

template <typename TGsState, typename TGsStateAllocator>
class T1PROJECT_API TGsStateMng : public TGsSharedMap<typename TGsState::TStateType, TGsState, TGsStateAllocator>
{
public:
	using TStateType = typename TGsState::TStateType;
	static_assert(TIsEnum<TStateType>::Value, "TStateType is not enumerator type");
	
	using Super = TGsSharedMap<TStateType, TGsState, TGsStateAllocator>;

	DECLARE_EVENT(TGsStateMng, MainEvent);
	DECLARE_EVENT_OneParam(TGsStateMng, StateEvent, TStateType);

protected:
	TWeakPtr<TGsState>	CurrentState = nullptr;

	MainEvent				OnInit;
	MainEvent				OnRemoveAll;

	StateEvent				OnEnterState;
	StateEvent				OnLeaveState;

	bool _isChangingState = false;

public:
	TGsStateMng<TGsState, TGsStateAllocator>()
		:
		TGsSharedMap<TStateType, TGsState, TGsStateAllocator>() {}

	virtual ~TGsStateMng<TGsState, TGsStateAllocator>() {}

public:
	//------------------------------------------------------------------------------
	// 추가(어떤상태가 있는지 상속에서 정의해서 추가해준다.)
	virtual void InitState()
	{
		OnInit.Broadcast();
	}

	virtual void RemoveAll()
	{ 
		OnRemoveAll.Broadcast();

		if (CurrentState.IsValid())
		{
			CurrentState.Pin()->Exit();
			CurrentState = nullptr;
		}
		
		Super::Clear();
	};

	//------------------------------------------------------------------------------
	virtual void ChangeState(TStateType inState)
	{
		if (CurrentState.IsValid())
		{
			if (CurrentState.Pin()->GetType() == inState)
			{
				GSLOG(Warning, TEXT("GSTStateMng : ChangeState [%d] call but SameType"), CurrentState.Pin()->GetType());
				return;
			}
		}

		// StateExit 로직을 타던 도중 ChangeState 불리는 상황 체크 위함 
		_isChangingState = true;
		
		auto instance = Super::Find(inState);

		if (auto state = CurrentState.Pin())
		{
			state->Exit();
			OnLeaveState.Broadcast(state->GetType());
			//GSLOG(Warning, TEXT("GSTStateMng : ChangeState [%d] Exit"), CurrentState->GetType());
		}

		CurrentState = instance;

		if (auto state = CurrentState.Pin())
		{
			state->SetType(inState);
			state->Enter();
			OnEnterState.Broadcast(state->GetType());
			//GSLOG(Warning, TEXT("GSTStateMng : ChangeState [%d] Enter"), CurrentState->GetType());
		}

		_isChangingState = false;
	}

public:
	MainEvent& OnInitEvent()		const { return OnInit; }
	MainEvent& OnRemoveAllEvent()	const { return OnRemoveAll; }
	StateEvent& OnEnterStateEvent()  { return OnEnterState; }
	StateEvent& OnLeaveStateEvent() const { return OnLeaveState; }
	bool IsChangingState() const { return _isChangingState; }

public:
	TWeakPtr<TGsState> GetCurrentState() const { return CurrentState; }
};
