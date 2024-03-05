/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSocialEmotion.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SocialEmotion/BSGsSocialEmotion"))
struct DATACENTER_API FGsSchemaSocialEmotion
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="socialId"))
	int64 socialId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="name"))
	FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="socialCategory"))
	UserSocialEmotion socialCategory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isLoopMotion"))
	bool isLoopMotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="emoticon"))
	FSoftObjectPath emoticon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="order"))
	int32 order;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isLock"))
	bool isLock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="이 모션이 선택되면 모션 전체에 적용되는 coolTime 임", DisplayName="secCoolTime"))
	int32 secCoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="itemId"))
	FGsSchemaItemCommonRow itemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="desc"))
	FString desc;
public:
	FGsSchemaSocialEmotion();
	
protected:
	bool operator== (const FGsSchemaSocialEmotion& __Other) const
	{
		return FGsSchemaSocialEmotion::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSocialEmotion::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSocialEmotion::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSocialEmotionRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSocialEmotionRow() : Super()
	{
	}
	
	const FGsSchemaSocialEmotion* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSocialEmotion* Row = nullptr;
		return Table->FindRow<FGsSchemaSocialEmotion>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSocialEmotionRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSocialEmotionRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSocialEmotion final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int64, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SocialEmotion");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSocialEmotion*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSocialEmotion>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->socialId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int64& InId, OUT const FGsSchemaSocialEmotion*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int64& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaSocialEmotion* Temp;
		return !FindRowById(FGsSchemaSocialEmotion().socialId, Temp);
	}
#endif
};
