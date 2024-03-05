/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCustomizePreset.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Customize/BSGsCustomizePreset"))
struct DATACENTER_API FGsSchemaCustomizePreset
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString desc;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 partsId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="faceIndex", Tooltip))
	int32 faceIndex;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 hairIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tattooIndex", Tooltip))
	int32 tattooIndex;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 hairColorIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="skinColorIndex", Tooltip))
	int32 skinColorIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pupilColorIndex", Tooltip))
	int32 pupilColorIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="browColorIndex", Tooltip))
	int32 browColorIndex;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(ClampMin="0", ClampMax="100"))
	int32 height;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(ClampMin="0", ClampMax="100"))
	int32 headSize;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(ClampMin="0", ClampMax="100"))
	int32 chestSize;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(ClampMin="0", ClampMax="100"))
	int32 waistSize;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(ClampMin="0", ClampMax="100"))
	int32 pelvisSize;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(ClampMin="0", ClampMax="100"))
	int32 legSize;
	
public:
	FGsSchemaCustomizePreset();
	
protected:
	bool operator== (const FGsSchemaCustomizePreset& __Other) const
	{
		return FGsSchemaCustomizePreset::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCustomizePreset::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCustomizePreset::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCustomizePresetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCustomizePresetRow() : Super()
	{
	}
	
	const FGsSchemaCustomizePreset* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCustomizePreset* Row = nullptr;
		return Table->FindRow<FGsSchemaCustomizePreset>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCustomizePresetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCustomizePresetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCustomizePreset final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Customize");
	}
	
};
