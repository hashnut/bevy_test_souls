#pragma once

#include "CoreMinimal.h"
#include "ETitleEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedTitleEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


/**
* file		GsTitleData.h
* @brief	칭호 시스템 전용 칭호 Data
* @author	PKT
* @date		2022/01/12
**/
class T1PROJECT_API FGsTitleData final
{
	/************************************************************************/
	/* Delegate                                                             */
	/************************************************************************/
protected:
	DECLARE_DELEGATE_TwoParams(FOnStateChangedTitleData, const FGsTitleData*, ETitleMessageReason);
	
	// 2022/01/13 PKT - 진행 중이지 않은 상탱의 초깃값
	const static int32 NOT_IN_PROGRESS_COUNT = 0;

	// 2022/01/17 PKT - Stat Detail Data
	struct StatDetailInfo
	{
		StatType _type = StatType::NONE;
		FText _name = FText::GetEmpty();
		int32 value = 0;
	};	

	/************************************************************************/
	/* Member                                                               */
	/************************************************************************/
private:
	// 2022/01/12 PKT - Title Static Data
	const struct FGsSchemaTitle* _data = nullptr;

	// 2022/01/17 PKT - Stat Detail Info
	TArray<StatDetailInfo> _statDetailInfos;

	// 2022/01/13 PKT - 카테고리 ID
	TitleCategoryId _categoryId = INVALID_TITLE_CATEGORY_ID;

	// 2022/01/24 PKT - Title 등급에 따른 컬러
	FLinearColor _colorTitle;

	// 2022/06/29 PKT - Title 등급에 따른 HUD 컬러(색이 너무 튄다는 이슈로 컬러값 따로 가짐)
	FLinearColor _colorTitleHUD;

	// 2022/01/14 PKT - Title 상태
	TitleStatus _status = TitleStatus::DISABLED;

	// 2022/01/12 PKT - 진행 중인 정보
	int32 _progressCount = NOT_IN_PROGRESS_COUNT;

	// 2022/01/12 PKT - 해당 칭호를 장착 했는가?
	bool _isEquip = false;

	FText _textDesc;

	// 2022/01/12 PKT - 진행 상태를 보여줘야 할 때가 있어 Desc에 대한 변수를 둔다.
	FText _textCombin;

	// 2022/01/12 PKT - 상태가 변경됨을 알림.
	FOnStateChangedTitleData _OnStateChangedTitleData;


	
public:
	FGsTitleData() = default;
	
	explicit FGsTitleData(const FGsSchemaTitle* InData);


private:
	TArray<StatDetailInfo> GetStatData(const struct FGsSchemaTitle* InData) const;

	TitleCategoryId GetCategoryIdByData(const struct FGsSchemaTitle* InData) const;

	FLinearColor GetTitleColor(const struct FGsSchemaTitle* InData) const;

	FLinearColor GetTitleHUDColor(const struct FGsSchemaTitle* InData) const;

	void DescTextCombine(int32 InCount);

	
	/************************************************************************/
	/* Changed Status                                                       */
	/************************************************************************/
public:
	// 2022/01/12 PKT - 현재 진행 상태 갱신
	void UpdateStatus(TitleStatus InStatus, int32 InConditionCount);

	// 2022/01/12 PKT - 장착 / 비장착
	void SetChangedEquipState(bool InIsEquip);

	// 2022/01/14 PKT - 한번 살펴 본 Title은 더이상 새로운 Title이 아님.(상태 변경)
	void Confirm();


	/************************************************************************/
	/* Get                                                                  */
	/************************************************************************/
public:
	
	// 2022/01/13 PKT - ID
	TitleId GetId() const;

	// 2022/01/13 PKT - Category Data
	TitleCategoryId GetCategoryId() const;

	// 2022/01/13 PKT - Equip Pos Type
	TitleEquipSlot GetEquipSlotType() const;

	// 2022/01/12 PKT - Title
	const FText& GetTitle() const;

	const FLinearColor& GetTitleColor() const;

	const FLinearColor& GetTitleHUDColor() const;

	// 2022/01/12 PKT - Condition Desc
	const FText& GetDesc() const;

	const FText& GetPassivitySubDesc() const;

	// 2022/01/12 PKT - Grade
	TitleGrade GetGrade() const;

	// 2022/01/12 PKT - 획득 여부?
	bool IsAcquire() const;

	// 2022/01/12 PKT - 장착 여부
	bool IsEquip() const;

	// 2022/01/14 PKT - 새롭게 획득한 타이틀 인가?
	bool IsNewAcquire() const;

	// 2022/01/13 PKT - Passivity
	PassivityId GetPassivityId() const;

	int32 GetStatCount() const;

	StatType GetStatType(int32 InIndex) const;

	const FText& GetStatName(int32 InIndex) const;

	int32 GetStatValue(int32 InIndex) const;		

	bool IsContainsStat(StatType InStatType) const;

	// 2022/01/13 PKT - Event Delegate
	FOnStateChangedTitleData& OnStateChangedDelegate();
};