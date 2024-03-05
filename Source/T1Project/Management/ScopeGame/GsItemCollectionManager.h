
#pragma once

/*
* file		GsItemCollectionManager.h
* date		2022/12/28 17:28
* author	PKT
* brief		아이템 컬렉션 Data 메니져
*/

#include "CoreMinimal.h"
#include "../Classes/GsManager.h"
#include "../Classes/Collection/GsCollectionDataContainer.h"
#include "../Classes/Collection/GsCollectionData.h"
#include "../RedDot/GsRedDotCheck.h"

#include "../Item/Collection/GsItemCollectionField.h"
#include "../Item/Collection/GsItemCollectionFilter.h"
#include "../Item/Collection/EItemCollectionEnumData.h"
#include "../Item/Collection/GsItemCollectionCategory.h"
#include "Item/Collection/EItemCollectionSubCategoryAttainmentUnit.h"

#include "../Message/GsMessageItem.h"
#include "../Message/GsMessageContents.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"


class T1PROJECT_API FGsItemCollectionManager final : public IGsManager, public IGsRedDotCheck
{
	/**
	 * Passivity Collection Data
	 */
	class FGsPassivityCollection : public IGsCollectionDataContainer, public IGsCollectionData
	{
	// 2022/12/28 PKT - 효과 받고 있는 Passivity
		TArray<PassivityId> _passivitys;
	public:
		virtual ~FGsPassivityCollection() {/**/ }
	public:	
		void Clear(bool InIsMessage = true);
		void SendUpdateMessage();
		//void Add(PassivityId InId, bool InIsMessage = true);
		void Add(const TArray<int32> InIds, bool InIsMessage = true);
		void Remove(const TArray<int32> InIds, bool InIsMessage = true);
		// 2022/12/29 PKT - Data
		const IGsCollectionData* GetCollectionData() const final;
		// 2022/12/29 PKT - Owner Type
		PassivityOwnerType GetPassivityOwnerType() const final;
		// 2022/12/29 PKT - Count
		int32 GetPassivityCount() const final;
		// 2022/12/29 PKT - find
		PassivityId GetPassivityId(int32 InIndex) const final;
	};


protected:
	TMap<CollectionId, FGsItemCollectionField> _dataHolder;
	// 2022/12/29 PKT - 조건 정보로 조건을 포함하고 있는 Field 정보 빠르게 찾기 위한 용도.
	TMap<CollectionConditionId, CollectionId> _fastFinderFieldSet;
	// 2022/12/30 PKT - ItemId로 Field 정보 찾기
	TMap<ItemId, TSet<CollectionId>> _validMaterialSet;
	// 2022/12/28 PKT - 효과 받고 있는 Passivity
	FGsPassivityCollection _passivityCollection;
	// 2022/12/29 PKT - Filed를 정렬 타입에 의해 그룹핑
	TMap<ECollectionSortOrder, TSet<const FGsItemCollectionField*>> _fieldGroupBySortOrder;
	// 2022/12/29 PKT - 각 항목별 정렬이 필요한 상태인지 체크
	TSet<ECollectionSortOrder> _SortDirty;
	// 2023/1/3 PKT - Main Category Datas
	mutable TArray<FGsItemCollectionCategory> _categoryDataHolder;
	// 2024/1/17 PKT - 해당 컨텐츠에 속하는 모든 Stat 정보
	mutable TSet<StatType> _contentsStatAll;
	// 2022/12/29 PKT - UI Data
	TArray<const FGsItemCollectionField*> _viewFiledSet;

	// 2022/10/25 PKT - message Holder
	uint32 _messageHolder = 0;

protected:
	// 2022/12/27 PKT - 아이템 관련 이벤트..
	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemActionDelegates;
	// 2022/12/27 PKT - 창고 아이템 관련 이벤트
	TArray<TPair<MessageContentItem, FDelegateHandle>>	_listContentItemActionDelegates;

public:
	virtual void Initialize() final;
	virtual void Finalize() final;
	virtual void Update(float inTick);

	// 2022/12/29 PKT - Update RedDot
	bool CheckRedDot(bool bInUpdate = true) override;

	void ClearConditionEntryMaterials();
	void FixedConditionEntryMaterialInfo(CollectionId InCollectionId, CollectionConditionId InCollectionConditionId, ItemId InMaterialId);

	const TArray<FGsItemCollectionCategory>& CategoryRootDatas() const;

	bool IsCategoryRedDot(EItemCollectionSubCategoryAttainmentUnit InCategory) const;

	/**
	 * Window View List 생성
	 * Param1 : InProgressFilter - 전체/완료/미완료 항목만 나열
	 * Param2 : InPropertyFilter - 각 Field 속성에 따라 필터링
	 */
	using PropertyFilter = FGsItemCollectionFilter<FGsItemCollectionField>;
	int32 BuildViewFieldSet(bool InIsBuildSkip, const TSet<ECollectionSortOrder>& InProgressFilter, const TArray<TSharedPtr<PropertyFilter>> InPropertyFilter);

	const TArray<const FGsItemCollectionField*>& ViewFieldSet() const;

	bool IsValidMaterial(const class FGsItem* InMaterial) const;

	const FGsPassivityCollection* PassivityCollectionData() const;

	const FGsItemCollectionField* FindFieldById(CollectionId InId) const;

	const FGsItemCollectionField* FindFieldByConditionId(CollectionConditionId InId) const;

	void FieldAll(TArray<const FGsItemCollectionField*>& OutData) const;

	bool IsValidMaterial(ItemId InMaterial) const;

	TArray<const FGsItemCollectionCondition*> FindConditionsByItemId(const class FGsItem* InMaterial) const;

	TArray<const FGsItemCollectionCondition*> FindConditionsByItemId(ItemId InMaterial) const;

	TSet<StatType>& ContentsStatAll() const;

	/************************************************************************/
	/* 서버 -> 클라                                                          */
	/************************************************************************/
public:
	// 2022/12/29 PKT - 월드 진입 시
	void NotifyInit(const TArray<CollectionData>& InCompletedFieldDataSet, const TArray<CollectionConditionData> InProgressConditions, const TArray<CollectionId> InBookMarkFieldIds);	
	// 2022/12/29 PKT - 컬렉션 상태 변경
	void AckUpdateField(CollectionData InFieldData, CollectionConditionId InConditionId, int32 InCount);
	// 2022/12/29 PKT - 즐겨찾기 상태 변경
	void AckBookMark(const CollectionId InFieldId, bool InIsEnable);
	// 2023/1/10 PKT - 기간제 컬렉션 변경
	void AckEventChanged(const CollectionId InFieldId, bool InIsEnable);

protected:
	void RegisterMessages();
	void UnegisterMessages();

	/**
	 * 재료 소지여부 변경에 따른 이벤트 함수's
	 */
	void OnAddToInventory(struct FGsItemMessageParamBase& InParam);
	void OnChangeToInventory(struct FGsItemMessageParamBase& InParam);
	void OnRemoveToInventory(struct FGsItemMessageParamBase& InParam);

	void OnAddToDepot(const struct IGsMessageParam* InParam);
	void OnChangeToDepot(const struct IGsMessageParam* InParam);
	void OnRemoveToDepot(const struct IGsMessageParam* InParam);

	// 2022/12/27 PKT - 조건 상태 변경에 따른 이벤트 함수
	bool ChangedCost(const class FGsItem* InCost);
	void ChangedCost(ItemId InCostId, ItemDBId InCostDBId);

	// 2022/12/29 PKT - Field 상태 변경 시 받는 이벤트 함수.( 최적화 제일 중요 한 곳!! - 컬렉션의 메인 )
	void OnChangedFieldDelegate(const FGsItemCollectionField* InField, const ECollectionSortOrder InPrevSortOrder);

	// 2022/12/29 PKT - Window 갱신 메시지
	void SendWindowMessage(CollectionId InId);

protected:
	// 2023/1/6 PKT - 이 함수를 사용 하는 곳이 별루 없음(삭제 고려)
	CollectionId FastFindFieldIdByConditionId(CollectionConditionId InId) const;

private:
	void BuildCollectionField();
};

#define GSItemCollection() UGsScopeHolder::GetGameManagerFType<FGsItemCollectionManager>(UGsScopeGame::EManagerType::ItemCollection)