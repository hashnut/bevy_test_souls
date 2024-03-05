#pragma once

#include "Classes/GsState.h"
#include "EItemCollectionSlotState.h"
#include "EItemCollectionState.h"
#include "GsItemCollectionSlotState.h"
#include "Item/Collection/EItemCollectionEnumData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Item/Collection/EItemCollectionSubCategoryAttainmentUnit.h"

class FGsItem;
class FGsItemCollectionState;
class FGsItemCollectionSlotState;
struct FDateTime;
struct FGsSchemaItemCollection;
struct StatInfo;
struct FGsSchemaItemCollectionSubCategoryAttainmentUnit;


DECLARE_DELEGATE_FourParams(FOnUpdateByItemCollection, const FGsItemCollectionState*, EItemCollectionSlotState, bool, ItemDBId);

/************************************************************************/
/* using                                                                */
/************************************************************************/
using subCategoryAttainmentUnit = EItemCollectionSubCategoryAttainmentUnit;

class FGsItemCollectionState final : public TGsState<EItemCollectionState>
{
    /************************************************************************/
    /* 멤버                                                                  */
    /************************************************************************/
private:
    const FGsSchemaItemCollection* _itemCollectionData = nullptr;

    CollectionId _collectionId = INVALID_COLLECTION_ID;

    FDateRange _periodDateTimeUtc;

    TArray<FGsItemCollectionSlotState> _innerSlotStates;

	/** 효과 스탯 정보 목록 */
	const TArray<StatInfo> _statInfos;

    /** 검색을 위해 아이템 이름을 캐쉬함.(지연 생성)
     *  실시간 언어 변경 시 캐쉬 텍스트 삭제 필요 체크
     */
    mutable FString _cachedAllItemNamesForSearch;

    /**
     * Lock 이유에 대한 캐쉬
     */
    FText _cachedLockReson;

    /**
     * Stat Effect Text
     */
    FText _cachedStatEffectText;

    bool _isActivatedBookMark = false;

    bool _isUnLock = true;

    subCategoryAttainmentUnit _subCategoryAttainmentUnit = subCategoryAttainmentUnit::None;

    // 2021/08/03 PKT - inner Slot 들에 대한 통합 타입( 타입 큰 값 적용 )
    EItemCollectionSlotState _smallestInnerSlotState = EItemCollectionSlotState::Impossible;

    /************************************************************************/
    /* 대리자                                                                */
    /************************************************************************/
public:
    FOnUpdateByItemCollection OnUpdateByItemCollection;

	bool operator==(const FGsItemCollectionState& InCollection) const
	{
		return (this->_collectionId == InCollection._collectionId);
	}

public:
    explicit FGsItemCollectionState(const FGsSchemaItemCollection* ItemCollectionData);

	/************************************************************************/
	/* TGsState implements                                                  */
	/************************************************************************/
public:
    void Enter() final {/*None*/ }
    void Exit() final {/*None*/ }
    void Update(float In_deltaTime) final {/*None*/ }

    /************************************************************************/
    /* logic                                                                */
    /************************************************************************/
    void Reset();

    void SetCollectionCompleted();

    void SetInnerSlotCompleated(const CollectionConditionId InIds);

    /**
     * 신규아이템 추가나 초기화 이벤트 발생 시, 아이템 컬렉션 상태를 업데이트하고자 할 경우 실행
     */
    void UpdateByNewItem(const FGsItem* InItem);

    // 2021/08/02 PKT - 소지하고 있던 기존 아이템이 업데이트 되었을 경우 실행
    void UpdateByUpdateItem(const FGsItem* InItem);

    // 2021/08/02 PKT - 소지하고 있던 기존 아이템이 삭제 되었을 경우 실행
    void UpdateByRemoveItem(ItemId InItemId, ItemDBId InItemDBId);

    /**
     * 레벨업 / 스킬등의 변화로 락 상태를 업데이트 한다.
     */
    void UpdateByLockState();

    void SetActivatedBookMark(bool InFlag);

private:    
    const FString& GetCachedAllItemNamesForSearch() const;

    //TPair<ItemCollectionUnlockType, int32> GetUnLockCondition() const;

    const FText& GetItemNameByTID(ItemId InItemId) const;

    void UpdateSmallestSlotState();

    /************************************************************************/
    /* 이벤트                                                                */
    /************************************************************************/
private:
    void OnResultByInnerSlotUpdate(const FGsItemCollectionSlotState* InInnerSlot
        , bool InIsMatchCondition
        , ItemDBId InItemDBId);

    void UpdateByProperty();

    void UpdateByType(EItemCollectionState InState);

public:
	CollectionId GetCollectionId() const;

	EItemCollectionSubCategoryAttainmentUnit GetSubCategoryAttainmentUnit() const;

	int32 GetInnerSlotCount() const;

    const FGsItemCollectionSlotState* GetInnerSlotByIndex(const int32 InIndex) const;

    const FGsItemCollectionSlotState* GetInnerSlotByCollectionConditionId(const CollectionConditionId InId) const;

    FText GetNameText() const;

    FText GetStatEffectText() const;

    FText GetDateTimeText() const;

    const TArray<StatInfo>& GetAllStatInfos() const;

    const TArray<int32>& GetPassivityId() const;

    EItemCollectionSlotState GetTheSmallestSlotState() const;
	
    bool SearchByItemName(const FString& InItemName) const;

    bool IsInPeriod(const FDateTime& _dateTimeUtc) const;
    
    bool CanCollectAnyInnerSlot(const FGsItem* InItem) const;

    bool IsEventItemCollection() const;

    bool IsActivatedBookMark() const;

    bool IsUnLock() const;

    FText ReasonLockMessage() const;

private:
	static FDateRange ParseDateTime(const FGsSchemaItemCollection* ItemCollectionData);
	static TArray<FGsItemCollectionSlotState> MakeItemCollectionItemSlotStates(const FGsItemCollectionState* InItemCollection
                                                                            , const FGsSchemaItemCollection* InItemCollectionData);
	static TArray<StatInfo> MakeStatInfos(const TArray<int32>& InPassivityIds);
	static FText MakeStatEffectText(const TArray<int32>& PassivityIds);
};