
#define  SPACECRACK_DEBUG true

#include "GsNetMessageHandlerSpaceCrack.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsSpaceCrackManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/GsMessageSystem.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "Input/GsInputBindingLocalPlayer.h"

#include "SharedPointer.h"
#include "SpaceCrack/GsSpaceCrackData.h"

void FGsNetMessageHandlerSpaceCrack::InitializeMessage(FGsNetManager* inManager)
{
	// 패킷 바인딩
	MProtocalWorld& protocolWorld = inManager->GetProtocalWorld();

	InitializeMessageDelegateList(&protocolWorld);

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SPACE_CRACK_SPAWN_ALARM,
		this, &FGsNetMessageHandlerSpaceCrack::SpaceCrackSpawnAlarm));

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SPACE_CRACK_DESPAWN_ALARM,
		this, &FGsNetMessageHandlerSpaceCrack::SpaceCrackDespawnAlarm));

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SPACE_CRACK_ACTIVATION_LIST,
		this, &FGsNetMessageHandlerSpaceCrack::SpaceCrackActivationList));

	AddMessageDelegate(protocolWorld.AddRaw(
		PD::SC::SCPacketId::SC_SPACE_CRACK_ACTIVATION_REFRESH,
		this, &FGsNetMessageHandlerSpaceCrack::SpaceCrackActivationRefresh));

	//카운트 다운에 사용할 공용 패킷
	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_OPEN_UI,
		this, &FGsNetMessageHandlerSpaceCrack::OpenUI));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_TIMER_START,
		this, &FGsNetMessageHandlerSpaceCrack::TimerStart));

	AddMessageDelegate(protocolWorld.AddRaw(PD::SC::SCPacketId::SC_COUNT_DOWN_START,
		this, &FGsNetMessageHandlerSpaceCrack::CountDownStart));
}

void FGsNetMessageHandlerSpaceCrack::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

// 시공의 틈새 활성화 정보 및 알람 해야하는지 알려주는 패킷
void FGsNetMessageHandlerSpaceCrack::SpaceCrackSpawnAlarm(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SPACE_CRACK_SPAWN_ALARM_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_SPACE_CRACK_SPAWN_ALARM_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
		return;

	FGsSpaceCrackManager* spaceCrackManager = GSSpaceCrack();
	if (spaceCrackManager == nullptr)
	{
		return;
	}

	TArray<SpaceCrackAlarm> spaceCrackAlarmArray;
	pkt->MakeSpaceCrackAlarmListVector<TArray<SpaceCrackAlarm>, SpaceCrackAlarm>(spaceCrackAlarmArray);
	for (const SpaceCrackAlarm& spaceCrackAlarm : spaceCrackAlarmArray)
	{
		spaceCrackManager->AddSpaceCrackAlarmInfo(spaceCrackAlarm);

#if SPACECRACK_DEBUG
		if (GLevel()->GetCurrentLevelId() == spaceCrackAlarm.mMapId)
		{
			GSLOG(Log, TEXT("SpaceCrack_DEBUG - Minimap - Spawn space crack id : %llu\tmap id : %d\tpos : %s"), spaceCrackAlarm.mGameId, spaceCrackAlarm.mMapId, *FVector(spaceCrackAlarm.mX, spaceCrackAlarm.mY, spaceCrackAlarm.mZ).ToString());
		}
#endif	

#if WITH_EDITOR
		GSLOG(Log, TEXT("[SpaceCrack] RECV SPACE_CRACK_SPAWN_ALARM -> GameId : %llu, SpawnPos : %0.f/%0.f/%0.f, MapId : %d, ChannelId : %d"),
			spaceCrackAlarm.mGameId, spaceCrackAlarm.mX, spaceCrackAlarm.mY, spaceCrackAlarm.mZ, spaceCrackAlarm.mMapId, spaceCrackAlarm.mChannelId);
#endif
	}
}

void FGsNetMessageHandlerSpaceCrack::SpaceCrackDespawnAlarm(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SPACE_CRACK_DESPAWN_ALARM_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_SPACE_CRACK_DESPAWN_ALARM_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
		return;

	FGsSpaceCrackManager* spaceCrackManager = GSSpaceCrack();
	if (spaceCrackManager == nullptr)
	{
		return;
	}

	TArray<MapId> mapIdArray;
	pkt->MakeMapIdListVector<TArray<MapId>, MapId>(mapIdArray);
	for (const int64& mapId : mapIdArray)
	{
		spaceCrackManager->DelSpaceCrackPortalInfo(mapId);

#if WITH_EDITOR
		GSLOG(Log, TEXT("[SpaceCrack] RECV SPACE_CRACK_DESPAWN_ALARM -> map id : %d"), mapId);
#endif
	}
}

void FGsNetMessageHandlerSpaceCrack::SpaceCrackActivationList(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SPACE_CRACK_ACTIVATION_LIST_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_SPACE_CRACK_ACTIVATION_LIST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
		return;

	FGsSpaceCrackManager* spaceCrackManager = GSSpaceCrack();
	if (spaceCrackManager == nullptr)
	{
		return;
	}

	TArray<SpaceCrackActivation> spaceCrackActivationArray;
	pkt->MakeSpaceCrackActivationListVector<TArray<SpaceCrackActivation>, SpaceCrackActivation>(spaceCrackActivationArray);

	for (const SpaceCrackActivation& inData : spaceCrackActivationArray)
	{
		spaceCrackManager->AddSpaceCrackActivationInfo(inData);
	}
}

void FGsNetMessageHandlerSpaceCrack::SpaceCrackActivationRefresh(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_SPACE_CRACK_ACTIVATION_REFRESH_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_SPACE_CRACK_ACTIVATION_REFRESH_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
		return;

	FGsSpaceCrackManager* spaceCrackManager = GSSpaceCrack();
	if (spaceCrackManager == nullptr)
	{
		return;
	}

	spaceCrackManager->UpdateSpaceCrackActivationInfo(pkt->SpaceCrackActivation());
}

// 특정 UI OPEN 해당하는 UI 실행 
void FGsNetMessageHandlerSpaceCrack::OpenUI(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_OPEN_UI_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_OPEN_UI_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	UIType uiType = pkt->UiType();

	switch (uiType)
	{
	case UIType::NONE:
		break;
	case UIType::SUMMONS:
	{
		FText main, sub;
		FText::FindText(TEXT("SpaceCrackText"), TEXT("MenuSummon"), main);
		FText::FindText(TEXT("SpaceCrackText"), TEXT("MenuSummonText"), sub);
		FGsUIHelper::TrayTickerCommon(main,sub);
	}		
		break;
	case UIType::BATTLE:
	{
		FText main, sub;
		FText::FindText(TEXT("SpaceCrackText"), TEXT("MenuFight"), main);
		FText::FindText(TEXT("SpaceCrackText"), TEXT("MenuFightText"), sub);
		FGsUIHelper::TrayTickerBattle(main, sub);
	}		
		break;
	case UIType::CONTENT_FAILURE:
		FGsUIHelper::TrayTickerFail();
		break;
	case UIType::CONTENT_SUCCESS:
		FGsUIHelper::TrayTickerClear();
		break;
	case UIType::REWARD:
		break;
	case UIType::SKILL_HUD:
		break;
	case UIType::MAX:
		break;
	default:
		break;
	}
}

// 타이머 시작
void FGsNetMessageHandlerSpaceCrack::TimerStart(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[SpaceCrack] TIMER_START"));
#endif

	PD::SC::PKT_SC_TIMER_START_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_TIMER_START_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	int32 startSecond = pkt->StartSecond();
	int32 finishSecond = pkt->FinishSecond();

#if WITH_EDITOR
	GSLOG(Log, TEXT("[SpaceCrack] TIMER_START startSecond: %d, finishSecond : %d"), startSecond, finishSecond);
#endif

	// 타임 Msg
	FGsUIMsgParamTime param = FGsUIMsgParamTime(startSecond, finishSecond);
	GMessage()->GetSystemParam().SendMessage(MessageSystem::SERVER_TIME_START, &param);
}

// 카운트 다운 시작 
void FGsNetMessageHandlerSpaceCrack::CountDownStart(const FGsNet::Packet& inPacket)
{
	PD::SC::PKT_SC_COUNT_DOWN_START_READ* pkt = reinterpret_cast<PD::SC::PKT_SC_COUNT_DOWN_START_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsUIHelper::TrayTickerCount(0 , pkt->Second());
}
