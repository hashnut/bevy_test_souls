#include "GsCostumeCollection.h"
#include "GsCostumeData.h"
#include "DataSchema/Costume/GsSchemaCostumeCollection.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "Management/ScopeGame/GsCostumeManager.h"

FGsCostumeCollection::FGsCostumeCollection(const FGsSchemaCostumeCollection* data)
{
	if (data)
	{
		for (const auto& e : data->costumeIdList)
		{
			auto conditionId = static_cast<CollectionConditionId>(e.conditionId);
			auto costumeId = static_cast<CostumeId>(e.costumeId);
			auto weak_data = GSCostume()->GetCostumeData(costumeId);
			if (auto costume_data = weak_data.Pin())
			{
				bool complete = _isComplete;

				_collections.Add(CostumeCollectionItem{ conditionId, costumeId, complete, false, weak_data });
			}
		}

		_id = data->id;
		_rewards.Empty(0);
		_rewards.Append(data->passivityIdList);
		_title = data->name;
		_isComplete = _collections.FindByPredicate([](const auto e) { return !e._complete; }) == nullptr;
	}
}

void FGsCostumeCollection::CheckCompleteElement(CostumeId id)
{
	if (_isComplete) return;

	if (auto collection = _collections.FindByPredicate([id](const auto e) { return e._id == id; }))
	{
		if (auto data = collection->_data.Pin())
		{
			if (data->isCollectedCostume())
			{
				if (!collection->_complete)
					collection->_isNew = true;
				collection->_complete = true;
			}
		}
	}
}

const bool FGsCostumeCollection::IsCompleted()
{
	if (!_isComplete)
	{
		_isComplete = _collections.FindByPredicate([](const auto e) { return !e._complete; }) == nullptr;

		if (_isComplete) _isNew = true;
	}

	return _isComplete;
}

const TArray<CostumeCollectionItem>& FGsCostumeCollection::GetCollectionList() const
{
	return _collections;
}

void FGsCostumeCollection::SetCollectionComplete(bool isNew)
{
	if (!_isComplete && isNew) _isNew = isNew;

	_isComplete = true;
	for (auto& e : _collections)
	{
		if (!e._complete && isNew) e._isNew = true;
		e._complete = true;
	}
}

void FGsCostumeCollection::SetCollectionCompleteReset()
{
	_isComplete = false;
}

bool FGsCostumeCollection::CheckCollectionItemComplete(CollectionConditionId id, bool isNew)
{
	if (_isComplete) return false;

	if (const auto collection = _collections.FindByPredicate([id](auto& e) { return e._conditionId == id; }))
	{
		if (!collection->_complete && isNew)
			collection->_isNew = true;
		collection->_complete = true;

		return true;
	}
	return false;
}

bool FGsCostumeCollection::CheckCollectionCompleteReset(CollectionConditionId id)
{
	if (const auto collection = _collections.FindByPredicate([id](auto& e) { return e._conditionId == id; }))
	{
		collection->_complete = false;
		collection->_isNew = false;

		return true;
	}
	return false;
}

void FGsCostumeCollection::RemoveNewMark()
{
	for (auto& e : _collections)
	{
		e._isNew = false;
	}

	_isNew = false;
}

FText FGsCostumeCollection::GetCollectioinRewardsText()
{
	FString statExplain;
	FString statUnitExplain;
	for (PassivityId statPassivity : _rewards)
	{
		if (statPassivity != INVALID_PASSIVITY_ID &&
			FGsEffectStringHelper::GetPassivityEffectString(statPassivity, statUnitExplain, 1))
		{
			statExplain.Append(statUnitExplain);
		}
	}

	return FText::FromString(statExplain);
}