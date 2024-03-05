/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemGroup.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="아이템 그룹 데이터.. 복구 아이템 정보.", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemGroup"))
struct DATACENTER_API FGsSchemaItemGroup
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId"))
	int32 itemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemGroupId"))
	int32 itemGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="동일 아이템그룹에서 복구할 때 사용할 아이템을 지정함.", DisplayName="isRecoveryItem"))
	bool isRecoveryItem;
public:
	FGsSchemaItemGroup();
	
protected:
	bool operator== (const FGsSchemaItemGroup& __Other) const
	{
		return FGsSchemaItemGroup::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemGroup::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemGroup::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemGroupRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemGroupRow() : Super()
	{
	}
	
	const FGsSchemaItemGroup* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemGroup* Row = nullptr;
		return Table->FindRow<FGsSchemaItemGroup>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemGroupRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemGroupRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemGroup final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
};
