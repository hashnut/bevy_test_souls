/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Unlock/GsSchemaUnlockContents.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSealPack.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="봉인팩", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Seal/BSGsSealPack"))
struct DATACENTER_API FGsSchemaSealPack
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="이름", DisplayName="sealPackName"))
	FText sealPackName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="봉인노드 연결 시 획득품 테이블ID", DisplayName="sealPackGainColorId"))
	int32 sealPackGainColorId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="잠금 최대 갯수", DisplayName="slotLockCountMax"))
	int32 slotLockCountMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="unlockContentsId"))
	FGsSchemaUnlockContentsRow unlockContentsId;
public:
	FGsSchemaSealPack();
	
protected:
	bool operator== (const FGsSchemaSealPack& __Other) const
	{
		return FGsSchemaSealPack::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSealPack::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSealPack::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSealPackRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSealPackRow() : Super()
	{
	}
	
	const FGsSchemaSealPack* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSealPack* Row = nullptr;
		return Table->FindRow<FGsSchemaSealPack>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSealPackRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSealPackRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSealPack final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Seal");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSealPack*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSealPack>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSealPack*& OutRow) const
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
		const FGsSchemaSealPack* Temp;
		return !FindRowById(FGsSchemaSealPack().id, Temp);
	}
#endif
};
