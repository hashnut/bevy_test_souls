/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/GameObject/Define/EGsTalkType.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaTalkBalloonDataSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/TalkBalloon/BSGsTalkBalloonDataSet"))
struct DATACENTER_API FGsSchemaTalkBalloonDataSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="text", Tooltip="출력할 대사"))
	FText text;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="keepTime", Tooltip="말풍선 유지시간"))
	float keepTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="intervalTime", Tooltip="다음 말풍선 대기시간"))
	float intervalTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dataSetType"))
	EGsTalkBalloonDataType dataSetType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="status"))
	EGsTalkBalloonState status;
public:
	FGsSchemaTalkBalloonDataSet();
	
protected:
	bool operator== (const FGsSchemaTalkBalloonDataSet& __Other) const
	{
		return FGsSchemaTalkBalloonDataSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTalkBalloonDataSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTalkBalloonDataSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTalkBalloonDataSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTalkBalloonDataSetRow() : Super()
	{
	}
	
	const FGsSchemaTalkBalloonDataSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTalkBalloonDataSet* Row = nullptr;
		return Table->FindRow<FGsSchemaTalkBalloonDataSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTalkBalloonDataSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTalkBalloonDataSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTalkBalloonDataSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("TalkBalloon");
	}
	
};
