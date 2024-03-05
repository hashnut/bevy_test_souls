/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/GsSchemaAbnormalityEffect.h"
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/GsSchemaAbnormalityMaterialInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/GsSchemaDurationProbabilityInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Common/GsSchemaScreenFilterInfo.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAbnormalitySet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Abnormality/BSGsAbnormalitySet"))
struct DATACENTER_API FGsSchemaAbnormalitySet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="name"))
	FName name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="인게임에 노출되는 상태이상 이름", DisplayName="ingameName"))
	FText ingameName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ingameDesc"))
	FText ingameDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="categoryId"))
	int32 categoryId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="AbnormalityStackType 참조", DisplayName="abnormalityStackType"))
	AbnormalityStackType abnormalityStackType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupId"))
	int32 groupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="stackGroupId"))
	int32 stackGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectGroupIdList"))
	TArray<int32> effectGroupIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="removeGroupIdList"))
	TArray<int32> removeGroupIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="우선순위", DisplayName="priority"))
	int32 priority;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="stack 될 수 있는 max Count", DisplayName="maxStackCount"))
	int32 maxStackCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="ax stack 에 도달 했을 때, 변경될 Abnormality Id (기존 것은 Remove 되고, 이 값이 add 된다) - 순환참조 문제로 RowPick불가", DisplayName="changeAbnormalityId"))
	int32 changeAbnormalityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isBuff"))
	bool isBuff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="설정된 무기 장착 시 상태이상 부여", DisplayName="requireWeaponType"))
	CreatureWeaponType requireWeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="removeConditionLogout"))
	bool removeConditionLogout;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="removeConditionDeath"))
	bool removeConditionDeath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="removeConditionRequireWeaponCheck"))
	bool removeConditionRequireWeaponCheck;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="removeConditionDespawn"))
	bool removeConditionDespawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="removeConditionDurationTimeCheck"))
	bool removeConditionDurationTimeCheck;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="removeConditionSystemForceRemovable"))
	bool removeConditionSystemForceRemovable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="removeConditionUseSkill"))
	bool removeConditionUseSkill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="removeConditionUseActiveSkill"))
	bool removeConditionUseActiveSkill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="스킬이 종료될 때 제거", DisplayName="removeConditionSkillEnd"))
	bool removeConditionSkillEnd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="true 설정된 이상상태는 페어리 교체시 해제", DisplayName="removeConditionChangeFairy"))
	bool removeConditionChangeFairy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="true 일 경우 안전 지대 진입 시 상태이상 제거", DisplayName="removeConditionInTown"))
	bool removeConditionInTown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="removeAbnormalityId"))
	int32 removeAbnormalityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip=" 프리셋 교체 시 삭제 플래그", DisplayName="removeConditionChangePreset"))
	bool removeConditionChangePreset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="true :  회귀시 모든 상태이상 제거시 영향 받지 않고 stable, false : stable 하지 않음", DisplayName="stableConditionOnReturnState"))
	bool stableConditionOnReturnState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="effectIdList"))
	TArray<int32> effectIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="상태이상 지속시간 최소값(고정시간은 MinMax 동일한 값으로 지정)", DisplayName="durationMin"))
	float durationMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="상태이상 지속시간 최대값(고정시간은 MinMax 동일한 값으로 지정)", DisplayName="durationMax"))
	float durationMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="durationProbability 참조 여부 결정 (서버전용)", DisplayName="useProbabilityDuartion"))
	bool useProbabilityDuartion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="useProbabilityDuartion : true일 경우 참조 (서버전용)", DisplayName="durationProbability"))
	TArray<FGsSchemaDurationProbabilityInfo> durationProbability;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tickTime"))
	float tickTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="상태이상 아이콘 이미지 경로", DisplayName="iconPath"))
	FSoftObjectPath iconPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="smallIconPath"))
	FSoftObjectPath smallIconPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="아이콘등 UI에 표시여부", DisplayName="hideUI"))
	bool hideUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="vfx, sound 처리 관련", DisplayName="effectInfo"))
	TArray<FGsSchemaAbnormalityEffect> effectInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="screenFilter"))
	FGsSchemaScreenFilterInfo screenFilter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="marterialInfo"))
	FGsSchemaAbnormalityMaterialInfo marterialInfo;
public:
	FGsSchemaAbnormalitySet();
	
protected:
	bool operator== (const FGsSchemaAbnormalitySet& __Other) const
	{
		return FGsSchemaAbnormalitySet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAbnormalitySet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAbnormalitySet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAbnormalitySetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAbnormalitySetRow() : Super()
	{
	}
	
	const FGsSchemaAbnormalitySet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAbnormalitySet* Row = nullptr;
		return Table->FindRow<FGsSchemaAbnormalitySet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAbnormalitySetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAbnormalitySetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAbnormalitySet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Abnormality");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaAbnormalitySet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaAbnormalitySet>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaAbnormalitySet*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int32& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaAbnormalitySet* Temp;
		return !FindRowById(FGsSchemaAbnormalitySet().id, Temp);
	}
#endif
};
