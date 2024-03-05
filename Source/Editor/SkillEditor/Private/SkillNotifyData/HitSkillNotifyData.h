/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once

#include "CoreMinimal.h"

#include "Shared/Client/SharedEnums/SharedCombatEnum.h"
#include "SkillNotifyData/SkillNotifyDataBase.h"

#include "HitSkillNotifyData.generated.h"

UCLASS(BlueprintType)
class UHitSkillNotifyData : public USkillNotifyDataBase
{
	GENERATED_BODY()
		//SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "elementalType"))
	DamageElementalType elementalType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "skillPowerRate"))
	int32 skillPowerRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "applyReactionId"))
	int32 applyReactionId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "abnormalitId"))
	int32 abnormalitId;

public:
	UHitSkillNotifyData() {};
	virtual ~UHitSkillNotifyData() = default;
};
