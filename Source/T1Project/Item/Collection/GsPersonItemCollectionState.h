#pragma once

#include "GsItemCollectionBase.h"
#include "EItemCollectionEnumData.h"
#include "Item/Collection/EItemCollectionSubCategoryAttainmentUnit.h"

struct FGsSchemaItemCollection;
struct FGsSchemaItemCollectionSubCategoryAttainmentUnit;
struct FGsSchemaItemCollectionSubCategoryStatUnit;

/**
* file		GsPersonItemCollectionState.h
* @brief	GsItemCollectionBase를 상속받은 개인 아이템 콜렉션( 기사단 콜렉션이 추가 됨에 따른 리뉴얼 )
* @author	PKT
* @date		2021/09/27
**/
class FGsPersonItemCollectionState final : public FGsItemCollectionBase
{
	/************************************************************************/
	/* 멤버                                                                  */
	/************************************************************************/
private:
	// 2021/09/27 PKT - 스키마 Data
	const FGsSchemaItemCollection* _itemCollectionData = nullptr;

	// 2021/09/27 PKT - Category Data
	EItemCollectionSubCategoryAttainmentUnit _subCategoryAttainmentUnit = EItemCollectionSubCategoryAttainmentUnit::None;

	// 2021/09/27 PKT - LockState
	bool _isUnLock = true;

	// 2021/09/27 PKT - Lock에 대한 이유
	FText _cachedLockReson;

	/************************************************************************/
	/* Constructor                                                          */
	/************************************************************************/
public:
	explicit FGsPersonItemCollectionState(const FGsSchemaItemCollection* InSchemaItemCollection);

	/************************************************************************/
	/* Logic                                                                */
	/************************************************************************/
public:
	virtual void ForceToReset(bool InIsBookNMakrReset = true) override;
	virtual void ForceToCompleted() override;

	// 2021/09/27 PKT - 활성화 상태 업데이트
	void UpdateByActiveState();

	/************************************************************************/
	/* Set/Get                                                              */
	/************************************************************************/
public:
	// 2021/10/01 PKT - Event 타입인가?
	bool IsEventItemCollection() const;

	// 2021/09/23 PKT - 현재 활성화 되어 있는 상태인가?
	virtual bool IsActive() const override;

	const FText& InActiveMessage() const override;

	EItemCollectionSubCategoryAttainmentUnit GetSubCategoryAttainmentUnit() const;

private:
	void MakeItemCollectionInnerSlot(const FGsSchemaItemCollection* InSchemaItemCollection, OUT SlotComponentDataList* OutInnerSlots);
};