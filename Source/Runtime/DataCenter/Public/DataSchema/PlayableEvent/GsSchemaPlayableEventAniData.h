/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/PlayableEvent/GsStatePlayableEventNPCType.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPlayableEventAniData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/PlayableEvent/BSGsPlayableEventAniData"))
struct DATACENTER_API FGsSchemaPlayableEventAniData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="npcAniSpawnId"))
	TArray<int32> npcAniSpawnId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="playAniType"))
	EGsStatePlayableEventNPC playAniType;
public:
	FGsSchemaPlayableEventAniData();
	
protected:
	bool operator== (const FGsSchemaPlayableEventAniData& __Other) const
	{
		return FGsSchemaPlayableEventAniData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPlayableEventAniData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPlayableEventAniData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPlayableEventAniDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPlayableEventAniDataRow() : Super()
	{
	}
	
	const FGsSchemaPlayableEventAniData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPlayableEventAniData* Row = nullptr;
		return Table->FindRow<FGsSchemaPlayableEventAniData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPlayableEventAniDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPlayableEventAniDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePlayableEventAniData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("PlayableEvent");
	}
	
};
