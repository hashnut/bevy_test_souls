/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Quest/Dialog/GsDialogType.h"
#include "DataSchema/NpcDialog/GsSchemaNpcDialogImgResData.h"
#include "GsSchemaNpcDialogImgData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NpcDialog/BSGsNpcDialogImgData"))
struct DATACENTER_API FGsSchemaNpcDialogImgData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="posType", Tooltip))
	EGsNpcDialogItemResPosType posType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemRes", Tooltip))
	FGsSchemaNpcDialogImgResDataRow itemRes;
public:
	FGsSchemaNpcDialogImgData();
	
protected:
	bool operator== (const FGsSchemaNpcDialogImgData& __Other) const
	{
		return FGsSchemaNpcDialogImgData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNpcDialogImgData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaNpcDialogImgData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNpcDialogImgDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNpcDialogImgDataRow() : Super()
	{
	}
	
	const FGsSchemaNpcDialogImgData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaNpcDialogImgData* Row = nullptr;
		return Table->FindRow<FGsSchemaNpcDialogImgData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNpcDialogImgDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaNpcDialogImgDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNpcDialogImgData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NpcDialog");
	}
	
};
