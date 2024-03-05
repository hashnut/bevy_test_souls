/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/User/WeaponEffect/GsSchemaWeaponPeaceEffect.h"
#include "Runtime/DataCenter/Public/DataSchema/User/WeaponSocket/GsSchemaWeaponSocketInfo.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/GsSchemaPlayerShape.h"
#include "Runtime/DataCenter/Public/DataSchema/Sound/GsSchemaSoundResData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class UGsHitEffectData;
#include "GsSchemaUserData.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="LocalPlayer Info Table", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/BSGsUserData"))
struct DATACENTER_API FGsSchemaUserData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="commonInfoId"))
	int32 commonInfoId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="캐릭터 외형정보", DisplayName="shapeId"))
	FGsSchemaPlayerShapeRow shapeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="genderType"))
	CreatureGenderType genderType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="플레이어용 공통 맞는 이펙트(override)", DisplayName="HitEffectDataOverride"))
	UGsHitEffectData* HitEffectDataOverride;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="플레이어용 공통 맞는 사운드(override)", DisplayName="HitSoundDataOverride"))
	FGsSchemaSoundResDataRow HitSoundDataOverride;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="weaponSocketList"))
	TMap<CreatureWeaponType,FGsSchemaWeaponSocketInfo> weaponSocketList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="weaponPeaceEffectList"))
	TMap<CreatureWeaponType,FGsSchemaWeaponPeaceEffect> weaponPeaceEffectList;
public:
	FGsSchemaUserData();
	
protected:
	bool operator== (const FGsSchemaUserData& __Other) const
	{
		return FGsSchemaUserData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaUserData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaUserData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaUserDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaUserDataRow() : Super()
	{
	}
	
	const FGsSchemaUserData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaUserData* Row = nullptr;
		return Table->FindRow<FGsSchemaUserData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaUserDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaUserDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableUserData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("User");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaUserData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaUserData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->commonInfoId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaUserData*& OutRow) const
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
		const FGsSchemaUserData* Temp;
		return !FindRowById(FGsSchemaUserData().commonInfoId, Temp);
	}
#endif
};
