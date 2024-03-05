/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattlePassLevelIcon.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="BattlePass Group Level 별 Icon", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Icon/BSGsBattlePassLevelIcon"))
struct DATACENTER_API FGsSchemaBattlePassLevelIcon
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="iconPath"))
	FSoftObjectPath iconPath;
public:
	FGsSchemaBattlePassLevelIcon();
	
protected:
	bool operator== (const FGsSchemaBattlePassLevelIcon& __Other) const
	{
		return FGsSchemaBattlePassLevelIcon::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBattlePassLevelIcon::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBattlePassLevelIcon::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBattlePassLevelIconRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBattlePassLevelIconRow() : Super()
	{
	}
	
	const FGsSchemaBattlePassLevelIcon* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBattlePassLevelIcon* Row = nullptr;
		return Table->FindRow<FGsSchemaBattlePassLevelIcon>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBattlePassLevelIconRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBattlePassLevelIconRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBattlePassLevelIcon final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Icon");
	}
	
};
