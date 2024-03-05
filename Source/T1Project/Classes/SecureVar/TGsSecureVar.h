#pragma once

#include "Runtime/Core/Public/Stats/Stats.h"

#if USE_HERCULES
#include "Hercules_App.h"
#endif

#include "T1Project.h"

struct IGsSecureVar
{
public:
	IGsSecureVar()
	{
	}

	virtual ~IGsSecureVar()
	{
	}
};

template<typename TVar>
struct TGsSecureVarBase : public IGsSecureVar
{
private:
	using Super = IGsSecureVar;

public:
	using VarType = TVar;

public:
	// obj = value;
	const TVar& operator=(const TVar& InVar)
	{
		Set(InVar);
		return InVar;
	}

	// obj = obj;
	inline const TVar operator=(const TGsSecureVarBase<TVar>& InRhs)
	{
		TVar Var;
		InRhs.Get(Var);
		Set(Var);
		return Var;
	}

	// value = obj;
	inline operator TVar()
	{
		TVar Var;
		Get(Var);
		return Var;
	}

public:
	virtual inline const void Get(OUT TVar& OutVar) const
	{
	}

	virtual void Set(const TVar& InVar)
	{
	}
};

// cf. Plugins\HerculesLib\Source\HerculesLib\libhercules\include\Hercules.h

#if !WITH_EDITOR && USE_HERCULES && (PLATFORM_ANDROID || PLATFORM_IOS || PLATFORM_WINDOWS)

template<typename TVarNumeric>
struct TGsSecureNumeric final : public TGsSecureVarBase<TVarNumeric>
{
private:
	using Super = TGsSecureVarBase<TVarNumeric>;

private:
	static constexpr SIZE_T TYPE_SIZE = sizeof(TVarNumeric);
	HERCULES _key;

public:
	TGsSecureNumeric()
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_TGsSecureNumericConstructor);
		TVarNumeric Var;
		_key = CHerculesPrefs::AddVar(&Var, TYPE_SIZE);
	}

	explicit TGsSecureNumeric(const TVarNumeric& InVar)
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_TGsSecureNumericTypeChange);
		_key = CHerculesPrefs::AddVar(&InVar, TYPE_SIZE);
	}

	explicit TGsSecureNumeric(const TGsSecureVarBase<TVarNumeric>& InRhs)
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_TGsSecureNumericCopy);
		TVarNumeric Var;
		InRhs.Get(Var);
		_key = CHerculesPrefs::AddVar(&Var, TYPE_SIZE);
	}

	~TGsSecureNumeric() final
	{
		CHerculesPrefs::RemoveVar(_key);
	}

public:
	inline const void Get(OUT TVarNumeric& OutVar) const final
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_TGsSecureNumericGet);
		CHerculesPrefs::GetVar(_key, &OutVar);
	}

	inline void Set(const TVarNumeric& InVar) final
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_TGsSecureNumericSet);
		CHerculesPrefs::SetVar(_key, &InVar);
	}
};

#else

template<typename TVarNumeric>
struct TGsSecureNumeric final : public TGsSecureVarBase<TVarNumeric>
{
private:
	using Super = TGsSecureVarBase<TVarNumeric>;

private:
	TVarNumeric _value;

public:
	TGsSecureNumeric()
		: _value(TVarNumeric())
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_TGsSecureNumericConstructor);
	}

	explicit TGsSecureNumeric(const TVarNumeric& InVar)
		: _value(InVar)
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_TGsSecureNumericTypeChange);
	}

	explicit TGsSecureNumeric(const TGsSecureVarBase<TVarNumeric>& InRhs)
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_TGsSecureNumericCopy);
		InRhs.Get(_value);
	}

public:
	inline const void Get(OUT TVarNumeric& OutVar) const final
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_TGsSecureNumericGet);
		OutVar = _value;
	}

	inline void Set(const TVarNumeric& InVar) final
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_TGsSecureNumericSet);
		_value = InVar;
	}
};

#endif
