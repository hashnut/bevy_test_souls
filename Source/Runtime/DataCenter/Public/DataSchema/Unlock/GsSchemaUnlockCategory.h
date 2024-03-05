/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Unlock/GsSchemaUnlockContents.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedUnlockEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaUnlockCategory.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Unlock/BSGsUnlockCategory"))
struct DATACENTER_API FGsSchemaUnlockCategory
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="unlockType"))
	UnlockCategory unlockType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="하위 컨텐츠", DisplayName="subContentsIdList"))
	TArray<FGsSchemaUnlockContentsRow> subContentsIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="lockDesc"))
	FText lockDesc;
public:
	FGsSchemaUnlockCategory();
	
protected:
	bool operator== (const FGsSchemaUnlockCategory& __Other) const
	{
		return FGsSchemaUnlockCategory::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaUnlockCategory::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaUnlockCategory::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaUnlockCategoryRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaUnlockCategoryRow() : Super()
	{
	}
	
	const FGsSchemaUnlockCategory* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaUnlockCategory* Row = nullptr;
		return Table->FindRow<FGsSchemaUnlockCategory>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaUnlockCategoryRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaUnlockCategoryRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableUnlockCategory final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<UnlockCategory, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Unlock");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaUnlockCategory*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaUnlockCategory>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->unlockType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const UnlockCategory& InId, OUT const FGsSchemaUnlockCategory*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const UnlockCategory& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaUnlockCategory* Temp;
		return !FindRowById(FGsSchemaUnlockCategory().unlockType, Temp);
	}
#endif
};
