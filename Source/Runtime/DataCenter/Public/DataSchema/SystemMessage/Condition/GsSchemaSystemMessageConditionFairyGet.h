/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/SystemMessage/Condition/GsSchemaSendingOption.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSystemMessageConditionFairyGet.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="Fairy 획득 System Message condition table", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SystemMessage/Condition/BSGsSystemMessageConditionFairyGet"))
struct DATACENTER_API FGsSchemaSystemMessageConditionFairyGet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="minFairyGrade"))
	FairyGrade minFairyGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="exceptGradeList"))
	TArray<FairyGrade> exceptGradeList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="sendingOption"))
	FGsSchemaSendingOption sendingOption;
public:
	FGsSchemaSystemMessageConditionFairyGet();
	
protected:
	bool operator== (const FGsSchemaSystemMessageConditionFairyGet& __Other) const
	{
		return FGsSchemaSystemMessageConditionFairyGet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSystemMessageConditionFairyGet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSystemMessageConditionFairyGet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSystemMessageConditionFairyGetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSystemMessageConditionFairyGetRow() : Super()
	{
	}
	
	const FGsSchemaSystemMessageConditionFairyGet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSystemMessageConditionFairyGet* Row = nullptr;
		return Table->FindRow<FGsSchemaSystemMessageConditionFairyGet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSystemMessageConditionFairyGetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSystemMessageConditionFairyGetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSystemMessageConditionFairyGet final : public UGsTable
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
		TMap<FName, const FGsSchemaSystemMessageConditionFairyGet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSystemMessageConditionFairyGet>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSystemMessageConditionFairyGet*& OutRow) const
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
		const FGsSchemaSystemMessageConditionFairyGet* Temp;
		return !FindRowById(FGsSchemaSystemMessageConditionFairyGet().id, Temp);
	}
#endif
};
