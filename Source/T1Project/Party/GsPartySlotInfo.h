#pragma once
#include "CoreMinimal.h"

/*
	파티 슬롯 정보
*/
class IGsPartySlotInfo
{
public:
	virtual ~IGsPartySlotInfo() = default;

public:
	// 이름 구하기
	virtual const FString& GetName() = 0;
	// 파티 멤버인지
	virtual bool GetIsMember() = 0;
};