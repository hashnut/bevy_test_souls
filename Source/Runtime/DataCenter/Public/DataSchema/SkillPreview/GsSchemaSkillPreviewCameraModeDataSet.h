/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillPreviewCameraModeDataSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SkillPreview/BSGsSkillPreviewCameraModeDataSet"))
struct DATACENTER_API FGsSchemaSkillPreviewCameraModeDataSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="transform"))
	FTransform transform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fieldOfView"))
	float fieldOfView;
public:
	FGsSchemaSkillPreviewCameraModeDataSet();
	
protected:
	bool operator== (const FGsSchemaSkillPreviewCameraModeDataSet& __Other) const
	{
		return FGsSchemaSkillPreviewCameraModeDataSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillPreviewCameraModeDataSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillPreviewCameraModeDataSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillPreviewCameraModeDataSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillPreviewCameraModeDataSetRow() : Super()
	{
	}
	
	const FGsSchemaSkillPreviewCameraModeDataSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillPreviewCameraModeDataSet* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillPreviewCameraModeDataSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillPreviewCameraModeDataSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillPreviewCameraModeDataSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillPreviewCameraModeDataSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SkillPreview");
	}
	
};
