/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Tutorial/GsTutorialEnumDefine.h"
#include "Runtime/DataCenter/Public/DataSchema/Tutorial/GsSchemaTutorialAction.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowPoly.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaTutorial.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Tutorial/BSGsTutorial"))
struct DATACENTER_API FGsSchemaTutorial
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="튜토리얼 타입", DisplayName="tutorialType"))
	EGsTutorialType tutorialType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="발동 조건 타입", DisplayName="conditionType"))
	EGsTutorialConditionType conditionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="필요시 입력. 발동 조건에 필요한 추가 테이블", DisplayName="conditionRow"))
	FGsSchemaRowPoly conditionRow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="UseStopAuto"))
	bool UseStopAuto;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="튜토리얼 동작 순서대로 입력", DisplayName="actionList"))
	TArray<FGsSchemaTutorialActionRow> actionList;
public:
	FGsSchemaTutorial();
	
protected:
	bool operator== (const FGsSchemaTutorial& __Other) const
	{
		return FGsSchemaTutorial::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTutorial::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTutorial::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTutorialRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTutorialRow() : Super()
	{
	}
	
	const FGsSchemaTutorial* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTutorial* Row = nullptr;
		return Table->FindRow<FGsSchemaTutorial>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTutorialRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTutorialRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTutorial final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Tutorial");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaTutorial*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaTutorial>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaTutorial*& OutRow) const
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
		const FGsSchemaTutorial* Temp;
		return !FindRowById(FGsSchemaTutorial().id, Temp);
	}
#endif
};
