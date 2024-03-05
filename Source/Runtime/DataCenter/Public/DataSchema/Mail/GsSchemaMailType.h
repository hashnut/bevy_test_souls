/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Mail/GsMailCategoryMain.h"
#include "Runtime/DataCenter/Public/Mail/GsMailCategorySub.h"
#include "Runtime/DataCenter/Public/DataSchema/Mail/GsSchemaMailStringData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMailType.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Mail/BSGsMailType"))
struct DATACENTER_API FGsSchemaMailType
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="categoryMain"))
	FGsMailCategoryMain categoryMain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="categorySub"))
	FGsMailCategorySub categorySub;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="expiredTimeH"))
	int32 expiredTimeH;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mailStringId"))
	FGsSchemaMailStringDataRow mailStringId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isLimitedQuantity"))
	bool isLimitedQuantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc"))
	FString desc;
public:
	FGsSchemaMailType();
	
protected:
	bool operator== (const FGsSchemaMailType& __Other) const
	{
		return FGsSchemaMailType::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMailType::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMailType::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMailTypeRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMailTypeRow() : Super()
	{
	}
	
	const FGsSchemaMailType* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMailType* Row = nullptr;
		return Table->FindRow<FGsSchemaMailType>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMailTypeRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMailTypeRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMailType final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Mail");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaMailType*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaMailType>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaMailType*& OutRow) const
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
		const FGsSchemaMailType* Temp;
		return !FindRowById(FGsSchemaMailType().id, Temp);
	}
#endif
};
