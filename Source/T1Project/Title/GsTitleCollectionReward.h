#pragma once

#include "CoreMinimal.h"
#include "GsTitleCollectionData.h"
#include "RedDot/GsRedDotCheck.h"
#include "GsTitleRewardData.h"



class T1PROJECT_API FGsTitleCollectionReward final : public FGsTitleCollectionData, public IGsRedDotCheck
{
	const static int32 ZERO_ACQUIRE_COUNT = 0;
	/************************************************************************/
	/* Member                                                               */
	/************************************************************************/
private:
	TArray<FGsTitleRewardData> _collectionRewardList;

	// 2022/01/26 PKT - UI에서 새롭게 갱신되어야 List
	TArray<TPair<int32, ETitleRewardState>> _updateEffectList;

	int32 _currentAcquire = ZERO_ACQUIRE_COUNT;

	int32 _maxAcquireCount = ZERO_ACQUIRE_COUNT;
	
	/************************************************************************/
	/* Construct                                                            */
	/************************************************************************/
public:
	FGsTitleCollectionReward();

	/************************************************************************/
	/* Build / Update                                                       */
	/************************************************************************/
private:
	void UpdateCollectionState(bool InReverse, bool IsInitData = false);

	void SendRedDotHUD();

	/************************************************************************/
	/* InIt / Final                                                         */
	/************************************************************************/
public:
	void BuildCollectionReward();

	void Initialized(int32 InAcquireCount, int32 maxAcquireCount);

	void Finalize();

	bool CheckRedDot(bool bInUpdate = true) override;

	void Confirm();

	/************************************************************************/
	/* Add / Rmove                                                          */
	/************************************************************************/
	void AddTitle();

	void RemoveTitle();

	/************************************************************************/
	/* Get / Find                                                           */
	/************************************************************************/
	// 2022/01/13 PKT - 칭호 획득 갯수
	int32 GetAcquire() const;

	// 2022/01/21 PKT - 칭호 최대 갯수
	int32 GetMaxAcquire() const;

	// 2022/01/14 PKT - 컬렉션 전체 갯수
	int32 GetCollectionCount() const;

	const TArray<TPair<int32, ETitleRewardState>>& GetUpdateEffectList() const;

	const FGsTitleRewardData* FindRewardData(int32 InIndex) const;

	// 2022/01/13 PKT - 각 Step별 패시비티 정보
	bool FindPassivityIds(int32 InStep, TArray<PassivityId>& OutPassivityListst);
};