/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaNetErrorUI.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Net/BSGsNetErrorUI"))
struct DATACENTER_API FGsSchemaNetErrorUI
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="result"))
	int32 result;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="text"))
	FText text;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="uiType"))
	EGsNetErrorProcUIType uiType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="popupKey"))
	FString popupKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="option"))
	int32 option;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isDefault"))
	bool isDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc"))
	FString desc;
public:
	FGsSchemaNetErrorUI();
	
protected:
	bool operator== (const FGsSchemaNetErrorUI& __Other) const
	{
		return FGsSchemaNetErrorUI::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNetErrorUI::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaNetErrorUI::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNetErrorUIRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNetErrorUIRow() : Super()
	{
	}
	
	const FGsSchemaNetErrorUI* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaNetErrorUI* Row = nullptr;
		return Table->FindRow<FGsSchemaNetErrorUI>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNetErrorUIRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaNetErrorUIRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNetErrorUI final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Net");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaNetErrorUI*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaNetErrorUI>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaNetErrorUI*& OutRow) const
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
		const FGsSchemaNetErrorUI* Temp;
		return !FindRowById(FGsSchemaNetErrorUI().id, Temp);
	}
#endif
};
