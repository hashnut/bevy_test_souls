
#include "GsNetMessageHandlerTitle.h"
#include "Message/GsMessageInvasion.h"
#include "Message/GsMessageDelegateList.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGame/GsTitleManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "T1Project/T1Project.h"
#include "../GameObject/ObjectClass/GsGameObjectBase.h"
#include "../GameObject/Define/GsGameObjectDefine.h"
#include "../GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "../GameObject/Data/GsGameObjectDataRemotePlayer.h"
#include "../GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "../GameObject/ObjectClass/GsGameObjectRemotePlayer.h"




void FGsNetMessageHandlerTitle::InitializeMessage(FGsNetManager* InManager)
{
	// 패킷 바인딩
	MProtocalWorld& WorldProtocol = InManager->GetProtocalWorld();

	InitializeMessageDelegateList(&WorldProtocol);

	// 2022/01/21 PKT - 칭호 정보 업데이트
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_TITLE_LIST
		, this
		, &FGsNetMessageHandlerTitle::Notify_TITLE_LIST));

	// 2022/01/21 PKT - 칭호 정보 업데이트	
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_TITLE_UPDATED
		, this
		, &FGsNetMessageHandlerTitle::Ack_TITLE_UPDATED));

	// 2022/01/21 PKT - 타 유저의 칭호 장착정보
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_NOTIFY_USER_TITLE_INFO
		, this
		, &FGsNetMessageHandlerTitle::Notify_USER_TITLE_INFO));

	// 2022/01/21 PKT - 칭호 장착 응답
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_ACK_TITLE_EQUIP
		, this
		, &FGsNetMessageHandlerTitle::Ack_TITLE_EQUIP));

	// 2022/08/30 PKT - 침공 전투 레벨
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_INVADE_BATTLE_LEVEL
		, this
		, &FGsNetMessageHandlerTitle::Notify_BattleLevel));

	// 2022/08/30 PKT - 타유저 침공 전투 레벨
	AddMessageDelegate(WorldProtocol.AddRaw(PD::SC::SCPacketId::SC_NOTIFY_INVADE_BATTLE_LEVEL
		, this
		, &FGsNetMessageHandlerTitle::Notify_User_BattleLevel));
}

void FGsNetMessageHandlerTitle::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

// 2022/01/21 PKT - 칭호 정보
void FGsNetMessageHandlerTitle::Notify_TITLE_LIST(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_TITLE_LIST_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_TITLE_LIST_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}
	
	TArray<TitleInfo> titleDataList;
	Packet->MakeTitleListVector<TArray<TitleInfo>, TitleInfo>(titleDataList);

	TPair<TitleId, TitleId> equipData = TPair<TitleId, TitleId>(Packet->FrontEquipTitle(), Packet->BackEquipTitle());

	FGsTitleManager* manager = GTitle();
	if (nullptr == manager)
	{
		GSLOG(Error, TEXT("nullptr == manager"));
		return;
	}

	manager->InitTitleData(titleDataList, equipData);
}

// 2022/01/21 PKT - 칭호 정보 업데이트
void FGsNetMessageHandlerTitle::Ack_TITLE_UPDATED(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_TITLE_UPDATED_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_TITLE_UPDATED_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}
		
	FGsTitleManager* manager = GTitle();
	if (nullptr == manager)
	{
		GSLOG(Error, TEXT("nullptr == manager"));
		return;
	}

	manager->UpdateTitle(Packet->TitleInfo());
}

// 2022/08/26 PKT - 침공 관련 배틀 등급 
void FGsNetMessageHandlerTitle::Notify_BattleLevel(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_INVADE_BATTLE_LEVEL_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_INVADE_BATTLE_LEVEL_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	/**
	 * HUD Update
	 */
	UGsGameObjectBase* object = GSGameObject() ? GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer) : nullptr;
	UGsGameObjectLocalPlayer* localPlayer = (nullptr != object) ? Cast<UGsGameObjectLocalPlayer>(object) : nullptr;
	FGsGameObjectDataLocalPlayer* localData = (nullptr != localPlayer) ? (localPlayer->GetCastData<FGsGameObjectDataLocalPlayer>()) : nullptr;
	if (nullptr == localData)
	{
		return;
	}

	localData->SetInvadeBattleLevel(Packet->InvadeBattleLevel());

	// 2022/02/03 PKT - Billboard Update
	localPlayer->UpdateBillboard(EGsBillboardUpdateType::Title);

	FGsMessageHolder* messageHolder = GMessage();
	if (messageHolder)
	{	// 2022/08/26 PKT - UI 갱신
		messageHolder->GetInvasion().SendMessage(MessageInvasion::INVALID_INVASION_USER_COUNT, nullptr);
	}
}

void FGsNetMessageHandlerTitle::Notify_User_BattleLevel(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_NOTIFY_INVADE_BATTLE_LEVEL_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_NOTIFY_INVADE_BATTLE_LEVEL_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}
	
	int64 InGameId = Packet->GameId();

	if (GSGameObject())
	{
		UGsGameObjectBase* object = GSGameObject()->FindObject(EGsGameObjectType::RemotePlayer, InGameId);
		UGsGameObjectRemotePlayer* remotePlayer = (nullptr != object) ? Cast<UGsGameObjectRemotePlayer>(object) : nullptr;
		FGsGameObjectDataRemotePlayer* remoteData = (nullptr != remotePlayer) ? remotePlayer->GetCastData<FGsGameObjectDataRemotePlayer>() : nullptr;
		if (nullptr == remotePlayer)
		{
			GSLOG(Error, TEXT("not find Remote Data [%lu]"), InGameId);
			return;
		}

		remoteData->SetInvadeBattleLevel(Packet->InvadeBattleLevel());

		remotePlayer->UpdateBillboard(EGsBillboardUpdateType::Title);
	}	
}

// 2022/01/21 PKT - 타 유저의 칭호 장착정보
void FGsNetMessageHandlerTitle::Notify_USER_TITLE_INFO(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_NOTIFY_USER_TITLE_INFO_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_NOTIFY_USER_TITLE_INFO_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}
		
	FGsTitleManager* manager = GTitle();
	if (nullptr == manager)
	{
		GSLOG(Error, TEXT("nullptr == manager"));
		return;
	}

	manager->RemotePlayerEquipTitle(Packet->GameId(), Packet->FrontEquipTitle(), Packet->BackEquipTitle());
}

// 2022/01/21 PKT - 칭호 장착 응답
void FGsNetMessageHandlerTitle::Ack_TITLE_EQUIP(const FGsNet::Packet& InPacket)
{
	PD::SC::PKT_SC_ACK_TITLE_EQUIP_READ* Packet = reinterpret_cast<PD::SC::PKT_SC_ACK_TITLE_EQUIP_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	const PD::Result Result = StaticCast<PD::Result>(Packet->Result());
	if (Result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::TraySectionMessageTickerNetError(StaticCast<PD::Result>(Packet->Result()));
		return;
	}

	FGsTitleManager* manager = GTitle();
	if (nullptr == manager)
	{
		GSLOG(Error, TEXT("nullptr == manager"));
		return;
	}

	manager->PlayerEquipTitle(Packet->EquipedTitleId(), Packet->UnEquipedTitleId());
}