/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/SystemMessage/Condition/GsSchemaSendingOption.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSystemMessageConditionCostumeGet.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="Costume 획득 System Message condition table", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SystemMessage/Condition/BSGsSystemMessageConditionCostumeGet"))
struct DATACENTER_API FGsSchemaSystemMessageConditionCostumeGet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="minCostumeGrade"))
	CostumeGrade minCostumeGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="exceptGradeList"))
	TArray<CostumeGrade> exceptGradeList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="sendingOption"))
	FGsSchemaSendingOption sendingOption;
public:
	FGsSchemaSystemMessageConditionCostumeGet();
	
protected:
	bool operator== (const FGsSchemaSystemMessageConditionCostumeGet& __Other) const
	{
		return FGsSchemaSystemMessageConditionCostumeGet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSystemMessageConditionCostumeGet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSystemMessageConditionCostumeGet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSystemMessageConditionCostumeGetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSystemMessageConditionCostumeGetRow() : Super()
	{
	}
	
	const FGsSchemaSystemMessageConditionCostumeGet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSystemMessageConditionCostumeGet* Row = nullptr;
		return Table->FindRow<FGsSchemaSystemMessageConditionCostumeGet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSystemMessageConditionCostumeGetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSystemMessageConditionCostumeGetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSystemMessageConditionCostumeGet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SystemMessage/Condition");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSystemMessageConditionCostumeGet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSystemMessageConditionCostumeGet>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaSystemMessageConditionCostumeGet*& OutRow) const
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
		const FGsSchemaSystemMessageConditionCostumeGet* Temp;
		return !FindRowById(FGsSchemaSystemMessageConditionCostumeGet().id, Temp);
	}
#endif
};
