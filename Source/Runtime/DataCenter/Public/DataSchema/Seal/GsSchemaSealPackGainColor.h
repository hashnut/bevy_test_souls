/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Item/GsStatTypeNameSelector.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSealPackGainColor.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="봉인팩 내 봉인노드 연결선 색상수별 획득 스탯", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Seal/BSGsSealPackGainColor"))
struct DATACENTER_API FGsSchemaSealPackGainColor
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="sealPackGainColorId"))
	int32 sealPackGainColorId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="봉인색상", DisplayName="lineColorType"))
	SealSlotColor lineColorType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="봉인연결라인수", DisplayName="lineColorCount"))
	uint8 lineColorCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="획득스탯타입", DisplayName="gainStatType"))
	FGsStatTypeNameSelector gainStatType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="획득스탯값", DisplayName="gainStatValue"))
	int32 gainStatValue;
public:
	FGsSchemaSealPackGainColor();
	
protected:
	bool operator== (const FGsSchemaSealPackGainColor& __Other) const
	{
		return FGsSchemaSealPackGainColor::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSealPackGainColor::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSealPackGainColor::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSealPackGainColorRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSealPackGainColorRow() : Super()
	{
	}
	
	const FGsSchemaSealPackGainColor* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSealPackGainColor* Row = nullptr;
		return Table->FindRow<FGsSchemaSealPackGainColor>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSealPackGainColorRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSealPackGainColorRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSealPackGainColor final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Seal");
	}
	
};
