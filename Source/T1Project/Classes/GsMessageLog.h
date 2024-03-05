#pragma once
#include "Runtime/Core/Public/Containers/StackTracker.h"


//-------------------------------------------------------------------------------------------------
// template 함수포인터 정의
//-------------------------------------------------------------------------------------------------
template <typename Param1, typename Param2>
using fPtrVoidParam2Type = void (*)(Param1, Param2);


//-------------------------------------------------------------------------------------------------
// 메시지 출력 Base
//-------------------------------------------------------------------------------------------------
template<typename TMessageId>
class MessageHandlerLog
{
protected:
	FString											_preTag;
	fPtrVoidParam2Type<TMessageId, FString&>		_printFn = nullptr;

public:
	MessageHandlerLog() = default;
	virtual ~MessageHandlerLog() = default;

public:
	void SetTag(const FString& inTag)								{ _preTag = inTag; }
	void SetLogger(fPtrVoidParam2Type<TMessageId, FString&> inFn)	{ _printFn = inFn; }

protected:
	void Log(TMessageId id)
	{
		if (nullptr != _printFn)
		{
			(*_printFn)(id, _preTag);
		}
	}

	void StackLog()
	{
		FStackTracker callStack;
		callStack.CaptureStackTrace();
		callStack.DumpStackTraces(20, *GLog);
	}
};

