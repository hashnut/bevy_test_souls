#pragma once

template <typename T>
struct PktIterator
{
private:
	using value_type = T;
	using pointer_type = T*;
	using reference_type = T&;


private:
	pointer_type mPointer;


public:
	PktIterator(pointer_type ptr) noexcept
		: 
		mPointer(ptr)
	{
	}

	PktIterator(const PktIterator& other) noexcept
		: 
		mPointer(other.mPointer)
	{
	}

	reference_type operator * () noexcept
	{
		return *mPointer;
	}

	pointer_type operator -> () noexcept
	{
		return mPointer;
	}

	pointer_type operator -> () const noexcept
	{
		return mPointer;
	}

	PktIterator& operator ++ () noexcept
	{
		Advance();
		return *this;
	}

	PktIterator operator ++ (int) noexcept
	{
		PktIterator temp = *this;
		Advance();
		return temp;
	}

	void Advance() noexcept
	{
		uchar* pktStart = (uchar*)mPointer - mPointer->mThisOffset;
		uint16 pktSize = *((uint16*)pktStart);

		if (mPointer->mNextOffset && pktSize >= mPointer->mNextOffset + sizeof(value_type))
		{
			pointer_type ptr = pointer_type(pktStart + mPointer->mNextOffset);
			if (mPointer->mNextOffset == ptr->mThisOffset)	// for boundary check
				mPointer = ptr;
			else
				mPointer = 0;
		}
		else
			mPointer = 0;
	}

	bool IsEqual(const PktIterator& other) const noexcept
	{
		return mPointer == other.mPointer;
	}

	bool operator == (const PktIterator& other) const noexcept
	{
		return IsEqual(other);
	}

	bool operator != (const PktIterator& other) const noexcept
	{
		return !IsEqual(other);
	}
};
