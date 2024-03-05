/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Level/GsSchemaSkillLevelInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Level/GsSchemaSkillUpgradeInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillLevelSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Level/BSGsSkillLevelSet"))
struct DATACENTER_API FGsSchemaSkillLevelSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기획용 설명", DisplayName="Desc"))
	FString Desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기획 확인용", DisplayName="skillId"))
	int32 skillId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="수동으로 강화 가능한 레벨정보", DisplayName="maxEnchantLevel"))
	int32 maxEnchantLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxUpgradeStep"))
	int32 maxUpgradeStep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="천마석소모개수", DisplayName="spiritShotConsumption"))
	int32 spiritShotConsumption;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="upgradeInfo"))
	FGsSchemaSkillUpgradeInfo upgradeInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="levelInfoList"))
	TArray<FGsSchemaSkillLevelInfo> levelInfoList;
public:
	FGsSchemaSkillLevelSet();
	
protected:
	bool operator== (const FGsSchemaSkillLevelSet& __Other) const
	{
		return FGsSchemaSkillLevelSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillLevelSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillLevelSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillLevelSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillLevelSetRow() : Super()
	{
	}
	
	const FGsSchemaSkillLevelSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillLevelSet* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillLevelSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillLevelSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillLevelSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillLevelSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill/Level");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSkillLevelSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSkillLevelSet>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->skillId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaSkillLevelSet*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int32& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaSkillLevelSet* Temp;
		return !FindRowById(FGsSchemaSkillLevelSet().skillId, Temp);
	}
#endif
};
