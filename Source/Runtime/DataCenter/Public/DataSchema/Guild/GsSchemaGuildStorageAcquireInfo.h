/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "GsSchemaGuildStorageAcquireInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/BSGsGuildStorageAcquireInfo"))
struct DATACENTER_API FGsSchemaGuildStorageAcquireInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemAcquireType"))
	GuildItemAcquireType itemAcquireType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="text"))
	FText text;
public:
	FGsSchemaGuildStorageAcquireInfo();
	
protected:
	bool operator== (const FGsSchemaGuildStorageAcquireInfo& __Other) const
	{
		return FGsSchemaGuildStorageAcquireInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaGuildStorageAcquireInfo::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaGuildStorageAcquireInfo::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaGuildStorageAcquireInfoRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaGuildStorageAcquireInfoRow() : Super()
	{
	}
	
	const FGsSchemaGuildStorageAcquireInfo* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaGuildStorageAcquireInfo* Row = nullptr;
		return Table->FindRow<FGsSchemaGuildStorageAcquireInfo>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaGuildStorageAcquireInfoRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaGuildStorageAcquireInfoRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableGuildStorageAcquireInfo final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Guild");
	}
	
};
