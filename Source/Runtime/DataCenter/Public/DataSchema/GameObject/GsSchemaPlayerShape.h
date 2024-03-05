/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/GsSchemaPlayerWeaponDetailInfo.h"
#include "Runtime/DataCenter/Public/GameObject/Define/EGsPartsType.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/GsSchemaPartsShapeData.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/GsSchemaShapeData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPlayerShape.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/BSGsPlayerShape"))
struct DATACENTER_API FGsSchemaPlayerShape
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="로컬플레이어 외형정보", DisplayName="localplayerShapeId"))
	FGsSchemaShapeDataRow localplayerShapeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="타플레이어 외형정보", DisplayName="remoteplayerShapeId"))
	FGsSchemaShapeDataRow remoteplayerShapeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="polymorpShapeId"))
	FGsSchemaShapeDataRow polymorpShapeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="defaultParts"))
	TMap<EGsPartsType,FGsSchemaPartsShapeDataRow> defaultParts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="weaponDetailInfo"))
	TArray<FGsSchemaPlayerWeaponDetailInfo> weaponDetailInfo;
public:
	FGsSchemaPlayerShape();
	
protected:
	bool operator== (const FGsSchemaPlayerShape& __Other) const
	{
		return FGsSchemaPlayerShape::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPlayerShape::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPlayerShape::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPlayerShapeRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPlayerShapeRow() : Super()
	{
	}
	
	const FGsSchemaPlayerShape* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPlayerShape* Row = nullptr;
		return Table->FindRow<FGsSchemaPlayerShape>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPlayerShapeRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPlayerShapeRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePlayerShape final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaPlayerShape*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaPlayerShape>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaPlayerShape*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int32& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaPlayerShape* Temp;
		return !FindRowById(FGsSchemaPlayerShape().id, Temp);
	}
#endif
};
