/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/PlayableEvent/GsSchemaPlayableEventAniData.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/PlayableEvent/EGsPlayableEventQuestGoalType.h"
#include "Runtime/DataCenter/Public/DataSchema/SubDialog/GsSchemaSubDialogData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPlayableEventQuest.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/PlayableEvent/BSGsPlayableEventQuest"))
struct DATACENTER_API FGsSchemaPlayableEventQuest
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="goalType"))
	EGsPlayableEventQuestGoalType goalType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="movePos"))
	FVector movePos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="killCount"))
	int32 killCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="goalText"))
	FText goalText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="subDialogData"))
	FGsSchemaSubDialogDataRow subDialogData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="acceptanceRadius"))
	float acceptanceRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="killTargetId"))
	TArray<int32> killTargetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="sequenceId"))
	int32 sequenceId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="npcAniData"))
	TArray<FGsSchemaPlayableEventAniData> npcAniData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="waitTimeSecond"))
	float waitTimeSecond;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="lockInput"))
	bool lockInput;
public:
	FGsSchemaPlayableEventQuest();
	
protected:
	bool operator== (const FGsSchemaPlayableEventQuest& __Other) const
	{
		return FGsSchemaPlayableEventQuest::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPlayableEventQuest::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPlayableEventQuest::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPlayableEventQuestRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPlayableEventQuestRow() : Super()
	{
	}
	
	const FGsSchemaPlayableEventQuest* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPlayableEventQuest* Row = nullptr;
		return Table->FindRow<FGsSchemaPlayableEventQuest>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPlayableEventQuestRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPlayableEventQuestRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePlayableEventQuest final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("PlayableEvent");
	}
	
};
