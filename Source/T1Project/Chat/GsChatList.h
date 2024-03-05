#pragma once
#include "Chat/GsChatItem.h"

using ChatArray = TArray<TWeakPtr<FGsChatItem>>;
using TabViewArray = TArray<TSharedPtr<FGsChatItem>>;

struct FGsChatList
{
	using SizeType = TArray<TSharedPtr<FGsChatItem>>::SizeType;

	EGsChatViewType _type{ EGsChatViewType::ALL };
	TabViewArray _list;
	TMap<uint64, ChatArray> _groupList;

	FGsChatList() = default;
	virtual ~FGsChatList() = default;

	FGsChatList(EGsChatViewType type) : _type(type)
	{
	}

	FGsChatList(const FGsChatList& other)
		: _type(other._type)
		, _list(CopyTemp(other._list))
		, _groupList(CopyTemp(other._groupList))
	{
	
	}

	FGsChatList(FGsChatList&& other) noexcept
		: _type(other._type)
		, _list(Forward<TabViewArray>(other._list))
		, _groupList(Forward<TMap<uint64, ChatArray>>(other._groupList))
	{
		
	}

	FGsChatList& operator=(const FGsChatList& other)
	{
		// Guard self assignment
		if (this == &other)
			return *this;

		_type = other._type;
		_list = CopyTemp(other._list);
		_groupList = CopyTemp(other._groupList);

		return *this;
	}

	FGsChatList& operator=(FGsChatList&& other) noexcept
	{
		if (this == &other)
			return *this;

		_type = other._type;
		_list = MoveTemp(other._list);
		_groupList = MoveTemp(other._groupList);

		other._type = EGsChatViewType::ALL;

		return *this;
	}

	operator TabViewArray& () { return _list; }

	const ChatArray& ChatList(int roomId = 0) {
		if (auto iter = _groupList.Find(roomId))
		{
			iter->RemoveAll([](auto e) {return e.IsValid() == false; });
			return *iter;
		}
		return _groupList.FindOrAdd(0);
	}

	TSharedPtr<FGsChatItem>& operator[](SizeType idx) { return _list[idx]; }
	const TSharedPtr<FGsChatItem>& operator[](SizeType idx) const { return _list[idx]; }

	void Reset() { _list.Reset(); _groupList.Reset(); }
	bool IsValidIndex(SizeType idx) { return _list.IsValidIndex(idx); }

	void AddData(TSharedPtr<FGsChatItem> item, bool useRoomId = false)
	{
		_list.Add(item);
		_groupList.FindOrAdd(useRoomId ? item->GetRoomId() : 0).Add(item);
	}

	void InsertData(TSharedPtr<FGsChatItem> item, int index, bool useRoomId = false)
	{
		_list.Add(item);
		_groupList.FindOrAdd(useRoomId ? item->GetRoomId() : 0).Insert(item, index);
	}

	void RemoveData(TSharedPtr<FGsChatItem> item, bool useRoomId = false)
	{
		_list.Remove(item);

		if (auto iter = _groupList.Find(useRoomId ? item->GetRoomId() : 0))
			iter->Remove(item);
	}

	template <class PREDICATE_CLASS>
	SizeType RemoveAll(const PREDICATE_CLASS& Predicate)
	{	
		if (_list.Num() <= 0) return 0;

		for (auto& pair : _groupList)
		{
			const SizeType OriginalNum = pair.Value.Num();

			SizeType WriteIndex = 0;
			SizeType ReadIndex = 0;
			bool NotMatch = !Predicate(pair.Value.GetData()[ReadIndex].Pin()); // use a ! to guarantee it can't be anything other than zero or one
			do
			{
				SizeType RunStartIndex = ReadIndex++;
				while (ReadIndex < OriginalNum && NotMatch == !Predicate(pair.Value.GetData()[ReadIndex].Pin()))
				{
					ReadIndex++;
				}
				SizeType RunLength = ReadIndex - RunStartIndex;
				checkSlow(RunLength > 0);
				if (NotMatch)
				{
					// this was a non-matching run, we need to move it
					if (WriteIndex != RunStartIndex)
					{
						FMemory::Memmove(&pair.Value.GetData()[WriteIndex], &pair.Value.GetData()[RunStartIndex], sizeof(TWeakPtr<FGsChatItem>) * RunLength);
					}
					WriteIndex += RunLength;
				}
				else
				{
					// this was a matching run, delete it
					DestructItems(pair.Value.GetData() + RunStartIndex, RunLength);
				}
				NotMatch = !NotMatch;
			} while (ReadIndex < OriginalNum);
		}

		return _list.RemoveAll(Predicate);
	}

	void RemoveAllRoomData(uint64 roomId)
	{
		_list.RemoveAll([roomId](auto e) { return e->GetRoomId() == roomId; });

		if (auto iter = _groupList.Find(roomId))
		{
			iter->Reset();
			if (roomId > 0) _groupList.Remove(roomId);
		}
	}

	int GetListItems(ChatArray& dataList, uint64 roomId = 0)
	{
		if (auto iter = _groupList.Find(roomId))
		{
			dataList.Append(*iter);
		}
		return dataList.Num();
	}
};
