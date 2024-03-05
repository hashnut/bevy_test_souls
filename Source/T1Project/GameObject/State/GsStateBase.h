// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <mutex>

#include "GsGameObjectStateManager.h"

/**
 * State 객체의 기본이 되는 Abstract 클래스 
 * 상태 클래스는 Static(Singleton) 클래스 이므로 GameObject 내부 로직 처리는 꼭 UGsGameObjectBase 클래스 안에서 처리
 */
class T1PROJECT_API IGsStateBase
{
public:
	IGsStateBase();
	virtual ~IGsStateBase();

public:
	// 체인지 가능 or 불가능(Black List | White List) 상태 정의
	virtual bool CanTransition(UGsGameObjectBase* Owner, int StateID) const;

public:
	// 이단계에서는 실제 상태 진입전에 특정 상황 판단에 의해 다른 상태로의 메세지를 던진다.
	virtual bool Ready(UGsGameObjectBase* Owner);
	virtual void Enter(UGsGameObjectBase* Owner);
	virtual void ReEnter(UGsGameObjectBase* Owner);
	virtual void Update(UGsGameObjectBase* Owner, float Delta);
	virtual void Exit(UGsGameObjectBase* Owner);

	// IGsStateBase* ChangeState 로 치환되어 무시되었음 
	virtual void IgnoreState(UGsGameObjectBase* Owner, IGsStateBase* ChangeState);

public:
	virtual IGsStateBase* GetState(uint8 StateID) const = 0;
	virtual uint8 GetStateID() const { return 0; }
	virtual FName GetStateName() const = 0;
	virtual FName GetBPStateName() const = 0;

	virtual bool IsSameState(uint8 StateID) const;
	virtual bool IsSameState(EGsStateBase StateType) const;
	bool IsMontagePlayState() const;

	// 상태 전환시 기존 몽타주가 플레이 되고있다면 강제 Stop 처리 확인
	virtual bool AllowMotionStop() const { return true; }

protected:
	// animBP 스테이트 머신의 디폴트 상태 정의
	static uint8 GetDefaultStateId(UGsGameObjectBase* Owner)
	{
		return static_cast<uint8>(EGsStateBase::Idle);
	}

public:
	virtual int GetAniRandomCount() const { return 0; }
};

/**
* State Singleton 
*/
template <typename T>
class T1PROJECT_API TGsStateSingleton
{
protected:
	TGsStateSingleton() = default;
	//StateSingleton(const StateSingleton<T>& rhs) = delete;
	//StateSingleton<T>& operator = (const StateSingleton<T>& rhs) = delete;

private:
	static TUniquePtr<T> _instance;
	//static std::once_flag _flag1;

public:
	static T* GetInstance() 
	{
		if(false == _instance.IsValid())
		{
			_instance = TUniquePtr<T>(new T());
		}
		return _instance.Get();
	}
};

template <typename T> TUniquePtr<T> TGsStateSingleton<T>::_instance;

// 공통적으로 묶여질 상태의 그룹을 정의

// 이동 관련 그룹
#define GS_STATE_GROUP_MOVE \
case EGsStateBase::ForwardWalk: \
case EGsStateBase::Run: \
case EGsStateBase::AutoMove \

// 특정 행위 종료 단계
#define GS_STATE_GROUP_ACTION_END \
case EGsStateBase::AttackEnd: \
case EGsStateBase::ReactionEnd: \
case EGsStateBase::CommonActionEnd: \
case EGsStateBase::InteractionEnd: \
case EGsStateBase::ChangeWeaponEnd \

// 특정 행위
#define GS_STATE_GROUP_ACTION \
case EGsStateBase::Casting: \
case EGsStateBase::Attack: \
case EGsStateBase::Interaction: \
case EGsStateBase::Looting: \
case EGsStateBase::SocialAction \

// 상태이상 (CC기) 관련 그룹
#define GS_STATE_GROUP_ABNORLITY \
case EGsStateBase::Freeze: \
case EGsStateBase::Reaction \

// 죽음 관련 그룹
#define GS_STATE_GROUP_DIE \
case EGsStateBase::Dying: \
case EGsStateBase::Dead: \
case EGsStateBase::RunAway \

// 상속 받는 상태 클래스 내부 오버라이드 함수 메크로 정의
#define GS_STATE_GENERATED_BODY(TSuperClass) \
private: \
	using Super = TSuperClass; \
\
public:\
	virtual bool CanTransition(UGsGameObjectBase* Owner, int StateID) const override;\
\
public:\
	virtual void Enter(UGsGameObjectBase* Owner) override; \
	virtual void Exit(UGsGameObjectBase* Owner) override; \
\
public: \
	virtual uint8 GetStateID() const override; \
	virtual FName GetStateName() const override; \
	virtual FName GetBPStateName() const override \

// Cpp 복붙용
//uint8 TCalss::GetStateID() const
//{
//	return static_cast<uint8>(EGsStateBase::);
//}
//FName TCalss::GetStateName() const
//{
//	return TEXT("");
//}
//FName TCalss::GetBPStateName() const
//{
//	return TEXT("");
//}
//
//bool TCalss::CanTransition(UGsGameObjectBase* Owner, int StateID) const
//{
//	if (Super::CanTransition(Owner, StateID))
//	{
//		switch (static_cast<EGsStateBase>(StateID))
//		{
//		default:
//			return false;
//		}
//	}
//	return false;
//}
//void TCalss::Enter(UGsGameObjectBase* Owner)
//{
//	Super::Enter(Owner);
//}
//
//void TCalss::Exit(UGsGameObjectBase* Owner)
//{
//	Super::Exit(Owner);
//}
