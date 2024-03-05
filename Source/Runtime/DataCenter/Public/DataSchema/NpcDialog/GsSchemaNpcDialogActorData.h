/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/NpcDialog/GsSchemaNpcDialogActorSpawnInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Quest/Dialog/GsDialogType.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/Skill/GsSchemaSkillCommon.h"
#include "GsSchemaNpcDialogActorData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NpcDialog/BSGsNpcDialogActorData"))
struct DATACENTER_API FGsSchemaNpcDialogActorData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="actorType", Tooltip))
	EGsNpcDialogActorType actorType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="npcData", Tooltip))
	FGsSchemaNpcDataRow npcData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isMotionPlay", Tooltip))
	bool isMotionPlay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="motionList", Tooltip))
	TArray<FGsSchemaSkillCommonRow> motionList;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="expressionType", Tooltip))
	EGsEmoticonType expressionType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="preActorState", Tooltip))
	EGsNpcDialogActorStateType preActorState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="PostActorState", Tooltip))
	EGsNpcDialogActorStateType PostActorState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnInfo", Tooltip))
	FGsSchemaNpcDialogActorSpawnInfo spawnInfo;
	
public:
	FGsSchemaNpcDialogActorData();
	
protected:
	bool operator== (const FGsSchemaNpcDialogActorData& __Other) const
	{
		return FGsSchemaNpcDialogActorData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNpcDialogActorData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaNpcDialogActorData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNpcDialogActorDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNpcDialogActorDataRow() : Super()
	{
	}
	
	const FGsSchemaNpcDialogActorData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaNpcDialogActorData* Row = nullptr;
		return Table->FindRow<FGsSchemaNpcDialogActorData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNpcDialogActorDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaNpcDialogActorDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNpcDialogActorData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NpcDialog");
	}
	
};
