/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/SkillPreview/GsSchemaSkillPreviewGenderInfo.h"
#include "DataSchema/SkillPreview/GsSchemaSkillPreviewInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsSchemaSkillPreviewWeaponSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SkillPreview/BSGsSkillPreviewWeaponSet"))
struct DATACENTER_API FGsSchemaSkillPreviewWeaponSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="WeaponType"))
	CreatureWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="playerInfo"))
	TMap<CreatureGenderType,FGsSchemaSkillPreviewGenderInfo> playerInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="skillPreviewInfo", Tooltip="Key : SkillID"))
	TMap<int32,FGsSchemaSkillPreviewInfo> skillPreviewInfo;
public:
	FGsSchemaSkillPreviewWeaponSet();
	
protected:
	bool operator== (const FGsSchemaSkillPreviewWeaponSet& __Other) const
	{
		return FGsSchemaSkillPreviewWeaponSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillPreviewWeaponSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillPreviewWeaponSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillPreviewWeaponSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillPreviewWeaponSetRow() : Super()
	{
	}
	
	const FGsSchemaSkillPreviewWeaponSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillPreviewWeaponSet* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillPreviewWeaponSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillPreviewWeaponSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillPreviewWeaponSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillPreviewWeaponSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SkillPreview");
	}
	
};
