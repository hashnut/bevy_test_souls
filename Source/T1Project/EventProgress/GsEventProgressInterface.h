#pragma once
#include "CoreMinimal.h"
#include "EventProgress/GsEventProgressDefine.h"
/*
	이벤트 진행 처리 인터페이스
	각 이벤트들을 실행하는 곳
*/

class IGsEventProgressInterface
{
	// 소멸자
public:
	virtual ~IGsEventProgressInterface() = default;

	// 가상 함수
public:
	// 이벤트 시작
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) {}
	// 이벤트 종료
	virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) {}
	
	// 이벤트 시작을 할 수 없을 때
	virtual void OnCanceledStartEvent() {}
};