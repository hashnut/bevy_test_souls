#pragma once
#include "CoreMinimal.h"

#include "Summon/GsSummonDefine.h"

#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"

// 뽑기 함수
class FGsSummonFunc
{
public:
	// 소환 대기 카운트 넘어갔는지 체크
	static bool CheckSummonWaitCount(EGsSummonType In_summonType);

	// 페어리 소환 확정 대기 카운트 체크
	static bool CheckFairySummonWaitCount(const bool InShowPopup = false, const bool InCheckOnly = false);
	// 코스튬 소환 확정 대기 카운트 체크
	static bool CheckCostumeSummonWaitCount(const bool InShowPopup = false, const bool InCheckOnly = false);
	// 페어리 등급 텍스트 구하기
	static FText GetFairyGradeText(FairyGrade In_fairyGrade);
	// 코스튬 등급 텍스트 구하기
	static FText GetCostumeGradeText(CostumeGrade In_costumeGrade);

	//https://jira.com2us.com/jira/browse/C2URWQ-5176
	// 소환 stack 갯수(9999)개 넘어 갔는지 체크
	static bool CheckSummonStackCount(EGsSummonType In_summonType);

	// 페어리 소환 stack 갯수(9999)개 넘어 갔는지 체크
	static bool CheckFairySummonStackCount(const bool InShowPopup = false, const bool InCheckOnly = false);
	// 코스튬 소환 stack 갯수(9999)개 넘어 갔는지 체크
	static bool CheckCostumeSummonStackCount(const bool InShowPopup = false, const bool InCheckOnly = false);
};