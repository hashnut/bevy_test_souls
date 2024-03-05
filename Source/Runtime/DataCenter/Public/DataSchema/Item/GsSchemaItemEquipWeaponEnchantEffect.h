/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEquipWeaponEnchantEffect.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEquipWeaponEnchantEffect"))
struct DATACENTER_API FGsSchemaItemEquipWeaponEnchantEffect
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Id"))
	int32 Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="EnchantLevel"))
	int32 EnchantLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="LightColor"))
	FVector LightColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="SwordEffect"))
	FSoftObjectPath SwordEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="DaggerEffect"))
	FSoftObjectPath DaggerEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="BowEffect"))
	FSoftObjectPath BowEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="WandEffect"))
	FSoftObjectPath WandEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="orbEffect"))
	FSoftObjectPath orbEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="shotgunEffect"))
	FSoftObjectPath shotgunEffect;
public:
	FGsSchemaItemEquipWeaponEnchantEffect();
	
protected:
	bool operator== (const FGsSchemaItemEquipWeaponEnchantEffect& __Other) const
	{
		return FGsSchemaItemEquipWeaponEnchantEffect::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEquipWeaponEnchantEffect::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEquipWeaponEnchantEffect::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEquipWeaponEnchantEffectRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEquipWeaponEnchantEffectRow() : Super()
	{
	}
	
	const FGsSchemaItemEquipWeaponEnchantEffect* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEquipWeaponEnchantEffect* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEquipWeaponEnchantEffect>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEquipWeaponEnchantEffectRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEquipWeaponEnchantEffectRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEquipWeaponEnchantEffect final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemEquipWeaponEnchantEffect*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEquipWeaponEnchantEffect>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->Id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEquipWeaponEnchantEffect*& OutRow) const
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
		const FGsSchemaItemEquipWeaponEnchantEffect* Temp;
		return !FindRowById(FGsSchemaItemEquipWeaponEnchantEffect().Id, Temp);
	}
#endif
};
