/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaNpcDropItem.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/MapBalance/BSGsNpcDropItem"))
struct DATACENTER_API FGsSchemaNpcDropItem
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="npcIdList"))
	TArray<FGsSchemaNpcDataRow> npcIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spotId"))
	int32 spotId;
public:
	FGsSchemaNpcDropItem();
	
protected:
	bool operator== (const FGsSchemaNpcDropItem& __Other) const
	{
		return FGsSchemaNpcDropItem::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
