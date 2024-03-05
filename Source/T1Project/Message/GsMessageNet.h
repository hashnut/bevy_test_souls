#pragma once

#include "Net/GsNet.h"

#include "Shared/Shared/SharedPackets/PD_Auth_Client.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "GsMessageDelegateList.h"

#include "Classes/GsMessageHandler.h"
#include "Classes/GsMessageHandlerPacket.h"
#include "Classes/GsMessageHandlerOneParam.h"

	// 로그인 패킷 연동 처리
using	MProtocalAuth = TGsMessageHandlerOneParam<PD::AC::ACPacketId, const struct FGsNet::Packet&>;
using	MResultAuth = TGsMessageHandler<PD::Result>;

// 게이트웨이 패킷 연동처리
using	MProtocalGateway = TGsMessageHandlerPacket<PD::GC::GCPacketId, const struct FGsNet::Packet&>;
using	MResultGateway = TGsMessageHandler<PD::Result>;

// 월드 패킷 연동처리
using	MProtocalWorld = TGsMessageHandlerPacket<PD::SC::SCPacketId, const struct FGsNet::Packet&>;
using	MResultWorld = TGsMessageHandler<PD::Result>;


using FGsNetMessageWorld = TGsMessageDelegateList<MProtocalWorld, PD::SC::SCPacketId>;
using FGsNetMessageAuth = TGsMessageDelegateList<MProtocalAuth, PD::AC::ACPacketId>;
using FGsNetMessageGateway = TGsMessageDelegateList<MProtocalGateway, PD::GC::GCPacketId>;
