#pragma once
#include "Array.h"

template<typename InElementType>
class FGsDequeue : public TArray<InElementType>
{
public:
	using Super = TArray<InElementType>;

	FORCEINLINE uint32 Num() const
	{
		return static_cast<uint32>(Super::Num());
	}

	FORCEINLINE uint32 Max() const
	{
		return  static_cast<uint32>(Super::Max());
	}

	FORCEINLINE bool IsEmpty() {
		return Num() <= 0;
	}

	InElementType* Peek()  {
		if (Num() <= 0) return nullptr;
		return Super::GetData();
	}

	const InElementType* Peek() const {
		if (Num() <= 0) return nullptr;
		return Super::GetData();
	}

	FORCEINLINE InElementType Pop(bool bAllowShrinking = true)
	{
		InElementType Result = MoveTempIfPossible(Super::GetData()[0]);
		Super::RemoveAt(0, 1, bAllowShrinking);
		return Result;
	}

	FORCEINLINE void PushFront(InElementType&& _Val) {
		Super::Insert( _Val, 0);
	}

	FORCEINLINE void PushFront(const InElementType& _Val) {
		Super::Insert(_Val, 0);
	}

	FORCEINLINE void PushBack(InElementType&& _Val) {
		Super::Add(_Val);
	}

	FORCEINLINE void PushBack(const InElementType& _Val) {
		Super::Add(_Val);
	}

	FORCEINLINE void EmplaceBack(const InElementType& _Val) {
		Super::Emplace(_Val);
	}

	InElementType* Front() {
		return Peek();
	}

	const FORCEINLINE InElementType* Front() const  {
		return Peek();
	}

	InElementType* Back() {
		if (IsEmpty()) return nullptr;
		return &Super::Last();
	}

	const InElementType* Back() const {
		if (IsEmpty()) return nullptr;
		return &Super::Last();
	}

	FORCEINLINE InElementType PopFront(bool bAllowShrinking = true)
	{
		InElementType Result = MoveTempIfPossible(Super::GetData()[0]);
		Super::RemoveAt(0, 1, bAllowShrinking);
		return Result;
	}

	FORCEINLINE InElementType PopBack(bool bAllowShrinking = true)
	{
		return Super::Pop(bAllowShrinking);
	}
};