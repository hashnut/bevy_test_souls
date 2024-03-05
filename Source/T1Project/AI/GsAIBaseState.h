#pragma once

#include "DataSchema/GsSchemaEnums.h"
#include "Classes/GsState.h"

//-----------------------------------
// ai 베이스 상태
//-----------------------------------

class UGsGameObjectLocalPlayer;
class FGsAIStateManager;
class FGsAIManager;

class FGsAIBaseState: public TGsState<EGsAIActionType>
{
	// 멤버
protected:
	FGsAIManager*				_aiManager = nullptr;
	FGsAIStateManager*			_aiStateManager = nullptr;
	UGsGameObjectLocalPlayer*	_targetPlayer = nullptr;


	// 생성자
public:
	FGsAIBaseState() : TGsState<EGsAIActionType>(EGsAIActionType::WAIT) {}
	explicit FGsAIBaseState(EGsAIActionType In_mode) : TGsState<EGsAIActionType>(In_mode) {}
	virtual ~FGsAIBaseState() = default;

	// 가상함수
public:
	virtual void Enter() override {}
	virtual void Exit() override {}
	virtual void Update(float In_deltaTime) override {}

	// 중간에 끊김, 초기화
	virtual void Stop() {}

	virtual bool IsHighPriority() { return false; }

	// 로직 함수
public:
	// 오브젝트 상태 idle로 만든다
	void SetObjStateIdle();
	// 이동 정지
	void SetMovementStop();
	// 캐릭터 삭제
	void RemoveCharacter();
	// get, set
public:
	void SetTarget(UGsGameObjectLocalPlayer* In_target);
	void SetManager(FGsAIManager* In_aiMng, FGsAIStateManager* In_aiStateMng);
	// 플레이어가 이동 가능한가
	bool GetMoveAble();
};
