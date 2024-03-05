#pragma once

#include "Message/GsMessageSystem.h"
#include "../Management/GsMessageHolder.h"

// bak1210 메시지 파리미터 기본타입
// 메시지 파라미터 전달때는 인터페이스 파라미터를 상속해서 사용한다.

struct IGsMessageParam
{
public:
	IGsMessageParam() = default;
	virtual ~IGsMessageParam() = default;

public:
	template <typename T = IGsMessageParam>
	T* Cast() const
	{
		static_assert(TIsDerivedFrom<T, IGsMessageParam>::IsDerived, "tCast must be derived from tOrigin");

		T* dest = static_cast<T*>(this);
		check(dest);
		return dest;
	}
};

// Primitive 타입(int32) 지원
// TODO : 지원타입이 증가하면 파일분리고려
struct FGsPrimitiveInt32 : public IGsMessageParam
{
public:
	FGsPrimitiveInt32() = default;
	FGsPrimitiveInt32(int32 inData) : _data(inData) {}
	virtual ~FGsPrimitiveInt32() = default;

public:
	int32 _data = 0;
};

struct FGsPrimitivePairInt32 : public IGsMessageParam
{
public:
	FGsPrimitivePairInt32() = default;
	FGsPrimitivePairInt32(int32 InFirst, int32 InSecond) : _first(InFirst), _second(InSecond) {}
	virtual ~FGsPrimitivePairInt32() = default;

public:
	int32 _first = 0;
	int32 _second = 0;
};

struct FGsPrimitiveUInt32 : public IGsMessageParam
{
public:
	FGsPrimitiveUInt32() = default;
	FGsPrimitiveUInt32(uint32 inData) : _data(inData) {}
	virtual ~FGsPrimitiveUInt32() = default;

public:
	uint32 _data = 0;
};

struct FGsPrimitiveUInt64 : public IGsMessageParam
{
public:	
	FGsPrimitiveUInt64() = default;
	FGsPrimitiveUInt64(uint64 inData) : _data(inData) {}
	virtual ~FGsPrimitiveUInt64() = default;

public:
	uint64 _data = 0;
};

struct FGsPrimitivePairUInt64 : public IGsMessageParam
{
public:
	FGsPrimitivePairUInt64() = default;
	FGsPrimitivePairUInt64(uint64 InFirst, uint64 InSecond) : _first(InFirst), _second(InSecond){}
	virtual ~FGsPrimitivePairUInt64() = default;

public:
	uint64 _first = 0;
	uint64 _second = 0;
};

struct FGsPrimitiveUInt8 : public IGsMessageParam
{
public:
	FGsPrimitiveUInt8() = default;
	FGsPrimitiveUInt8(uint8 inData) : _data(inData) {}
	virtual ~FGsPrimitiveUInt8() = default;

public:
	uint8 _data;
};

struct FGsPrimitiveString final : public IGsMessageParam
{
public:
	FGsPrimitiveString(const FString& InData) : _data(InData) {}
	virtual ~FGsPrimitiveString() = default;
public:
	FString _data;
};

// Primitive 타입(FText) 지원
// TODO : 지원타입이 증가하면 파일분리고려
struct FGsPrimitiveText final : public IGsMessageParam
{
public:
	FGsPrimitiveText(FText inData) : _data(MoveTemp(inData)) {}
	virtual ~FGsPrimitiveText() = default;
public:
	const FText _data;
};

struct FGsPrimitiveName final : public IGsMessageParam
{
public:
	FGsPrimitiveName(const FName& InData) : _data(InData) {}
	virtual ~FGsPrimitiveName() = default;
public:
	FName _data;
};

struct FGsPrimitivePairFloat : public IGsMessageParam
{
public:
	FGsPrimitivePairFloat() = default;
	FGsPrimitivePairFloat(float InFirst, float InSecond) : _first(InFirst), _second(InSecond) {}
	virtual ~FGsPrimitivePairFloat() = default;

public:
	float _first = 0;
	float _second = 0;
};

struct FGsSharedParamData : public TSharedFromThis<FGsSharedParamData>
{
};

struct FGsSharedParam final : public IGsMessageParam
{
public:
	explicit FGsSharedParam(TSharedPtr<FGsSharedParamData> InData) : _data(InData) {}
	// explicit FGsSharedParam(FGsSharedParamData* InData) : _data((nullptr != InData && InData->DoesSharedInstanceExist()) ? InData->AsShared() : MakeShared<FGsSharedParamData>()) {}
	virtual ~FGsSharedParam() {}
	template<typename T>
	const T* GetParam() const
	{
		return _data.IsValid() ? StaticCastSharedPtr<T>(_data).Get() : nullptr;
	}

private:
	TSharedPtr<FGsSharedParamData> _data;
};

enum class EGsSequnceParamPosType
{
	None,			// 없음
	Actor,			// 액터
	Transform,		// 트랜스폼
};
class AActor;
struct FGsSequenceMessageParam final : public IGsMessageParam
{
public:
	// 시퀀서 리소스 테이블 아이디
	int32 _sequenceId = 0;
	// 시퀀스 위치 맞출 액터
	AActor* _transformOriginActor = nullptr;
	// 시퀀스 맞출 위치
	FTransform _transformOrigin = FTransform::Identity;
	// 저장할지(true면 계속 들고 있어서 다시 로드하지 않음)
	bool _isStore = false;
	// ui hide 할까?
	bool _isUIHide = true;

	bool _isSkip = false;

	// 위치 조정 타입
	EGsSequnceParamPosType _posType = EGsSequnceParamPosType::None;

public:
	FGsSequenceMessageParam() = default;
	virtual ~FGsSequenceMessageParam() = default;
	FGsSequenceMessageParam(int32 inSequenceId) : _sequenceId(inSequenceId)
	{
	}
	FGsSequenceMessageParam(int32 inSequenceId, bool In_isStore, bool In_isUIHide = false)
		: _sequenceId(inSequenceId), _isStore(In_isStore), _isUIHide(In_isUIHide)
	{
		
	}
	FGsSequenceMessageParam(int32 inSequenceId, AActor* In_actor, bool In_isStore, bool In_isUIHide = false)
		: _sequenceId(inSequenceId),_transformOriginActor(In_actor),_isStore(In_isStore), _isUIHide(In_isUIHide)
	{
		_posType = EGsSequnceParamPosType::Actor;
	}
	FGsSequenceMessageParam(int32 inSequenceId, const FTransform& In_addTr, bool In_isStore, bool In_isUIHide = false)
		: _sequenceId(inSequenceId), _transformOrigin(In_addTr),_isStore(In_isStore), _isUIHide(In_isUIHide)
	{
		_posType = EGsSequnceParamPosType::Transform;
	}
	
public:
	void SetData(const FGsSequenceMessageParam* In_data)
	{
		if (In_data == nullptr)
		{
			return;
		}

		_sequenceId = In_data->_sequenceId;
		_transformOriginActor = In_data->_transformOriginActor;
		_transformOrigin = In_data->_transformOrigin;
		_isStore = In_data->_isStore;

		_posType = In_data->_posType;
	}
};

