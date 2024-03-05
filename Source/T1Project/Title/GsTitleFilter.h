#pragma once

/**
* file		GsTitleFilter.h
* @brief	Title 에서 사용 하게 될 전용 필터
* @author	PKT
* @date		2022/01/17
**/


#include "CoreMinimal.h"
#include "ETitleEnum.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"

class FGsTitleData;


class T1PROJECT_API IGsTitleFilterConditionBase
{
public:
	virtual ~IGsTitleFilterConditionBase() {}
	virtual void Execute(const TArray<const FGsTitleData*>& InList, OUT TArray<const FGsTitleData*>& OutList) {};
};


// 2022/01/17 PKT - View Type 
class T1PROJECT_API FGsTitleViewTypeFilter : public IGsTitleFilterConditionBase
{
private:
	TFunction<void(const FGsTitleData*, OUT TArray<const FGsTitleData*>&)> _filter = nullptr;

public:
	virtual ~FGsTitleViewTypeFilter() {}

	void SetFilter(ETitleViewListType InType);

public:
	// 2022/01/17 PKT - Filter
	virtual void Execute(const TArray<const FGsTitleData*>& InList, OUT TArray<const FGsTitleData*>& OutList) override;
};


// 2022/01/17 PKT - Stat List
class T1PROJECT_API FGsTitleStatFilter : public IGsTitleFilterConditionBase
{
private:
	TArray<StatType> _filterStats;

public:
	virtual ~FGsTitleStatFilter() {}

	void SetFilter(const TArray<StatType>& InData);

public:
	// 2022/01/17 PKT - Filter
	virtual void Execute(const TArray<const FGsTitleData*>& InList, OUT TArray<const FGsTitleData*>& OutList) override;
};