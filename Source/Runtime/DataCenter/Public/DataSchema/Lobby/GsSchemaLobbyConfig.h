/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaLobbyConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Lobby/BSGsLobbyConfig"))
struct DATACENTER_API FGsSchemaLobbyConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="캐릭터 삭제 가능 레벨"))
	int32 deleteUserMinLevel;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="캐릭터 삭제 유예기간"))
	int32 deletionCooldownHour;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="캐릭터 기본 슬롯"))
	int32 characterSlotCount;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="캐릭터 유료 슬롯"))
	int32 characterPaidSlotCount;
	
public:
	FGsSchemaLobbyConfig();
	
protected:
	bool operator== (const FGsSchemaLobbyConfig& __Other) const
	{
		return FGsSchemaLobbyConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaLobbyConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaLobbyConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaLobbyConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaLobbyConfigRow() : Super()
	{
	}
	
	const FGsSchemaLobbyConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaLobbyConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaLobbyConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaLobbyConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaLobbyConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableLobbyConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Lobby");
	}
	
};
