#pragma once

#include "CoreMinimal.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

/*
* 이벤트와 상태를 Enum 으로 받는 클래스 정의
* 기본적으로 Event 와 EnumState 일대일 대응을 가정하고 만들었다.
*/ 
template <typename TEnumEvent, typename TEnumState/*, typename TMapType*/>
class TGsEnumEventValidatorBase
{
	static_assert(TIsEnumClass<TEnumEvent>::Value, "TEnumEvent is not enumerator type");
	static_assert(TIsEnumClass<TEnumState>::Value, "TEnumState is not enumerator type");
	//static_assert(TIsDerivedFrom<TMapType, TMapBase>::IsDerived, "TMapType is not derived from TMapBase");
	//using MapType = typename TEnableIf<TIsDerivedFrom<TMapType, TMapBase>::IsDerived, TMapType>::Type;

protected:
	TOptional<TMap<TEnumEvent, TEnumState>> _validatorMap;

public:
	TGsEnumEventValidatorBase() = default;
		
	TGsEnumEventValidatorBase(TGsEnumEventValidatorBase&& Rhs) : _validatorMap(Forward<TOptional<TMap<TEnumEvent, TEnumState>>>(Rhs._validatorMap)) {}
	TGsEnumEventValidatorBase(const TGsEnumEventValidatorBase& Rhs) = delete;
	TGsEnumEventValidatorBase& operator =(const TGsEnumEventValidatorBase& InRhs) = delete;
	
	TGsEnumEventValidatorBase(TOptional<TMap<TEnumEvent, TEnumState>>&& InEventAllowed) = delete;
	TGsEnumEventValidatorBase(const TOptional<TMap<TEnumEvent, TEnumState>>& InEventAllowed) = delete;

	virtual ~TGsEnumEventValidatorBase() {}

	// Logic Functions
public:
	virtual bool IsValidEventToState(TEnumEvent InEvent, TEnumState InState) const
	{
		if (_validatorMap.IsSet())
		{
			return _validatorMap->Contains(InEvent);
		}

		return false;
	}
	virtual bool IsValidStateToState(TEnumState InStateFlag, TEnumState InState) const
	{
		return EnumHasAnyFlags(InState, InStateFlag);
	}

	virtual void AddValidEventToState(TEnumEvent InEvent, TEnumState InState)
	{
		if (!_validatorMap.IsSet())
		{
			_validatorMap.Emplace();
		}

		if (!_validatorMap->Contains(InEvent))
		{
			_validatorMap->Emplace(InEvent, InState);
		}
	}

	virtual void RemoveValidEventToState(TEnumEvent InEvent, TEnumState InState)
	{
		if (!_validatorMap.IsSet()) 
		{
			return;
		}

		if (_validatorMap->Contains(InEvent))
		{
			_validatorMap->Remove(InEvent);
		}
	}

	// 필요시 구현
	virtual void GetValidStatesFromEvent(TEnumEvent InEvent, OUT TArray<TEnumState>& OutEventList) const {}
};


// 특정 Key 입력이 왔을 때, 특정 상황에 따라 해당 키 입력이 허용될 수 있는지 없는지를 체크한다
class FGsInputKeyEventValidator final : public TGsEnumEventValidatorBase<EGsKeyMappingType, EGsKeyMappingInputFlags>
{
private:
	// KeyMapping 과 flags 를 연결해주는 역할 -> _validatorMap 이용
	//TOptional<TMap<EGsKeyMappingType, EGsKeyMappingInputFlags>> _keyMappingToFlags;

public:
	FGsInputKeyEventValidator();

protected:
	void InitializeValidator();

public:
	// Key 입력 처리의 경우, State 로 Setting 을 사용한다
	
	// IsValidEventToState 의 경우 InState 에 현재 세팅 상황 (e.g. _blockInputFlag) 를 넣는다
	virtual bool IsValidEventToState(EGsKeyMappingType InEvent, EGsKeyMappingInputFlags InState) const /*override*/;

	// AddValidEventToSetting 의 경우 InSetting 에 특정 State (e.g. INPUT_LOCK_NPC) 를 넣는다
	virtual void AddValidEventToState(EGsKeyMappingType InEvent, EGsKeyMappingInputFlags InState) /*override*/;
};