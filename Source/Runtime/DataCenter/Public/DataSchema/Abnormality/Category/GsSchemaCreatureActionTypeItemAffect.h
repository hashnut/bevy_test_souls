/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCreatureActionTypeItemAffect.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="상태이상 일때 사용 불가능한 아이템 타입", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Abnormality/Category/BSGsCreatureActionTypeItemAffect"))
struct DATACENTER_API FGsSchemaCreatureActionTypeItemAffect
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupId"))
	int32 groupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="affectItemSubTypeList"))
	TArray<ItemCategorySub> affectItemSubTypeList;
public:
	FGsSchemaCreatureActionTypeItemAffect();
	
protected:
	bool operator== (const FGsSchemaCreatureActionTypeItemAffect& __Other) const
	{
		return FGsSchemaCreatureActionTypeItemAffect::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCreatureActionTypeItemAffect::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCreatureActionTypeItemAffect::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCreatureActionTypeItemAffectRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCreatureActionTypeItemAffectRow() : Super()
	{
	}
	
	const FGsSchemaCreatureActionTypeItemAffect* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCreatureActionTypeItemAffect* Row = nullptr;
		return Table->FindRow<FGsSchemaCreatureActionTypeItemAffect>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCreatureActionTypeItemAffectRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCreatureActionTypeItemAffectRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCreatureActionTypeItemAffect final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Abnormality/Category");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCreatureActionTypeItemAffect*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCreatureActionTypeItemAffect>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->groupId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaCreatureActionTypeItemAffect*& OutRow) const
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
		const FGsSchemaCreatureActionTypeItemAffect* Temp;
		return !FindRowById(FGsSchemaCreatureActionTypeItemAffect().groupId, Temp);
	}
#endif
};
