/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/EventProgress/GsEventProgressDefine.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaEventProgress.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/EventProgress/BSGsEventProgress"))
struct DATACENTER_API FGsSchemaEventProgress
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_type"))
	EGsEventProgressType _type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_order"))
	int32 _order;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_lowerOrder"))
	EGsEventProgressWorkType _lowerOrder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_higherOrder"))
	EGsEventProgressWorkType _higherOrder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_makeBackupTypes"))
	TArray<EGsEventProgressType> _makeBackupTypes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_isDirect"))
	bool _isDirect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_forcedWaitCurrentTypes"))
	TArray<EGsEventProgressType> _forcedWaitCurrentTypes;
public:
	FGsSchemaEventProgress();
	
protected:
	bool operator== (const FGsSchemaEventProgress& __Other) const
	{
		return FGsSchemaEventProgress::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaEventProgress::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaEventProgress::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaEventProgressRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaEventProgressRow() : Super()
	{
	}
	
	const FGsSchemaEventProgress* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaEventProgress* Row = nullptr;
		return Table->FindRow<FGsSchemaEventProgress>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaEventProgressRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaEventProgressRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableEventProgress final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsEventProgressType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("EventProgress");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaEventProgress*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaEventProgress>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->_type, Row.Key);
		}
	}
	
public:
	bool FindRowById(const EGsEventProgressType& InId, OUT const FGsSchemaEventProgress*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsEventProgressType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaEventProgress* Temp;
		return !FindRowById(FGsSchemaEventProgress()._type, Temp);
	}
#endif
};
