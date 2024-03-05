// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"


/**
 * DeliveryBox 데이터. 
 * 장비 복구, 페어리 및 코스튬 재합성 데이터
 */
class T1PROJECT_API FGsItemDeliveryBoxData final
{
protected:
	TArray<ItemDataCreated> _repairableItemList;
	TArray<FairyGrade> _recomposableFairyList;
	TArray<CostumeGrade> _recomposableCostumeList;

	ItemGrade _minGrade;
	ItemGrade _maxGrade;

	FDateTime _startTime;
	FDateTime _endTime;

	//ItemCategoryMain _repairCategoryMain;
	ContentsEventDeliveryBoxType _deliveryBoxType;

	ItemDBId _usedBoxItemDBId = INVALID_ITEM_DB_ID;

	uint64 _waitSoundId;
	uint64 _successSoundId;

public:
	FGsItemDeliveryBoxData() = default;
	~FGsItemDeliveryBoxData() = default;

public:
	void Initialize();
	void Reset();

public:
	void SetEquipData(const TArray<ItemDataCreated>& InItemList);
	void SetFairyData(const TArray<FairyGrade>& InGradeList);
	void SetCostumeData(const TArray<CostumeGrade>& InGradeList);
	void SetUsedBoxItemDBId(ItemDBId InItemDBId);

	const TArray<ItemDataCreated>& GetRepairableItemList();
	const TArray<FairyGrade>& GetRecomposableFairyList();
	const TArray<CostumeGrade>& GetRecomposableCostumeList();

	void GetFairyGradeRange(OUT FairyGrade& OutMinGrade, OUT FairyGrade& OutMaxGrade);
	void GetCostumeGradeRange(OUT CostumeGrade& OutMinGrade, OUT CostumeGrade& OutMaxGrade);

	FDateTime GetStartTime();
	FDateTime GetEndTime();

	bool IsActiveFairy();
	bool IsActiveCostume();

	ItemDBId GetUsedBoxItemDBId();

public:
	void ShowRepairResult(const ItemDataCreated& InItemData);

protected:
	void PlayEffectSound(bool InIsWait);
	void StopEffectSound(bool InIsWait);
};