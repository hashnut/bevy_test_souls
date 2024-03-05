#pragma once

#include "Map/GsMapIconType.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

struct FGsSchemaNpcFunctionData;

class UUserWidget;

/*
 미니맵 스태틱 함수
*/
class FGsMinimapFunc
{
public:
	// npc 테이블의 function 타입으로 미니맵 아이콘 타입 구하는것
	static EGsMapIconType GetMapIconTypeByNpcFunctionType(NpcFunctionType In_npcFunctionType);
	// npc balance 테이블의 rank 타입으로 미니맵 아이콘 타입 구하는것
	static EGsMapIconType GetMapIconTypeByCreatureRankType(
		CreatureRankType In_rankType, int In_npcTblId);

	// npc function data 접근
	static const  FGsSchemaNpcFunctionData* GetNpcFunctionData(NpcFunctionType InType);
	// 파티 인지(아이콘 타입도 가져오기)
	static bool GetPartyIconType(int64 In_gameId, EGsMapIconType& Out_iconType);

	// check remote iff, red or blue icon
	static void GetRemoteIffIconType(int64 In_gameId, EGsMapIconType& Out_iconType);
	// 방향 표시 위젯 만들기
	static UUserWidget* MakeArrowWidget();
	// 클래스로 위젯 생성
	static UUserWidget* Claim(UClass* In_class);
	// 위젯 해제
	static void ReleaseWidget(UUserWidget* In_widget);
};