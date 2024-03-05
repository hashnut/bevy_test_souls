/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/MonsterKnowledgeCollection/GsSchemaSlateSpecialNode.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMonsterKnowledgeSpecialNode.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/MonsterKnowledgeCollection/SchemaOnly/BSGsMonsterKnowledgeSpecialNode"))
struct DATACENTER_API FGsSchemaMonsterKnowledgeSpecialNode
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="order"))
	int32 order;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="specialNodeId"))
	FGsSchemaSlateSpecialNodeRow specialNodeId;
public:
	FGsSchemaMonsterKnowledgeSpecialNode();
	
protected:
	bool operator== (const FGsSchemaMonsterKnowledgeSpecialNode& __Other) const
	{
		return FGsSchemaMonsterKnowledgeSpecialNode::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
