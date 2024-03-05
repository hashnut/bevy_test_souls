/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Sound/GsSchemaSoundResData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSealNodeLevelUpSound.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="봉인 경험치 변화량 사운드", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Seal/BSGsSealNodeLevelUpSound"))
struct DATACENTER_API FGsSchemaSealNodeLevelUpSound
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="봉인노드 레벨", DisplayName="expDiff"))
	int32 expDiff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="필요 경험치", DisplayName="soundResDataRow"))
	FGsSchemaSoundResDataRow soundResDataRow;
public:
	FGsSchemaSealNodeLevelUpSound();
	
protected:
	bool operator== (const FGsSchemaSealNodeLevelUpSound& __Other) const
	{
		return FGsSchemaSealNodeLevelUpSound::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSealNodeLevelUpSound::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSealNodeLevelUpSound::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSealNodeLevelUpSoundRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSealNodeLevelUpSoundRow() : Super()
	{
	}
	
	const FGsSchemaSealNodeLevelUpSound* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSealNodeLevelUpSound* Row = nullptr;
		return Table->FindRow<FGsSchemaSealNodeLevelUpSound>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSealNodeLevelUpSoundRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSealNodeLevelUpSoundRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSealNodeLevelUpSound final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Seal");
	}
	
};
