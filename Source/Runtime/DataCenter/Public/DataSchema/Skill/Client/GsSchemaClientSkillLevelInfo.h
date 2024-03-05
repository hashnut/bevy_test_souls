/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Skill/Client/GsSchemaClientSkillLevelNotifyInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "GsSchemaClientSkillLevelInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Client/BSGsClientSkillLevelInfo"))
struct DATACENTER_API FGsSchemaClientSkillLevelInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta=(DisplayName="notifyInfoList", Tooltip="Level을 인덱스로 접근하기 위해 첫번째 배열 무조건 생성"))
	TArray<FGsSchemaClientSkillLevelNotifyInfo> notifyInfoList;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta=(DisplayName="abnormalityList"))
	TArray<FGsSchemaAbnormalitySetRow> abnormalityList;
public:
	FGsSchemaClientSkillLevelInfo();
	
protected:
	bool operator== (const FGsSchemaClientSkillLevelInfo& __Other) const
	{
		return FGsSchemaClientSkillLevelInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaClientSkillLevelInfo::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaClientSkillLevelInfo::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaClientSkillLevelInfoRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaClientSkillLevelInfoRow() : Super()
	{
	}
	
	const FGsSchemaClientSkillLevelInfo* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaClientSkillLevelInfo* Row = nullptr;
		return Table->FindRow<FGsSchemaClientSkillLevelInfo>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaClientSkillLevelInfoRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaClientSkillLevelInfoRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableClientSkillLevelInfo final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill/Client");
	}
	
};
