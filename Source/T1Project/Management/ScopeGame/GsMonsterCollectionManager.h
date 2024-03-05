#pragma once

/**
* file		GsMonsterCollectionManager.h
* @brief	Monster 지식 / 지식 석판에 대한 메니져 클랙스
* @author	PKT
* @date		2023/1/30
**/

#include "CoreMinimal.h"
#include "../Classes/GsManager.h"
#include "../RedDot/GsRedDotCheck.h"
#include "../Currency/GsCostPackage.h"
#include "MonsterKnowledgeCollection/EMonsterKnowledgeSlateCategory.h"
#include "../MonsterKnowledgeCollection/EMonsterKnowledgeEnumData.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeData.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeBook.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeSlate.h"
#include "../PassivityCollection/GsPassivityCollection.h"
#include "IDelegateInstance.h"
#include "../Message/GsMessageContents.h"
#include "../Message/GsMessageGameObject.h"
#include "../Message/GsMessageItem.h"
#include "Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"



class T1PROJECT_API FGsMonsterCollectionManager final : public IGsManager, public IGsRedDotCheck
{
private:
	// 2023/1/30 PKT - 몬스터 지식 정보
	TArray<FGsMonsterKnowledgeBook> _bookHolder;
	// 2023/1/30 PKT - 지식의 석판 정보
	TArray <FGsMonsterKnowledgeSlate> _slateHolder;
	// 2023/1/30 PKT - 패시피티 콜렉션 정보
	FGsPassivityCollectionData _passivityCollectionData;
	// 2023/2/10 PKT - Monster Collection에서 사용 될 모든 Stat
	mutable TSet<StatType> _contentsStatAllSet;
	// 2023/2/21 PKT - Slate Category Set
	mutable TArray<TPair<EMonsterKnowledgeSlateCategory, FText>> _slateCategoryDataSet;

	// 2023/1/31 PKT - 레드닷
	TSet<uint32> _redDotBookSet;
	TSet<MonsterKnowledgeSlateId> _redDotSlateSet;

	// 2023/1/31 PKT - Reset Cost Item
	FGsCostPackage _costResetItem;
	// 2023/1/31 PKT - Book Exp Cost Items
	TArray<FGsCostPackage> _costBookExpItemSet;

	// 2023/2/2 PKT - Book Exp Table
	TMap<ItemId, MonsterKnowledgeExp> _amountIncreaseBookExpItemSet;

	// 2022/10/25 PKT - message Holder
	uint32 _messageHolder = 0;

	// 2023/2/3 PKT - Window View Set Only
	TArray<const FGsMonsterKnowledgeBook*> _bookViewSet;
	TArray<const FGsMonsterKnowledgeSlate*> _slateViewSet;

private:
	TArray<TPair<MessageContentMonsterKnowledge, FDelegateHandle>>	_listMonsterKnowledgeDelegates;
	TArray<TPair<MessageUserInfo, FDelegateHandle>> _listUserInfoDelegates;
	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemActionDelegates;
	TArray<TPair<MessageContentItem, FDelegateHandle>>	_listDepotItemActionDelegates;
	
	MsgGameObjHandleArray	_listGameObjectDelegate;	

public:
	virtual void Initialize() final;
	virtual void Finalize() final;

	virtual bool CheckRedDot(bool bInUpdate = true) override;
	virtual bool CheckRedDot(EMonsterCollectionMainTab InPage) const;
	virtual bool CheckRedDot(MonsterKnowledgeBookGrade InGrade) const;
	virtual bool CheckRedDot(EMonsterKnowledgeSlateCategory InCategory) const;
protected:
	void RegisterMessages();
	void UnegisterMessages();

	void OnSlateUseActiveItem(const struct IGsMessageParam* InParam);
	void OnSlateUseResetItem(const struct IGsMessageParam* InParam);
	void OnUpdateCurrency(uint64 InData);
	void OnInventoryAddItem(FGsItemMessageParamBase& InParam);
	void OnInventoryUpdateItem(FGsItemMessageParamBase& InParam);
	void OnInventoryRemoveItem(FGsItemMessageParamBase& InParam);
	void OnAddToDepot(const struct IGsMessageParam* InParam);
	void OnChangeToDepot(const struct IGsMessageParam* InParam);
	void OnRemoveToDepot(const struct IGsMessageParam* InParam);

	// 2023/1/31 PKT - Monster Knowledge Book
	void OnStateChangedBook(const FGsMonsterKnowledgeBook* InData);
	// 2023/1/31 PKT - Monster Knowledge Slate
	void OnStateChangedSlate(const FGsMonsterKnowledgeSlate* InData);

	void OnChangedSpecialNodeCurrency(const FGsMonsterKnowledgeSlate* InSlate, const class FGsSlateSpecialNode* InSpecialNode);

	void SendMessageToWindow(MessageContentMonsterKnowledge InMessageContents, const struct IGsMessageParam* InParam);

	/**
	 * 서버 -> 클라
	 */
public:
	// 2023/2/10 PKT - 인게임 입장 시 서버 동기화를 위한 Nofi.. 많은 정보를 끊어서 전달 하기 때문에 마지막 패킷에 InIsCompleted = true 온다.
	void Notify_BookDataSet(const TArray<const struct MonsterKnowledge*>& InDataSet, bool InIsCompleted);
	// 2023/2/10 PKT - 실시간 동기화
	void Notify_BookData(uint32 InId, MonsterKnowledgeExp InExp);
	// 2021/12/01 PKT - 단일 몬스터 Item 사용으로 인한 정보 갱신에 대한 응답
	void Ack_BookData(const FGsMonsterCollectionBook& InData);
	// 2023/2/6 PKT - 복수 몬스터 갱신
	void Ack_BookDataSet(const TArray<FGsMonsterCollectionBook>& InData);

	 // 2021/11/19 PKT - 서버 -> 클라 : 지식 석판 해금 상태를 받는다.
	void Notify_ActiveSlate(const TArray<MonsterKnowledgeSlateId>& InActiveSlateIdSet, const TArray<MonsterKnowledgeSlateSlotId>& InActiveNodeIdSet, const TArray<SlateSpecialNodeId>& InSpecialNodeIdList);
	// 2021/11/19 PKT - 서버 -> 클라 : 지식 석판에 대한 해금 요청을 받는다.
	void Ack_OpenSlate(MonsterKnowledgeSlateId InSlateId);
	// 2021/11/19 PKT - 서버 -> 클라 : 지식 석판에 대한 상태(트리 정보 포함)을 받는다.
	void Ack_SlateNodeSet(MonsterKnowledgeSlateId InSlateId, const TArray<MonsterKnowledgeSlateSlotId>& InActiveNodeIdSet);
	void Ack_SlateNodeSet(MonsterKnowledgeSlateId InSlateId, const TArray<MonsterKnowledgeSlateSlotId>& InActiveNodeIdSet, const TArray<SlateSpecialNodeId>& InSpecialNodeIdList);
	// 2021/11/19 PKT - 전체 스탯 초기화
	void Ack_SlatActiveNodeReset();

	/** 석판 > 지식 탐구 특수 노드 개방 응답 */
	void AckActivateSlateSpecialNode(bool InSuccess, MonsterKnowledgeSlateId InSlateId, SlateSpecialNodeId InSpecialNodeId);

public:
	
	// 2023/2/8 PKT - Book
	const void* BookGradeDetailInfo(MonsterKnowledgeBookGrade InGrade) const;
	int32 BuildBookViewSet(MonsterKnowledgeBookGrade InBookGrade);
	const TArray<const FGsMonsterKnowledgeBook*>& BookViewSet() const;	
	const FGsMonsterKnowledgeBook* BookDataById(uint32 InId) const;
	const TArray<FGsCostPackage>& BookExpCostSet() const;	
	MonsterKnowledgeExp AmountIncreasedUsedExpItem(ItemId InItemId) const;

	// 2023/2/8 PKT - Slate
	int32 BuildSlateCategory() const;
	const TPair<EMonsterKnowledgeSlateCategory, FText>* SlateCategoryData(int32 InIndex) const;
	int32 BuildSlateViewSet(EMonsterKnowledgeSlateCategory InCategory, const TArray<StatType>& InFilterStatSet);
	const TArray<const FGsMonsterKnowledgeSlate*>& SlateViewSet() const;
	int32 SlateMaxCount() const;
	const FGsMonsterKnowledgeSlate* SlateDataAt(int32 InIndex) const;
	bool IsHaveCostSlateNodeAllReset() const;
	ItemId ResetCostId() const;
	
	// 2023/2/8 PKT - passivity
	const FGsPassivityCollectionData* PassivityCollectionData() const;
	TSet<StatType>& ContentsStatAll() const;

private:
	void BuildCostItem(FGsCostPackage& OutResetItem
		, TArray<FGsCostPackage>& OutBookExpItemSet
		, TMap<ItemId, MonsterKnowledgeExp>& OutAmountIncreaseBookExpItemSet
		, TMap<MonsterKnowledgeSlateId, TSet<ItemId>>& OutSlateOpenItemSet) const;

	void BuildBookSet(TArray<FGsMonsterKnowledgeBook>& OutBookHolder) const;

	void BuildSlate(const TMap<MonsterKnowledgeSlateId, TSet<ItemId>>& InSlateOpenItemSet
		, TArray <FGsMonsterKnowledgeSlate>& OutSlateHolder) const;
};

#define GsMonsterCollection() UGsScopeHolder::GetGameManagerFType<FGsMonsterCollectionManager>(UGsScopeGame::EManagerType::MonsterCollection)