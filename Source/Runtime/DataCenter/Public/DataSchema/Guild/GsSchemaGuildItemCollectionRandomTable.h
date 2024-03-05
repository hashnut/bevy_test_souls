/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Item/Collection/GsItemCollectionCommonData.h"
#include "GsSchemaGuildItemCollectionRandomTable.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/BSGsGuildItemCollectionRandomTable"))
struct DATACENTER_API FGsSchemaGuildItemCollectionRandomTable
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mainItemId"))
	int32 mainItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mainEnchantLevel"))
	int32 mainEnchantLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="subSituteItemData"))
	TArray<FGsItemCollectionSubSituteItemData> subSituteItemData;
public:
	FGsSchemaGuildItemCollectionRandomTable();
	
protected:
	bool operator== (const FGsSchemaGuildItemCollectionRandomTable& __Other) const
	{
		return FGsSchemaGuildItemCollectionRandomTable::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaGuildItemCollectionRandomTable::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaGuildItemCollectionRandomTable::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaGuildItemCollectionRandomTableRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaGuildItemCollectionRandomTableRow() : Super()
	{
	}
	
	const FGsSchemaGuildItemCollectionRandomTable* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaGuildItemCollectionRandomTable* Row = nullptr;
		return Table->FindRow<FGsSchemaGuildItemCollectionRandomTable>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaGuildItemCollectionRandomTableRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaGuildItemCollectionRandomTableRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableGuildItemCollectionRandomTable final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Guild");
	}
	
};
