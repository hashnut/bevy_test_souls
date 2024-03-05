/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Tutorial/GsTutorialEnumDefine.h"
#include "Runtime/DataCenter/Public/DataSchema/SubDialog/GsSchemaSubDialogCutData.h"
#include "Runtime/DataCenter/Public/DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowPoly.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaTutorialAction.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Tutorial/BSGsTutorialAction"))
struct DATACENTER_API FGsSchemaTutorialAction
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="동작 타입", DisplayName="actionType"))
	EGsTutorialActionType actionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="필요 시 입력. 동작에 필요한 추가 테이블", DisplayName="actionRow"))
	FGsSchemaRowPoly actionRow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="UI인터렉션 정보", DisplayName="actionUIInteractionRow"))
	FGsSchemaTutorialActionUIInteractionRow actionUIInteractionRow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="액션 종료 조건 타입", DisplayName="endCondition"))
	EGsTutorialActionConditionType endCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="필요시 입력. 액션 종료 조건에 필요한 추가 테이블", DisplayName="endConditionRow"))
	FGsSchemaRowPoly endConditionRow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="subDialogCutDataRow"))
	FGsSchemaSubDialogCutDataRow subDialogCutDataRow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="서브 대화창 방향", DisplayName="subDialogDirection"))
	EGsTutorialUIDialogDirection subDialogDirection;
public:
	FGsSchemaTutorialAction();
	
protected:
	bool operator== (const FGsSchemaTutorialAction& __Other) const
	{
		return FGsSchemaTutorialAction::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTutorialAction::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTutorialAction::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTutorialActionRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTutorialActionRow() : Super()
	{
	}
	
	const FGsSchemaTutorialAction* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTutorialAction* Row = nullptr;
		return Table->FindRow<FGsSchemaTutorialAction>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTutorialActionRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTutorialActionRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTutorialAction final : public UGsTable
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
		TMap<FName, const FGsSchemaTutorialAction*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaTutorialAction>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaTutorialAction*& OutRow) const
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
		const FGsSchemaTutorialAction* Temp;
		return !FindRowById(FGsSchemaTutorialAction().id, Temp);
	}
#endif
};
