/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsSchemaMarketFilterSetting.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Market/BSGsMarketFilterSetting"))
struct DATACENTER_API FGsSchemaMarketFilterSetting
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemGradeList", Tooltip))
	TArray<ItemGrade> itemGradeList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantBonusList", Tooltip))
	TArray<EGsItemEnchantBonusType> enchantBonusList;
public:
	FGsSchemaMarketFilterSetting();
	
protected:
	bool operator== (const FGsSchemaMarketFilterSetting& __Other) const
	{
		return FGsSchemaMarketFilterSetting::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMarketFilterSetting::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMarketFilterSetting::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMarketFilterSettingRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMarketFilterSettingRow() : Super()
	{
	}
	
	const FGsSchemaMarketFilterSetting* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMarketFilterSetting* Row = nullptr;
		return Table->FindRow<FGsSchemaMarketFilterSetting>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMarketFilterSettingRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMarketFilterSettingRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMarketFilterSetting final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Market");
	}
	
};
