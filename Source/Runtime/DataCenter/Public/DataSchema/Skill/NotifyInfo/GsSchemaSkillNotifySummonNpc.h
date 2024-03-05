/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillNotifyEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillNotifySummonNpc.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/NotifyInfo/BSGsSkillNotifySummonNpc"))
struct DATACENTER_API FGsSchemaSkillNotifySummonNpc
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="소환 위치를 계산하기 위한 방식에 따른 타입 구분", DisplayName="summoneeSpawnType"))
	SummoneeSpawnType summoneeSpawnType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기준 위치(SummoneeSpawnType 에 따라 상이할 수 있음)에서 어느 방향으로 소환할 것인지 값", DisplayName="summoneeSpawnDirection"))
	int32 summoneeSpawnDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기준 위치(SummoneeSpawnType 에 따라 상이할 수 있음)에서 어느 거리에서 소환할 것인지 값", DisplayName="summoneeSpawnDistance"))
	int32 summoneeSpawnDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="creatureId"))
	int32 creatureId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="피소환 Npc 레벨", DisplayName="creatureLevel"))
	int32 creatureLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="소환 개체 수", DisplayName="summonCount"))
	int32 summonCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="해당 스킬 효과로 인해 소환된 소환수의 현재 HP 값을 소환자의 현재 HP 값으로 사용할지 여부", DisplayName="isSummonSameHp"))
	bool isSummonSameHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="소환자 레벨(Lv)과 동일 여부로 소환 당시 소환수 레벨을 소환자 레벨과 동일하게 설정", DisplayName="isSummonSameLv"))
	bool isSummonSameLv;
public:
	FGsSchemaSkillNotifySummonNpc();
	
protected:
	bool operator== (const FGsSchemaSkillNotifySummonNpc& __Other) const
	{
		return FGsSchemaSkillNotifySummonNpc::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillNotifySummonNpc::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillNotifySummonNpc::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillNotifySummonNpcRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillNotifySummonNpcRow() : Super()
	{
	}
	
	const FGsSchemaSkillNotifySummonNpc* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillNotifySummonNpc* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillNotifySummonNpc>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillNotifySummonNpcRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillNotifySummonNpcRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillNotifySummonNpc final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill/NotifyInfo");
	}
	
};
