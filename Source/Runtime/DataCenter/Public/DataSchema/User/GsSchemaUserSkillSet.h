/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillSet.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaUserSkillSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/BSGSUserSkillSet"))
struct DATACENTER_API FGsSchemaUserSkillSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="note"))
	FString note;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="maleSkillId"))
	FGsSchemaSkillSetRow maleSkillId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="femaleSkillId"))
	FGsSchemaSkillSetRow femaleSkillId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="icon"))
	FSoftObjectPath icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="descText"))
	FText descText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="acquisitionId"))
	int32 acquisitionId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="defaultSlotIndex"))
	int32 defaultSlotIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="스킬 대상 선택 UI 추가 ", DisplayName="addTargetButton"))
	bool addTargetButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tier"))
	int32 tier;
public:
	FGsSchemaUserSkillSet();
	
protected:
	bool operator== (const FGsSchemaUserSkillSet& __Other) const
	{
		return FGsSchemaUserSkillSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaUserSkillSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaUserSkillSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaUserSkillSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaUserSkillSetRow() : Super()
	{
	}
	
	const FGsSchemaUserSkillSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaUserSkillSet* Row = nullptr;
		return Table->FindRow<FGsSchemaUserSkillSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaUserSkillSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaUserSkillSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableUserSkillSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("User");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaUserSkillSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaUserSkillSet>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaUserSkillSet*& OutRow) const
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
		const FGsSchemaUserSkillSet* Temp;
		return !FindRowById(FGsSchemaUserSkillSet().id, Temp);
	}
#endif
};
