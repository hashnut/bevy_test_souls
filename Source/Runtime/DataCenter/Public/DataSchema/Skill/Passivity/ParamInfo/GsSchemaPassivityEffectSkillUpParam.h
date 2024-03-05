/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPassivityEffectSkillUpParam.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Passivity/ParamInfo/BSGsPassivityEffectSkillUpParam"))
struct DATACENTER_API FGsSchemaPassivityEffectSkillUpParam
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="userSkillSetId", Tooltip="참조할 user Skill Set Id (UserSkillSet 데이터시트 참조)"))
	int32 userSkillSetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="value", Tooltip="레벨 상승치"))
	int32 value;
public:
	FGsSchemaPassivityEffectSkillUpParam();
	
protected:
	bool operator== (const FGsSchemaPassivityEffectSkillUpParam& __Other) const
	{
		return FGsSchemaPassivityEffectSkillUpParam::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
