/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillCasting.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillChain.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillCollision.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillCutRemain.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillTargetFilter.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillTriggerCondition.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Client/GsSchemaClientSkillLevelInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/NotifyInfo/GsSchemaSkillNotify.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/RootMotion/GsSchemaRootmotionSet.h"
#include "Runtime/DataCenter/Public/DataSchema/Sound/GsSchemaSoundResData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class UGsHitEffectData;
#include "GsSchemaSkillSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/BSGsSkillSet"))
struct DATACENTER_API FGsSchemaSkillSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta=(Tooltip="기획 확인용(추후 변경 여지 있음)", DisplayName="nameText"))
	FString nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="category"))
	SkillCategory category;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="requireWeapon"))
	CreatureWeaponType requireWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="TimeLine 이벤트 사용하지 않음", DisplayName="noTimeline"))
	bool noTimeline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="skillTargetMethod"))
	SkillTargetMethod skillTargetMethod;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="Stat의 AttackSpeed 영향을 받는지 여부", DisplayName="applyAttackSpeed"))
	bool applyAttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="스킬 사용중 이동 가능스킬 여부", DisplayName="movable"))
	bool movable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="스킬 최종 길이", DisplayName="totalRuntime"))
	float totalRuntime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="몬스터들용 (몬스터는 레벨정보가 존재하지 않는다.), PC는 SkillLevelSet정보 이용", DisplayName="cooldownTime"))
	float cooldownTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="skillCoefficientValue"))
	int32 skillCoefficientValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="대상에게 스킬사용시 접근 거리", DisplayName="approachRange"))
	int32 approachRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="스킬 사용 가능거리값", DisplayName="maxApproachRange"))
	int32 maxApproachRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="SkillNotifySet 의 targetFilterInfo를 사용할지 여부. false: SkillSet - targetFilterInfo 사용. true: SkillNotify - targetFilterInfo 사용", DisplayName="useSkillNotifyTargetFilter"))
	bool useSkillNotifyTargetFilter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="SkillNotifySet 의 collisionInfoList를 사용할지 여부. false: SkillSet - collisionInfoList 사용. true: SkillNotify - collisionInfoList 사용", DisplayName="useSkillNotifyCollision"))
	bool useSkillNotifyCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="활성화시 \t첫 번째 노티파이가 명중하면, 해당 스킬의 노티파이는 모두 명중", DisplayName="hitDecisionSameAllNotify"))
	bool hitDecisionSameAllNotify;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="NotTarget 타입에서 스킬 판정 영역 스케일 적용 유무", DisplayName="nontargetApplyCollisionScale"))
	bool nontargetApplyCollisionScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="aniPath"))
	FSoftObjectPath aniPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="스킬 타격시 대상에게 이펙트 출력 정보 (override)", DisplayName="hiteffectData"))
	UGsHitEffectData* hiteffectData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="스킬 타격시 대상에게 이펙트 출력 정보 (override)", DisplayName="hitSoundData"))
	FGsSchemaSoundResDataRow hitSoundData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="skillSequenceType"))
	SkillSequenceType skillSequenceType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="skillChainCondition"))
	SkillChainCondition skillChainCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="스킬 발동 조건(IffApplyType::Self 만 가능, 조건에서는 SkillTargetType::CASTER만 가능)", DisplayName="skillTriggerConditionInfo"))
	FGsSchemaSkillTriggerCondition skillTriggerConditionInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="SkillCategory::CAST 타입에 필요한 구조체 정보", DisplayName="sequenceInfo"))
	FGsSchemaSkillChain sequenceInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="collisionInfoList"))
	TArray<FGsSchemaSkillCollision> collisionInfoList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="targetFilterInfo"))
	FGsSchemaSkillTargetFilter targetFilterInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="켄슬 가능 타이밍 정보", DisplayName="cutRemainInfo"))
	FGsSchemaSkillCutRemain cutRemainInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="castInfo"))
	FGsSchemaSkillCasting castInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="공통(서버/클라) Notify List", DisplayName="notifyInfoList"))
	TArray<FGsSchemaSkillNotify> notifyInfoList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="\"category\": \"BUFF\", 일때 입력되어야 하는 ID 리스트", DisplayName="passivityIdList"))
	TArray<FGsSchemaPassivitySetRow> passivityIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="클라이언트용 Notify List", DisplayName="clientNotifyInfoList"))
	TArray<FGsSchemaSkillNotify> clientNotifyInfoList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기본(평타) 스킬 여부", DisplayName="isNormal"))
	bool isNormal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="코스튬 스킬 체크", DisplayName="isCostume"))
	bool isCostume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="클라용 : ALL타입중 스킬창에 표시 되어야 하는 스킬 정보", DisplayName="isBlessSkill"))
	bool isBlessSkill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade"))
	ItemGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="천마석 연출 효과 유무", DisplayName="spiritShotEffectShow"))
	bool spiritShotEffectShow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="스킬 그룹 아이디. 동일 스킬 슬롯에 동시 등록이 될 수 있는 스킬인지 판단을 위한 Id(동일 Id라면 동시 등록되지 않음). 0일 경우 동시 등록 체크 제외", DisplayName="skillGroupId"))
	int32 skillGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rootMotionId"))
	FGsSchemaRootmotionSetRow rootMotionId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta=(DisplayName="clientLevelInfo"))
	TArray<FGsSchemaClientSkillLevelInfo> clientLevelInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="physicsAnimation"))
	bool physicsAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enableDelayTime"))
	float enableDelayTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="스킬 사용시 대상을 향할때 회전 보간을 사용할지 유무", DisplayName="smoothRotaion"))
	bool smoothRotaion;
public:
	FGsSchemaSkillSet();
	
protected:
	bool operator== (const FGsSchemaSkillSet& __Other) const
	{
		return FGsSchemaSkillSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillSetRow() : Super()
	{
	}
	
	const FGsSchemaSkillSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillSet* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSkillSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSkillSet>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSkillSet*& OutRow) const
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
		const FGsSchemaSkillSet* Temp;
		return !FindRowById(FGsSchemaSkillSet().id, Temp);
	}
#endif
};
