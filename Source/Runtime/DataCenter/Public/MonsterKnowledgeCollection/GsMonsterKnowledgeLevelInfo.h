#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsMonsterKnowledgeLevelInfo.generated.h"


USTRUCT(BlueprintType)
struct DATACENTER_API FGsMonsterKnowledgeLevelInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int32 level = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterKnowledgeExp")
	int32 levelUpExpRequired = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "monsterBookPoint")
	int32 monsterBookPoint = 0;
};