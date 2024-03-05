/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/InterServer/World/GsSchemaWorldData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaWorldGroupSiegeWarfare.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/InterServer/BSGsWorldGroupSiegeWarfare"))
struct DATACENTER_API FGsSchemaWorldGroupSiegeWarfare
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="worldGroupId"))
	int32 worldGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기획참고용", DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="worldGroupName"))
	FText worldGroupName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="worldList"))
	TArray<FGsSchemaWorldDataRow> worldList;
public:
	FGsSchemaWorldGroupSiegeWarfare();
	
protected:
	bool operator== (const FGsSchemaWorldGroupSiegeWarfare& __Other) const
	{
		return FGsSchemaWorldGroupSiegeWarfare::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaWorldGroupSiegeWarfare::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaWorldGroupSiegeWarfare::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaWorldGroupSiegeWarfareRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaWorldGroupSiegeWarfareRow() : Super()
	{
	}
	
	const FGsSchemaWorldGroupSiegeWarfare* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaWorldGroupSiegeWarfare* Row = nullptr;
		return Table->FindRow<FGsSchemaWorldGroupSiegeWarfare>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaWorldGroupSiegeWarfareRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaWorldGroupSiegeWarfareRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableWorldGroupSiegeWarfare final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("InterServer");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaWorldGroupSiegeWarfare*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaWorldGroupSiegeWarfare>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->worldGroupId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaWorldGroupSiegeWarfare*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int32& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaWorldGroupSiegeWarfare* Temp;
		return !FindRowById(FGsSchemaWorldGroupSiegeWarfare().worldGroupId, Temp);
	}
#endif
};
