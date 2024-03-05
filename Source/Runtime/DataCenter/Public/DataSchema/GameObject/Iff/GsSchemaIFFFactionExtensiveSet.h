/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedIffEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaIFFFactionExtensiveSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Iff/BSGsIFFFactionExtensiveSet"))
struct DATACENTER_API FGsSchemaIFFFactionExtensiveSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="iffFactionType"))
	IffFactionType iffFactionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enemyFactionTypeList"))
	TArray<IffFactionType> enemyFactionTypeList;
public:
	FGsSchemaIFFFactionExtensiveSet();
	
protected:
	bool operator== (const FGsSchemaIFFFactionExtensiveSet& __Other) const
	{
		return FGsSchemaIFFFactionExtensiveSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaIFFFactionExtensiveSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaIFFFactionExtensiveSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaIFFFactionExtensiveSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaIFFFactionExtensiveSetRow() : Super()
	{
	}
	
	const FGsSchemaIFFFactionExtensiveSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaIFFFactionExtensiveSet* Row = nullptr;
		return Table->FindRow<FGsSchemaIFFFactionExtensiveSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaIFFFactionExtensiveSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaIFFFactionExtensiveSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableIFFFactionExtensiveSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject/Iff");
	}
	
};
