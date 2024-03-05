#pragma once

#include "CoreMinimal.h"

/*
* ai 공용 함수
*/

class UGsGameObjectBase;
class UGsGameObjectLocalPlayer;
class FGsInteractionHandlerLocalPlayer;

class FGsAIFunc
{
public:
	// 현재 사용할 skill id 얻어오기
	static const struct FGsSchemaSkillSet* FindNowUseSkillID(UGsGameObjectLocalPlayer* In_player);

	// 지금 사용가능한 스킬있는가(평타뺴고)
	static bool HasNowUseSkill(UGsGameObjectLocalPlayer* In_player);
	// 일반 스킬(평타) id 가져오기
	static int GetNormalSkillId();
	// 다음 자동 스킬 범위
	static float GetNextAutoSkillRange(UGsGameObjectLocalPlayer* In_player);
	// npc function slot move
	static void StartMoveNPCFunctionSlot(int In_tblId);

	static void PostAutoMoveRequetedNPCFunction();

	static bool IsNPCMoveTarget(
		UGsGameObjectBase* In_localBase, TArray<int> In_arrayNpcid, bool In_isCheckFSM = true);
};