// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "DataSchema/Reward/GsSchemaRewardBox.h"

struct FGsSchemaRewardItemBagData;
struct FGsSchemaRewardBoxData;
struct FGsSchemaRewardData;
struct FGsRewardIconDataWrapper;

/**
 * 보상 정보 데이터 모음
 */
struct T1PROJECT_API FGsRewardUnboxingData
{
public:
	RewardType _rewardType;
	TPair<Exp, Exp> _expPair; // RewardData의 Key: Min값, Value: Max값
	CurrencyType _currencyType;
	TPair<Currency, Currency> _currencyPair; // RewardData의 Key: Min값, Value: Max값
	TArray<const FGsSchemaRewardItemBagData*> _itemDataList;

	// 보상 표현 정보
	// RewardData의 재화를 아이콘으로 출력할 것인가? (위 _currencyType 정보)
	// ItemBagData 안의 재화를 말하는 것이 아님에 유의
	bool _bUseCurrencyIcon = false; 

public:
	// 여러 보상을 합쳐서 표현할 경우에만 활용되며 평소엔 비어있습니다!
	bool _bIsAdded = false;
	TMap<CurrencyType, TPair<Currency, Currency>> _tempCurrencyMap;
public:
	FGsRewardUnboxingData() 
	{ 
		Clear(); 
	}

public:
	void Clear();
	void SetData(const FGsRewardUnboxingData* InData);

	bool IsExp() const;
	Exp GetCommonExp() const;
	double GetExpPercent() const;
	
	bool IsCurrency() const;
	Currency GetCommonCurrencyAmount() const;
	void GetCurrencyList(OUT TArray<TPair<CurrencyType, Currency>>& OutList) const;	
	void ItemSort();
	void ItemSortCurrencyItemPriority(); // 1순위: 재화형 아이템, 2순위: ItmeGrade

public:
	// 여러 보상을 합쳐서 표현할 경우에만 활용됩니다!
	void AddData(const FGsRewardUnboxingData& InData);
};

//----------------------------------------------------------------------------------------------------------------------
/**
 * 보상관련 static 함수 모음
 */
class T1PROJECT_API FGsRewardHelper
{
public:
	// RewardTId로 테이블 값(FGsSchemaRewardData)을 분해하여 FGsRewardUnboxingData에 저장
	static bool UnboxingRewardData(RewardId InRewardTId, OUT FGsRewardUnboxingData& OutData);
	static bool UnboxingRewardData(const FGsSchemaRewardData* InRow, OUT FGsRewardUnboxingData& OutData);
	static bool UnboxingRewardData(TArray<RewardId>& InRewardTId, OUT FGsRewardUnboxingData& OutData);

	// RewardBoxTId로 테이블 값(FGsSchemaRewardBoxData)을 분해하여 FGsSchemaRewardItemBagData 정보를 채운다
	static bool UnboxingRewardBox(RewardBoxId InRewardBoxTId, OUT TArray<const FGsSchemaRewardItemBagData*>& OutList);

	// 해당 레벨의 Max값(누적 아님)
	static Exp GetLevelMaxExp(Level InLevel);
	// 해당 레벨까지의 누적Max값
	static Exp GetLevelAccumMaxExp(Level InLevel);	
	// 주의: 기준 레벨에 따라 1이 넘는 값이 리턴될 수 있음, 최대레벨 체크 하지 않음. bIsExpAccum으로 InExp가 누적값인지 구분
	static float GetExpRate(Level InLevel, Exp InExp, bool bIsAccumExp);
	// 누적 Exp값으로 해당 레벨을 찾음
	static Level GetLevelByExp(Exp InAccumExp);
	// 서브 보상 타입인지 체크
	static bool IsSubRewardType(RewardType InType);
	
	// PICK_REWARD_BOX_LIST 일 때 FGsRewardUnboxingData를 채움
	static RewardType GetRewardType(RewardId InRewardId);
	static bool GetRewardBoxItemIdPairList(QuestId InQuestId, QuestIndex InQuestIndex, OUT TArray<RewardBoxItemIdPair>& OutList);
	static bool UnboxingRewardData(RewardId InRewardId, const TArray<RewardBoxItemIdPair>& InList, OUT FGsRewardUnboxingData& OutData);

	static const FGsSchemaRewardItemBagData* GetItemBagDataByPairId(const RewardBoxItemIdPair& InPair);

	// 보상 아이콘을 통일되게 표현하기 위한 FGsRewardIconDataWrapper를 리스트로 얻어옴
	static void GetIconDataList(const FGsRewardUnboxingData* InData, OUT TArray<FGsRewardIconDataWrapper>& OutList);


	static bool CheckInventoryState(RewardId InMainRewardTId, RewardId InSubRewardTId = INVALID_REWARD_ID, bool bUseMessage = true);

	static bool GetRewardDataFromId(const RewardId InRewardTId, OUT const FGsSchemaRewardData*& OutRewardData);

	// 각 RewardBox 별 Max Weight (RANDOM_ONE, PICK_ONE, PICK_REWARD_BOX_LIST 일때 사용)
	static int32 GetMaxWeightFromRewardData(const FGsSchemaRewardData* InRewardData);
	// RewardBox 별로 ItemBagList 에 들어갈 모든 아이템의 무게를 합산한 값
	static int32 GetWeightSumFromRewardBox(const FGsSchemaRewardBox& InRewardBox);

	/** 해당 RewardId 에 대해, 해당 RewardId 에 들어간 보상을 수령 가능한지 체크. 티커로 NetText 사용 */
	static bool CheckRewardState(RewardId InRewardTId, bool bUseMessage = true);

private:
	/** 해당 RewardId 에 대해, 해당 RewardId 에 들어간 보상을 수령 가능한지 체크 */
	static bool CheckMainRewardState(RewardId InMainRewardTId, bool bUseMessage = true);

	/** RewardId 2개 에 대해, 해당 RewardId 에 들어간 보상을 수령 가능한지 체크 */
	static bool CheckMainSubRewardState(RewardId InMainRewardTId, RewardId InSubRewardTId, bool bUseMessage = true);

	// 인벤 슬롯 부족 티커 (퀘스트)
	static void InventorySlotErrorTickerQuest(bool bUseMessage = true);
	// 인벤 무게 초과 티커 (퀘스트)
	static void InventoryWeightErrorTickerQuest(bool bUseMessage = true);

	// 인벤 슬롯 부족 티커 (공용)
	static void InventorySlotErrorTickerNet(bool bUseMessage = true);
	// 인벤 무게 초과 티커 (공용)
	static void InventoryWeightErrorTickerNet(bool bUseMessage = true);
};
//----------------------------------------------------------------------------------------------------------------------
