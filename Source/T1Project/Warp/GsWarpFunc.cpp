#include "GsWarpFunc.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Currency/GsCurrencyHelper.h"

#include "Shared/Shared/SharedInclude/SharedDefine.h"


// 재화 충분한지
bool FGsWarpFunc::IsCurrencyEnougth(CurrencyType In_type, int In_cost)
{
	uint64 nowCurrency = FGsCurrencyHelper::GetCurrencyAmount(In_type);

	bool isEnough = (nowCurrency >= In_cost) ? true : false;
	// 모자르면 티커 출력
	if (isEnough == false)
	{
		// 이동에 필요한 재화가 부족합니다
		FText findText;
		FText::FindText(TEXT("WorldMapText"), TEXT("MenuWarpCostNotEnough"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
	}

	return isEnough;
}

// 워프 실패 체크
bool FGsWarpFunc::CheckWarpFailure(PD::Result In_result)
{
	if (In_result == PACKET_RESULT_SUCCESS)
	{
		return true;
	}

	GMessage()->GetSystem().SendMessage(MessageSystem::WARP_FAILUER);

	switch (In_result)
	{
	case PD::Result::WARP_ERROR_INVALID_STATUS:
	case PD::Result::PARTY_ERROR_INVALID_LEADER_MAP:
	case PD::Result::PARTY_ERROR_INVALID_LEADER_MAP_SPWAN_SPOT:
	case PD::Result::PARTY_ERROR_LEADER_IN_INSTANCE_MAP:
	//https://jira.com2us.com/jira/browse/C2URWQ-1760
	case PD::Result::PARTY_ERROR_UNKNOWN:
	{
		// 대상의 위치로 이동할 수 없습니다
		FText findText;
		FText::FindText(TEXT("PartyText"), TEXT("PartyChannelFail"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return true;
	}
	case PD::Result::PARTY_ERROR_NOT_JOINED:
	{
		// 파티를 먼저 생성해야 합니다
		FText findText;
		FText::FindText(TEXT("PartyText"), TEXT("PartyTicker7"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return true;
	}
	case PD::Result::SELECT_CHANNEL_ERROR_CONGESTION_FULL:
	{
		// 해당 채널에 유저가 많아서 이동할 수 없습니다.
		FText findText;
		FText::FindText(TEXT("UICommonText"), TEXT("ChannelChangeDeny"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return true;
	}
	case PD::Result::WARP_ERROR_IN_COMBAT:
	{
		// 전투중에는 이동할 수 없습니다.
		FText findText;
		FText::FindText(TEXT("UICommonText"), TEXT("ChannelCombatDeny"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return true;
	}
	case PD::Result::PARTY_ERROR_WARP_TO_LEADER_MYSELF:
	{
		// 대상의 위치로 이동할 수 없습니다.
		FText findText;
		FText::FindText(TEXT("PartyText"), TEXT("PartyChannelFail"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return true;
	}
	case PD::Result::QUEST_ERROR_INVALID_WARP_QUEST_NOT_TELEPORTABLE_NOT_ACCEPTED:
	{
		// QUEST - 워프 하려는 Quest가 수락중이 아니여서 텔레포트가 불가능합니다
		FText findText;
		FText::FindText(TEXT("NetText"), TEXT("QUEST_ERROR_INVALID_WARP_QUEST_ID"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return true;
	}
	case PD::Result::QUEST_ERROR_INVALID_WARP_NOT_MAP_WORLD:
	{
		// QUEST - 월드서버 내의 있는 맵이 아닙니다. Map이 아닙니다.
		FGsUIHelper::TraySectionMessageTickerNetError(In_result);
		return true;
	}	
	}

	return false;
}