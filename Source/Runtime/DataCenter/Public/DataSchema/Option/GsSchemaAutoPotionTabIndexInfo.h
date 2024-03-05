/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAutoPotionTabIndexInfo.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="옵션창에서 자동물약 Rate 설정 탭 인덱스", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Option/BSGsAutoPotionTabIndexInfo"))
struct DATACENTER_API FGsSchemaAutoPotionTabIndexInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="parentTabIndex"))
	int32 parentTabIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="childTabIndex"))
	int32 childTabIndex;
public:
	FGsSchemaAutoPotionTabIndexInfo();
	
protected:
	bool operator== (const FGsSchemaAutoPotionTabIndexInfo& __Other) const
	{
		return FGsSchemaAutoPotionTabIndexInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAutoPotionTabIndexInfo::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAutoPotionTabIndexInfo::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAutoPotionTabIndexInfoRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAutoPotionTabIndexInfoRow() : Super()
	{
	}
	
	const FGsSchemaAutoPotionTabIndexInfo* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAutoPotionTabIndexInfo* Row = nullptr;
		return Table->FindRow<FGsSchemaAutoPotionTabIndexInfo>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAutoPotionTabIndexInfoRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAutoPotionTabIndexInfoRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAutoPotionTabIndexInfo final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Option");
	}
	
};
