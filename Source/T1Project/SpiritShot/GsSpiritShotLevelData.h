#pragma once

#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsSchemaSpiritShotLevel;
class FGsSpiritShotStatData;
class FGsCostPackage;

/**
 * 천마석 강화에 필요한 CostPackage 생성을 위한 구조체
 */
struct SpiritShotEnchantCost
{
	TArray<int32>	_itemIdList;
	ItemAmount		_itemAmount			= MIN_ITEM_AMOUNT;		// 아이템 갯수
	ItemAmount		_modifyItemAmount	= MIN_ITEM_AMOUNT;		// 확률 조절 할때, 재료 갯수 증감 값.

	explicit SpiritShotEnchantCost() = default;
	explicit SpiritShotEnchantCost(TArray<int32> InItemIdList, int32 InAmount, int32 InModifyItemAmount) :
		_itemIdList(InItemIdList), _itemAmount(InAmount), _modifyItemAmount(InModifyItemAmount)
	{
	}
};

/**
 * 천마석 레벨 데이터. 각 레벨별로 사용할 데이터를 담고 있다.
 * 
 * 스키마를 직접 참조하지 않고 해당 클래스를 만든 이유는 확장성과 편의성 때문이다.
 * 예를 들어, 강화를 5번 실패할 시, 더 이상 실패하지 않는다면... 해당 정보를 레벨 데이터에 캐싱한다
 */

class FGsSpiritShotLevelData final
{
	friend class FGsSpiritShotManager;

private:
	const FGsSchemaSpiritShotLevel* _levelData;

	/** 현재 레벨에 부여되는 스탯 효과 데이터 */
	TSharedRef<FGsSpiritShotStatData> _statData;

	/** 1회 강화시, 필요한 아이템 리스트 */
	TArray<SpiritShotEnchantCost> _enchantRequireItemList;

	/** 다음 도달 레벨의 레벨 id. 기본값 및 마지막일 경우 0 */
	int32 _nextLevelId;

	/** 최대 확률 조절 카운트 */
	int32 _maxModifyCount;

	/************************************************************************/
	/* Constructor                                                          */
	/************************************************************************/
public:
	explicit FGsSpiritShotLevelData(const FGsSchemaSpiritShotLevel* InShotLevelData);

	/************************************************************************/
	/* Getter                                                               */
	/************************************************************************/
public:
	/** 레벨 데이터 Id */
	SpiritShotLevelId GetLevelId() const;

	/** 속한 카테고리 Id */
	SpiritShotCategoryId GetCategoryId() const;

	/** 현재 레벨 */
	int32 GetLevel() const;


	/** 기본 강화 확률 (최솟값) */
	int32 GetDefaultSuccessRate() const;
	
	/** 최대 강화 확률 (최댓값) */
	int32 GetMaxSuccessRate() const;

	/** 강화 실패 시, 레벨이 하락할 확률 */
	int32 GetLevelFallRate() const;

	/** 확률 조절 1회 기준, 확률 증가/감소 값 */
	int32 GetRateModifyValue() const;
	
	/** 최대 확률 조절 횟수 */
	int32 GetMaxModifyCount() const;
	
	/** 1회 강화에 필요한 재료 아이템 리스트 */
	const TArray<SpiritShotEnchantCost>& GetRequireItemList() const;

	/** 강화 실패 시, 인벤토리 지급할 보상 Id */
	RewardId GetFailRewardId() const;

	/** 레벨 달성 시, 지급할 보상의 PassivityEffectStatSet 데이터 Id */
	PassivityId GetPassivityEffectStatSetId() const;

	/** 해당 레벨에 표시할 페이지 */
	FSoftObjectPath GetStepImagePath() const;

	/** 현재 레벨의 스탯 데이터 조회 */
	TSharedRef<FGsSpiritShotStatData> GetStatData() const;

	/** 다음 레벨의 Id 조회 */
	SpiritShotLevelId GetNextLevelId() const;
	
	/** 현재 레벨의 필요 Gold 값 조회 */
	int32 GetRequireGold() const;


	/************************************************************************/
	/* Setter (const 가 아닌 Setter 함수는 private 임에 주의하라)           */
	/************************************************************************/
private:
	/** 다음 레벨의 id 세팅 */
	void SetNextLevelId(int32 InLevel);

	/** 스탯 데이터 세팅. 스탯값이 레벨 누적이라, 매니저 클래스에서 생성하여 넣어줄 것이다 */
	void SetStatData(const TArray<PassivityId>& InIdList);


	/************************************************************************/
	/* Logic                                                                */
	/************************************************************************/
public:
	/** ModifyCount 에 따른 필요 아이템 CostPackage 생성 */
	TSharedRef<FGsCostPackage> CreateItemCostPackage(int32 InModifyCount = 0) const;

	/** ModifyCount 에 따른 필요 아이템 및 재화 CostPackage 생성 */
	TSharedRef<FGsCostPackage> CreateTotalCostPackage(int32 InModifyCount = 0) const;


	/************************************************************************/
	/* Constructor Helper                                                   */
	/************************************************************************/
private:
	static TArray<SpiritShotEnchantCost> MakeRequireItemList(const FGsSchemaSpiritShotLevel* InLevelData);
};