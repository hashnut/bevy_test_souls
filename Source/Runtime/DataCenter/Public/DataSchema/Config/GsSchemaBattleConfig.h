/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattleConfig.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="전투관련 서버와 같이 쓸 값", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Config/BSGsBattleConfig"))
struct DATACENTER_API FGsSchemaBattleConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="초", DisplayName="regenerationTic"))
	float regenerationTic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="스킬사용주기최대시간(초)", DisplayName="maxSkillIntervalSecond"))
	int32 maxSkillIntervalSecond;
public:
	FGsSchemaBattleConfig();
	
protected:
	bool operator== (const FGsSchemaBattleConfig& __Other) const
	{
		return FGsSchemaBattleConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBattleConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBattleConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBattleConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBattleConfigRow() : Super()
	{
	}
	
	const FGsSchemaBattleConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBattleConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaBattleConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBattleConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBattleConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBattleConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Config");
	}
	
};
