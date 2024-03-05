#pragma once

#include "CoreMinimal.h"

/*
	인터랙션 npc 정보
*/
struct FGsSchemaNpcData;

class FGsInteractionNpcInfo
{
	// 멤버 변수
public:
	int _npcId;
	FVector _pos;

	// 생성자
public:
	FGsInteractionNpcInfo(int In_id, FVector In_pos) :
		_npcId(In_id),
		_pos(In_pos)
	{
	}
};

// 마을 interaction npc 정보
class FGsTownInteractionNpcInfo
{
	// 멤버 변수
public:
	int _npcId;
	FVector _pos;
	// npc tbl
	const FGsSchemaNpcData* _npcData;
	
	// 생성자
public:
	FGsTownInteractionNpcInfo(int In_id, FVector In_pos,
		const FGsSchemaNpcData* In_data) :
		_npcId(In_id),
		_pos(In_pos),
		_npcData(In_data)
	{
	}

public:
	void Finalize()
	{
		_npcData = nullptr;
	}
};