#include "GsNetMessageHandlerSeal.h"

#include "Runtime/Core/Public/Containers/StringFwd.h"

#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "Management/ScopeGame/GsSealManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Message/GsMessageNet.h"
#include "Message/MessageParam/GsSealMessageParam.h"
#include "Net/GSNet.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Management/GsScopeGame.h"

void FGsNetMessageHandlerSeal::InitializeMessage(FGsNetManager* InManager)
{
	// 패킷 바인딩
	MProtocalWorld& WorldProtocol = InManager->GetProtocalWorld();

	InitializeMessageDelegateList(&WorldProtocol);

	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_SEAL, this, &FGsNetMessageHandlerSeal::SEAL));
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_SEAL_LEVEL_UP, this, &FGsNetMessageHandlerSeal::ACK_SEAL_LEVEL_UP));
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_SEAL_SLOT_CHANGE_ALL, this, &FGsNetMessageHandlerSeal::ACK_SEAL_SLOT_CHANGE_ALL));
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_SEAL_SLOT_LOCK_SAVE, this, &FGsNetMessageHandlerSeal::ACK_SEAL_SLOT_LOCK_SAVE));
}

void FGsNetMessageHandlerSeal::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerSeal::SEAL(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_SEAL_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_SEAL_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	TArray<SealPacketData> SealPacketDatas;
	
	// Iterator 순회하면서 LevelId 읽기
	using PacketIterator = PD::SC::PKT_SC_SEAL_READ::SealPackListIterator;
	using SealDataListIterator = PD::SC::PKT_SC_SEAL_READ::SealPackList::SealDataListIterator;
	using SealSlotDataListIterator = PD::SC::PKT_SC_SEAL_READ::SealPackList::SealSlotDataListIterator;
	using SealBonusDataListIterator = PD::SC::PKT_SC_SEAL_READ::SealPackList::SealBonusDataListIterator;

	for (PacketIterator iter = Packet->GetFirstSealPackListIterator();
		iter != Packet->GetLastSealPackListIterator(); ++iter)
	{
		SealPackId PackId;
		TArray<SealData> SealDatas;
		TArray<SealSlotData> SealSlotDatas;
		TArray<SealBonusData> SealBonusDatas;

		PackId = iter->SealPackId();
		
		for (SealDataListIterator SealDataIter = iter->GetFirstSealDataListIterator(); SealDataIter != iter->GetLastSealDataListIterator(); ++SealDataIter)
		{
			SealDatas.Emplace(SealDataIter->SealData());
		}

		for (SealSlotDataListIterator SealSlotDataIter = iter->GetFirstSealSlotDataListIterator(); SealSlotDataIter != iter->GetLastSealSlotDataListIterator(); ++SealSlotDataIter)
		{
			SealSlotDatas.Emplace(SealSlotDataIter->SealSlotData());
		}

		for (SealBonusDataListIterator SealBonusDataIter = iter->GetFirstSealBonusDataListIterator(); SealBonusDataIter != iter->GetLastSealBonusDataListIterator(); ++SealBonusDataIter)
		{
			SealBonusDatas.Emplace(SealBonusDataIter->SealBonusData());
		}

		SealPacketDatas.Emplace(PackId, SealDatas, SealSlotDatas, SealBonusDatas);
	}
	

	//Packet->MakeSealDataListVector<TArray<SealData>, SealData>(SealDatas);
	//Packet->MakeSealSlotDataListVector<TArray<SealSlotData>, SealSlotData>(SealSlotDatas);
	//Packet->MakeSealBonusDataListVector<TArray<SealBonusData>, SealBonusData>(SealBonusDatas);

#if !NO_LOGGING
	for (SealPacketData PacketData : SealPacketDatas)
	{
		FString Logs;
		Logs.Append(TEXT("PKT_SC_SEAL_READ\n"));
		for (int32 i = 0; i < PacketData._sealDatas.Num(); ++i)
		{
			const SealData& CurrentSealData = PacketData._sealDatas[i];
			Logs.Appendf(TEXT(" - SealDatas[%d]: mSealId:%s, mLevel:%s, mExp:%s\n"), i, *LexToString(CurrentSealData.mSealId), *LexToString(CurrentSealData.mLevel), *LexToString(CurrentSealData.mExp));
		}
		for (int32 i = 0; i < PacketData._sealSlotDatas.Num(); ++i)
		{
			const SealSlotData& CurrentSealSlotData = PacketData._sealSlotDatas[i];
			Logs.Appendf(TEXT(" - SealSlotDatas[%d]: mSealId:%s, mSealSlotId:%s, mSealSlotColor:%s, mStatType:%s, mStatValue:%s\n"), i, *LexToString(CurrentSealSlotData.mSealId), *LexToString(CurrentSealSlotData.mSealSlotId), *SealSlotColorEnumToString(CurrentSealSlotData.mSealSlotColor), *StatTypeEnumToString(CurrentSealSlotData.mStatType), *LexToString(CurrentSealSlotData.mStatValue));
		}
		for (int32 i = 0; i < PacketData._sealBonusDatas.Num(); ++i)
		{
			const SealBonusData& CurrentSealBonusData = PacketData._sealBonusDatas[i];
			Logs.Appendf(TEXT(" - SealBonusData[%d]: mSealSlotColor:%s, mCount:%s\n"), i, *SealSlotColorEnumToString(CurrentSealBonusData.mSealSlotColor), *LexToString(CurrentSealBonusData.mCount));
		}
		GSLOG(Log, TEXT("%s"), *Logs);
	}
#endif
	
	const FGsSealMessageParamSeal MessageParam(SealPacketDatas);
	FGsSealManager* SealManager = GSSeal();
	if (nullptr != SealManager)
	{
		SealManager->Seal(&MessageParam);
	}
	MessageParam.Send();
}

void FGsNetMessageHandlerSeal::SEAL_SLOT_LOCK(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_SEAL_SLOT_LOCK_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_SEAL_SLOT_LOCK_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	TArray<SealSlotLock> SealSlotLockList;

	// Iterator 순회하면서 LevelId 읽기
	using PacketIterator = PD::SC::PKT_SC_SEAL_SLOT_LOCK_READ::SealSlotLockListIterator;

	for (PacketIterator iter = Packet->GetFirstSealSlotLockListIterator();
		iter != Packet->GetLastSealSlotLockListIterator(); ++iter)
	{
		SealSlotLock SlotLock;
		SlotLock._sealSlotId = iter->SealSlotId();
		SlotLock._isStatLock = iter->IsStatLock();
		SlotLock._isColorLock = iter->IsColorLock();

		SealSlotLockList.Emplace(SlotLock);
	}

	const FGsSealMessageParamSealSlotLock MessageParam(SealSlotLockList);
	FGsSealManager* SealManager = GSSeal();
	if (nullptr != SealManager)
	{
		SealManager->SealSlotLock(&MessageParam);
	}
	MessageParam.Send();
}

void FGsNetMessageHandlerSeal::ACK_SEAL_LEVEL_UP(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_SEAL_LEVEL_UP_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_ACK_SEAL_LEVEL_UP_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

#if !NO_LOGGING
	FString Logs;
	Logs.Appendf(TEXT("PKT_SC_ACK_SEAL_LEVEL_UP_READ Packet->Result(): %s(%s)\n"), *ResultEnumToString((PD::Result)Packet->Result()), *LexToString(Packet->Result()));
	const SealData& CurrentSealData = Packet->ConstRefUpdateSealData();
	Logs.Appendf(TEXT(" - SealDataPacket: mSealId:%s, mLevel:%s, mExp:%s\n"), *LexToString(CurrentSealData.mSealId), *LexToString(CurrentSealData.mLevel), *LexToString(CurrentSealData.mExp));
	GSLOG(Log, TEXT("%s"), *Logs);
#endif

	const PD::Result CurrentResult = StaticCast<PD::Result>(Packet->Result());
	const bool PacketNoError = (PACKET_RESULT_SUCCESS == CurrentResult);
	if (!PacketNoError)
	{
		FGsUIHelper::PopupNetError(CurrentResult);
	}

	const FGsSealMessageParamAckSealLevelUp MessageParam(PacketNoError, Packet->SealPackId(), Packet->ConstRefUpdateSealData());
	FGsSealManager* SealManager = GSSeal();
	if (nullptr != SealManager)
	{
		SealManager->AckSealLevelUp(&MessageParam);
	}
	MessageParam.Send();
}

void FGsNetMessageHandlerSeal::ACK_SEAL_SLOT_CHANGE_ALL(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_SEAL_SLOT_CHANGE_ALL_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_ACK_SEAL_SLOT_CHANGE_ALL_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	SealPackId PackId;
	TArray<SealSlotData> SealSlotDatas;
	TArray<SealBonusData> SealBonusDatas;

	PackId = Packet->SealPackId();
	Packet->MakeSealSlotDataListVector<TArray<SealSlotData>, SealSlotData>(SealSlotDatas);
	Packet->MakeSealBonusDataListVector<TArray<SealBonusData>, SealBonusData>(SealBonusDatas);

#if !NO_LOGGING
	FString Logs;
	Logs.Appendf(TEXT("PKT_SC_ACK_SEAL_SLOT_CHANGE_ALL_READ Packet->Result(): %s(%s)\n"), *ResultEnumToString((PD::Result)Packet->Result()), *LexToString(Packet->Result()));
	for (int32 i = 0; i < SealSlotDatas.Num(); ++i)
	{
		const SealSlotData& CurrentSealSlotData = SealSlotDatas[i];
		Logs.Appendf(TEXT(" - SealSlotDatas[%d]: mSealId:%s, mSealSlotId:%s, mSealSlotColor:%s, mStatType:%s, mStatValue:%s\n"), i, *LexToString(CurrentSealSlotData.mSealId), *LexToString(CurrentSealSlotData.mSealSlotId), *SealSlotColorEnumToString(CurrentSealSlotData.mSealSlotColor), *StatTypeEnumToString(CurrentSealSlotData.mStatType), *LexToString(CurrentSealSlotData.mStatValue));
	}
	for (int32 i = 0; i < SealBonusDatas.Num(); ++i)
	{
		const SealBonusData& CurrentSealBonusData = SealBonusDatas[i];
		Logs.Appendf(TEXT(" - SealBonusData[%d]: mSealSlotColor:%s, mCount:%s\n"), i, *SealSlotColorEnumToString(CurrentSealBonusData.mSealSlotColor), *LexToString(CurrentSealBonusData.mCount));
	}
	GSLOG(Log, TEXT("%s"), *Logs);
#endif

	const PD::Result CurrentResult = StaticCast<PD::Result>(Packet->Result());
	const bool PacketNoError = (PACKET_RESULT_SUCCESS == CurrentResult);
	if (!PacketNoError)
	{
		FGsUIHelper::PopupNetError(CurrentResult);
	}

	const FGsSealMessageParamAckSealSlotChangeAll MessageParam(PacketNoError, PackId, SealSlotDatas, SealBonusDatas);
	FGsSealManager* SealManager = GSSeal();
	if (nullptr != SealManager)
	{
		SealManager->AckSealSlotChangeAll(&MessageParam);
	}
	MessageParam.Send();
}

void FGsNetMessageHandlerSeal::ACK_SEAL_SLOT_LOCK_SAVE(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_SEAL_SLOT_LOCK_SAVE_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_ACK_SEAL_SLOT_LOCK_SAVE_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	const PD::Result CurrentResult = StaticCast<PD::Result>(Packet->Result());
	const bool PacketNoError = (PACKET_RESULT_SUCCESS == CurrentResult);
	if (!PacketNoError)
	{
		// 만약 슬롯 정보 저장을 실패했으면, 로비로 돌려보낸다
		GLevel()->BackToLobby();
	}

	const FGsSealMessageParamSlotLockSave MessageParam;
	FGsSealManager* SealManager = GSSeal();
	if (nullptr != SealManager)
	{
		SealManager->AckSealSlotLockSave(&MessageParam);
	}
	MessageParam.Send();
}
