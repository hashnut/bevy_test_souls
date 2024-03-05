/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillPreviewTransformSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SkillPreview/BSGsSkillPreviewTransformSet"))
struct DATACENTER_API FGsSchemaSkillPreviewTransformSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="owner", Tooltip="스폰되는 플레이어수 (1 기본)"))
	FTransform owner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="party_left", Tooltip="스폰되는 몬스터수"))
	FTransform party_left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="party_right"))
	FTransform party_right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enemy_center"))
	FTransform enemy_center;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enemy_left"))
	FTransform enemy_left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enemy_right"))
	FTransform enemy_right;
public:
	FGsSchemaSkillPreviewTransformSet();
	
protected:
	bool operator== (const FGsSchemaSkillPreviewTransformSet& __Other) const
	{
		return FGsSchemaSkillPreviewTransformSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillPreviewTransformSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillPreviewTransformSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillPreviewTransformSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillPreviewTransformSetRow() : Super()
	{
	}
	
	const FGsSchemaSkillPreviewTransformSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillPreviewTransformSet* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillPreviewTransformSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillPreviewTransformSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillPreviewTransformSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillPreviewTransformSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SkillPreview");
	}
	
};
