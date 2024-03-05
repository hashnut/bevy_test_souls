/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Tutorial/GsTutorialEnumDefine.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaTutorialActionUIInteraction.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Tutorial/BSGsTutorialActionUIInteraction"))
struct DATACENTER_API FGsSchemaTutorialActionUIInteraction
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="DTGsPathDataUI Key값", DisplayName="wbpKey"))
	FName wbpKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="대상 위젯이 있는 WBP내 Root 이름", DisplayName="targetRootWidget"))
	FString targetRootWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="인터렉션할 대상 이름", DisplayName="targetWidget"))
	FString targetWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="필요 시 입력. targetWidget과 크기가 다를 경우", DisplayName="visualRootWidget"))
	FString visualRootWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="필요 시 입력. targetWidget과 크기가 다를 경우", DisplayName="visualTargetWidget"))
	FString visualTargetWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="targetWidget과의 인터렉션 타입", DisplayName="targetInteractionType"))
	EGsTutorialUIInteractionType targetInteractionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="화살표 방향", DisplayName="arrowDirection"))
	EGsTutorialUIArrowDirection arrowDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="영역 중앙에 추가로 출력할 이펙트", DisplayName="effectType"))
	EGsTutorialUIEffectType effectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="인터렉션 완료 후 UI Visible 여부", DisplayName="isVisibleAfterInteraction"))
	bool isVisibleAfterInteraction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="targetingDelay"))
	float targetingDelay;
public:
	FGsSchemaTutorialActionUIInteraction();
	
protected:
	bool operator== (const FGsSchemaTutorialActionUIInteraction& __Other) const
	{
		return FGsSchemaTutorialActionUIInteraction::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTutorialActionUIInteraction::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTutorialActionUIInteraction::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTutorialActionUIInteractionRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTutorialActionUIInteractionRow() : Super()
	{
	}
	
	const FGsSchemaTutorialActionUIInteraction* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTutorialActionUIInteraction* Row = nullptr;
		return Table->FindRow<FGsSchemaTutorialActionUIInteraction>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTutorialActionUIInteractionRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTutorialActionUIInteractionRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTutorialActionUIInteraction final : public UGsTable
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
		TMap<FName, const FGsSchemaTutorialActionUIInteraction*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaTutorialActionUIInteraction>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaTutorialActionUIInteraction*& OutRow) const
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
		const FGsSchemaTutorialActionUIInteraction* Temp;
		return !FindRowById(FGsSchemaTutorialActionUIInteraction().id, Temp);
	}
#endif
};
