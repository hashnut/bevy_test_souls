/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "Runtime/DataCenter/Public/Option/GsOptionEnumType.h"
#include "Runtime/Engine/Classes/Engine/EngineBaseTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Runtime/Slate/Public/Framework/Commands/InputChord.h"
#include "GsSchemaKeyMapping.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/KeyMapping/BSGsKeyMapping"))
struct DATACENTER_API FGsSchemaKeyMapping
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="keyMappingType"))
	EGsKeyMappingType keyMappingType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="keyName"))
	FName keyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="text"))
	FText text;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="keyInputChord"))
	FInputChord keyInputChord;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="keyInputEvents"))
	TArray<TEnumAsByte<EInputEvent> > keyInputEvents;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="keyScale"))
	float keyScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="keyBindType"))
	EGsOptionKeyBindType keyBindType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="keyResType"))
	EGsOptionKeyResType keyResType;
public:
	FGsSchemaKeyMapping();
	
protected:
	bool operator== (const FGsSchemaKeyMapping& __Other) const
	{
		return FGsSchemaKeyMapping::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaKeyMapping::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaKeyMapping::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaKeyMappingRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaKeyMappingRow() : Super()
	{
	}
	
	const FGsSchemaKeyMapping* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaKeyMapping* Row = nullptr;
		return Table->FindRow<FGsSchemaKeyMapping>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaKeyMappingRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaKeyMappingRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableKeyMapping final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsKeyMappingType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("KeyMapping");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaKeyMapping*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaKeyMapping>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->keyMappingType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const EGsKeyMappingType& InId, OUT const FGsSchemaKeyMapping*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsKeyMappingType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaKeyMapping* Temp;
		return !FindRowById(FGsSchemaKeyMapping().keyMappingType, Temp);
	}
#endif
};
