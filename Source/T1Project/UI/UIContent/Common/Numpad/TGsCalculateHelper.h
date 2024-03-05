#pragma once

#include "Runtime/Core/Public/Delegates/DelegateCombinations.h"
#include "Runtime/Core/Public/Math/NumericLimits.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"

#include "T1Project.h"

template<typename T>
class TGsCalculateHelper final
{

public:
	DECLARE_DELEGATE_OneParam(FOnChanged, T);

private:
	T _current = TNumericLimits<T>::Min();
	T _min = TNumericLimits<T>::Min();
	T _max = TNumericLimits<T>::Max();

public:
	FOnChanged _onChanged;
	
	/**
	 * setters
	 */

public:
	void Set(const T& InCurrent, const T& InMin, const T& InMax)
	{
		_min = InMin;
		_max = InMax;

		if (_min > _max)
		{
			GSLOG(Warning, TEXT("_min > _max"));
			_max = _min;
		}

		SetCurrent(InCurrent);
	}

	void SetCurrent(T InCurrent)
	{
		InCurrent = FMath::Clamp(InCurrent, _min, _max);
		if (InCurrent != _current)
		{
			_current = InCurrent;
			_onChanged.ExecuteIfBound(_current);
		}
	}

	void SetMin(const T& InMin)
	{
		_min = InMin;

		if (_min > _max)
		{
			GSLOG(Warning, TEXT("_min > _max"));
			_max = _min;
		}

		SetCurrent(_current);
	}

	void SetMax(const T& InMax)
	{
		_max = InMax;

		if (_min > _max)
		{
			GSLOG(Warning, TEXT("_min > _max"));
			_min = _max;
		}

		SetCurrent(_current);
	}

	/**
	 * getters
	 */

public:
	const T& GetCurrent() const
	{
		return _current;
	}

	const T& GetMin() const
	{
		return _min;
	}

	const T& GetMax() const
	{
		return _max;
	}

	bool IsMin() const
	{
		return _current == _min;
	}

	bool IsMax() const
	{
		return _current == _max;
	}

	/**
	 * calculators
	 */

public:
	void Add(const T& InValue)
	{
		SetCurrent(GetCurrent() + InValue);
	}

	void ToMin()
	{
		SetCurrent(GetMin());
	}

	void ToMax()
	{
		SetCurrent(GetMax());
	}

};