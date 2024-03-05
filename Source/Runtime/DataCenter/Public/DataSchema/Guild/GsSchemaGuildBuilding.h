/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Guild/GsSchemaGuildBuildingLevelData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaGuildBuilding.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/BSGsGuildBuilding"))
struct DATACENTER_API FGsSchemaGuildBuilding
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기획팀확인용", DisplayName="note"))
	FString note;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="건물 시작 레벨", DisplayName="initialLevel"))
	int32 initialLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="guildStatType"))
	GuildStatType guildStatType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="건설시 효과에 대한 설명글", MultiLine="true", DisplayName="openText"))
	FText openText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="GuildStatValue랑 조합 설명글", DisplayName="guildStatValueText"))
	FText guildStatValueText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="서버 버프 제거 관련하여 필요한 값", DisplayName="removeGroupId"))
	int32 removeGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="levelList"))
	TArray<FGsSchemaGuildBuildingLevelData> levelList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="iconPath"))
	FSoftObjectPath iconPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="buffIconPath"))
	FSoftObjectPath buffIconPath;
public:
	FGsSchemaGuildBuilding();
	
protected:
	bool operator== (const FGsSchemaGuildBuilding& __Other) const
	{
		return FGsSchemaGuildBuilding::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaGuildBuilding::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaGuildBuilding::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaGuildBuildingRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaGuildBuildingRow() : Super()
	{
	}
	
	const FGsSchemaGuildBuilding* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaGuildBuilding* Row = nullptr;
		return Table->FindRow<FGsSchemaGuildBuilding>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaGuildBuildingRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaGuildBuildingRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableGuildBuilding final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Guild");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaGuildBuilding*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaGuildBuilding>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaGuildBuilding*& OutRow) const
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
		const FGsSchemaGuildBuilding* Temp;
		return !FindRowById(FGsSchemaGuildBuilding().id, Temp);
	}
#endif
};
