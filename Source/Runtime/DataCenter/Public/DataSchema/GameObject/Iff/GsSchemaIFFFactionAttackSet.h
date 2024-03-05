/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedIffEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaIFFFactionAttackSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Iff/BSGsIFFFactionAttackSet"))
struct DATACENTER_API FGsSchemaIFFFactionAttackSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="iffFactionType"))
	IffFactionType iffFactionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="enemyFactionTypeList"))
	TArray<IffFactionType> enemyFactionTypeList;
public:
	FGsSchemaIFFFactionAttackSet();
	
protected:
	bool operator== (const FGsSchemaIFFFactionAttackSet& __Other) const
	{
		return FGsSchemaIFFFactionAttackSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaIFFFactionAttackSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaIFFFactionAttackSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaIFFFactionAttackSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaIFFFactionAttackSetRow() : Super()
	{
	}
	
	const FGsSchemaIFFFactionAttackSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaIFFFactionAttackSet* Row = nullptr;
		return Table->FindRow<FGsSchemaIFFFactionAttackSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaIFFFactionAttackSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaIFFFactionAttackSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableIFFFactionAttackSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject/Iff");
	}
	
};
