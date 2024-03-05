#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Classes/Collection/GsCollectionGetter.h"

struct FGsCostumeData;
struct FGsSchemaCostumeCollection;
class T1PROJECT_API FGsCostumeCollection : public FGsCollectionFilterData
{
public:
	struct CostumeCollectionItem
	{
		CollectionConditionId _conditionId{ INVALID_COLLECTION_CONDITION_ID };
		CostumeId		_id;
		bool _complete{ false };
		bool _isNew{ false };

		TWeakPtr<FGsCostumeData> _data;
	};

	bool _isNew{ false }; //new는 true일 때, 한번 본 후 false 해둔다.(서버알림 ack에서 처리하지 않아도 됨)
	FText _title;

private:
	TArray<CostumeCollectionItem> _collections;

public:
	virtual ~FGsCostumeCollection() = default;
	FGsCostumeCollection(const FGsSchemaCostumeCollection* data);
	const bool IsCompleted() override;

	void CheckCompleteElement(CostumeId id);

	CollectionId GetCollectionId() { return _id; }
	const TArray<CostumeCollectionItem>& GetCollectionList() const;

	void SetCollectionComplete(bool isNew = false);
	void SetCollectionCompleteReset();
	bool CheckCollectionItemComplete(CollectionConditionId id, bool isNew = false);
	bool CheckCollectionCompleteReset(CollectionConditionId id);

	void RemoveNewMark();
	FText GetCollectioinRewardsText();
	FText GetCollectionName() { return _title; }
};

using CostumeCollectionItem = FGsCostumeCollection::CostumeCollectionItem;