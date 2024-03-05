/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaNetAddress.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="접속 서버 ip 리스트입니다.", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BSGsNetAddress"))
struct DATACENTER_API FGsSchemaNetAddress
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Address;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Port;
	
public:
	FGsSchemaNetAddress();
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNetAddress::StaticStruct());
	}
	
protected:
	virtual const UScriptStruct* GetThisScriptStruct() const override
	{
		return FGsSchemaNetAddress::StaticStruct();
	}
	
	bool operator== (const FGsSchemaNetAddress& __Other) const
	{
		return FGsSchemaNetAddress::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNetAddressRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNetAddressRow() : Super()
	{
	}
	
	const UGsTable* GetTable() const
	{
		return FGsSchemaNetAddress::GetStaticTable();
	}
	
	const FGsSchemaNetAddress* GetRow() const
	{
		const FGsSchemaNetAddress* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaNetAddress>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNetAddressRow>
	: public TStructOpsTypeTraitsBase2<FGsSchemaNetAddressRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNetAddress : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const override
	{
		return TEXT("");
	}
	
};
