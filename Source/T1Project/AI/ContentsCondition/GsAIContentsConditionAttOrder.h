#pragma once
#include "CoreMinimal.h"
#include "Message/GsMessageGameObject.h"

//-------------------------------
// ai 컨텐츠 조건: 공격 우선 순위
// 제자리 상속 받음(타겟 변경 필요한지 체크 함수(IsNeedTargetChange)에서 제자리 쪽 데이터 필요)
//-------------------------------

class UGsGameObjectBase;
class UGsGameObjectLocalPlayer;
class FGsAIManager;

class FGsAIContentsConditionAttOrder
{
	// 멤버 변수
private:
	// 공격 우선순위 있는가?(타겟)
	//bool _isUseAttOrder = false;
	// 우선순위 높은 특정 id npc(테스트로 엘더 우드)(타겟)
	int _highOrderNPCId = 11000;
	// 시야반경 제곱값(타겟)
	float _squaredPlayerSearchTargetRange;

	// 참조 데이터
protected:
	// 플레이어
	UGsGameObjectLocalPlayer* _localPlayer;
	FGsAIManager* _aiManager;

	// 메시지 바인딩 해제용
private:
	// 공격 순서 메시지
	//MsgGameObjHandle _delegateUseAttOrder;
	
	// 생성자, 소멸자
public:
	~FGsAIContentsConditionAttOrder();

	// 로직함수
public:
	// 초기화
	void Initialize(FGsAIManager* In_mng);
	// 해제
	void Finalize();
	void SetCharacter(UGsGameObjectLocalPlayer* In_player);
	void RemoveCharacter();

	// 메시지 바인딩
public:
	// 공격 우선순위 on, off(타겟)
	//void OnUseAttOrder(const struct IGsMessageParam*  In_isActive);

	// 체크함수
public: 
	// 타겟 변경 필요한지(타겟)
	//bool IsNeedTargetChange();

	// get, set
public:
	// 공격 우선순위 있는지 세팅(타겟)
	//void SetIsUseAttOrder(bool In_val) { _isUseAttOrder = In_val; }
	// 플레이어 서치 범위 제곲값(타겟)
	float GetSquaredPlayerSearchTargetRange() { return _squaredPlayerSearchTargetRange; }
	// 공격 우선순위 있는지?(타겟)
	//bool GetIsUseAttOrder() { return _isUseAttOrder; }
	// 우선순위 높은 npc id(타겟)
	int GetHighOrderNPCId() { return _highOrderNPCId; }
	// 정렬 함수 가져오기(타겟)
	//TFunction<bool(const TPair<UGsGameObjectBase*, float> & lhs, const TPair<UGsGameObjectBase*, float > & rhs)>
	//	GetFindObjSortFunc();
};