#pragma once

#include "T1Project.h"

//------------------------------------------------------------------------------
// 메시지에 파라미터를 전달하기 위한 기본 객체 클래스
// TEnumId : enum타입의 메시지
// TParam  : 파라미터
//------------------------------------------------------------------------------
template <typename TEnumId, typename TParam>
class TGsMessage
{
	static_assert(TIsEnum<TEnumId>::Value, "TEnumId is not enumerator type");

private:
	TEnumId _id;
	TParam	_param;

public:
	explicit TGsMessage(TEnumId id, TParam param) : _id(id), _param(Forward<TParam>(param))
	{
		//GSLOG(Warning, TEXT("TGsMessage : TGsMessage(TEnumId& id, const TParam& param)"));
	}
	explicit TGsMessage(const TGsMessage<TEnumId, TParam>& other) : _id(other.GetId()), _param(other.GetParam())
	{
		//GSLOG(Warning, TEXT("TGsMessage : TGsMessage(const GSTMessage<TEnumId, TParam>& other)"));
	}

public:
	virtual ~TGsMessage() = default;

public:
	TEnumId GetId() const { return _id; }

	TParam& GetParam() { return _param; }
	const TParam& GetParam() const { return _param; }
};
