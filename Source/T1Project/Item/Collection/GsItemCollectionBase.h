#pragma once

#include "Classes/GsState.h"
#include "EItemCollectionSlotState.h"
#include "EItemCollectionState.h"
#include "GsItemCollectionSlotState.h"
#include "Item/Collection/EItemCollectionSlotState.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Item/Collection/EItemCollectionEnumData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


class FGsItem;
class FGsItemCollectionBase;
class FGsItemCollectionSlotState;

struct FDateTime;
struct FGsSchemaItemCollection;
struct FGsSchemaPassivitySet;

// 2021/10/07 PKT - Inner Slot 생성시 필요한 정보
struct ParserSlotComponentData
{
	CollectionConditionId collectionConditionId = INVALID_COLLECTION_CONDITION_ID;
	FGsItemCollectionSlotState::MsterialSet materialItemSet;
	int32 rewardId;
};
using SlotComponentDataList = TArray<ParserSlotComponentData>;



/**
* 2021/09/23 PKT - FGsItemCollection 관리자에게 상태 변경을 알림.
* 
* @ param1 		: ECollectionStateChangeType -> 변경 타입
* @ param2 		: const FGsItemCollectionBase* -> Message 를 보내는 주체(this)
* @ param3 		: Prev EItemCollectionState -> 변경 전 아이템 콜렉션 상태
* @ param4 		: Prev EItemCollectionState -> 변경 전 아이템 콜렉션 상태
* @ return 		: void
*/
DECLARE_DELEGATE_FourParams(FOnCollectionStateChange, ECollectionStateChangeType, const FGsItemCollectionBase*, EItemCollectionState, EItemCollectionSlotState);

/**
 * 2021/09/23 PKT - UI에서 Book Mark 인터렉션 발생 시 FGsItemCollectionBase 관리자에게 통보할 대리자.
 * 개인/일반 컬렉션이 상속구조로 변경 되어지는 구조로 인해 생김.
 */
DECLARE_DELEGATE_OneParam(FOnCollectionBookMarkToggle, const FGsItemCollectionBase*);

/**
* file		GsItemCollectionBase.h
* @brief	ItemCollection Base ( 기사단 / 개인 콜렉션의 상위 )
* @author	PKT
* @date		2021/09/16
**/
class FGsItemCollectionBase : public TGsState<EItemCollectionState>
{
protected:
	using Super = FGsItemCollectionBase;

	/************************************************************************/
	/* 멤버                                                                 */
	/************************************************************************/
protected:
	// 2021/09/16 PKT - 고유 아이디
	CollectionId _collectionId = INVALID_COLLECTION_ID;

	// 2021/09/17 PKT - Collection Name
	FText _name = FText::GetEmpty();

	// 2021/09/16 PKT - 유효 기간
	FDateRange _timeOfValidity = FDateRange::Empty();

	// 2021/09/16 PKT - 등록 대상 목록( 아이템 목록 )
	TArray<FGsItemCollectionSlotState> _innerSlots;

	// 2021/08/03 PKT - inner Slot 들에 대한 통합 타입( 타입 큰 값 적용 )
	EItemCollectionSlotState _globalSlotState = EItemCollectionSlotState::Impossible;

	TArray<PassivityId> _passivityIds;
	
	// 2021/09/16 PKT - 효과 스탯 정보 목록
	TArray<StatInfo> _statInfos;

	// 2021/09/17 PKT - 즐겨찾기 Flag
	bool _bookMark = false;

	// 2022/02/21 PKT - 
	bool _isUnableToRegister = false;

	/**
	 * cached data
	 */
	// 2021/09/16 PKT - 빠른 Text 검색 용이 하도록 캐쉬( 콜렉션 이름 + 등록 대상 아이템 이름 )
	mutable FString _cachedTrimVaildNames;

	// 2021/09/16 PKT - Stat 이름
	FText _cachedStatEffectName;

	// 2021/09/27 PKT - 1회성 레드닷의 경우 한번 확인 이후 꺼줄때 사용되는 Flag
	bool _volatilityRedDot = false;


	/************************************************************************/
	/* Delegate                                                             */
	/************************************************************************/
public:
	FOnCollectionStateChange OnCollectionStateChange;
	FOnCollectionBookMarkToggle OnCollectionBookMarkToggle;

	/************************************************************************/
	/* Constructor                                                          */
	/************************************************************************/
public:

	explicit FGsItemCollectionBase(const CollectionId InCollectionId, const FText& InName);

	// 2021/10/14 PKT - Person Item Collection 사용..
	explicit FGsItemCollectionBase(const CollectionId InCollectionId
								, const FText& InName
								, const FString& InStartDate
								, const FString& InEndData
								, const TArray<int32>& InPassivityIds);

	// 2021/10/14 PKT - Guild Item Collection 사용..
	/*explicit FGsItemCollectionBase(const CollectionId InCollectionId
								, const FText& InName
								, const FString& InStartDate
								, const FString& InEndData
								, const TArray<int32>& InPassivityIds
								, const SlotComponentDataList& InCollectionSlotList);*/

	bool operator==(const FGsItemCollectionBase& InCollectionBase) const
	{
		return (this->_collectionId == InCollectionBase._collectionId);
	}

protected:

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
public:
	virtual void SlotRemoveAll();
	virtual void ForceToReset(bool InIsBookNMakrReset = true);
	virtual void ForceToCompleted();

	virtual void SetUnableToRegister(bool InFlag);

	/**
	 * Item Action 에 대한 처리
	 */
	 // 2021/09/17 PKT - 새로운 아이템
	void UpdateByNewItem(const FGsItem* InItem);

	// 2021/09/17 PKT - 소지하고 있던 기존 아이템이 업데이트 되었을 경우 실행
	void UpdateByUpdateItem(const FGsItem* InItem);

	// 2021/09/17 PKT - 소지하고 있던 기존 아이템이 삭제 되었을 경우 실행
	void UpdateByRemoveItem(ItemId InItemId, ItemDBId InItemDBId);

	// 2021/09/17 PKT - 즐격찾기
	void UpdateByBookMarkFlag(bool InFlag);

	// 2021/09/30 PKT - 등록
	void RegisterCollection(CollectionConditionId InCollectionConditionId);

	
protected:
	// 2021/09/17 PKT - InnerSlot 생성
	virtual void CreateCollectionInnerSlotItems(const SlotComponentDataList& InSlotComponentDataList);

	// 2021/09/17 PKT - 버프 효과
	virtual void BuildStatInfo(const TArray<int32>& InPassivityIds);
	
	// 2021/09/23 PKT - 유효기간
	virtual void MakeTimeOfValidity(const FString& InBeginDate, const FString& InEndDate);

	// 2021/09/23 PKT - 슬롯 상태들에 대한 통합 상태
	void UpdateOverrallSlotState();

	// 2021/09/27 PKT - reload Reddot 설정
	void ActiveVolatilityRedDot();


	/************************************************************************/
	/* Invaildate                                                           */
	/************************************************************************/
protected:

	// 2021/09/27 PKT - Item Collection 의 속성이 변경
	virtual void UpdateByState(ECollectionStateChangeType InChangeType);
	

	/************************************************************************/
	/* Event                                                                */
	/************************************************************************/
public:
	// 2021/10/15 PKT - UI에서 BookMark를 Toggle
	virtual void OnBookMarkToggle() const;

	/************************************************************************/
	/* Set/Get                                                              */
	/************************************************************************/
public:
	// 2021/09/17 PKT - Collection ID
	CollectionId GetCollectionId() const;

	// 2021/09/17 PKT - Collection Name
	const FText& GetName() const;

	// 2021/09/17 PKT - 유효한 기간인가?
	bool IsValidPeriod(const FDateTime& _dateTimeUtc) const;

	// 2021/09/17 PKT - 휴효 기간 Text
	FText GetPeriodText() const;

	// 2021/09/17 PKT - 스탯 효과 이름
	const FText& GetStatEffectText() const;

	// 2021/09/17 PKT - Collection 효과 
	const TArray<StatInfo>& GetAllStatInfos() const;

	// 2021/09/17 PKT - Collection Passivity List
	const TArray<PassivityId>& GetPassivityId() const;

	// 2021/09/17 PKT - Collection Items에 의한 Collection 상태값
	EItemCollectionSlotState GetGlobalSlotState() const;

	// 2021/09/17 PKT - Collection Item Count
	int32 GetCollectionItemsCount() const;

	// 2021/09/17 PKT - Index로 Inner Slot 반환
	const FGsItemCollectionSlotState* FindInnerSlotByIndex(int32 InIndex) const;

	// 2021/09/17 PKT - Inner Slot Id 로 Inner Slot 반환
	const FGsItemCollectionSlotState* FindInnerSlotByCollectionConditionId(CollectionConditionId InId) const;

	// 2021/10/15 PKT - Inner Slot Id 로 Index를 얻어온다.
	int32 FindInnerSlotIndexByCollectionConditionId(CollectionConditionId InId) const;

	// 2021/09/17 PKT - 즐겨 찾기 플레그
	bool IsOnBookMark() const;

	// 2021/10/01 PKT - 검색용으로 해당 이름이 컬렉션 아이템에 포함되어 있는지를 검사.
	bool TitleAndCollectionItemNameContains(const FString& InSerachText) const;

	// 2021/09/27 PKT - Item 정보로 해당 아이템이 유효한 아이템인가를 검사 한다.
	virtual bool IsItemAvailableItem(const FGsItem* InItem) const;

	// 2021/10/15 PKT - Item 정보로 해당 아이템이 즉시 등록 가능한 아이템인가를 검사 한다.
	virtual bool IsItemCanCollectItem(const FGsItem* InItem) const;

	// 2021/09/23 PKT - 현재 활성화 되어 있는 상태인가?( 필요 한 경우 상속 받는 곳에서 재정의 필요함.)
	virtual bool IsActive() const { return true; }

	virtual const FText& InActiveMessage() const { return FText::GetEmpty(); }

	// 2021/09/23 PKT - RedDot 대응( 필요 한 경우 상속 받는 곳에서 재정의 필요함.)
	virtual bool IsRedDot() const;

	// 2021/09/27 PKT - 한번 확인한 레드닷을 꺼줘야 할 때 이 함수에 정의 하면 됨.
	virtual void VolatilityRedDotOff();

	// 2022/02/21 PKT - 어떤 조건에 의해 아이템을 등록 할 수 없는 상태인지 체크(기사단 컬렉션 같은 경우 일일 할당량을 채우면 더 이상 등록 할 수 없다.)
	virtual bool IsUnableToRegister() const;

	// 2022/02/23 PKT - 조건에 따라 UI ItemCollection BackGround가 변경
	virtual bool IsHighlightBackGround() const;

private:
	// 2021/09/23 PKT - 유효 기간 설정
	FDateRange ParseDateTime(const FString& InStartDate, const FString& InEndData) const;

	// 2021/09/23 PKT - 패시비티 정보로 Stat과 효과 이름등을 설정
	void MakeStatInfoAndEffectText(const TArray<const FGsSchemaPassivitySet*>& InPassivityList);	
};