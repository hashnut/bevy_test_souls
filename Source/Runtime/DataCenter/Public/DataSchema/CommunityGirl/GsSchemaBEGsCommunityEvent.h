/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommunityEventEnum.h"
#include "Runtime/DataCenter/Public/Community/GsCommunityDefine.h"
#include "Runtime/DataCenter/Public/DataSchema/CommunityGirl/GsSchemaBEGsCommunityEventDetail.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowPoly.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBEGsCommunityEvent.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/CommunityGirl/BEGsCommunityEvent"))
struct DATACENTER_API FGsSchemaBEGsCommunityEvent
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tabNameText"))
	FText tabNameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tabIndex"))
	int32 tabIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventTime"))
	FGsEventTimeData eventTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventType"))
	CommunityEventType eventType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="typeInfo"))
	FGsSchemaRowPoly typeInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bannerImgPath"))
	FSoftObjectPath bannerImgPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="detail"))
	FGsSchemaBEGsCommunityEventDetailRow detail;
public:
	FGsSchemaBEGsCommunityEvent();
	
protected:
	bool operator== (const FGsSchemaBEGsCommunityEvent& __Other) const
	{
		return FGsSchemaBEGsCommunityEvent::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBEGsCommunityEvent::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBEGsCommunityEvent::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBEGsCommunityEventRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBEGsCommunityEventRow() : Super()
	{
	}
	
	const FGsSchemaBEGsCommunityEvent* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBEGsCommunityEvent* Row = nullptr;
		return Table->FindRow<FGsSchemaBEGsCommunityEvent>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBEGsCommunityEventRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBEGsCommunityEventRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBEGsCommunityEvent final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("CommunityGirl");
	}
	
};
