
#pragma once

/*
* file		GsItemCollectionCondition.h
* date		2022/12/28 11:30
* author	PKT
* brief		Item Collection Filed 내 조건(비용)에 대한 정보
*/

#include "CoreMinimal.h"
#include "EItemCollectionEnumData.h"
#include "../GsItem.h"
#include "../Currency/GsCostPackage.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


class FGsItemCollectionCondition
{
	/**
	 * Type Override
	 */
public:
	// 2022/12/28 PKT - Pair Cost Info - (ItemId / Level)
	using CollectMaterialData = TPair<ItemId, Level>;

	/**
	 * Member Variable
	 */
protected:
	// 2022/12/28 PKT - Id
	CollectionConditionId _id = INVALID_COLLECTION_CONDITION_ID;
	// 2022/12/29 PKT - 완료에 필요한 갯수
	Currency _conditionCount = 1;
	// 2022/12/29 PKT - 등록한 갯수
	Currency _registerCount = 0;
	// 2022/12/28 PKT - State( 진행 상황 )
	ECollectionMaterialActionType _actionType = ECollectionMaterialActionType::Impossible;
	// 2022/12/28 PKT - 비용
	mutable TSharedPtr<FGsCostPackage> _materialPackage;
	// 2022/12/28 PKT - 
	TArray<CollectMaterialData> _collectMaterialSet;
	// 2022/12/28 PKT - 검색 전용 키워드
	mutable FString _serachKeyWord;

	// 2023/1/10 PKT - window 에서 아이콘 출력시 대처 아이템을 지정 할 수 있는데 해당 대처 아이템을 저장하기 위한 변수
	static const int32 UNFIXED_MATERIAL_INDEX = -1;
	int32 _fixedEntryMaterialIndex = UNFIXED_MATERIAL_INDEX;

	/**
	 * Delegate
	 */
public:
	DECLARE_DELEGATE(FOnChangedActionType);
	FOnChangedActionType OnChangedConditionStateDelegate;		// 2022/12/27 PKT - 상태를 관리자에게 알릴 필요가 있을 경우 호출 됨.

	/**
	 * Constructor
	 */
public:
	// 2022/12/29 PKT - Id / 재료 정보 / 필요한 수량
	explicit FGsItemCollectionCondition(CollectionConditionId InCollectionConditionId, const TArray<CollectMaterialData>& InMaterialDataSet, Currency InMaxAmount = 1);
	void Init();
	// 2022/12/29 PKT - 등록 갯수( 등록 되었거나 상태를 초기화 하는 경우 호출)
	void Register(Currency InRegisterCount);	

	void UnRegister();
	// 2022/12/27 PKT - 창고 및 인벤토리 아이템에 변화(Add / Change)
	bool DetectChangeCost(const class FGsItem* InMaterial);
	// 2022/12/27 PKT - 창고 및 인벤토리 아이템에 삭제(Remove)
	void DetectChangeCost(ItemId InMaterialId, ItemDBId InMaterialDBId);	
	// 2023/1/10 PKT - 대표 재료를 선택 한다.
	void FixedEntryMaterial(ItemId InMaterialId);

protected:
	// 2022/12/26 PKT - 비용 판별
	ECollectionMaterialActionType IsWorthMaterial(const class FGsItem* InMaterial) const;
	// 2022/12/27 PKT - 상태 변경
	void ChangedActionType(bool InIsCompleted = false);
	// 2022/12/27 PKT - 재료/비용 검증( 재료 인가? )
	bool IsMaterial(const class FGsItem* InMaterial) const;
	bool IsMaterial(ItemId InMaterialId) const;
	// 2022/12/27 PKT - 재료/비용 검증( 등록 가능 한가? )
	bool IsCanCollect(const class FGsItem* InMaterial) const;

	// 2023/1/10 PKT - 재료 아이디로 재료의 정보를 찾는다.
	const TPair<ItemId, Level> MaterialInfo(ItemId InMaterial) const;

	/************************************************************************/
	/* Getter                                                               */
	/************************************************************************/
public:	
	ECollectionMaterialActionType ActionType() const;

	CollectionConditionId Id() const;	
	
	Currency ConditionCount() const;

	Currency RegisterCount() const;

	const TPair<ItemId, Level> EntryMaterialInfo() const;

	// 2022/12/27 PKT - Data Table에 입력된 모든 재료 정보
	const TArray<CollectMaterialData>& MaterialAll() const;

	// 2022/12/27 PKT - 창고/인벤토리에 소유 하고 있는 모든 재료 아이템 리스트..
	bool HasMaterials(TArray<TWeakPtr<FGsItem>>& OutHasMaterials) const;

	// 2023/1/4 PKT - 창고/인벤토리에 소유 하고 있는 모든 재료 아이템 리스트..
	bool HasCanCollectMaterials(TArray<TWeakPtr<FGsItem>>& OutHasMaterials) const;

	// 2022/12/27 PKT - 검색 전용 필드
	bool ContainsSerachWord(const FString& InKeyWord) const;

	// 2023/1/13 PKT - 비용 판별
	ECollectionMaterialActionType IsPossiveAction(ItemId InMateriId) const;

private:
	const FGsCostPackage* GetMaterialPackage() const;
	//FGsCostPackage* BuildMaterialPackage(const TArray<CollectMaterialData>& InMaterials, Currency InAmount) const;
};