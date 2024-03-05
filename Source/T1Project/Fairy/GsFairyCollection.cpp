#include "GsFairyCollection.h"
#include "GsFairyData.h"
#include "DataSchema/Fairy/GsSchemaFairyCollection.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "Management/ScopeGame/GsFairyManager.h"

FGsFairyCollection::FGsFairyCollection(const FGsSchemaFairyCollection* data) 
{
	if (data)
	{
		for (const auto& e : data->fairyIdLevelList)
		{
			auto weak_data = GSFairy()->GetFairyData(e.fairyId);
			if (auto fairy_data = weak_data.Pin())
			{
				bool complete = _isComplete;

				_collections.Add(FairyCollectionItem{ static_cast<CollectionConditionId>(e.conditionId), 
					static_cast<FairyId>(e.fairyId), e.fairyLevel, complete, false, weak_data });
			}
		}

		_id = data->id;
		_rewards.Empty(0);
		_rewards.Append(data->passivityIdList);
		_title = data->name;
		_isComplete = _collections.FindByPredicate([](const auto e) { return !e._complete; }) == nullptr;
	}
}

void FGsFairyCollection::CheckCompleteElement(FairyId id, SkillLevel lv)
{
	if (_isComplete) return;

	if (auto collection = _collections.FindByPredicate([id, lv](const auto e) { return e._id == id && e._needLv <= lv; }))
	{
		if (auto data = collection->_data.Pin())
		{
			if (data->isCollectedFairy())
			{
				if(!collection->_complete) 
					collection->_isNew = true ;
				collection->_complete = true;
			}
		}
	}
}

const bool FGsFairyCollection::IsCompleted() 
{ 
	if (!_isComplete)
	{
		_isComplete = _collections.FindByPredicate([](const auto e) { return !e._complete; }) == nullptr;

		if (_isComplete) _isNew = true;
	}

	return _isComplete;
}

const TArray<FairyCollectionItem>& FGsFairyCollection::GetCollectionList() const
{ 
	return _collections; 
}

void FGsFairyCollection::SetCollectionComplete(bool isNew)
{
	if(!_isComplete && isNew) _isNew = isNew;

	_isComplete = true;
	for (auto& e : _collections)
	{
		if (!e._complete && isNew) e._isNew = true;
		e._complete = true;
	}
}

bool FGsFairyCollection::CheckCollectionItemComplete(CollectionConditionId id, bool isNew)
{
	if (_isComplete) return false;

	if (const auto collection = _collections.FindByPredicate([id](auto& e) { return e._conditionId == id; }))
	{
		if(!collection->_complete && isNew)
			collection->_isNew = true;
		collection->_complete = true;

		return true;
	}
	return false;
}

void FGsFairyCollection::RemoveNewMark()
{
	for (auto& e : _collections)
	{
		e._isNew = false;
	}

	_isNew = false;
}

FText FGsFairyCollection::GetCollectioinRewardsText()
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