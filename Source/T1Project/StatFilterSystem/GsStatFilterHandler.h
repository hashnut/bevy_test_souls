#pragma once

#include "CoreMinimal.h"
#include "GsStatFilterUnit.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"

class T1PROJECT_API FGsStatFilterHandler
{
private:
	TArray<FGsStatFilterCategoryItem> _categoryItemSet;
	mutable TArray<FGsStatFilterItem> _statFilterItemSet;

public:
	FGsStatFilterHandler() = default;
	virtual ~FGsStatFilterHandler();

	void Init();
	void Close();
	void OnReconection();

	void OpenWidget(const TArray<StatType>& InContentsAllStatSet, const TArray<StatType>& InSelectedStatSet, TFunction<void(TArray<StatType>&)> InCallback);

protected:
	void Reset();
	void MakeCategoryItems();

public:
	int32 CategoryMaxCount() const;
	const FGsStatFilterCategoryItem* FindCatgegoryItem(int32 InIndex) const;
	
	/**
	 * 2024/1/17 PKT - 주의 : 요 함수를 사용 할때 해당 포인터 사용에 주의. 내부에서 그때그때 만들어서 배열에 넣은 후 리턴하는데 주소가 변경될 수 있음.
	 *                       휘발성으로 사용 할 것!!
	 */
	const FGsStatFilterItem* FindOrAddStatFilterItem(StatType InStatType) const;
};