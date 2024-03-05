/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Camera/Define/EGsCameraLobbyState.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCustomizeCameraData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Camera/BSGsCustomizeCameraData"))
struct DATACENTER_API FGsSchemaCustomizeCameraData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="CameraLobbyState"))
	EGsCameraLobbyState CameraLobbyState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="HeadAddPos"))
	FVector HeadAddPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="WholeBodyAddPos"))
	FVector WholeBodyAddPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="SpringArmLengthMin"))
	float SpringArmLengthMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="SpringArmLengthMax"))
	float SpringArmLengthMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="InitPitchRotation"))
	float InitPitchRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="HeadShiftCameraPosY"))
	float HeadShiftCameraPosY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="WholeBodyShiftCameraPosY"))
	float WholeBodyShiftCameraPosY;
public:
	FGsSchemaCustomizeCameraData();
	
protected:
	bool operator== (const FGsSchemaCustomizeCameraData& __Other) const
	{
		return FGsSchemaCustomizeCameraData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCustomizeCameraData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCustomizeCameraData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCustomizeCameraDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCustomizeCameraDataRow() : Super()
	{
	}
	
	const FGsSchemaCustomizeCameraData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCustomizeCameraData* Row = nullptr;
		return Table->FindRow<FGsSchemaCustomizeCameraData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCustomizeCameraDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCustomizeCameraDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCustomizeCameraData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsCameraLobbyState, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Camera");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCustomizeCameraData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCustomizeCameraData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->CameraLobbyState, Row.Key);
		}
	}
	
public:
	bool FindRowById(const EGsCameraLobbyState& InId, OUT const FGsSchemaCustomizeCameraData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsCameraLobbyState& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaCustomizeCameraData* Temp;
		return !FindRowById(FGsSchemaCustomizeCameraData().CameraLobbyState, Temp);
	}
#endif
};
