/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/User/GsSchemaUserData.h"
#include "GsSchemaSkillPreviewGenderInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SkillPreview/BSGsSkillPreviewGenderInfo"))
struct DATACENTER_API FGsSchemaSkillPreviewGenderInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="playerBP"))
	FSoftObjectPath playerBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="playerUserData"))
	FGsSchemaUserDataRow playerUserData;
public:
	FGsSchemaSkillPreviewGenderInfo();
	
protected:
	bool operator== (const FGsSchemaSkillPreviewGenderInfo& __Other) const
	{
		return FGsSchemaSkillPreviewGenderInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillPreviewGenderInfo::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillPreviewGenderInfo::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillPreviewGenderInfoRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillPreviewGenderInfoRow() : Super()
	{
	}
	
	const FGsSchemaSkillPreviewGenderInfo* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillPreviewGenderInfo* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillPreviewGenderInfo>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillPreviewGenderInfoRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillPreviewGenderInfoRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillPreviewGenderInfo final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SkillPreview");
	}
	
};
