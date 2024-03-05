/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityStat.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAbnormalityStatSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Abnormality/AbnormalityEffect/BSGsAbnormalityStatSet"))
struct DATACENTER_API FGsSchemaAbnormalityStatSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	AbnormalityEffectType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param"))
	TArray<FGsSchemaAbnormalityStat> param;
public:
	FGsSchemaAbnormalityStatSet();
	
protected:
	bool operator== (const FGsSchemaAbnormalityStatSet& __Other) const
	{
		return FGsSchemaAbnormalityStatSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAbnormalityStatSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAbnormalityStatSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAbnormalityStatSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAbnormalityStatSetRow() : Super()
	{
	}
	
	const FGsSchemaAbnormalityStatSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAbnormalityStatSet* Row = nullptr;
		return Table->FindRow<FGsSchemaAbnormalityStatSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAbnormalityStatSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAbnormalityStatSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAbnormalityStatSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Abnormality/AbnormalityEffect");
	}
	
};
