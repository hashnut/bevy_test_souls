/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSoundClassLobbyData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Sound/BSGsSoundClassLobbyData"))
struct DATACENTER_API FGsSchemaSoundClassLobbyData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	EGsSoundVolumeType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="soundClassPath"))
	FSoftObjectPath soundClassPath;
public:
	FGsSchemaSoundClassLobbyData();
	
protected:
	bool operator== (const FGsSchemaSoundClassLobbyData& __Other) const
	{
		return FGsSchemaSoundClassLobbyData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSoundClassLobbyData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSoundClassLobbyData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSoundClassLobbyDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSoundClassLobbyDataRow() : Super()
	{
	}
	
	const FGsSchemaSoundClassLobbyData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSoundClassLobbyData* Row = nullptr;
		return Table->FindRow<FGsSchemaSoundClassLobbyData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSoundClassLobbyDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSoundClassLobbyDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSoundClassLobbyData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Sound");
	}
	
};
