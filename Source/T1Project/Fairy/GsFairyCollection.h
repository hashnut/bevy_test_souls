#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Classes/Collection/GsCollectionGetter.h"

struct FGsFairyData;
struct FGsSchemaFairyCollection;

class T1PROJECT_API FGsFairyCollection : public FGsCollectionFilterData
{
public:
	struct FairyCollectionItem
	{
		CollectionConditionId _conditionId{ INVALID_COLLECTION_CONDITION_ID };
		FairyId		_id;
		SkillLevel	_needLv{ 0 };
		bool _complete{ false };
		bool _isNew{ false };

		TWeakPtr<FGsFairyData> _data;
	};

	bool _isNew{ false }; //new는 true일 때, 한번 본 후 false 해둔다.(서버알림 ack에서 처리하지 않아도 됨)
	FText _title;

private:
	TArray<FairyCollectionItem> _collections;

public:
	virtual ~FGsFairyCollection() = default;
	FGsFairyCollection(const FGsSchemaFairyCollection* data);

	const bool IsCompleted() override;

	void CheckCompleteElement(FairyId id, SkillLevel lv);

	CollectionId GetCollectionId() { return _id;  }
	const TArray<FairyCollectionItem>& GetCollectionList() const;

	void SetCollectionComplete(bool isNew = false);
	bool CheckCollectionItemComplete(CollectionConditionId id, bool isNew = false);

	void RemoveNewMark();

	FText GetCollectioinRewardsText();
	FText GetCollectionName() { return _title; }
};

using FairyCollectionItem = FGsFairyCollection::FairyCollectionItem;

