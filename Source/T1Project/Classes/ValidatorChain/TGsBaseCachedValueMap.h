#pragma once

#include "GsBaseCachedValue.h"

class FGsBaseCachedValueMap
{

public:
	virtual ~FGsBaseCachedValueMap() {}

protected:
	virtual void InitializeCachedValue(FGsBaseCachedValue* InCachedValue) const
	{
		check(false);
	}

};

template<typename TCachedValue, typename ... TKeys>
class TGsBaseCachedValueMap : public FGsBaseCachedValueMap
{

public:
	using Super = TGsBaseCachedValueMap<TCachedValue, TKeys ...>;
	using KeyType = TTuple<TKeys...>;
	using ValueType = TWeakPtr<typename TEnableIf<TIsDerivedFrom<TCachedValue, FGsBaseCachedValue>::IsDerived, TCachedValue>::Type>;

private:
	TMap<KeyType, ValueType> _cachedValueMap;

public:
	TSharedRef<TCachedValue> GetCachedValue(const TKeys& ... InKeys)
	{
		KeyType Key = MakeTuple(InKeys ...);
		ValueType* CachedValue = _cachedValueMap.Find(Key);
		if (nullptr == CachedValue || !CachedValue->IsValid())
		{
			TSharedRef<TCachedValue> NewCachedValue = MakeShared<TCachedValue>(InKeys ...);
			InitializeCachedValue(StaticCast<FGsBaseCachedValue*>(&(NewCachedValue.Get())));
			_cachedValueMap.Emplace(Key, NewCachedValue);
			return NewCachedValue;
		}

		return CachedValue->Pin().ToSharedRef();
	}

	typename TCachedValue::ValueType GetValue(const TKeys& ... InKeys)
	{
		return GetCachedValue(InKeys ...)->GetValue();
	}

	void CollectGarbage()
	{
		TArray<KeyType&> RemovedKeys;

		for (auto It : _cachedValueMap)
		{
			ValueType& CachedValue = It.Value();
			if (!CachedValue.IsValid())
			{
				RemovedKeys.Add(It.Key());
			}
		}

		for (KeyType& RemovedKey : RemovedKeys)
		{
			_cachedValueMap.Remove(RemovedKey);
		}
	}

protected:
	void SetDirty(const TKeys& ... InKeys)
	{
		ValueType* CachedValue = _cachedValueMap.Find(MakeTuple(InKeys ...));
		if (nullptr == CachedValue || !CachedValue->IsValid())
		{
			return;
		}
		CachedValue->Pin()->SetDirty();
	}

};