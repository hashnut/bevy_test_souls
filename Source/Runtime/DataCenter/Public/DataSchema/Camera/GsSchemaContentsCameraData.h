/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaContentsCameraData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Camera/BSGsContentsCameraData"))
struct DATACENTER_API FGsSchemaContentsCameraData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cameraType"))
	EGsContentsCameraType cameraType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="FOV"))
	float FOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="CamearPositionOffset"))
	FVector CamearPositionOffset;
public:
	FGsSchemaContentsCameraData();
	
protected:
	bool operator== (const FGsSchemaContentsCameraData& __Other) const
	{
		return FGsSchemaContentsCameraData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaContentsCameraData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaContentsCameraData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaContentsCameraDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaContentsCameraDataRow() : Super()
	{
	}
	
	const FGsSchemaContentsCameraData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaContentsCameraData* Row = nullptr;
		return Table->FindRow<FGsSchemaContentsCameraData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaContentsCameraDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaContentsCameraDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableContentsCameraData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsContentsCameraType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Camera");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaContentsCameraData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaContentsCameraData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->cameraType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const EGsContentsCameraType& InId, OUT const FGsSchemaContentsCameraData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsContentsCameraType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaContentsCameraData* Temp;
		return !FindRowById(FGsSchemaContentsCameraData().cameraType, Temp);
	}
#endif
};
