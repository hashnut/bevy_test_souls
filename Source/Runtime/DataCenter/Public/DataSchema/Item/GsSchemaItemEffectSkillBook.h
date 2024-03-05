/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEffectSkillBook.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEffectSkillBook"))
struct DATACENTER_API FGsSchemaItemEffectSkillBook
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectId", Tooltip))
	int32 effectId;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(DisplayName="개발자용노트"))
	FString note;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="userSkillSetId", Tooltip))
	int32 userSkillSetId;
	
public:
	FGsSchemaItemEffectSkillBook();
	
protected:
	bool operator== (const FGsSchemaItemEffectSkillBook& __Other) const
	{
		return FGsSchemaItemEffectSkillBook::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEffectSkillBook::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEffectSkillBook::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEffectSkillBookRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEffectSkillBookRow() : Super()
	{
	}
	
	const FGsSchemaItemEffectSkillBook* GetRow() const
	{
		const FGsSchemaItemEffectSkillBook* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaItemEffectSkillBook>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEffectSkillBookRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEffectSkillBookRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEffectSkillBook final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemEffectSkillBook*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEffectSkillBook>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->effectId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEffectSkillBook*& OutRow) const
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
		const FGsSchemaItemEffectSkillBook* Temp;
		return !FindRowById(FGsSchemaItemEffectSkillBook().effectId, Temp);
	}
#endif
};
