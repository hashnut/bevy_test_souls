/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/TalkBalloon/GsSchemaTalkBalloonDataSet.h"
#include "Runtime/DataCenter/Public/GameObject/Define/EGsTalkType.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaTalkBalloonData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/TalkBalloon/BSGsTalkBalloonData"))
struct DATACENTER_API FGsSchemaTalkBalloonData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="talkRange", Tooltip="출력을 시작하는 거리"))
	float talkRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="talkRandomRate", Tooltip="노출 여부를 결정하기 위한 값"))
	int32 talkRandomRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="talkType", Tooltip="출력 순서 유형"))
	EGsTalkType talkType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="talkBalloonDataSetList", Tooltip="대사 데이터 목록"))
	TArray<FGsSchemaTalkBalloonDataSet> talkBalloonDataSetList;
public:
	FGsSchemaTalkBalloonData();
	
protected:
	bool operator== (const FGsSchemaTalkBalloonData& __Other) const
	{
		return FGsSchemaTalkBalloonData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTalkBalloonData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTalkBalloonData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTalkBalloonDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTalkBalloonDataRow() : Super()
	{
	}
	
	const FGsSchemaTalkBalloonData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTalkBalloonData* Row = nullptr;
		return Table->FindRow<FGsSchemaTalkBalloonData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTalkBalloonDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTalkBalloonDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTalkBalloonData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("TalkBalloon");
	}
	
};
