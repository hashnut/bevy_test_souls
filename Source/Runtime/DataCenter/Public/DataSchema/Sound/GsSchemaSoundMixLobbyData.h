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
#include "GsSchemaSoundMixLobbyData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Sound/BSGsSoundMixLobbyData"))
struct DATACENTER_API FGsSchemaSoundMixLobbyData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mixTypeID"))
	EGsSoundMixType mixTypeID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="soundMixPath"))
	FSoftObjectPath soundMixPath;
public:
	FGsSchemaSoundMixLobbyData();
	
protected:
	bool operator== (const FGsSchemaSoundMixLobbyData& __Other) const
	{
		return FGsSchemaSoundMixLobbyData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSoundMixLobbyData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSoundMixLobbyData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSoundMixLobbyDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSoundMixLobbyDataRow() : Super()
	{
	}
	
	const FGsSchemaSoundMixLobbyData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSoundMixLobbyData* Row = nullptr;
		return Table->FindRow<FGsSchemaSoundMixLobbyData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSoundMixLobbyDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSoundMixLobbyDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSoundMixLobbyData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Sound");
	}
	
};
