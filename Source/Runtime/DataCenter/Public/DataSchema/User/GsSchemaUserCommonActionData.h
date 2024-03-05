/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Common/GsSchemaIdleEmotionActionData.h"
#include "Runtime/DataCenter/Public/DataSchema/User/GsSchemaUserCommonActionInfo.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillCommon.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaUserCommonActionData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/BSGsUserCommonActionData"))
struct DATACENTER_API FGsSchemaUserCommonActionData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="genderType"))
	CreatureGenderType genderType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="commonActionInfo"))
	TArray<FGsSchemaUserCommonActionInfo> commonActionInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="Type::SOCIAL 에 해당 하는 데이터 등록", DisplayName="socialActionInfo"))
	TArray<FGsSchemaSkillCommonRow> socialActionInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="emotionActionInfo"))
	FGsSchemaIdleEmotionActionData emotionActionInfo;
public:
	FGsSchemaUserCommonActionData();
	
protected:
	bool operator== (const FGsSchemaUserCommonActionData& __Other) const
	{
		return FGsSchemaUserCommonActionData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaUserCommonActionData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaUserCommonActionData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaUserCommonActionDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaUserCommonActionDataRow() : Super()
	{
	}
	
	const FGsSchemaUserCommonActionData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaUserCommonActionData* Row = nullptr;
		return Table->FindRow<FGsSchemaUserCommonActionData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaUserCommonActionDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaUserCommonActionDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableUserCommonActionData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<CreatureGenderType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("User");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaUserCommonActionData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaUserCommonActionData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->genderType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const CreatureGenderType& InId, OUT const FGsSchemaUserCommonActionData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const CreatureGenderType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaUserCommonActionData* Temp;
		return !FindRowById(FGsSchemaUserCommonActionData().genderType, Temp);
	}
#endif
};
