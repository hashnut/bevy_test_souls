/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/InterServer/GsSchemaWorldMatching.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaInvadeWorldTargetData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/InterServer/BSGsInvadeWorldTargetData"))
struct DATACENTER_API FGsSchemaInvadeWorldTargetData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="worldMatching"))
	TArray<FGsSchemaWorldMatching> worldMatching;
public:
	FGsSchemaInvadeWorldTargetData();
	
protected:
	bool operator== (const FGsSchemaInvadeWorldTargetData& __Other) const
	{
		return FGsSchemaInvadeWorldTargetData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaInvadeWorldTargetData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaInvadeWorldTargetData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaInvadeWorldTargetDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaInvadeWorldTargetDataRow() : Super()
	{
	}
	
	const FGsSchemaInvadeWorldTargetData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaInvadeWorldTargetData* Row = nullptr;
		return Table->FindRow<FGsSchemaInvadeWorldTargetData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaInvadeWorldTargetDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaInvadeWorldTargetDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableInvadeWorldTargetData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("InterServer");
	}
	
};
