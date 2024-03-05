/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Mail/EGsMailStringParamterType.h"
#include "GsSchemaMailStringData.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="메일 스트링 데이블입니다.", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Mail/BSGsMailStringData"))
struct DATACENTER_API FGsSchemaMailStringData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="id", Tooltip="서버키와 매핑되는 값"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="title", Tooltip="제목"))
	FText title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="content", Tooltip="내용"))
	FText content;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="titleParamterType"))
	TArray<EGsMailStringParamterType> titleParamterType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="contentParamterType"))
	TArray<EGsMailStringParamterType> contentParamterType;
public:
	FGsSchemaMailStringData();
	
protected:
	bool operator== (const FGsSchemaMailStringData& __Other) const
	{
		return FGsSchemaMailStringData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMailStringData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMailStringData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMailStringDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMailStringDataRow() : Super()
	{
	}
	
	const FGsSchemaMailStringData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMailStringData* Row = nullptr;
		return Table->FindRow<FGsSchemaMailStringData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMailStringDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMailStringDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMailStringData final : public UGsTable
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
		TMap<FName, const FGsSchemaMailStringData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaMailStringData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaMailStringData*& OutRow) const
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
		const FGsSchemaMailStringData* Temp;
		return !FindRowById(FGsSchemaMailStringData().id, Temp);
	}
#endif
};
