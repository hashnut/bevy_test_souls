/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Event/EventAction/GsSchemaEventActionGroup.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaQuestConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Quest/Config/BSGsQuestConfig"))
struct DATACENTER_API FGsSchemaQuestConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="initEventActionGroupId", Tooltip="캐릭터 첫 생성 후 월드진입 시 하는 이벤트액션"))
	FGsSchemaEventActionGroupRow initEventActionGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="repeatStoryGradeIconNormal"))
	FSoftObjectPath repeatStoryGradeIconNormal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="repeatStoryGradeIconMagic"))
	FSoftObjectPath repeatStoryGradeIconMagic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="repeatStoryGradeIconRare"))
	FSoftObjectPath repeatStoryGradeIconRare;
public:
	FGsSchemaQuestConfig();
	
protected:
	bool operator== (const FGsSchemaQuestConfig& __Other) const
	{
		return FGsSchemaQuestConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaQuestConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaQuestConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaQuestConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaQuestConfigRow() : Super()
	{
	}
	
	const FGsSchemaQuestConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaQuestConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaQuestConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaQuestConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaQuestConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableQuestConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Quest/Config");
	}
	
};
