#pragma once

#include "CoreMinimal.h"

#include "Message/GsMessageContentHud.h"



/*
* 로비 스킬 ui 핸들러
*/

class FGsLobbySkillUIHandler
{
private:
	// 메시지 바인딩(해제용)
	MsgHudHandleArray _msgContentHudHandleList;

	int _slotIndex = 0;

	// 메시지
public:
	void OnActiveSkill(int32 InSlotId);

	// 로직 함수
public:
	// 초기화(최초 한번)
	void Init();
	// 정리(마지막 한번)
	void Close();
};