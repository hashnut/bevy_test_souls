#pragma once

#include "CoreMinimal.h"

#include "GsState.h"
#include "EItemCollectionSlotState.h"
#include "Item/Collection/EItemCollectionEnumData.h"
#include "Currency/GsCostPackage.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsItem;

class FGsItemCollectionBase;
class FGsItemCollectionSlotState;
struct FGsSchemaRewardData;

/**
* 2021/09/23 PKT - 상태 변화를 CollectionSlot 관리자에게 알림.
*
*/
DECLARE_DELEGATE_OneParam(FOnCollectionInnnerSlotChangeState, ECollectionStateChangeType);

/**
* file		GsItemCollectionSlotState.h
* @brief	ItemCollection Slot 에 대한 Data
* @author	PKT
* @date		2021/09/23
**/
class FGsItemCollectionSlotState : public TGsState<EItemCollectionSlotState>
{
public:
	// 2022/03/03 PKT - 필요한 재료
	using MaterialData = TPair<ItemId, Level>;
	using MsterialSet = TArray<MaterialData>;

	/************************************************************************/
	/* 멤버                                                                  */
	/************************************************************************/
private:
	// 2021/09/23 PKT - 식별 ID
	CollectionConditionId _itemCollectionConditionId;

	// 2022/03/03 PKT - 즉시 등록 가능한 Cost
	TSharedRef<const FGsCostPackage> _immediatePossibleCostPackage;

	// 2022/03/03 PKT - 조건부 등록 가능한 Cost
	TSharedRef<const FGsCostPackage> _conditionalPossibleCostPackage;

	mutable MsterialSet _cachedMaterialDataSet;

	// 2021/10/01 PKT - 검색 전용 유효한 아이템 리스트 이름 전부를 캐시
	mutable FText _cachedMaterialItemName = FText::GetEmpty();

	// 2021/10/06 PKT - Collection 등록시 보상( 개인 X / 길드 O )
	const struct FGsSchemaRewardData* _rewardData = nullptr;

	// 2022/02/24 PKT - 기사단에서는 케릭터 명으로 사용
	FText _textBuffer;

	/************************************************************************/
	/* Delegate                                                             */
	/************************************************************************/
public:
	FOnCollectionInnnerSlotChangeState OnCollectionInnnerSlotChangeState;
	
	/************************************************************************/
	/* Constructor                                                          */
	/************************************************************************/
public:
	
	explicit FGsItemCollectionSlotState(
		CollectionConditionId InCollectionConditionId, const MsterialSet& InMaterialSet);

	explicit FGsItemCollectionSlotState(
		CollectionConditionId InCollectionConditionId, const MsterialSet& InMaterialSet, int32 InrewardId);

	bool operator==(const FGsItemCollectionSlotState& InInnerSlot) const
	{
		return (this->_itemCollectionConditionId == InInnerSlot._itemCollectionConditionId);
	}

	/************************************************************************/
	/* TGsState implements                                                  */
	/************************************************************************/
public:
	void Enter() final {/*None*/ }
	void Exit() final {/*None*/ }
	void Update(float In_deltaTime) final {/*None*/ }

	/************************************************************************/
	/* Logic                                                                */
	/************************************************************************/
private: 
	// 2022/03/03 PKT - Build Cost Package
	TSharedRef<const FGsCostPackage> MakeCostPackage(const MsterialSet& InMaterialSet, bool IsImmediatePossible);

	// 2022/03/03 PKT - 재료 아이템 인가?
	bool IsMaterialForItem(const FGsItem* InItem) const;

	// 2022/03/03 PKT - 등록 할 수 있는 아이템인가?
	bool IsCanCollectForItem(const FGsItem* InItem) const;

	// 2022/03/03 PKT - 아이템으로 변경 가능한 상태를 구한다.
	EItemCollectionSlotState IsExpectedStateForItem(const FGsItem* InItem) const;

	// 2022/03/03 PKT - 상태 변경
	void UpdateState(bool InIsCompleted = false);

	/************************************************************************/
	/* update                                                               */
	/************************************************************************/
public:
	/**
	 * 신규아이템 추가나 초기화 이벤트 발생 시, 아이템 컬렉션 상태를 업데이트하고자 할 경우 실행
	 */
	void UpdateByNewItem(const FGsItem* InItem);

	// 2021/08/02 PKT - 소지하고 있던 기존 아이템이 업데이트 되었을 경우 실행
	void UpdateByUpdateItem(const FGsItem* InItem);

	// 2021/08/02 PKT - 소지하고 있던 기존 아이템이 삭제 되었을 경우 실행
	void UpdateByRemoveItem(ItemId InItemId, ItemDBId InItemDBId);

	// 2021/08/03 PKT - 완료로 설정
	void UpdateByCompleted();

	// 2022/02/24 PKT - 기사단 컬렉션 같은 경우 완료 시 등록한 길드원의 이름을 기록.
	void UpdateByCompleted(const FText& InName);

	/************************************************************************/
	/* Request Func                                                         */
	/************************************************************************/
public:
	// 2021/09/23 PKT - 상태 초기화
	void Reset();

	// 2021/09/23 PKT - 콜렉션 식별 값
	CollectionConditionId GetItemCollectionConditionId() const;

	// 2021/09/23 PKT - 메인 아이템 정보
	const MaterialData GetMainMaterialData() const;

	// 2022/03/03 PKT - 메인 아이템 + 대체 아이템에 대한 정보
	const MsterialSet& GetMaterialDataSet() const;

	// 2022/03/03 PKT - 
	const FText& GetMaterialItemNames() const;

	// 2021/10/06 PKT - 등록 시 보상이 있는가?
	bool IsHaveReward() const;

	// 2021/10/06 PKT - 등록 시 보상 정보
	const struct FGsSchemaRewardData* GetRewardData() const;

	const FText& GetTextBuffer() const;

	// 2021/09/23 PKT - 즉시 등록 가능한 아이템인가?
	bool IsCanCollect(const FGsItem* InItem) const;

	// 2021/09/23 PKT - 등록기 가능한 유효한 아이템 인가?
	bool IsMaterial(const FGsItem* InItem) const;

	// 2022/03/07 PKT - 즉시 등록 가능한 아이템 목록
	void GetHasMaterialItemIdSet(TArray<TPair<EItemCollectionSlotState, MaterialData>>& OutMaterialSet) const;
	//void GetHasImmediatePossibleTIdSet(MsterialSet& OutMaterialSet) const;

	//void GetHasConditionalPossibleTIdSet(TArray<ItemId> OutDataSet) const;

	//void GetHasItemTIdSet(TArray<ItemId>& OutImmediatePossibleTIdSet, TArray<ItemId>& OutConditionalPossibleTIdSet) const;

	// 2022/03/03 PKT - 해당 Slot과 관련 있는 아이템에 대한 DBId전달
	void GetHaveItemDBIdSet(TArray<ItemDBId>& OutImmediatePossibleDBIdSet, TArray<ItemDBId>& OutConditionalPossibleDBIdSet) const;

private:
	void MakeRewardData(const int32 InRewardId);
};