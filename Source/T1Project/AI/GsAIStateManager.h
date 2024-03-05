#pragma once

#include "Classes/Container/GsSharedMap.h"
#include "Classes/GsStateMng.h"

#include "Data/GsAIDefine.h"
#include "DataSchema/GsSchemaEnums.h"

#include "GsAIBaseState.h"

class UGsGameObjectLocalPlayer;
class FGsInputEventMsgBase;
class FGsAITaskResolver;
class FGsAITask;
class FGsAIManager;
class FGsAIReserveHandler;

//----------------------------
// ai 상태 할당 관리자
//----------------------------

class FGsAIStateAllocator : TGsSharedMapAllocator <EGsAIActionType, FGsAIBaseState>
{
	// 생성자, 소멸자
public:
	FGsAIStateAllocator() {}
	virtual ~FGsAIStateAllocator() {}
	virtual FGsAIBaseState* Alloc(EGsAIActionType In_type) override;

};

//--------------------------------------
// ai 상태 매니저
//--------------------------------------
class FGsAIStateManager : public TGsStateMng<FGsAIBaseState, FGsAIStateAllocator>
{
	using Super = TGsStateMng<FGsAIBaseState, FGsAIStateAllocator>;

	// 멤버 변수들

	// 공용 변수
private:

	// ai 데이터
	const FGsAITaskResolver* _taskResolver;

	
	float _delayTime = 0.0f;
	int64 _delayStartTime = 0;
	bool _isWaitChangeState = false;

	float _waitRemakeListTime = 0.0f;
	int64 _waitStartRemakeListTime = 0;
	bool _isWaitRemakeList = false;

	bool _isDirtyRemakeList = false;
	// get by character
	FGsAIReserveHandler* _aiReserveHandler;

	// test
private:
	// 로그 출력할지
	bool _isShowLog = true;

	// 생성자 소멸자
public:
	virtual ~FGsAIStateManager() = default;

	// 가상함수
public: 
	virtual void ChangeState(TStateType In_state) override;

	// 로직 함수들
public:
	// 초기화(처음 한번만)
	void Initialize(FGsAIManager* In_aiManager);
	// 정리
	void Finalize();
	// 캐릭터 세팅
	void SetCharacter(UGsGameObjectLocalPlayer* In_char);
	// 캐릭터 삭제
	void RemoveCharacter();
	// 갱신
	void Update(float In_deltaTime);
	
public:
	// 전체 순회 조건 평가
	void EvaluateConditions();
	// 단일 컨디션 평가 후 전환
	bool EvaluateCondition(const FGsAITask* In_info);	

	bool MakeJobsByEvaluateConditions(FGsAIReserveHandler* In_aiReserveHandler, 
		bool In_isHighPriority);
	bool MakeJobByEvaluateCondition(const FGsAITask* In_info, FGsAIReserveHandler* In_aiReserveHandler,
		bool In_isHighPriority);
public:
	// 상태 초기화
	void ClearState();
	// 컨텐츠 변경에 따른 데이터 변경
	void ChangeContentsType(const FGsAITaskResolver* In_data);
	// 상태 멈춤
	void StopState();

	void ProcessReserveJob();

	void ChangeStateByCurrentReserveJob();

	// get, set
public:
	// 로그 출력할지 세팅
	void SetIsShowLog(bool In_val) { _isShowLog = In_val; }
	// 해당 상태인가
	bool IsState(EGsAIActionType In_type);

	const FGsAITask* FindTaskByActionType(EGsAIActionType In_actionType);

	FGsAIReserveHandler* GetAIReserveHandler()
	{
		return _aiReserveHandler;
	}
};
