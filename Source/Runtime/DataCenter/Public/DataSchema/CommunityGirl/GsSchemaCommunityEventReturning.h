/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCommunityEventReturning.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/CommunityGirl/BSGsCommunityEventReturning"))
struct DATACENTER_API FGsSchemaCommunityEventReturning
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="comeBackTerm"))
	int32 comeBackTerm;
public:
	FGsSchemaCommunityEventReturning();
	
protected:
	bool operator== (const FGsSchemaCommunityEventReturning& __Other) const
	{
		return FGsSchemaCommunityEventReturning::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCommunityEventReturning::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCommunityEventReturning::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCommunityEventReturningRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCommunityEventReturningRow() : Super()
	{
	}
	
	const FGsSchemaCommunityEventReturning* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCommunityEventReturning* Row = nullptr;
		return Table->FindRow<FGsSchemaCommunityEventReturning>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCommunityEventReturningRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCommunityEventReturningRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCommunityEventReturning final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("CommunityGirl");
	}
	
};
