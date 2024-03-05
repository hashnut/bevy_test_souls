/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/User/GsSchemaUserReactionInfo.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaUserReactionData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/BSGsUserReactionData"))
struct DATACENTER_API FGsSchemaUserReactionData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="genderType"))
	CreatureGenderType genderType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="reactionInfo"))
	TArray<FGsSchemaUserReactionInfo> reactionInfo;
public:
	FGsSchemaUserReactionData();
	
protected:
	bool operator== (const FGsSchemaUserReactionData& __Other) const
	{
		return FGsSchemaUserReactionData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaUserReactionData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaUserReactionData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaUserReactionDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaUserReactionDataRow() : Super()
	{
	}
	
	const FGsSchemaUserReactionData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaUserReactionData* Row = nullptr;
		return Table->FindRow<FGsSchemaUserReactionData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaUserReactionDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaUserReactionDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableUserReactionData final : public UGsTable
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
		TMap<FName, const FGsSchemaUserReactionData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaUserReactionData>(RowsMap))
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
	bool FindRowById(const CreatureGenderType& InId, OUT const FGsSchemaUserReactionData*& OutRow) const
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
		const FGsSchemaUserReactionData* Temp;
		return !FindRowById(FGsSchemaUserReactionData().genderType, Temp);
	}
#endif
};
