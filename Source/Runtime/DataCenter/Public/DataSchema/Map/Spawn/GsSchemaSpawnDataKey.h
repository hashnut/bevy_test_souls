/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpawnDataKey.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="SpawnData 테이블에서 키가 되는 MapId 와 SpawnGroupId 만 추출한 테이블. 클라에서 사용하지 않는 테이블을 로드하지 않지만, 서버와의 통신을 위해 키값이 필요하여 생성하여 사용함.", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Spawn/BSGsSpawnDataKey"))
struct DATACENTER_API FGsSchemaSpawnDataKey
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MapId;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="Territory zone unique id"))
	int32 SpawnGroupId;
	
public:
	FGsSchemaSpawnDataKey();
	
protected:
	bool operator== (const FGsSchemaSpawnDataKey& __Other) const
	{
		return FGsSchemaSpawnDataKey::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSpawnDataKey::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSpawnDataKey::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSpawnDataKeyRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSpawnDataKeyRow() : Super()
	{
	}
	
	const FGsSchemaSpawnDataKey* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSpawnDataKey* Row = nullptr;
		return Table->FindRow<FGsSchemaSpawnDataKey>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSpawnDataKeyRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSpawnDataKeyRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSpawnDataKey final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/Spawn");
	}
	
};
