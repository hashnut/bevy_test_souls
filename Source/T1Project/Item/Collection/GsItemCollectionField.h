
#pragma once

#include "CoreMinimal.h"
#include "EItemCollectionEnumData.h"
#include "GsItemCollectionCondition.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


class FGsItemCollectionField
{
	/**
	 * Member Variable
	 */
protected:
	// 2022/12/27 PKT - Schema
	const struct FGsSchemaItemCollection* _schemaData = nullptr;

	CollectionId _id = INVALID_COLLECTION_ID;

	// 2022/12/27 PKT - 컬렉션 조건
	TArray<FGsItemCollectionCondition> _conditionSet;

	// 2022/12/27 PKT - Stat Filter 전용 Data
	mutable TArray<StatType> _statTypeSet;

	// 2022/12/27 PKT - Stat 별 효과 Text 캐싱
	mutable FText _statText = FText::GetEmpty();

	bool _haveValidDateRange = false;
	
	// 2023/1/4 PKT - 유효 기간인가?( 이벤트 필드가 아닐 시 항상 true, 이벤트 필드인 경우 서버 동기화 )
	bool _isEnableFlag = true;

	// 2022/12/27 PKT - 이벤트 컬렉션은 유효 기간을 갖는다.( 주의 : 서버 기준 시!!! )
	mutable FDateRange _validDateRange;

	// 2022/12/27 PKT - 유효 기간 Text 캐싱
	mutable FText _validDateRangeText = FText::GetEmpty();

	// 2022/12/27 PKT - 즐겨찾기 설정 여부( 서버 저장 )
	bool _bookMarkFlag = false;

	// 2022/12/27 PKT - 우선 순위( Window 정렬 우선 순위 )
	ECollectionSortOrder _sortOrder = ECollectionSortOrder::Idel;

	mutable FString _serachKeyWord;

	int64 _endDate = 0;

public:
	// 2022/12/27 PKT - 상태를 관리자에게 알릴 필요가 있을 경우 호출 됨.
	DECLARE_DELEGATE_TwoParams(FOnChangedFiled, const FGsItemCollectionField*, const ECollectionSortOrder);
	FOnChangedFiled OnChangedFieldDelegate;

public:
	explicit FGsItemCollectionField(const CollectionId InId, const struct FGsSchemaItemCollection* InSchema);

	bool operator==(const FGsItemCollectionField& InData) const
	{
		return (this->Id() == InData.Id());
	}

	bool operator==(const FGsItemCollectionField* InData) const
	{
		return (nullptr != InData && this->Id() == InData->Id());
	}
	
	/**
	 * Sync
	 */
	void Init();
	void Update();
	// 2023/1/6 PKT - 완료
	void ForceCompleted();
	// 2023/1/6 PKT - 등록
	void Register(int64 InEndDate, CollectionConditionId InId, Currency InRegisterCount);
	void Register(CollectionConditionId InId, Currency InRegisterCount);	

	void UnRegister();

	void SetEndTime(int64 InEndTime);
	// 2022/12/27 PKT - 즐겨찾기
	void BookMarkEnable(bool InEnable);
	// 2023/1/10 PKT - 유효 기간 설정
	void EventEnableFlag(bool InEnable);
	// 2023/1/10 PKT - Entry Material 설정
	void ClearEntryMaterials();
	void FixedEntryMaterial(CollectionConditionId InId, ItemId InMaterialId);
	
	// 2023/1/6 PKT - 재료 상태 변화
	bool UpdateMaterial(const class FGsItem* InMaterial);
	// 2023/1/6 PKT - 재료 삭제
	void RemoveMaterial(ItemId InMaterialId, ItemDBId InMaterialDBId);
	

protected:
	// 2022/12/27 PKT - 조건 상태 변경에 따른 이벤트 함수
	void OnChangedCondition();

	FGsItemCollectionCondition* FindCondition(CollectionConditionId InId);

	/************************************************************************/
	/* Getter                                                               */
	/************************************************************************/
public:
	const struct FGsSchemaItemCollection* SchemaData() const;

	// 2022/12/27 PKT - Id
	CollectionId Id() const;

	// 2022/12/27 PKT - Title
	const FText& Title() const;

	// 2022/12/27 PKT - Sort Order
	ECollectionSortOrder SortOrder() const;

	// 2022/12/27 PKT - 즐겨 찾기 여부
	bool IsBookMarkEnable() const;

	// 2022/12/27 PKT - 이벤트 Filed 인가?
	bool IsEventField() const;

	// 2022/12/27 PKT - 활성화 되어 있는가?
	bool IsEnableFlag() const;

	// 2023/11/16 PKT - 만료 시간이 있는가?(이벤트 시간과는 다르다.
	bool IsHaveLimitTime() const;

	const FText TextDateTime() const;
	
	// 2023/1/5 PKT - 조건 리스트
	const TArray<FGsItemCollectionCondition>& Conditions() const;

	// 2022/12/27 PKT - PassivitySet
	const TArray<int32>& PassivitySet() const;

	// 2022/12/27 PKT - Stat Type Set
	const TArray<StatType> StatTypeSet() const;

	// 2022/12/29 PKT - Stat Text
	const FText& StatSetText() const;

	bool ContainsSerachWord(const FString& InKeyWord) const;

	// 2023/1/5 PKT - 정렬 규칙에 의거하여 현재 상태가 조건부 등록 일때 진행 사항을 구해서 정렬규칙에 추가 함.
	double ProgressRatio() const;

public:
	// 2022/12/28 PKT - Condition
	void BuildConditionSet();
private:
	// 2022/12/27 PKT - 유효 기간.
	void BuildValidDateRange(FDateRange& OutValidDateRange);

	// 2022/12/27 PKT - 이벤트 기간 Text
	const FText& ValidDateRangeText() const;

	// 2023/11/16 PKT - 
	const FText HaveLimitTimeText() const;
};
