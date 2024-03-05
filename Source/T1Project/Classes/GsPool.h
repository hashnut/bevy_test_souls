// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//----------------------------------------------------------------------------------------------------------------------
/**
* 기본 Pool (UObject 상속 아닌 항목, SharedPtr 사용 안함에 주의)
* 런타임에 필요시 생성, 반납 (빈목록이 없을때 리스트가 커지는 구조
*/
template <typename T>
struct T1PROJECT_API TGsPoolDefault final
{
private:
	TArray<T*> _poolArray;
	TArray<T*> _inactiveArray;

public:
	TGsPoolDefault() = default;
	~TGsPoolDefault()
	{
		RemovePool();
	}

public:
	T* Claim()
	{
		T* data = nullptr;

		if (0 == _inactiveArray.Num())
		{
			data = new T();
			_poolArray.Emplace(data);
		}
		else
		{
			data = _inactiveArray.Pop();
		}

		return data;
	}

	void Release(T* InData)
	{
		_inactiveArray.Emplace(InData);
	}

	// 풀로 모두 반납
	void ReleaseAll()
	{
		_inactiveArray.Empty();
		_inactiveArray.Append(_poolArray);
	}

	// 풀 내용 전부 삭제
	void RemovePool()
	{
		_inactiveArray.Empty();
		for (T* data : _poolArray)
		{
			delete data;
			data = nullptr;
		}
		_poolArray.Empty();
	}
};
//----------------------------------------------------------------------------------------------------------------------
// 초기에 지정된 사이즈 만큼 미리 할당하고 재활용 하는 풀
// GetFreeLink 로 얻고 Release 로 삭제함
//----------------------------------------------------------------------------------------------------------------------
template <class Type> class TGsPoolPreSizeAlloc 
{
private:
	int   mMaxItems;
	Type*			mCurrent;	// current iteration location.
	TArray<Type>	mData;
	Type*			mHead;		// head of used list.
	Type*			mFree;		// head of free list.
	int   mUsedCount;
	int   mFreeCount;

public:
	TGsPoolPreSizeAlloc(void)
	{
		mHead = nullptr;
		mFree = nullptr;
		mCurrent = nullptr;

		mFreeCount = 0;
		mUsedCount = 0;
	};

	~TGsPoolPreSizeAlloc(void)
	{
	};

	void ReleasePool(void)
	{
		mHead = nullptr;
		mFree = nullptr;
		mCurrent = nullptr;

		mData.Empty();

		mFreeCount = 0;
		mUsedCount = 0;
	};

	void SetPoolCount(int maxitems)
	{
		mMaxItems = maxitems;

		mData.SetNum(maxitems);
		mFree = &mData[0];

		mHead = nullptr;

		int last = 0;
		for (int i = 0; i < (mMaxItems - 1); i++, last++)
		{
			mData[i].SetNext(&mData[i + 1]);
			if (i == 0)
			{
				mData[i].SetPrevious(nullptr);
			}
			else
			{
				mData[i].SetPrevious(&mData[i - 1]);
			}
				
		}

		mData[last].SetNext(nullptr);
		mData[last].SetPrevious(&mData[last - 1]);

		mCurrent = nullptr;

		mFreeCount = maxitems;
		mUsedCount = 0;
	};
	

	bool IsEmpty(void) const
	{
		return (nullptr == mHead) ? true : false;
	};
	
	int GetUsedCount(void) const { return mUsedCount; };
	int GetFreeCount(void) const { return mFreeCount; };
	
	void Release(Type* t)
	{
		if (t == mCurrent)
		{
			mCurrent = t->GetNext();
		}

		// first patch old linked list.. his previous now points to his next
		Type* prev = t->GetPrevious();

		if (nullptr != prev)
		{
			Type* next = t->GetNext();
			prev->SetNext(next); // my previous now points to my next
			if (nullptr != next)
			{
				next->SetPrevious(prev);
			}
			// list is patched!
		}
		else
		{
			// 이전이 없다면 맨앞
			Type* next = t->GetNext();
			mHead = next;
			if (nullptr != mHead)
			{
				mHead->SetPrevious(nullptr);
			}
		}

		// 타겟을 해지목록 맨앞으로 지정
		Type* temp = mFree; // old head of free list.
		mFree = t; // new head of linked list.
		t->SetPrevious(nullptr);
		t->SetNext(temp);

		mUsedCount--;
		mFreeCount++;
	};

	Type* GetFreeLink(void)
	{
		// Free allocated items are always added to the head of the list
		if (nullptr == mFree)
		{
			check(0);
			return 0;
		}

		//빈목록을 갱신해두고
		Type* ret = mFree;
		mFree = ret->GetNext();

		//할당된 객체는 Head로 지정한다.
		Type* temp = mHead; 
		mHead = ret;

		if (nullptr != temp)
		{
			temp->SetPrevious(ret);
		}
		mHead->SetNext(temp);
		mHead->SetPrevious(nullptr);
		mUsedCount++;
		mFreeCount--;
		return ret;
	};

	void AddAfter(Type* t, Type* item)
	{
		// Add 'item' after 't'
		if (nullptr != t )
		{
			Type* tprev = t->GetPrevious();
			Type* tnext = t->GetNext();
			t->SetNext(item);
			item->SetNext(tnext);
			item->SetPrevious(t);
			if (nullptr != tnext)
			{
				tnext->SetPrevious(item);
			}
		}
		else
		{
			// 타겟이 맨앞일때
			mHead = item;
			item->SetPrevious(nullptr);
			item->SetNext(nullptr);
		}
	}

	void AddBefore(Type* t, Type* item)
	{
		// Add 'item' before 't'
		Type* tprev = t->GetPrevious();
		Type* tnext = t->GetNext();

		if (nullptr == tprev)
		{
			mHead = item;
		}
		else
		{
			tprev->SetNext(item);
		}

		item->SetPrevious(tprev);
		item->SetNext(t);

		t->SetPrevious(item);
	}

protected:
	Type* GetNext(void)
	{
		if (nullptr == mHead)
		{
			return nullptr; // there is no data to process.
		}

		Type* ret;

		if (nullptr == mCurrent)
		{
			ret = mHead;
		}
		else
		{
			ret = mCurrent;
		}

		if (nullptr != ret)
		{
			mCurrent = ret->GetNext();
		}

		return ret;
	};

	// 이터레이션을 위한 기능
	//int count = mVisible.Begin();
	//for (int i = 0; i < count; i++)
	//{
		//CircleItem* item = mVisible.GetNext();
	//}
	int Begin(void)
	{
		mCurrent = mHead;
		return mUsedCount;
	};
};
