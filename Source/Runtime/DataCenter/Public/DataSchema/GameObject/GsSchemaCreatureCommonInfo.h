/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/ActorComponentEx/EGsMeshShaderEffect.h"
#include "Runtime/DataCenter/Public/DataSchema/Sound/GsSchemaSoundResData.h"
#include "Runtime/Engine/Classes/Curves/CurveFloat.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class UGsHitEffectData;
#include "GsSchemaCreatureCommonInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/BSGsCreatureCommonInfo"))
struct DATACENTER_API FGsSchemaCreatureCommonInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="name"))
	FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cylinderRadius"))
	int32 cylinderRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cylinderHeight"))
	int32 cylinderHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="moveSpeed"))
	int32 moveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="walkSpeed"))
	int32 walkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="대쉬 상태의 이동속도", DisplayName="dashSpeed"))
	int32 dashSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="NonPlayer용 공통 맞는 이펙트(override)", DisplayName="hitEffectDataOverride"))
	UGsHitEffectData* hitEffectDataOverride;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="NonPlayer용 공통 맞는 사운드(override)", DisplayName="hitSoundDataOverride"))
	FGsSchemaSoundResDataRow hitSoundDataOverride;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="히트가 발생되는 타이밍에 오브젝트 자체 사운드 설정(설정시 무조건 출력)", DisplayName="hitAnimationSoundData"))
	FGsSchemaSoundResDataRow hitAnimationSoundData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="meshShader"))
	TMap<EGsMeshShaderEffect,UCurveFloat*> meshShader;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="원본 상태이상 이펙트 출력 배율 (기본값 \"0\" : 값이 0일 경우 기존 GlobalConstant의 영향을 받아 출력) ", DisplayName="abnormalityParticleSizeRate"))
	float abnormalityParticleSizeRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="원본 Hit 이펙트 출력 배율 (기본값 \"0\" : 값이 0일 경우 기본 이펙트 스케일 적용) ", DisplayName="hitEffectParticleSizeRate"))
	float hitEffectParticleSizeRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="Bounding Direction 계산시 대상이 되는 Radius를 이값으로 변환 (0일 경우 모델의 Capsule Radius)", DisplayName="hitEffectModelRadius"))
	float hitEffectModelRadius;
public:
	FGsSchemaCreatureCommonInfo();
	
protected:
	bool operator== (const FGsSchemaCreatureCommonInfo& __Other) const
	{
		return FGsSchemaCreatureCommonInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCreatureCommonInfo::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCreatureCommonInfo::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCreatureCommonInfoRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCreatureCommonInfoRow() : Super()
	{
	}
	
	const FGsSchemaCreatureCommonInfo* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCreatureCommonInfo* Row = nullptr;
		return Table->FindRow<FGsSchemaCreatureCommonInfo>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCreatureCommonInfoRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCreatureCommonInfoRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCreatureCommonInfo final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCreatureCommonInfo*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCreatureCommonInfo>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaCreatureCommonInfo*& OutRow) const
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
		const FGsSchemaCreatureCommonInfo* Temp;
		return !FindRowById(FGsSchemaCreatureCommonInfo().id, Temp);
	}
#endif
};
