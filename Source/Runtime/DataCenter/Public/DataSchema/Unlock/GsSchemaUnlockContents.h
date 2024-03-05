/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Unlock/GsUnlockDefine.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/Main/GsSchemaQuestMainStory.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedUnlockEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaUnlockContents.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Unlock/BSGsUnlockContents"))
struct DATACENTER_API FGsSchemaUnlockContents
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="serialNumber"))
	int32 serialNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="contentsType"))
	EGsUnlockContentsType contentsType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="contentsName"))
	FText contentsName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="lockType"))
	UnlockCondition lockType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="requireMainStoryId"))
	FGsSchemaQuestMainStoryRow requireMainStoryId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="requireLevel"))
	int32 requireLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="conditionDesc"))
	FText conditionDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="unlockDesc"))
	FText unlockDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bugDesc"))
	FText bugDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="updateDesc"))
	FText updateDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="showUnlockTicker"))
	bool showUnlockTicker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="icon"))
	FSoftObjectPath icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCategorySubList"))
	TArray<ItemCategorySub> itemCategorySubList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mapIdList"))
	TArray<FGsSchemaMapDataRow> mapIdList;
public:
	FGsSchemaUnlockContents();
	
protected:
	bool operator== (const FGsSchemaUnlockContents& __Other) const
	{
		return FGsSchemaUnlockContents::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaUnlockContents::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaUnlockContents::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaUnlockContentsRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaUnlockContentsRow() : Super()
	{
	}
	
	const FGsSchemaUnlockContents* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaUnlockContents* Row = nullptr;
		return Table->FindRow<FGsSchemaUnlockContents>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaUnlockContentsRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaUnlockContentsRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableUnlockContents final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Unlock");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaUnlockContents*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaUnlockContents>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaUnlockContents*& OutRow) const
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
		const FGsSchemaUnlockContents* Temp;
		return !FindRowById(FGsSchemaUnlockContents().id, Temp);
	}
#endif
};
