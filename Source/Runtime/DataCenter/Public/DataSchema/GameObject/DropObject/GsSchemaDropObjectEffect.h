/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsSchemaDropObjectEffect.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/DropObject/BSGsDropObjectEffect"))
struct DATACENTER_API FGsSchemaDropObjectEffect
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id", Tooltip))
	ItemGrade id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="particle", Tooltip))
	FSoftObjectPath particle;
	
public:
	FGsSchemaDropObjectEffect();
	
protected:
	bool operator== (const FGsSchemaDropObjectEffect& __Other) const
	{
		return FGsSchemaDropObjectEffect::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaDropObjectEffect::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaDropObjectEffect::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaDropObjectEffectRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaDropObjectEffectRow() : Super()
	{
	}
	
	const FGsSchemaDropObjectEffect* GetRow() const
	{
		const FGsSchemaDropObjectEffect* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaDropObjectEffect>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaDropObjectEffectRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaDropObjectEffectRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableDropObjectEffect final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<ItemGrade, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject/DropObject");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaDropObjectEffect*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaDropObjectEffect>(RowsMap))
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
	bool FindRowById(const ItemGrade& InId, OUT const FGsSchemaDropObjectEffect*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const ItemGrade& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaDropObjectEffect* Temp;
		return !FindRowById(FGsSchemaDropObjectEffect().id, Temp);
	}
#endif
};
