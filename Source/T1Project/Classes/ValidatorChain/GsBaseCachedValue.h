#pragma once

#include "T1Project.h"

class FGsBaseCachedValue
{
protected:
	uint8 _isDirty:1;

public:
	FGsBaseCachedValue()
		: _isDirty(1)
	{
	}

	virtual ~FGsBaseCachedValue() {}

public:
	void SetDirty()
	{
		if (!_isDirty)
		{
			_isDirty = true;
		}
	}

protected:
	virtual void UpdateValue() { }

};

template<typename TValue>
class TGsBaseCachedValue : public FGsBaseCachedValue
{

public:
	using ValueType = TValue;

private:
	ValueType _value;

public:
	const ValueType& GetValue()
	{
		if (_isDirty)
		{
			UpdateValue();
			_isDirty = false;
		}

		return _value;
	}

protected:
	void SetValue(const ValueType& InValue)
	{
		_value = InValue;
	}

};