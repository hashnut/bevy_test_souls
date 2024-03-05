/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillNotifyAoeVarySetData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/BSGsSkillNotifyAoeVarySetData"))
struct DATACENTER_API FGsSchemaSkillNotifyAoeVarySetData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="aoeVaryId"))
	int32 aoeVaryId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기본 값: 1, (맞는 대상이 0 명일 때는 없기 때문에 0 보다 커야 합니다)", DisplayName="targetCount"))
	int32 targetCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="damageRate"))
	int32 damageRate;
public:
	FGsSchemaSkillNotifyAoeVarySetData();
	
protected:
	bool operator== (const FGsSchemaSkillNotifyAoeVarySetData& __Other) const
	{
		return FGsSchemaSkillNotifyAoeVarySetData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillNotifyAoeVarySetData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillNotifyAoeVarySetData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillNotifyAoeVarySetDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillNotifyAoeVarySetDataRow() : Super()
	{
	}
	
	const FGsSchemaSkillNotifyAoeVarySetData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillNotifyAoeVarySetData* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillNotifyAoeVarySetData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillNotifyAoeVarySetDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillNotifyAoeVarySetDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillNotifyAoeVarySetData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill");
	}
	
};
