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
#include "GsSchemaWeaponUIInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/BSGsWeaponUIInfo"))
struct DATACENTER_API FGsSchemaWeaponUIInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_weaponType", Tooltip))
	CreatureWeaponType _weaponType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_weaponTypeImagePath", Tooltip))
	FSoftObjectPath _weaponTypeImagePath;
	
public:
	FGsSchemaWeaponUIInfo();
	
protected:
	bool operator== (const FGsSchemaWeaponUIInfo& __Other) const
	{
		return FGsSchemaWeaponUIInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaWeaponUIInfo::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaWeaponUIInfo::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaWeaponUIInfoRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaWeaponUIInfoRow() : Super()
	{
	}
	
	const FGsSchemaWeaponUIInfo* GetRow() const
	{
		const FGsSchemaWeaponUIInfo* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaWeaponUIInfo>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaWeaponUIInfoRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaWeaponUIInfoRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableWeaponUIInfo final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<CreatureWeaponType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaWeaponUIInfo*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaWeaponUIInfo>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->_weaponType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const CreatureWeaponType& InId, OUT const FGsSchemaWeaponUIInfo*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const CreatureWeaponType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaWeaponUIInfo* Temp;
		return !FindRowById(FGsSchemaWeaponUIInfo()._weaponType, Temp);
	}
#endif
};
