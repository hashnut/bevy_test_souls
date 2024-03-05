#pragma once
#include "CoreMinimal.h"
#include "GsInputBlockDefine.h"
#include "Message/GsMessageGameObject.h"

/*
	특정 패킷 서버 전송시
	응답 오기전까지 막는 처리하는 곳
*/
struct IGsMessageParam;

class FGsInputBlockHandler
{
private:
	// 조건(한개라도 있으면 막아야함)
	TSet<EGsInputBlockConditionType> _setCondition;
	// game object 메세지 해제용
	MsgGameObjHandleArray _gameObjectMsg;

	// 메시지
	// 로컬 스폰 끝
	void OnLocalPlayerSpawnComplete(const IGsMessageParam*);

	// 로직 함수
public:
	// 초기화
	void Initialize();
	// 정리
	void Finalize();
	// 조건 정리
	void ClearCondition();
	
	// 조건 등록(막는 ui 켜야함)
	void AddBlockConditions(TArray<EGsInputBlockConditionType> In_conditions);
	// 조건 해제(set에서 빼고 크기가 0이면 막는 ui 꺼야함)
	void DeleteBlockCondition(EGsInputBlockConditionType In_condition);

	// get
public:
	// 잠겨있나
	bool IsBlocked();
};