#pragma once
#include "Message/GsMessageDelegateList.h"


//----------------------------------------------------------------
// 매지시 핸들러 클래스 기본인터페이스
//----------------------------------------------------------------
class IGsMessageHandler
{
public:
	virtual ~IGsMessageHandler() {}

public:
	virtual void InitializeMessage() = 0;
	virtual void FinalizeMessage() = 0;
};






