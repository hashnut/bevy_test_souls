/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Camera/Define/EGsDialogCameraViewType.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCameraDialogViewData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Camera/BSGsCameraDialogViewData"))
struct DATACENTER_API FGsSchemaCameraDialogViewData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	EGsDialogCameraViewType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="armLength"))
	float armLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="바라보는 쪽에서 추가되는 오프셋(방향 변경)", DisplayName="rootOffset"))
	FVector rootOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="방향에 따라 이동 시키는 값", DisplayName="shiftOffset"))
	FVector shiftOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="lookTargetOffset"))
	FVector lookTargetOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="FOV"))
	float FOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bOverride_DepthOfFieldFstop"))
	bool bOverride_DepthOfFieldFstop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="DepthOfFieldFstop", Tooltip="Defines the opening of the camera lens, Aperture is 1/fstop, typical lens go down to f/1.2 (large opening), larger numbers reduce the DOF effect"))
	float DepthOfFieldFstop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bOverride_DepthOfFieldFocalDistance"))
	bool bOverride_DepthOfFieldFocalDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="DepthOfFieldFocalDistance", Tooltip="Distance in which the Depth of Field effect should be sharp, in unreal units (cm)"))
	float DepthOfFieldFocalDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bOverride_DepthOfFieldFocalRegion"))
	bool bOverride_DepthOfFieldFocalRegion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="DepthOfFieldFocalRegion", Tooltip=" Artificial region where all content is in focus, starting after DepthOfFieldFocalDistance, in unreal units  (cm)"))
	float DepthOfFieldFocalRegion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isCameraMoveable"))
	bool isCameraMoveable;
public:
	FGsSchemaCameraDialogViewData();
	
protected:
	bool operator== (const FGsSchemaCameraDialogViewData& __Other) const
	{
		return FGsSchemaCameraDialogViewData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCameraDialogViewData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCameraDialogViewData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCameraDialogViewDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCameraDialogViewDataRow() : Super()
	{
	}
	
	const FGsSchemaCameraDialogViewData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCameraDialogViewData* Row = nullptr;
		return Table->FindRow<FGsSchemaCameraDialogViewData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCameraDialogViewDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCameraDialogViewDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCameraDialogViewData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Camera");
	}
	
};
