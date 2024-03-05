/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityCrowdControl.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAbnormalityCrowdControlSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Abnormality/AbnormalityEffect/BSGsAbnormalityCrowdControlSet"))
struct DATACENTER_API FGsSchemaAbnormalityCrowdControlSet
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
	FGsSchemaAbnormalityCrowdControl param;
public:
	FGsSchemaAbnormalityCrowdControlSet();
	
protected:
	bool operator== (const FGsSchemaAbnormalityCrowdControlSet& __Other) const
	{
		return FGsSchemaAbnormalityCrowdControlSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAbnormalityCrowdControlSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAbnormalityCrowdControlSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAbnormalityCrowdControlSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAbnormalityCrowdControlSetRow() : Super()
	{
	}
	
	const FGsSchemaAbnormalityCrowdControlSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAbnormalityCrowdControlSet* Row = nullptr;
		return Table->FindRow<FGsSchemaAbnormalityCrowdControlSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAbnormalityCrowdControlSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAbnormalityCrowdControlSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAbnormalityCrowdControlSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Abnormality/AbnormalityEffect");
	}
	
};
