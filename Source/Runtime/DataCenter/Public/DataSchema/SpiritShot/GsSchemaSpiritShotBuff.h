/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpiritShotBuff.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SpiritShot/BSGsSpiritShotBuff"))
struct DATACENTER_API FGsSchemaSpiritShotBuff
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="keyType"))
	PassivityOwnerType keyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="descText"))
	FText descText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="iconPath"))
	FSoftObjectPath iconPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="smallIconPath"))
	FSoftObjectPath smallIconPath;
public:
	FGsSchemaSpiritShotBuff();
	
protected:
	bool operator== (const FGsSchemaSpiritShotBuff& __Other) const
	{
		return FGsSchemaSpiritShotBuff::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSpiritShotBuff::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSpiritShotBuff::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSpiritShotBuffRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSpiritShotBuffRow() : Super()
	{
	}
	
	const FGsSchemaSpiritShotBuff* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSpiritShotBuff* Row = nullptr;
		return Table->FindRow<FGsSchemaSpiritShotBuff>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSpiritShotBuffRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSpiritShotBuffRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSpiritShotBuff final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<PassivityOwnerType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SpiritShot");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSpiritShotBuff*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSpiritShotBuff>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->keyType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const PassivityOwnerType& InId, OUT const FGsSchemaSpiritShotBuff*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const PassivityOwnerType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaSpiritShotBuff* Temp;
		return !FindRowById(FGsSchemaSpiritShotBuff().keyType, Temp);
	}
#endif
};
