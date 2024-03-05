/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Dungeon/GsSchemaEventDungeonScheduleData.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "Runtime/DataCenter/Public/DataSchema/Dungeon/GsSchemaDungeonData.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapSpotInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaResetTimeTicketConfig.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "Runtime/DataCenter/Public/DataSchema/Unlock/GsSchemaUnlockContents.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaDungeonGroupData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Dungeon/BSGsDungeonGroupData"))
struct DATACENTER_API FGsSchemaDungeonGroupData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="던전 대표 이름", DisplayName="dungeonGroupName"))
	FText dungeonGroupName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="던전 대표 설명", DisplayName="dungeonGroupDesc"))
	FText dungeonGroupDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dungeonCategory"))
	DungeonCategory dungeonCategory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bindingType"))
	ContentsBindingType bindingType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="던전 대표 이미지", DisplayName="dungeonGroupSpriteImg"))
	FSoftObjectPath dungeonGroupSpriteImg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="dungeonGroupTexutreImg"))
	FSoftObjectPath dungeonGroupTexutreImg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="던전 출력 순서 - 낮은 숫자 일수록 리스트 상단에 표시", DisplayName="listOrder"))
	int32 listOrder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="해당 던전을 리스트에 출력할지의 여부 - 특정 던전을 만들어 두고, 필요 시에만 오픈하도록 처리", DisplayName="visible"))
	bool visible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="deadTimeCountCheck"))
	bool deadTimeCountCheck;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기본 제공 던전 플레이 가능 시간 (분)", DisplayName="baseTimeLimit"))
	int32 baseTimeLimit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="최대 추가 가능 던전 플레이 시간 (분)", DisplayName="maxAddTimeLimit"))
	int32 maxAddTimeLimit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="던전 시간 종료 후 임의의 여유 시간 (분) - 진행 중인 전투를 마무리 할 수 있는 여유 제공", DisplayName="marginTime"))
	int32 marginTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="추가 시간 충전 아이템 종류", DisplayName="chargeCostItemId"))
	FGsSchemaItemCommonRow chargeCostItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="시간 초기화 주기", DisplayName="resetCycling"))
	FGsSchemaResetTimeTicketConfigRow resetCycling;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="레벨/층 구분자", DisplayName="dungeonGroupType"))
	EGsDungeonGroupType dungeonGroupType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="던전 목록", DisplayName="dungeonList"))
	TArray<FGsSchemaDungeonDataRow> dungeonList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="UIContinentalMapSpotTable"))
	FGsSchemaMapSpotInfo UIContinentalMapSpotTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="시간 충전 불가 처리", DisplayName="disableTimeCharge"))
	bool disableTimeCharge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventDungeonSchedule"))
	FGsSchemaEventDungeonScheduleData eventDungeonSchedule;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dungeonMainRewardId"))
	FGsSchemaRewardDataRow dungeonMainRewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="dungeonKey"))
	FGsSchemaItemCommonRow dungeonKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="unLockContentsId"))
	FGsSchemaUnlockContentsRow unLockContentsId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="checkQuest"))
	bool checkQuest;
public:
	FGsSchemaDungeonGroupData();
	
protected:
	bool operator== (const FGsSchemaDungeonGroupData& __Other) const
	{
		return FGsSchemaDungeonGroupData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaDungeonGroupData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaDungeonGroupData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaDungeonGroupDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaDungeonGroupDataRow() : Super()
	{
	}
	
	const FGsSchemaDungeonGroupData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaDungeonGroupData* Row = nullptr;
		return Table->FindRow<FGsSchemaDungeonGroupData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaDungeonGroupDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaDungeonGroupDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableDungeonGroupData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Dungeon");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaDungeonGroupData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaDungeonGroupData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaDungeonGroupData*& OutRow) const
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
		const FGsSchemaDungeonGroupData* Temp;
		return !FindRowById(FGsSchemaDungeonGroupData().id, Temp);
	}
#endif
};
