/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedEventEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaContentsEventListItem.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="컨텐츠이벤트 리스트아이템", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/ContentsEvent/BSGSContentsEventListItem"))
struct DATACENTER_API FGsSchemaContentsEventListItem
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventType"))
	ContentsEventType eventType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventId"))
	int32 eventId;
public:
	FGsSchemaContentsEventListItem();
	
protected:
	bool operator== (const FGsSchemaContentsEventListItem& __Other) const
	{
		return FGsSchemaContentsEventListItem::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
