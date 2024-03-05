/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/UI/GsSchemaIconImgInfo.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaWeaponTypeMenu.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/UI/BSGsWeaponTypeMenu"))
struct DATACENTER_API FGsSchemaWeaponTypeMenu
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	CreatureWeaponType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="iconInfo"))
	TArray<FGsSchemaIconImgInfo> iconInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="메뉴 사용 유무", DisplayName="useMenu"))
	bool useMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="order"))
	uint8 order;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="사용하는 곳에서 구분이 필요한 경우 넣어두고 체크한다", DisplayName="option"))
	TArray<EGsHelpContents> option;
public:
	FGsSchemaWeaponTypeMenu();
	
protected:
	bool operator== (const FGsSchemaWeaponTypeMenu& __Other) const
	{
		return FGsSchemaWeaponTypeMenu::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaWeaponTypeMenu::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaWeaponTypeMenu::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaWeaponTypeMenuRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaWeaponTypeMenuRow() : Super()
	{
	}
	
	const FGsSchemaWeaponTypeMenu* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaWeaponTypeMenu* Row = nullptr;
		return Table->FindRow<FGsSchemaWeaponTypeMenu>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaWeaponTypeMenuRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaWeaponTypeMenuRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableWeaponTypeMenu final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("UI");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaWeaponTypeMenu*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaWeaponTypeMenu>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaWeaponTypeMenu*& OutRow) const
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
		const FGsSchemaWeaponTypeMenu* Temp;
		return !FindRowById(FGsSchemaWeaponTypeMenu().id, Temp);
	}
#endif
};
