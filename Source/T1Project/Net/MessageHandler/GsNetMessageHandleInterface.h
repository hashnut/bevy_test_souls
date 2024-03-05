#pragma once

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Message/GsMessageNet.h"


class FGsNetManager;
//----------------------------------------------------------------
// 매지시 핸들러 클래스 기본인터페이스
//----------------------------------------------------------------
class IGsNetMessageHandler
{
public:
	virtual ~IGsNetMessageHandler() = default;

public:
	virtual void InitializeMessage(FGsNetManager* inManager) = 0;
	virtual void FinalizeMessage() = 0;
};

