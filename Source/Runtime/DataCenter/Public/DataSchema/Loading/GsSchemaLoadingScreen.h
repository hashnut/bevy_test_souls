/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaLoadingScreen.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Loading/BSGsLoadingScreen"))
struct DATACENTER_API FGsSchemaLoadingScreen
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="imagePath"))
	FSoftObjectPath imagePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="titleText"))
	FText titleText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="titleColor"))
	FColor titleColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="descText"))
	FText descText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="descColor"))
	FColor descColor;
public:
	FGsSchemaLoadingScreen();
	
protected:
	bool operator== (const FGsSchemaLoadingScreen& __Other) const
	{
		return FGsSchemaLoadingScreen::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaLoadingScreen::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaLoadingScreen::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaLoadingScreenRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaLoadingScreenRow() : Super()
	{
	}
	
	const FGsSchemaLoadingScreen* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaLoadingScreen* Row = nullptr;
		return Table->FindRow<FGsSchemaLoadingScreen>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaLoadingScreenRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaLoadingScreenRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableLoadingScreen final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Loading");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaLoadingScreen*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaLoadingScreen>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaLoadingScreen*& OutRow) const
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
		const FGsSchemaLoadingScreen* Temp;
		return !FindRowById(FGsSchemaLoadingScreen().id, Temp);
	}
#endif
};
