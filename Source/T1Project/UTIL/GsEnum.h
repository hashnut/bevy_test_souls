#pragma once
#include <string>
#include "Find.h"


class FGsEnumUtil
{
	// Example usage
	//GetEnumValueAsString<EVictoryEnum>("EVictoryEnum", VictoryEnum)));
public:
	template<typename TEnum>
	static FORCEINLINE int32 GetEnumNum(const FString& Name)
	{
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);

		if (!enumPtr)
		{
			return INT32_MIN;
		}

		return enumPtr->NumEnums();
	}
};

#define ENABLE_ENUM_OPERATORS(E)	\
template<>                          \
struct EnableEnumOperators<E>		\
{                                   \
    static const bool enable = true;\
};

template<typename E>
struct EnableEnumOperators
{
	static const bool enable = false;
};

template <typename E, typename = typename TEnableIf<TIsEnumClass<E>::Value>::Type>
struct intEnumType
{
	int _value;
	constexpr intEnumType(const E value) : _value(static_cast<int>(value)) {}

	constexpr operator E() const
	{
		return static_cast<E>(_value);
	}

	constexpr operator int() const
	{
		return _value;
	}

	intEnumType& operator++()
	{
		++_value;
		return *this;
	}

	const intEnumType operator++(int)
	{
		intEnumType e(*this);
		++(*this);
		return e;
	}

	intEnumType& operator+=(const intEnumType& rhs)
	{
		_value += rhs._value;
		return *this;
	}

	intEnumType operator+(int num)
	{
		intEnumType result(static_cast<E>(this->_value + num));
		return result;
	}

	intEnumType operator+(const intEnumType& ref)
	{
		intEnumType result(0);
		result._value = this->_value + ref._value;

		return result;
	}

	intEnumType operator-(int num)
	{
		intEnumType result(static_cast<E>(this->_value - num));
		return result;
	}

	bool operator<(const intEnumType& rhs)
	{
		return _value < rhs._value;
	}
};

template <typename E, typename = typename TEnableIf<TIsEnumClass<E>::Value, int>::Type>
struct intEnumArray
{
	using KeyType = typename TEnableIf<TIsEnumClass<E>::Value, int>::Type;
	TArray<int> _valArray;

	intEnumArray() { _valArray.Add(0); }
	constexpr intEnumArray(const E sz) {
		_valArray.AddZeroed(static_cast<int>(sz));
	}

	int& operator[](E index)
	{
		auto numIndex = static_cast<KeyType>(index);
		return _valArray[numIndex];
	}

	int operator[](E index) const
	{
		auto numIndex = static_cast<KeyType>(index);
		return _valArray[numIndex];
	}

	constexpr operator TArray<int>&() const
	{
		return _valArray;
	}

	operator TArray<int>() const
	{
		return _valArray;
	}
};

template<typename E>
typename TEnableIf<TIsEnumClass<E>::Value && EnableEnumOperators<E>::enable, E>::Type
operator++(E& rhs)
{
	rhs = ++intEnumType<E>(rhs);
	return rhs;
}

template<typename E>
typename TEnableIf<TIsEnumClass<E>::Value && EnableEnumOperators<E>::enable, E>::Type
operator++(E& lhs, int)
{
	E result = lhs;
	++lhs;
	return result;
}

template<typename E>
typename TEnableIf<TIsEnumClass<E>::Value && EnableEnumOperators<E>::enable, E>::Type
operator+=(E& lhs, const E& rhs)
{
	lhs = E(intEnumType<E>(lhs) += rhs);
	return lhs;
}

template<typename E>
typename TEnableIf<TIsEnumClass<E>::Value && EnableEnumOperators<E>::enable, E>::Type
operator+(const E& lhs, const E& rhs)
{
	return E(intEnumType<E>(lhs) + intEnumType<E>(rhs));
}

template<typename E>
typename TEnableIf<TIsEnumClass<E>::Value && EnableEnumOperators<E>::enable, E>::Type
operator+(const E& lhs, int num)
{
	return E(intEnumType<E>(lhs) + num);
}

template<typename E>
typename TEnableIf<TIsEnumClass<E>::Value&& EnableEnumOperators<E>::enable, E>::Type
operator-(const E& lhs, const E& rhs)
{
	return E(intEnumType<E>(lhs) - intEnumType<E>(rhs));
}

template<typename E>
typename TEnableIf<TIsEnumClass<E>::Value&& EnableEnumOperators<E>::enable, E>::Type
operator-(const E& lhs, int num)
{
	return E(intEnumType<E>(lhs) - num);
}

template<typename E>
typename TEnableIf<TIsEnumClass<E>::Value && EnableEnumOperators<E>::enable, bool>::Type
operator<(const E& lhs, const E& rhs)
{
	return intEnumType<E>(lhs) < intEnumType<E>(rhs);
}

template<typename E>
typename TEnableIf<TIsEnumClass<E>::Value&& EnableEnumOperators<E>::enable, bool>::Type
operator>(const E& lhs, const E& rhs)
{
	return intEnumType<E>(lhs) > intEnumType<E>(rhs);
}

template <typename E>
constexpr typename TEnableIf<TIsEnumClass<E>::Value && EnableEnumOperators<E>::enable, int>::Type
intType(E rhs)
{
	return intEnumType<E>(rhs)._value;
}

template <typename E>
typename TEnableIf<TIsEnumClass<E>::Value && EnableEnumOperators<E>::enable, bool>::Type
IsValidRange(E val)
{
	TArray<E> _range;
	for (auto e : TEnumRange<E>()) _range.Add(e);

	return _range.Find(val) != INDEX_NONE;
}

template <typename E>
typename TEnableIf<TIsEnumClass<E>::Value&& EnableEnumOperators<E>::enable, bool>::Type
IsValidRange(E val, E InBegin, E InEnd)
{
	// Don't need, Can't use
	//static_assert(start >= end, "If E is Enum Class, 'InBegin' have to come before the 'InEnd'");

	if (val < InBegin || val > InEnd)
		return false;

	return true;
}