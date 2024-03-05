#pragma once

#include "EMonsterKnowledgeSlateCategory.generated.h"

// 2022/02/16 PKT - 몬스터 지식의 석판 카테고리
UENUM(BlueprintType)
enum class EMonsterKnowledgeSlateCategory : uint8
{
	ATTACK			UMETA(DisplayName = "ATTACK"),
	DEFENCE			UMETA(DisplayName = "DEFENCE"),
	HIT				UMETA(DisplayName = "HIT"),
	RESISTANCE		UMETA(DisplayName = "RESISTANCE"),
	SURVIVAL		UMETA(DisplayName = "SURVIVAL"),
	AMPLIFICATION	UMETA(DisplayName = "AMPLIFICATION"),

	MAX				UMETA(DisplayName = "Max"),
};
