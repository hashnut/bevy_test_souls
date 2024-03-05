/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedEventEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowPoly.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaEventAction.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="이벤트액션", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Event/EventAction/BSGsEventAction"))
struct DATACENTER_API FGsSchemaEventAction
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="clientOnly"))
	bool clientOnly;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="type"))
	EventActionType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="typeInfo"))
	FGsSchemaRowPoly typeInfo;
public:
	FGsSchemaEventAction();
	
protected:
	bool operator== (const FGsSchemaEventAction& __Other) const
	{
		return FGsSchemaEventAction::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
