#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"

#include "Message/GsMessageItem.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"

#include "GsGuildItemCollectionState.h"

#include "Classes/Collection/GsCollectionData.h"
#include "Classes/Collection/GsCollectionDataContainer.h"

#include "Item/Collection/GsItemCollectionFilterBase.h"
#include "Item/Collection/EItemCollectionState.h"
#include "Item/Collection/EItemCollectionSlotState.h"
#include "Item/Collection/EItemCollectionEnumData.h"

#include "RedDot/GsRedDotCheck.h"
#include "../Item/Collection/GsItemCollectionFilter.h"

/**
* file		FGsGuildItemCollectionData.h
* @brief	길드 아이템 콜렉션 , 콜렉션 버프, 콜렉션 레드닷 관리
*           XXData라 이름을 칭한 이유는 기존 기사단의 Tab 별 컨텐츠가 XXXData로 관리되어 지고 있음.
*			실제로는 GuildItemCollectionManager 급에 준한 기능을 갖고 있는다.
* @author	PKT
* @date		2021/09/16
**/

// 2022/11/01 PKT - 임시 패킷 수정으로 만든 임시 구조체 이거 나중에 따로 옮길 곳 찾아서 옮길 것!!!!
struct GuildCollectionRegisterData
{
	CollectionConditionId _collectionConditionId = INVALID_COLLECTION_CONDITION_ID;
	FText _registerUserName = FText::GetEmpty();
};

using GuildItemCollectionFilter = FGsItemCollectionFilterBase<FGsGuildItemCollectionState>;

class T1PROJECT_API FGsGuildItemCollectionData final : public IGsCollectionDataContainer, public IGsCollectionData, public IGsRedDotCheck
{
public:

	// 2021/09/30 PKT - 등록 가능 갯수 초기화 값
	const static int32 INVALID_REGISTER_USE_COUNT = -1;

	/************************************************************************/
	/* Using                                                                */
	/************************************************************************/
	using ArrayCollectionList_Ptr = TArray<FGsGuildItemCollectionState*>;
	using ArrayCollectionList_CPtr = TArray<const FGsGuildItemCollectionState*>;

	using SetCollectionList_CPtr = TSet<const FGsGuildItemCollectionState*>;


	// 2021/10/07 PKT - 카테고리 값
	enum class ECategoryValue
	{
		Incomplete = 1 << 0,	// 2021/10/07 PKT - 미완료
		Complete = 1 << 1,		// 2021/10/07 PKT - 완료
		BookMark = 1 << 2,		// 2021/10/07 PKT - 즐겨 찾기
		All = Incomplete | Complete | BookMark,	// 2021/10/07 PKT - 전체
	};

	/************************************************************************/
	/* 멤버 변수                                                             */
	/************************************************************************/
private:

	// 2021/09/27 PKT - 길드 메니져
	const class FGsGuildManager* _guildManager = nullptr;

	// 2021/09/27 PKT - Guild Item Collection Data
	TMap<CollectionId, FGsGuildItemCollectionState> _itemCollectionHolder;

	// 2021/09/27 PKT - 단순 빠르게 검색하기 위한 배열
	ArrayCollectionList_Ptr _itemCollectionList;

	// 2021/09/27 PKT - 아이템 아이디로 콜렉션을 검색 및 수정
	mutable TMap<ItemId, ArrayCollectionList_Ptr> _searchToItemcollectionsByItemId;

	// 2021/10/08 PKT - 아이템 콜렉션 슬롯 아이디로 아이템 콜렉션을 찾는다.
	mutable TMap<CollectionConditionId, FGsGuildItemCollectionState*> _searchToItemcollectionByCollectionConditionId;

	// 2021/10/07 PKT - 즐겨찾기를 관리할 전용 Map
	mutable ArrayCollectionList_CPtr _arrangeItemCollectionByBookMark;

	// 2021/10/07 PKT - 정렬에 맞게 분류 하여 ItemCollection 을 관리
	mutable TMap<ESortPriority, ArrayCollectionList_CPtr> _arrangeItemCollectionList;

	// 2021/09/27 PKT - 정렬이 필요한 타입 저장
	mutable TSet<ESortPriority> _IsSortDirty;

	// 2021/09/27 PKT - UI 에 보여질 Item Collection List
	mutable ArrayCollectionList_CPtr _itemCollectionViewList;

	// 2021/07/31 PKT - 현재 활성화 되어 있는 stat
	TArray<PassivityId> _activatedPassivityIds;

	// 2021/09/27 PKT - 아이템 관련 이벤트..
	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemActionDelegates;
	
	// 2021/09/27 PKT - 창고 아이템 관련 이벤트
	TArray<TPair<MessageContentItem, FDelegateHandle>>	_itemContentsActionTypeDelegates;

	// 2021/10/28 PKT - 게임 오프젝트 관련 이벤트
	MsgGameObjHandleArray _listGameObjectDelegate;

	// 2021/09/29 PKT - 현재 등록한 아이템 갯수
	int32 _registedUseCount = 0;

	
	// 2021/10/08 PKT - Item Collection Build가 전체적으로 완료가 되었는가?
	bool _ItemCollectionBuildSuccess = false;

	/************************************************************************/
	/* Constructor                                                          */
	/************************************************************************/
public:
	FGsGuildItemCollectionData();

	// 2021/09/27 PKT - 입장 및 InGame 상황에서 길드에 접속.
	void Enter(const class FGsGuildManager* InGuildManager);

	// 2021/09/27 PKT - 초기화 상황에서의 리셋.
	void ResetList();

	// 2021/09/27 PKT - 길드 탈퇴
	void Leave();

	/************************************************************************/
	/* IGsCollectionDataContainer implements                                */
	/************************************************************************/
public:
	const IGsCollectionData* GetCollectionData() const;

	/************************************************************************/
	/* IGsCollectionData implements                                         */
	/************************************************************************/
public:
	PassivityOwnerType GetPassivityOwnerType() const final;

	int32 GetPassivityCount() const final;

	PassivityId GetPassivityId(int32 InIndex) const final;

	/************************************************************************/
	/* IGsRedDotCheck implements                                         */
	/************************************************************************/
public:
	// 2021/08/13 PKT - RedDot관련 업데이트
	bool CheckRedDot(bool bInUpdate = true) override;


	/************************************************************************/
	/* Message                                                              */
	/************************************************************************/
private:
	
	// 2021/09/27 PKT - 이벤트 메세지 등록
	void RegisterMessages();

	// 2021/09/27 PKT - 이벤트 메세지 해제
	void UnRegisterMessages();

	// 2021/10/28 PKT - spawn
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam* InParam);

	// 2021/09/27 PKT - 소유하고 있는 아이템 추가
	void AddItem(const FGsItem * InItem);

	// 2021/09/27 PKT - 소유 하고 있는 아이템 변경
	void UpdateItem(const FGsItem * InItem);

	// 2021/09/27 PKT - 소유 하고 있는 아이템 삭제
	void RemoveItem(const ItemId InTId, const ItemDBId InDBId);

	// 2021/09/27 PKT - inventory Item - New ..
	void OnInventoryAddItem(FGsItemMessageParamBase& InParam);

	// 2021/09/27 PKT - inventory Item - Update ..
	void OnInventoryUpdateItem(FGsItemMessageParamBase& InParam);

	// 2021/09/27 PKT - inventory Item - Remove ..
	void OnInventoryRemoveItem(FGsItemMessageParamBase& InParam);

	// 2021/09/27 PKT - Depot Item - New ..
	void OnDepotAddItem(const IGsMessageParam * InParam);

	// 2021/09/27 PKT - Depot Item - Update ..
	void OnDepotUpdateItem(const IGsMessageParam * InParam);

	// 2021/09/27 PKT - Depot Item - Remove ..
	void OnDepotRemoveItem(const IGsMessageParam * InParam);


	/************************************************************************/
	/* 서버 <--> 클라 동기화                                                 */
	/************************************************************************/
public:
	// 2021/09/27 PKT - 서버에게 Item Collection List를 요청
	void Request_ItemCollectionSlotList() const;

	// 2021/09/27 PKT - 서버에게 Item Collection Item 등록 요청
	void Request_ItemCollection_Register(const CollectionConditionId InCollectionConditionId, const ItemDBId InItemId) const;

	// 2021/09/27 PKT - 서버에게 Item Collection 즐겨 찾기 세팅 요청
	void Request_ItemCollection_BookMark(const CollectionId InCollectionId, bool InActive) const;

	// 2021/10/05 PKT - 자신 or 길드원에 의해 ItemCollection 완료
	void Ack_CompleteItemCollectionCondition(const CollectionConditionId InCollectionConditionId
											, const FText& InRegisterName
											, const int32 InRegisterCount = INVALID_REGISTER_USE_COUNT);

	// 2021/10/05 PKT - 길드 아이템 콜렉션 리스트 정보
	void Ack_ItemCollectionSlotListAll(const TArray<GuildItemCollectionInfo>& InBuildItemCollectionList
									, const TArray<GuildCollectionRegisterData>& InRegisterCollectionConditionList
									, const TArray<CollectionId>& bookMarkCollectionList
									, const int32 InRegisterCount);

	// 2021/10/05 PKT - 아이템 콜렉션 북마크 상태 변경
	void Ack_ItemCollectionBookMark(const CollectionId InCollectionId, bool InFlag);
	
	
	/************************************************************************/
	/* Logic                                                                */
	/************************************************************************/
private:
	ESortPriority GetSortPriorityType(EItemCollectionState InState, EItemCollectionSlotState InGlobalSlotState);

	// 2021/09/27 PKT - 모든 Item Collection 정보 삭제
	void ItemCollectionCleanUp();

	void ItemCollectionSlotCleanUp();

	// 2021/10/07 PKT - Item Collection을 Data Schema를 근거로 미리 만들어 둔다.
	void BuildItemCollectionList();

	// 2021/10/26 PKT - Item Collection Condition List를 만든다.
	void BuildItemCollectionConditionList(const TArray<GuildItemCollectionInfo>& InBuildItemCollectionList);

	// 2021/09/27 PKT - Item Collection 상태 변화에 따른 대응 하기 위한 콜렉터
	void ItemCollectionStateChangemCollector(ECollectionStateChangeType InStateChangeType
		, const FGsItemCollectionBase* InItemCollectionBase
		, EItemCollectionState InPrevItemCollectionState
		, EItemCollectionSlotState InPrevItemCollectionSlotState);

	// 2021/09/27 PKT - Item Collection Inner Slot State 변경
	void ArrangeItemCollectionByItemCollectionState(ECollectionStateChangeType InType
		, const FGsGuildItemCollectionState* InItemCollection
		, EItemCollectionState InPrevItemCollectionState
		, EItemCollectionSlotState InPrevItemCollectionSlotState);

	void OnItemCollectionBookMarkToggle(const FGsItemCollectionBase* InItemCollection);

	void ComplectedEffectTicker(const CollectionId InCollectionId) const;


	/************************************************************************/
	/* Send Message To Window                                               */
	/************************************************************************/
private:
	// 2021/09/27 PKT - Window에 갱신 요청
	void SendMessageToWindow(ECollectionStateChangeType InType, TArray<CollectionId> InItemCollectionId);

	// 2021/09/27 PKT - Item Collection View List를 다시 만들어 준다.
	void ReadjustItemCollectionViewList(const ECategoryValue InCategoryValue, OUT ArrayCollectionList_CPtr& InItemCollectionList) const;


	/************************************************************************/
	/* Get...                                                               */
	/************************************************************************/
public:
	// 2021/09/27 PKT - Item Collection 에 등록이 가능한 아이템인가?
	bool IsItemAvailableForItemCollection(const FGsItem* InItem) const;

	// 2021/09/27 PKT - window에 보여줘야 하는 Item Collcetion 을 만든다.
	int32 BuildItemCollectionViewList(const ECategoryValue InCategoryValue, const TArray<TSharedPtr<GuildItemCollectionFilter>>& InFilters) const;

	// 2021/09/27 PKT - 현재 Window에 보여지고 있는 View List
	const ArrayCollectionList_CPtr& ItemCollectionViewList() const;

	const FGsGuildItemCollectionState* FindItemCollectionViewListByCollectionId(CollectionId InId) const;

	const FGsGuildItemCollectionState* FindItemCollectionByCollectionConditionId(CollectionConditionId InCollectionConditionId) const;

	// 2021/10/08 PKT - 등록 가능한 갯수를 카운팅
	void GetRegisterUsedCount(OUT int32* InUseCount, OUT int32* InMaxCount) const;

	// 2021/10/08 PKT - 등록 가능한 횟수를 초과 하였는가?
	bool IsRegisterOverCount() const;
};