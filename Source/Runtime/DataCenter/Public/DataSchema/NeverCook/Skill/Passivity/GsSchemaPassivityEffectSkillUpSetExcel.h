/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPassivityEffectSkillUpSetExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Skill/Passivity/BSGsPassivityEffectSkillUpSetExcel"))
struct DATACENTER_API FGsSchemaPassivityEffectSkillUpSetExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passivityEffectSkillUpSetGroupId"))
	int32 passivityEffectSkillUpSetGroupId;
public:
	FGsSchemaPassivityEffectSkillUpSetExcel();
	
protected:
	bool operator== (const FGsSchemaPassivityEffectSkillUpSetExcel& __Other) const
	{
		return FGsSchemaPassivityEffectSkillUpSetExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPassivityEffectSkillUpSetExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPassivityEffectSkillUpSetExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPassivityEffectSkillUpSetExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPassivityEffectSkillUpSetExcelRow() : Super()
	{
	}
	
	const FGsSchemaPassivityEffectSkillUpSetExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPassivityEffectSkillUpSetExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaPassivityEffectSkillUpSetExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPassivityEffectSkillUpSetExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPassivityEffectSkillUpSetExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePassivityEffectSkillUpSetExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Skill/Passivity");
	}
	
};
