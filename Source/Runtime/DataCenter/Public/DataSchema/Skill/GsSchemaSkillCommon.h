/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillCommon.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/BSGsSkillCommon"))
struct DATACENTER_API FGsSchemaSkillCommon
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
	CommonActionType category;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="totalRuntime"))
	float totalRuntime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(AllowedClasses="AnimMontage", DisplayName="aniPath"))
	FSoftObjectPath aniPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="physicsAnimation"))
	bool physicsAnimation;
public:
	FGsSchemaSkillCommon();
	
protected:
	bool operator== (const FGsSchemaSkillCommon& __Other) const
	{
		return FGsSchemaSkillCommon::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillCommon::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillCommon::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillCommonRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillCommonRow() : Super()
	{
	}
	
	const FGsSchemaSkillCommon* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillCommon* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillCommon>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillCommonRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillCommonRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillCommon final : public UGsTable
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
		TMap<FName, const FGsSchemaSkillCommon*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSkillCommon>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSkillCommon*& OutRow) const
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
		const FGsSchemaSkillCommon* Temp;
		return !FindRowById(FGsSchemaSkillCommon().id, Temp);
	}
#endif
};
