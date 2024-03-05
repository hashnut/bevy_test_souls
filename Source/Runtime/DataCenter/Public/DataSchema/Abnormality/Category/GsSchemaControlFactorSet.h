/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaControlFactorSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Abnormality/Category/BSGsControlFactorSet"))
struct DATACENTER_API FGsSchemaControlFactorSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Desc"))
	FString Desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="activeImmediateStop"))
	bool activeImmediateStop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureActionTypeMoveDefault"))
	bool creatureActionTypeMoveDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureActionTypeMoveRootMotion"))
	bool creatureActionTypeMoveRootMotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureActionTypeMoveLocation"))
	bool creatureActionTypeMoveLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureActionTypeMoveFear"))
	bool creatureActionTypeMoveFear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureActionTypeSkillBasic"))
	bool creatureActionTypeSkillBasic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureActionTypeSkill"))
	bool creatureActionTypeSkill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureActionTypeSkillDash"))
	bool creatureActionTypeSkillDash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureActionTypeSkillBuff"))
	bool creatureActionTypeSkillBuff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureActionTypeItem"))
	bool creatureActionTypeItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureActionTypeAffectItemGroupId"))
	int32 creatureActionTypeAffectItemGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureActionTypeSocial"))
	bool creatureActionTypeSocial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureActionTypeWarp"))
	bool creatureActionTypeWarp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureActionTypeTakeDamage"))
	bool creatureActionTypeTakeDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureActionTypeChangePahse"))
	bool creatureActionTypeChangePahse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureActionTypeRecovery"))
	bool creatureActionTypeRecovery;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureActionTypeChangeTarget"))
	bool creatureActionTypeChangeTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="해당 상태이상에 탈 것 이용에 대한 행동 제약 설정 여부", DisplayName="creatureActionTypeMountVehicle"))
	bool creatureActionTypeMountVehicle;
public:
	FGsSchemaControlFactorSet();
	
protected:
	bool operator== (const FGsSchemaControlFactorSet& __Other) const
	{
		return FGsSchemaControlFactorSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaControlFactorSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaControlFactorSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaControlFactorSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaControlFactorSetRow() : Super()
	{
	}
	
	const FGsSchemaControlFactorSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaControlFactorSet* Row = nullptr;
		return Table->FindRow<FGsSchemaControlFactorSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaControlFactorSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaControlFactorSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableControlFactorSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Abnormality/Category");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaControlFactorSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaControlFactorSet>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaControlFactorSet*& OutRow) const
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
		const FGsSchemaControlFactorSet* Temp;
		return !FindRowById(FGsSchemaControlFactorSet().id, Temp);
	}
#endif
};
