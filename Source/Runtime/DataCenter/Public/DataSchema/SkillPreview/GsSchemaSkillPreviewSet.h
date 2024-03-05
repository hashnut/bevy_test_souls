/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/SkillPreview/GsSchemaSkillPreviewWeaponSet.h"
#include "DataSchema/User/GsSchemaUserData.h"
#include "GsSchemaSkillPreviewSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SkillPreview/BSGsSkillPreviewSet"))
struct DATACENTER_API FGsSchemaSkillPreviewSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="partyBP", Tooltip="파티원으로 표시될 캐릭터 BP"))
	FSoftObjectPath partyBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="npcBP", Tooltip="피격 대상이될 몬스터 BP"))
	FSoftObjectPath npcBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="partyUserData", Tooltip="파티 UserData 정보"))
	FGsSchemaUserDataRow partyUserData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="npcData", Tooltip="피격 대상이될 몬스터 NpcData정보"))
	FGsSchemaNpcDataRow npcData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="startingDelay", Tooltip="미리보기 스타팅 딜레이 지정"))
	float startingDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="sceneActor", Tooltip="씬 액터 정보"))
	FSoftObjectPath sceneActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="sceneActorPosition", Tooltip="씬 액터 좌표 정보"))
	FVector sceneActorPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="skillPreviewWeaponInfo"))
	TMap<CreatureWeaponType,FGsSchemaSkillPreviewWeaponSetRow> skillPreviewWeaponInfo;
public:
	FGsSchemaSkillPreviewSet();
	
protected:
	bool operator== (const FGsSchemaSkillPreviewSet& __Other) const
	{
		return FGsSchemaSkillPreviewSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillPreviewSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillPreviewSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillPreviewSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillPreviewSetRow() : Super()
	{
	}
	
	const FGsSchemaSkillPreviewSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillPreviewSet* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillPreviewSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillPreviewSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillPreviewSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillPreviewSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SkillPreview");
	}
	
};
