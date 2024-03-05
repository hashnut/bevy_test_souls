#pragma once

#include "EGsSpiritShotState.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsSpiritShotLevelData.h"

struct FGsSchemaSpiritShotLevel;
struct FGsSchemaSpiritShotCategory;
class  FGsSpiritShotLevelData;


/**
 * 천마석 카테고리 데이터. 카테고리 별로 보여줄 데이터와 해당하는 레벨 데이터를 참조
 * 
 * 카테고리는 모든 레벨 데이터를 담고 있을 필요는 없다. 카테고리별로 도달한 레벨은 1개일 것이기 때문이다.
 */
class FGsSpiritShotCategoryData final
{
	friend class FGsSpiritShotManager;

private:
	/** 카테고리 스키마 */
	const FGsSchemaSpiritShotCategory* _categoryData;

	/** 참조할 레벨 데이터. 레벨 데이터 자체는 변경하지 않지만, 가리키는 대상은 변경될 것이다 */
	TSharedRef<FGsSpiritShotLevelData> _levelData;

	/** 현재 상태. 잠금 여부에 따라 UI 도 달라질 수 있다 */
	EGsSpiritShotEnchantState _currentState;


	/************************************************************************/
	/* Constructor                                                          */
	/************************************************************************/
public:
	explicit FGsSpiritShotCategoryData(const FGsSchemaSpiritShotCategory* InSpiritShotCategory, TSharedRef<FGsSpiritShotLevelData> InLevelData, EGsSpiritShotEnchantState InState = EGsSpiritShotEnchantState::UnKnown);


	/************************************************************************/
	/* Getter                                                                */
	/************************************************************************/
public:
	/** 카테고리 Id */
	int32 GetId() const;

	/** 카테고리 이름 */
	FText GetCategoryName() const;

	/** 해당 카테고리 언락 Id */
	int32 GetUnlockContentsId() const;

	/** 탭 별로 보여줄 코어 이미지 경로 */
	FSoftObjectPath GetCoreImagePath() const;

	/** 레벨 데이터 조회 */
	TSharedRef<FGsSpiritShotLevelData> GetLevelData() const;

	/** 현재 상태 조회 (언락, 완료 등) */
	EGsSpiritShotEnchantState GetCurrentState() const;


	/************************************************************************/
	/* Setter (const 가 아닌 Setter 함수는 private 임에 주의하라)           */
	/************************************************************************/
private:
	/** 현재 레벨 데이터 매칭 */
	void SetLevelData(TSharedRef<FGsSpiritShotLevelData> InLevelData);

	/** 언락 상태 변경 */
	void SetCurrentState(EGsSpiritShotEnchantState InState);
};