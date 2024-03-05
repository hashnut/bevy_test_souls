#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsCollectionGetter.h"
#include "GsCollectionDataContainer.h"

#include "UTIL/GsTableUtil.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectStatSet.h"
#include "Management/ScopeGame/GsEffectTextManager.h"
#include "Algo/AnyOf.h"

#include <type_traits>

template<typename T>
class T1PROJECT_API IGsCollectionFilter
{
public:
	using TYPE = typename TEnableIf<TIsDerivedFrom<T, IGsCollectionGetter>::IsDerived, T>::Type;
	enum class SearchType : int8 { NONE = -1, All = 0, Complete, InProgress };

protected:
	virtual int GetCollection(TArray<TWeakPtr<TYPE>>& collections) const = 0;

public:
	virtual void GetCollectionSearchItems(SearchType searchType, StatType statType, TArray<TWeakPtr<TYPE>>& outList)
	{
		TArray<TWeakPtr<TYPE>> list;
		if (GetCollection(list) < 0) return;

		outList = list;
		if (searchType == SearchType::Complete)
		{
			outList = outList.FilterByPredicate([](TWeakPtr<TYPE> e) { return e.IsValid() && e.Pin()->IsCompleted(); });
		}
		else if (searchType == SearchType::InProgress)
		{
			outList = outList.FilterByPredicate([](TWeakPtr<TYPE> e) { return e.IsValid() && !e.Pin()->IsCompleted(); });
		}

		SortCollectionsByStat(statType, outList);
	}

	virtual void GetCollectionSearchItems(SearchType searchType, FText statName, TArray<TWeakPtr<TYPE>>& outList)
	{
		TArray<TWeakPtr<TYPE>> list;
		if (GetCollection(list) < 0) return;

		outList = list;
		if (searchType == SearchType::Complete)
		{
			outList = outList.FilterByPredicate([](TWeakPtr<TYPE> e) { return e.IsValid() && e.Pin()->IsCompleted(); });
		}
		else if (searchType == SearchType::InProgress)
		{
			outList = outList.FilterByPredicate([](TWeakPtr<TYPE> e) { return e.IsValid() && !e.Pin()->IsCompleted(); });
		}

		SortCollectionsByStatName(statName, outList);
	}

	virtual void GetCollectionSearchItems(SearchType searchType, TArray<StatType> statTypeList, TArray<TWeakPtr<TYPE>>& outList)
	{
		TArray<TWeakPtr<TYPE>> list;
		if (GetCollection(list) < 0) return;

		outList = list;
		if (searchType == SearchType::Complete)
		{
			outList = outList.FilterByPredicate([](TWeakPtr<TYPE> e) { return e.IsValid() && e.Pin()->IsCompleted(); });
		}
		else if (searchType == SearchType::InProgress)
		{
			outList = outList.FilterByPredicate([](TWeakPtr<TYPE> e) { return e.IsValid() && !e.Pin()->IsCompleted(); });
		}

		SortCollectionsByStatList(statTypeList, outList);
	}

protected:
	virtual void SortCollectionsByStat(StatType statType, TArray<TWeakPtr<TYPE>>& collections)
	{
		auto tempCollection = collections;

		collections.Empty(0);
		collections = tempCollection.FilterByPredicate([statType](TWeakPtr<TYPE> e) {

			bool ret = false;
			if (e.IsValid())
			{
				for (auto passivityId : e.Pin()->GetRewardPassivity())
				{
					const FGsSchemaPassivitySet* passivityData = UGsTableUtil::FindRowById<UGsTablePassivitySet, FGsSchemaPassivitySet>(passivityId);
					if (passivityData && passivityData->type == PassivityType::STAT)
					{
						for (FGsSchemaPassivityEffectInfo info : passivityData->effectInfoList)
						{
							const FGsSchemaPassivityEffectStatSet* statData
								= UGsTableUtil::FindRowById<UGsTablePassivityEffectStatSet, FGsSchemaPassivityEffectStatSet>(info.effectId);

							if (statData->param.FindByPredicate([statType](auto& stat) { return stat.type == statType; }))
							{
								ret = true;
								break;
							}
						}
					}
					if (ret) break;
				}
			}
			return ret;
		});
	}

	virtual void SortCollectionsByStatList(TArray<StatType> statTypeList, TArray<TWeakPtr<TYPE>>& collections)
	{
		auto tempCollection = collections;

		collections.Empty(0);
		collections = tempCollection.FilterByPredicate([&statTypeList](TWeakPtr<TYPE> e) {

			bool ret = false;
			if (e.IsValid())
			{
				if (0 >= statTypeList.Num())
				{	// 2024/1/17 PKT - Filter 하고자 하는 Stat이 없다면 무조건 성공
					return true;
				}
				for (auto passivityId : e.Pin()->GetRewardPassivity())
				{
					const FGsSchemaPassivitySet* passivityData = UGsTableUtil::FindRowById<UGsTablePassivitySet, FGsSchemaPassivitySet>(passivityId);
					if (passivityData && passivityData->type == PassivityType::STAT)
					{
						for (FGsSchemaPassivityEffectInfo info : passivityData->effectInfoList)
						{
							const FGsSchemaPassivityEffectStatSet* statData
								= UGsTableUtil::FindRowById<UGsTablePassivityEffectStatSet, FGsSchemaPassivityEffectStatSet>(info.effectId);
							
							ret = Algo::AnyOf(statData->param, [&statTypeList](auto& stat) { return statTypeList.Find(stat.type) != INDEX_NONE; });
							if (ret) break;
						}
					}
					if (ret) break;
				}
			}
			return ret;
		});
	}

	virtual void SortCollectionsByStatName(FText statName, TArray<TWeakPtr<TYPE>>& collections)
	{
		if (statName.IsEmpty()) return; //전체

		auto tempCollection = collections;

		collections.Empty(0);
		collections = tempCollection.FilterByPredicate([statName](TWeakPtr<TYPE> e) {

			bool ret = false;
			if (e.IsValid())
			{
				for (auto passivityId : e.Pin()->GetRewardPassivity())
				{
					TArray<TPair<FText, FText>> textList;
					if (GSEffectText()->GetEffectTextListPassivity(passivityId, textList))
					{
						ret = textList.FindByPredicate([statName](auto& effinfo) { return effinfo.Key.EqualTo(statName); }) != nullptr;
						if (ret) break;
					}
				}
			}
			return ret;
		});
	}
};

template<typename T, PassivityOwnerType type>
class T1PROJECT_API CollectionCompleteReward : public IGsCollectionData
{
public:
	CollectionCompleteReward() = default;
	virtual ~CollectionCompleteReward() = default;

	virtual PassivityOwnerType GetPassivityOwnerType() const override
	{
		return _collectionType;
	}

	virtual int32 GetPassivityCount() const override
	{
		return _completePassivity.Num();
	}

	virtual PassivityId GetPassivityId(int32 InIndex) const override
	{
		if (_completePassivity.IsValidIndex(InIndex))
			return _completePassivity[InIndex];
		return INVALID_PARTY_ID;
	}

public:
	void UpdatePassivity(TArray<TWeakPtr<typename TEnableIf<TIsDerivedFrom<T, IGsCollectionGetter>::IsDerived, T>::Type>> collections)
	{
		_completePassivity.Empty(0);

		for (auto e : collections)
		{
			if (e.IsValid())
			{
				_completePassivity.Append(e.Pin()->GetRewardPassivity());
			}
		}
	}

protected:
	PassivityOwnerType  _collectionType{ type };
	TArray<PassivityId> _completePassivity;
};

