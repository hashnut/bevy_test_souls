/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMiniMapData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/MiniMap/BSGsMiniMapData"))
struct DATACENTER_API FGsSchemaMiniMapData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="RealMapSize", Tooltip))
	FVector2D RealMapSize;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="MiniMapSize", Tooltip))
	FVector2D MiniMapSize;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="MiniMapImagePath", Tooltip))
	FSoftObjectPath MiniMapImagePath;
	
public:
	FGsSchemaMiniMapData();
	
protected:
	bool operator== (const FGsSchemaMiniMapData& __Other) const
	{
		return FGsSchemaMiniMapData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMiniMapData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMiniMapData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMiniMapDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMiniMapDataRow() : Super()
	{
	}
	
	const FGsSchemaMiniMapData* GetRow() const
	{
		const FGsSchemaMiniMapData* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaMiniMapData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMiniMapDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMiniMapDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMiniMapData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/MiniMap");
	}
	
};
