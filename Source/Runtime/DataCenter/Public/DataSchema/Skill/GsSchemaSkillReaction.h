/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/RootMotion/GsSchemaRootmotionSet.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillReaction.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/BSGsSkillReaction"))
struct DATACENTER_API FGsSchemaSkillReaction
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta=(DisplayName="nameText"))
	FString nameText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta=(DisplayName="category"))
	EReactionType category;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="totalRuntime"))
	float totalRuntime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(AllowedClasses="AnimMontage", DisplayName="aniPath"))
	FSoftObjectPath aniPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rootMotionId"))
	FGsSchemaRootmotionSetRow rootMotionId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="physicsAnimation"))
	bool physicsAnimation;
public:
	FGsSchemaSkillReaction();
	
protected:
	bool operator== (const FGsSchemaSkillReaction& __Other) const
	{
		return FGsSchemaSkillReaction::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillReaction::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillReaction::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillReactionRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillReactionRow() : Super()
	{
	}
	
	const FGsSchemaSkillReaction* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillReaction* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillReaction>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillReactionRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillReactionRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillReaction final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSkillReaction*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSkillReaction>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSkillReaction*& OutRow) const
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
		const FGsSchemaSkillReaction* Temp;
		return !FindRowById(FGsSchemaSkillReaction().id, Temp);
	}
#endif
};
