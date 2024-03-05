#pragma once

#include "CoreMinimal.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageSystem.h"
/*
* 락온 핸들러
*/

struct IGsMessageParam;
class UGsGameObjectBase;

class FGsLockOnHandler
{
private:
	// game object 메세지 해제용
	MsgGameObjHandleArray _gameObjectMsg;
	// system 메세지 해제용
	MsgSystemHandleArray _systemMsg;

	// 메시지 수신
public:
	

	// 로직 함수
public:
	// 초기화
	void Init();
	// 종료
	void Close();
	// 락온 타게팅
	void SetLockOnTarget(UGsGameObjectBase* In_target, bool In_isCallByScanList = false);
};