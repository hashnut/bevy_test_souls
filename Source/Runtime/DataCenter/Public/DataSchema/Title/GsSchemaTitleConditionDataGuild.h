/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "GsSchemaTitleConditionDataGuild.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Title/BSGsTitleConditionDataGuild"))
struct DATACENTER_API FGsSchemaTitleConditionDataGuild
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mActivityType"))
	GuildDailyActivityType mActivityType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mGradeType"))
	GuildMemberGradeType mGradeType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="guildLevel"))
	int32 guildLevel;
public:
	FGsSchemaTitleConditionDataGuild();
	
protected:
	bool operator== (const FGsSchemaTitleConditionDataGuild& __Other) const
	{
		return FGsSchemaTitleConditionDataGuild::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTitleConditionDataGuild::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTitleConditionDataGuild::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTitleConditionDataGuildRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTitleConditionDataGuildRow() : Super()
	{
	}
	
	const FGsSchemaTitleConditionDataGuild* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTitleConditionDataGuild* Row = nullptr;
		return Table->FindRow<FGsSchemaTitleConditionDataGuild>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTitleConditionDataGuildRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTitleConditionDataGuildRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTitleConditionDataGuild final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Title");
	}
	
};
