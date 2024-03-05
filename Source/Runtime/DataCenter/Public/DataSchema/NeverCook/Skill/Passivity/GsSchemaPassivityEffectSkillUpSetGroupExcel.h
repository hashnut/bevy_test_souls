/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPassivityEffectSkillUpSetGroupExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Skill/Passivity/BSGsPassivityEffectSkillUpSetGroupExcel"))
struct DATACENTER_API FGsSchemaPassivityEffectSkillUpSetGroupExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passivityEffectSkillUpSetGroupId"))
	int32 passivityEffectSkillUpSetGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="userSkillSetId"))
	int32 userSkillSetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="value"))
	int32 value;
public:
	FGsSchemaPassivityEffectSkillUpSetGroupExcel();
	
protected:
	bool operator== (const FGsSchemaPassivityEffectSkillUpSetGroupExcel& __Other) const
	{
		return FGsSchemaPassivityEffectSkillUpSetGroupExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPassivityEffectSkillUpSetGroupExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPassivityEffectSkillUpSetGroupExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPassivityEffectSkillUpSetGroupExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPassivityEffectSkillUpSetGroupExcelRow() : Super()
	{
	}
	
	const FGsSchemaPassivityEffectSkillUpSetGroupExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPassivityEffectSkillUpSetGroupExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaPassivityEffectSkillUpSetGroupExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPassivityEffectSkillUpSetGroupExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPassivityEffectSkillUpSetGroupExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePassivityEffectSkillUpSetGroupExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Skill/Passivity");
	}
	
};
