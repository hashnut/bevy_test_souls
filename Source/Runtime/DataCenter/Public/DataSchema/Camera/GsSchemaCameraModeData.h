/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCameraModeData.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="카메라 모드 데이터 입니다.", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Camera/BSGsCameraModeData"))
struct DATACENTER_API FGsSchemaCameraModeData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="Mode"))
	EGsCameraMode Mode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="ArmLengthTo"))
	float ArmLengthTo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="ControllerPitch"))
	float ControllerPitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="FOV"))
	float FOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="SocketOffset"))
	FVector SocketOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="ViewPitchMin"))
	float ViewPitchMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="ViewPitchMax"))
	float ViewPitchMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="CamArmLengthMax"))
	float CamArmLengthMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="CamArmLengthMin"))
	float CamArmLengthMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="CamShiftPos"))
	FVector CamShiftPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ZoomMinDepthOfFieldFocalRegionMobile"))
	float ZoomMinDepthOfFieldFocalRegionMobile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ZoomMinFOV"))
	float ZoomMinFOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="UseZoomMinDOF"))
	bool UseZoomMinDOF;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="UseZoomMinFOV"))
	bool UseZoomMinFOV;
public:
	FGsSchemaCameraModeData();
	
protected:
	bool operator== (const FGsSchemaCameraModeData& __Other) const
	{
		return FGsSchemaCameraModeData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCameraModeData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCameraModeData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCameraModeDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCameraModeDataRow() : Super()
	{
	}
	
	const FGsSchemaCameraModeData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCameraModeData* Row = nullptr;
		return Table->FindRow<FGsSchemaCameraModeData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCameraModeDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCameraModeDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCameraModeData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsCameraMode, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Camera");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCameraModeData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCameraModeData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->Mode, Row.Key);
		}
	}
	
public:
	bool FindRowById(const EGsCameraMode& InId, OUT const FGsSchemaCameraModeData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsCameraMode& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaCameraModeData* Temp;
		return !FindRowById(FGsSchemaCameraModeData().Mode, Temp);
	}
#endif
};
