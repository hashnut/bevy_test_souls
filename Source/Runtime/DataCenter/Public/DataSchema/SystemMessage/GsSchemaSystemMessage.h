/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSystemMessageEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowPoly.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSystemMessage.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SystemMessage/BSGsSystemMessage"))
struct DATACENTER_API FGsSchemaSystemMessage
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	SystemMessageType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="conditionId"))
	TArray<FGsSchemaRowPoly> conditionId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="systemMessage"))
	FText systemMessage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="useTicker"))
	bool useTicker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="useDelayTicker"))
	bool useDelayTicker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="excludeMineTicker"))
	bool excludeMineTicker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="useCofigGradeLimit"))
	bool useCofigGradeLimit;
public:
	FGsSchemaSystemMessage();
	
protected:
	bool operator== (const FGsSchemaSystemMessage& __Other) const
	{
		return FGsSchemaSystemMessage::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSystemMessage::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSystemMessage::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSystemMessageRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSystemMessageRow() : Super()
	{
	}
	
	const FGsSchemaSystemMessage* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSystemMessage* Row = nullptr;
		return Table->FindRow<FGsSchemaSystemMessage>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSystemMessageRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSystemMessageRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSystemMessage final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SystemMessage");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSystemMessage*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSystemMessage>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSystemMessage*& OutRow) const
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
		const FGsSchemaSystemMessage* Temp;
		return !FindRowById(FGsSchemaSystemMessage().id, Temp);
	}
#endif
};
