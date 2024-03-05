/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaTutorialConditionOpenUI.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Tutorial/BSGsTutorialConditionOpenUI"))
struct DATACENTER_API FGsSchemaTutorialConditionOpenUI
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="DTGsPathDataUI Key. 대소문자 유의", DisplayName="widgetKey"))
	FName widgetKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="상점처럼 같은 UI사용하는 다른 UI의경우 구분자로 사용", DisplayName="paramString"))
	FString paramString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="paramInt"))
	int32 paramInt;
public:
	FGsSchemaTutorialConditionOpenUI();
	
protected:
	bool operator== (const FGsSchemaTutorialConditionOpenUI& __Other) const
	{
		return FGsSchemaTutorialConditionOpenUI::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTutorialConditionOpenUI::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTutorialConditionOpenUI::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTutorialConditionOpenUIRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTutorialConditionOpenUIRow() : Super()
	{
	}
	
	const FGsSchemaTutorialConditionOpenUI* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTutorialConditionOpenUI* Row = nullptr;
		return Table->FindRow<FGsSchemaTutorialConditionOpenUI>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTutorialConditionOpenUIRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTutorialConditionOpenUIRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTutorialConditionOpenUI final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Tutorial");
	}
	
};
