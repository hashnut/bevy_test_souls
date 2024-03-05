/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCashCurrency.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="통화 출력에 사용. 키는 Currency Code(ISO 4217).", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BSGsCashCurrency"))
struct DATACENTER_API FGsSchemaCashCurrency
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="통화 코드 (ISO 4217)", DisplayName="currencyCode"))
	EGsBMShopCashCurrency currencyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="텍스트형 통화 코드", DisplayName="text"))
	FText text;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="이미지형 통화 코드(아이템 아이콘)", DisplayName="iconPath"))
	FSoftObjectPath iconPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="설명", MultiLine="true", DisplayName="desc"))
	FText desc;
public:
	FGsSchemaCashCurrency();
	
protected:
	bool operator== (const FGsSchemaCashCurrency& __Other) const
	{
		return FGsSchemaCashCurrency::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCashCurrency::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCashCurrency::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCashCurrencyRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCashCurrencyRow() : Super()
	{
	}
	
	const FGsSchemaCashCurrency* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCashCurrency* Row = nullptr;
		return Table->FindRow<FGsSchemaCashCurrency>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCashCurrencyRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCashCurrencyRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCashCurrency final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsBMShopCashCurrency, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCashCurrency*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCashCurrency>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->currencyCode, Row.Key);
		}
	}
	
public:
	bool FindRowById(const EGsBMShopCashCurrency& InId, OUT const FGsSchemaCashCurrency*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsBMShopCashCurrency& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaCashCurrency* Temp;
		return !FindRowById(FGsSchemaCashCurrency().currencyCode, Temp);
	}
#endif
};
