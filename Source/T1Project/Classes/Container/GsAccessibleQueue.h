// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreTypes.h"
#include "Templates/UnrealTemplate.h"
#include "HAL/PlatformMisc.h"

/**
 * TQueue의 일부 기능을 제거/수정한 클래스.
 * - 노드 접근이 가능하도록 private -> protected 로 수정.
 * - Multiple-producers single-consumer (MPSC) 기능 제거.
 */

// TQueue를 노드 접근 가능한 버젼으로 수정
template<typename ItemType>
class TAccessibleQueue
{
public:

	/** Default constructor. */
	TAccessibleQueue()
	{
		Head = Tail = new TNode();
	}

	/** Destructor. */
	virtual ~TAccessibleQueue()
	{
		while (Tail != nullptr)
		{
			TNode* Node = Tail;
			Tail = Tail->NextNode;

			delete Node;
		}
	}

	/**
	 * Removes and returns the item from the tail of the queue.
	 *
	 * @param OutValue Will hold the returned value.
	 * @return true if a value was returned, false if the queue was empty.
	 * @note To be called only from consumer thread.
	 * @see Empty, Enqueue, IsEmpty, Peek, Pop
	 */
	bool Dequeue(ItemType& OutItem)
	{
		TNode* Popped = Tail->NextNode;

		if (Popped == nullptr)
		{
			return false;
		}
		
		OutItem = MoveTemp(Popped->Item);

		TNode* OldTail = Tail;
		Tail = Popped;
		Tail->Item = ItemType();
		delete OldTail;

		return true;
	}

	/**
	 * Empty the queue, discarding all items.
	 *
	 * @note To be called only from consumer thread.
	 * @see Dequeue, IsEmpty, Peek, Pop
	 */
	void Empty()
	{
		while (Pop());
	}

	/**
	 * Adds an item to the head of the queue.
	 *
	 * @param Item The item to add.
	 * @return true if the item was added, false otherwise.
	 * @note To be called only from producer thread(s).
	 * @see Dequeue, Pop
	 */
	bool Enqueue(const ItemType& Item)
	{
		TNode* NewNode = new TNode(Item);

		if (NewNode == nullptr)
		{
			return false;
		}

		TNode* OldHead = Head;
		Head = NewNode;
		OldHead->NextNode = NewNode;

		return true;
	}

	/**
	 * Adds an item to the head of the queue.
	 *
	 * @param Item The item to add.
	 * @return true if the item was added, false otherwise.
	 * @note To be called only from producer thread(s).
	 * @see Dequeue, Pop
	 */
	bool Enqueue(ItemType&& Item)
	{
		TNode* NewNode = new TNode(MoveTemp(Item));

		if (NewNode == nullptr)
		{
			return false;
		}

		TNode* OldHead = Head;
		Head = NewNode;
		OldHead->NextNode = NewNode;

		return true;
	}

	/**
	 * Checks whether the queue is empty.
	 *
	 * @return true if the queue is empty, false otherwise.
	 * @note To be called only from consumer thread.
	 * @see Dequeue, Empty, Peek, Pop
	 */
	bool IsEmpty() const
	{
		return (Tail->NextNode == nullptr);
	}

	/**
	 * Peeks at the queue's tail item without removing it.
	 *
	 * @param OutItem Will hold the peeked at item.
	 * @return true if an item was returned, false if the queue was empty.
	 * @note To be called only from consumer thread.
	 * @see Dequeue, Empty, IsEmpty, Pop
	 */
	bool Peek(ItemType& OutItem) const
	{
		if (Tail->NextNode == nullptr)
		{
			return false;
		}

		OutItem = Tail->NextNode->Item;

		return true;
	}

	/**
	 * Removes the item from the tail of the queue.
	 *
	 * @return true if a value was removed, false if the queue was empty.
	 * @note To be called only from consumer thread.
	 * @see Dequeue, Empty, Enqueue, IsEmpty, Peek
	 */
	bool Pop()
	{
		TNode* Popped = Tail->NextNode;

		if (Popped == nullptr)
		{
			return false;
		}

		TNode* OldTail = Tail;
		Tail = Popped;
		Tail->Item = ItemType();
		delete OldTail;

		return true;
	}

protected:

	/** Structure for the internal linked list. */
	struct TNode
	{
		/** Holds a pointer to the next node in the list. */
		TNode* volatile NextNode;

		/** Holds the node's item. */
		ItemType Item;

		/** Default constructor. */
		TNode()
			: NextNode(nullptr)
		{ }

		/** Creates and initializes a new node. */
		explicit TNode(const ItemType& InItem)
			: NextNode(nullptr)
			, Item(InItem)
		{ }

		/** Creates and initializes a new node. */
		explicit TNode(ItemType&& InItem)
			: NextNode(nullptr)
			, Item(MoveTemp(InItem))
		{ }
	};

	/** Holds a pointer to the head of the list. */
	MS_ALIGN(16) TNode* volatile Head GCC_ALIGN(16);

	/** Holds a pointer to the tail of the list. */
	TNode* Tail;

protected:

	/** Hidden copy constructor. */
	TAccessibleQueue(const TAccessibleQueue&) = delete;

	/** Hidden assignment operator. */
	TAccessibleQueue& operator=(const TAccessibleQueue&) = delete;
};
