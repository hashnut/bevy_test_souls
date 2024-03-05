#include "GsNetSendServiceSeal.h"

#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"

#if !NO_LOGGING

FString LexToString(const PD::CS::PKT_CS_REQ_SEAL_LEVEL_UP_WRITE& InPacket)
{
	PD::CS::PKT_CS_REQ_SEAL_LEVEL_UP_READ* ReadPacket = reinterpret_cast<PD::CS::PKT_CS_REQ_SEAL_LEVEL_UP_READ*>(InPacket.mPktWriter.mBuf);
	return FString::Printf(TEXT("PKT_CS_REQ_SEAL_LEVEL_UP_READ: mSealId: %s\n"), *LexToString(ReadPacket->ConstRefSealId()));
}

FString LexToString(const PD::CS::PKT_CS_REQ_SEAL_SLOT_CHANGE_ALL_WRITE& InPacket)
{
	PD::CS::PKT_CS_REQ_SEAL_SLOT_CHANGE_ALL_READ* ReadPacket = reinterpret_cast<PD::CS::PKT_CS_REQ_SEAL_SLOT_CHANGE_ALL_READ*>(InPacket.mPktWriter.mBuf);

	TSet<SealSlotId> GradeLockIdList;
	TSet<SealSlotId> ColorLockIdList;

	ReadPacket->MakeGradeLockIdListVector<TSet<SealSlotId>, SealSlotId>(GradeLockIdList);
	ReadPacket->MakeColorLockIdListVector<TSet<SealSlotId>, SealSlotId>(ColorLockIdList);

	FString Logs;
	Logs.Appendf(TEXT("PKT_CS_REQ_SEAL_SLOT_CHANGE_ALL_READ: SealId: %s\n"), *LexToString(ReadPacket->SealId()));
	for (const SealSlotId& GradeLockId : GradeLockIdList)
	{
		Logs.Appendf(TEXT(" - GradeLockIdList: %s\n"), *LexToString(GradeLockId));
	}
	for (const SealSlotId& GradeLockId : ColorLockIdList)
	{
		Logs.Appendf(TEXT(" - ColorLockIdList: %s\n"), *LexToString(GradeLockId));
	}
	return Logs;
}

FString LexToString(const PD::CS::PKT_CS_REQ_SEAL_SLOT_LOCK_SAVE_WRITE& InPacket)
{
	PD::CS::PKT_CS_REQ_SEAL_SLOT_LOCK_SAVE_READ* ReadPacket = reinterpret_cast<PD::CS::PKT_CS_REQ_SEAL_SLOT_LOCK_SAVE_READ*>(InPacket.mPktWriter.mBuf);

	FString Logs;
	Logs.Appendf(TEXT("PKT_CS_REQ_SEAL_SLOT_LOCK_SAVE_READ\n"));
	return Logs;
}

#endif
