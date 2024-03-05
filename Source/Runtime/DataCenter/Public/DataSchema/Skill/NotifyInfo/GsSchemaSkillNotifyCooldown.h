/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillNotifyCooldown.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/NotifyInfo/BSGsSkillNotifyCooldown"))
struct DATACENTER_API FGsSchemaSkillNotifyCooldown
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="DamageStatType", DisplayName="targetAllUserSkillSetId"))
	bool targetAllUserSkillSetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="targetUserSkillSetIdList"))
	TArray<int32> targetUserSkillSetIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="targetCooldownRate"))
	int32 targetCooldownRate;
public:
	FGsSchemaSkillNotifyCooldown();
	
protected:
	bool operator== (const FGsSchemaSkillNotifyCooldown& __Other) const
	{
		return FGsSchemaSkillNotifyCooldown::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillNotifyCooldown::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillNotifyCooldown::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillNotifyCooldownRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillNotifyCooldownRow() : Super()
	{
	}
	
	const FGsSchemaSkillNotifyCooldown* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillNotifyCooldown* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillNotifyCooldown>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillNotifyCooldownRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillNotifyCooldownRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillNotifyCooldown final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill/NotifyInfo");
	}
	
};
