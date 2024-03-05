#pragma once

//------------------------------------------------------------------------------
// 상태(State)를 가지는 최상위 클래스
// 상태머신을 활용하려는 클래스들은 이를 상속받아, TGsStateMng와 함께 사용하면 된다
//------------------------------------------------------------------------------
template <typename TEnumState>
class TGsState
{
	static_assert(TIsEnum<TEnumState>::Value, "TEnumState is not enumerator type");

public:
	using TStateType = TEnumState;

private:
	TStateType	Type;

public:
	TGsState(TStateType inType) : Type(inType) {}
	virtual ~TGsState() {}

public:
	virtual void Init() {}
	virtual void Close() {}

public:
	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void Update(float In_deltaTime) = 0;

public:
	TStateType GetType() const			{ return Type; }
	void SetType(TStateType In_type)	{ Type = In_type; }

public:
	virtual bool CanTransition(TStateType inType) { return true; }
};
