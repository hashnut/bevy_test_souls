// Fill out your copyright notice in the Description page of Project Settings.


#include "GsContentsManagerGame.h"

#include "Engine/Engine.h"
#include "Engine/World.h"

#include "GSGameInstance.h"

#include "GameFlow/GameContents/GsContentsMode.h"

#include "Net/GsNetSendService.h"
#include "Shared/Client/SharedEnums/KickReasonEnum.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_gateWay_Client.h"

#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Management/ScopeGlobal/GsBMShopManager.h"

#include "UTIL/GsLevelUtil.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "GsGameStateOption.h"
#include "GsGameStateQuest.h"
#include "GsGameStateSkill.h"
#include "GsGameStateContinentalMap.h"
#include "GsGameStateRegionMap.h"
#include "GsGameStateFairy.h"
#include "GsGameStateGuild.h"
#include "GsGameStateCostume.h"
#include "GsGameStateMail.h"
#include "GsGameStateItemCollection.h"
#include "GsGameStateMarket.h"
#include "GsGameStateCraft.h"
#include "GsGameStateMultiLevelRank.h"
#include "GsGameStateOfflinePlay.h"
#include "GsGameStateBMShop.h"
#include "GsGameStateCommunity.h"
#include "GsGameStateRanking.h"
#include "GsGameStateMonsterCollection.h"
#include "GsGameStateTitle.h"
#include "GsGameStateGameStatistics.h"
#include "GsGameStateUserProfile.h"
#include "GsGameStateAchievement.h"
#include "GsGameStateDungeon.h"
#include "GsGameStateBoss.h"
#include "GsGameStateQuestBoard.h"
#include "GsGameStatePKBook.h"
#include "GsGameStateRepair.h"
#include "GsGameStateRecompose.h"
#include "GsGameStateKeyBoardMapping.h"
#include "GsGameStateInvade.h"
#include "GsGameStateSeal.h"
#include "GsGameStateBattlePass.h"
#include "GsGameStateSiegeWarfare.h"
#include "GsGameStateExchangeCostume.h"
#include "GsGameStateSpiritShot.h"
#include "GsGameStateAgitDungeon.h"
#include "GsGameStateStarLegacy.h"

#include "Hud/GsGameStateHud.h"
#include "Depot/GsGameStateNpcDepot.h"
#include "NpcShop/GsGameStateNpcShop.h"

#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsMessageParam.h"


FGsContentsGameBase* FGsContentsGameAllocator::Alloc(FGsContentsMode::InGame inMode)
{
	switch (inMode)
	{
	case FGsContentsMode::ContentsHud:
		return new FGsGameStateHud();
	case FGsContentsMode::InGame::ContentsOption:
		return new FGsGameStateOption();
	case FGsContentsMode::InGame::ContentsQuest:
		return new FGsGameStateQuest();
	case FGsContentsMode::InGame::ContentsSkill:
		return new FGsGameStateSkill();
	case FGsContentsMode::ContentsContinentalMap:
		return new FGsGameStateContinentalMap();
	case FGsContentsMode::ContentsRegionMap:
		return new FGsGameStateRegionMap();
	case  FGsContentsMode::ContentsFairy:
		return new FGsGameStateFairy();
	case  FGsContentsMode::ContentsNpcShop:
		return new FGsGameStateNpcShop();
	case FGsContentsMode::ContentsGuild:
		return new FGsGameStateGuild();
	case FGsContentsMode::ContentsCostume:
		return new FGsGameStateCostume();
	case FGsContentsMode::ContentsMail:
		return new FGsGameStateMail();
	case FGsContentsMode::ContentsItemCollection:
		return new FGsGameStateItemCollection();
	case FGsContentsMode::ContentsDungeon:
		return new FGsGameStateDungeon();
	case FGsContentsMode::ContentsNpcDepot:
		return new FGsGameStateNpcDepot();
	case FGsContentsMode::ContentsMarket:
		return new FGsGameStateMarket();
	case FGsContentsMode::ContentsCraft:
		return new FGsGameStateCraft();
	case FGsContentsMode::ContentsMultiLevelRank:
		return new FGsGameStateMultiLevelRank();	
	case FGsContentsMode::ContentsOfflinePlay:
		return new FGsGameStateOfflinePlay();
	case FGsContentsMode::ContentsBMShop:
		return new FGsGameStateBMShop();
	case FGsContentsMode::ContentsCommunity:
		return new FGsGameStateCommunity();
	case FGsContentsMode::ContentsRanking:
		return new FGsGameStateRanking();
	case FGsContentsMode::ContentsAchievement:
		return new FGsGameStateAchievement();	
	case FGsContentsMode::ContentsMonsterCollection:
		return new FGsGameStateMonsterCollection();
	case FGsContentsMode::ContentsTitle:
		return new FGsGameStateTitle();
	case FGsContentsMode::ContentsGameStatistics:
		return new FGsGameStateGameStatistics();
	case FGsContentsMode::ContentsUserProfile:
		return new FGsGameStateUserProfile();
	case FGsContentsMode::ContentsBoss:
		return new FGsGameStateBoss();
	case FGsContentsMode::ContentsQuestBoard:
		return new FGsGameStateQuestBoard();
	case FGsContentsMode::ContentsPKBook:
		return new FGsGameStatePKBook();
	case FGsContentsMode::ContentsRepair:
		return new FGsGameStateRepair();
	case FGsContentsMode::ContentsRecompose:
		return new FGsGameStateRecompose();
	case FGsContentsMode::ContentsKeyBoardMapping:
		return new FGsGameStateKeyBoardMapping();
	case FGsContentsMode::ContentsInvade:
		return new FGsGameStateInvade();
	case FGsContentsMode::ContentsSeal:
		return new FGsGameStateSeal();
	case FGsContentsMode::ContentsBattlePass:
		return new FGsGameStateBattlePass();
	case FGsContentsMode::ContentsSiegeWarfare:
		return new FGsGameStateSiegeWarfare();
	case FGsContentsMode::ContentsExchangeCostume:
		return new FGsGameStateExchangeCostume();
	case FGsContentsMode::ContentsSpiritShot:
		return new FGsGameStateSpiritShot();
	case FGsContentsMode::ContentsAgitDungeon:
		return new FGsGameStateAgitDungeon();
	case FGsContentsMode::ContentsStarLegacy:
		return new FGsGameStateStarLegacy();
	}

	return nullptr;
}



//------------------------------------------------------------------------------
// 게임모드를 관리
//------------------------------------------------------------------------------
void FGsContentsManagerGame::RemoveAll()
{
	for (auto& iter : GetContainer())
	{
		iter.Value->Close();
	}

	auto& protocalGatewayHandler = GNet()->GetProtocalGateway();
	for (TPair<PD::GC::GCPacketId, FDelegateHandle>& elemStage : _listGcpacketDelegate)
	{
		protocalGatewayHandler.Remove(elemStage);
	}
	_listGcpacketDelegate.Empty();

	auto& protocalWorldHandler = GNet()->GetProtocalWorld();
	for (TPair<PD::SC::SCPacketId, FDelegateHandle>& elemStage : _listScPacketDelegate)
	{
		protocalWorldHandler.Remove(elemStage);
	}
	_listScPacketDelegate.Empty();

	auto& contentsHandler = GMessage()->GetContents();
	for (TPair<MessageContents, FDelegateHandle>& elemStage : _listContentsDelegate)
	{
		contentsHandler.Remove(elemStage);
	}
	_listContentsDelegate.Empty();
		
	MUI& msgUI = GMessage()->GetUI();
	for (MsgUIHandle& handle : _listUIDelegate)
	{
		msgUI.Remove(handle);
	}
	_listUIDelegate.Empty();	

	Super::RemoveAll();
}
void FGsContentsManagerGame::InitState()
{
	const FGsContentsMode::InGame allMode[] =
	{
		FGsContentsMode::InGame::ContentsHud,
		FGsContentsMode::InGame::ContentsOption,
		FGsContentsMode::InGame::ContentsQuest,
		FGsContentsMode::InGame::ContentsQuestBoard,
		FGsContentsMode::InGame::ContentsSkill,
		FGsContentsMode::InGame::ContentsContinentalMap,
		FGsContentsMode::InGame::ContentsRegionMap,
		FGsContentsMode::InGame::ContentsFairy,
		FGsContentsMode::InGame::ContentsNpcShop,
		FGsContentsMode::InGame::ContentsGuild,
		FGsContentsMode::InGame::ContentsCostume,
		FGsContentsMode::InGame::ContentsMail,
		FGsContentsMode::InGame::ContentsItemCollection,
		FGsContentsMode::InGame::ContentsDungeon,
		FGsContentsMode::InGame::ContentsNpcDepot,
		FGsContentsMode::InGame::ContentsMarket,
		FGsContentsMode::InGame::ContentsCraft,
		FGsContentsMode::InGame::ContentsMultiLevelRank,
		FGsContentsMode::InGame::ContentsOfflinePlay,
		FGsContentsMode::InGame::ContentsBMShop,
		FGsContentsMode::InGame::ContentsCommunity,
		FGsContentsMode::InGame::ContentsRanking,
		FGsContentsMode::InGame::ContentsAchievement,
		FGsContentsMode::InGame::ContentsMonsterCollection,
		FGsContentsMode::InGame::ContentsTitle,
		FGsContentsMode::InGame::ContentsGameStatistics,
		FGsContentsMode::InGame::ContentsUserProfile,
		FGsContentsMode::InGame::ContentsBoss,
		FGsContentsMode::InGame::ContentsPKBook,
		FGsContentsMode::InGame::ContentsRepair,
		FGsContentsMode::InGame::ContentsRecompose,
		FGsContentsMode::InGame::ContentsKeyBoardMapping,
		FGsContentsMode::InGame::ContentsInvade,
		FGsContentsMode::InGame::ContentsSeal,
		FGsContentsMode::InGame::ContentsBattlePass,
		FGsContentsMode::InGame::ContentsSiegeWarfare,
		FGsContentsMode::InGame::ContentsExchangeCostume,
		FGsContentsMode::InGame::ContentsSpiritShot,
		FGsContentsMode::InGame::ContentsAgitDungeon,
		FGsContentsMode::InGame::ContentsStarLegacy,
	};

	for (auto& e : allMode)
	{
		auto inst = MakeInstance(e);
		inst->Init();
	}

	Super::InitState();

	InitMsgHandle();
}

void FGsContentsManagerGame::Update(float InDeltaTime)
{
	if (TSharedPtr<FGsContentsGameBase> state = GetCurrentState().Pin())
	{
		// HUD State는 항상 Update를 돌린다
		if (FGsContentsMode::InGame::ContentsHud != state->GetType())
		{
			if (TSharedPtr<FGsContentsGameBase> hudState = Find(FGsContentsMode::InGame::ContentsHud))
			{
				hudState->Update(InDeltaTime);
			}
		}

		if (state.IsValid())
		{
			state->Update(InDeltaTime);
		}
	}
}

void FGsContentsManagerGame::OnReconnectionEnd()
{
	if (TSharedPtr<FGsContentsGameBase> state = GetCurrentState().Pin())
	{
		state->OnReconectionEnd();
	}
}

void FGsContentsManagerGame::InitMsgHandle()
{
	auto& protocalWorldHandler = GNet()->GetProtocalWorld();
	for (TPair<PD::SC::SCPacketId, FDelegateHandle>& elemStage : _listScPacketDelegate)
	{
		protocalWorldHandler.Remove(elemStage);
	}
	_listScPacketDelegate.Empty();

	auto& contentsHandler = GMessage()->GetContents();
	for (TPair<MessageContents, FDelegateHandle>& elemStage : _listContentsDelegate)
	{
		contentsHandler.Remove(elemStage);
	}
	_listContentsDelegate.Empty();

	FGsMessageHolder* msg = GMessage();
	
	// hud
	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_HUD,
		this, &FGsContentsManagerGame::OnStateHud));

	// option
	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_OPTION,
		this, &FGsContentsManagerGame::OnStateOption));

	// quest
	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_QUEST,
		this, &FGsContentsManagerGame::OnStateQuest));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_SKILL,
		this, &FGsContentsManagerGame::OnStateSkill));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_CONTINENTALMAP,
		this, &FGsContentsManagerGame::OnStateContinentalMap));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_REGIONMAP,
		this, &FGsContentsManagerGame::OnStateRegionMap));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_FAIRY,
		this, &FGsContentsManagerGame::OnStateFairy));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_NPC_SHOP,
		this, &FGsContentsManagerGame::OnStateNpcShop));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_GUILD,
		this, &FGsContentsManagerGame::OnStateGuild));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_COSTUME,
		this, &FGsContentsManagerGame::OnStateCostume));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_MAIL,
		this, &FGsContentsManagerGame::OnStateMail));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_ITEM_COLLECTION,
		this, &FGsContentsManagerGame::OnStateItemCollection));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_DUNGEON,
		this, &FGsContentsManagerGame::OnStateDungeon));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_NPC_DEPOT,
		this, &FGsContentsManagerGame::OnStateNpcDepot));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_MARKET,
		this, &FGsContentsManagerGame::OnStateMarket));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_CRAFT,
		this, &FGsContentsManagerGame::OnStateCraft));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_MULTI_LEVEL_RANK,
		this, &FGsContentsManagerGame::OnMultiLevelRank));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_OFFLINEPLAY,
		this, &FGsContentsManagerGame::OnOfflinePlay));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_BMSHOP,
		this, &FGsContentsManagerGame::OnStateBMShop));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_COMMUNITY,
		this, &FGsContentsManagerGame::OnCommunity));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_RANKING,
		this, &FGsContentsManagerGame::OnStateRanking));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_ACHIEVEMENT,
		this, &FGsContentsManagerGame::OnStateAchievement));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_MONSTER_COLLECTION,
		this, &FGsContentsManagerGame::OnStateMonsterCollection));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_TITLE,
		this, &FGsContentsManagerGame::OnStateTitle));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_GAME_STATISTICS,
		this, &FGsContentsManagerGame::OnStateGameStatistics));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_USER_PROFILE,
		this, &FGsContentsManagerGame::OnStateUserProfile));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_BOSS,
		this, &FGsContentsManagerGame::OnStateBoss));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_QUEST_BOARD,
		this, &FGsContentsManagerGame::OnStateQuestBoard));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_PK_BOOK,
		this, &FGsContentsManagerGame::OnStatePKBook));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_REPAIR,
		this, &FGsContentsManagerGame::OnStateRepair));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_RECOMPOSE,
		this, &FGsContentsManagerGame::OnStateRecompose));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_KEYBOARD_MAPPING,
		this, &FGsContentsManagerGame::OnStateKeyBoardMapping));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_INVADE,
		this, &FGsContentsManagerGame::OnStateInvade));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_SEAL,
		this, &FGsContentsManagerGame::OnStateSeal));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_BATTLEPASS,
		this, &FGsContentsManagerGame::OnStateBattlePass));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_SIEGE_WARFARE,
		this, &FGsContentsManagerGame::OnStateSiegeWarfare));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_EXCHANGE_CUSTUME,
		this, &FGsContentsManagerGame::OnStateExchangeCostume));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_SPIRIT_SHOT,
		this, &FGsContentsManagerGame::OnStateSpiritShot));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_AGIT_DUNGEON,
		this, &FGsContentsManagerGame::OnStateAgitDungeon));

	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_WINDOW_STAR_LEGACY,
		this, &FGsContentsManagerGame::OnStateStarLegacy));

	// 기본 State(HUD)로 돌리는 메시지. 로비도 사용하는 UI 공용로직에서 부르는 거라 ACTIVE_HUD를 사용하지 않았음
	MUI& msgUI = GMessage()->GetUI();
	_listUIDelegate.Emplace(msgUI.AddRaw(MessageUI::RECOVER_TO_DEFAULT_STATE,
		this, &FGsContentsManagerGame::OnRecoverToDefaultState));
}


void FGsContentsManagerGame::OnStateHud()
{
	ChangeState(FGsContentsMode::InGame::ContentsHud);
}

void FGsContentsManagerGame::OnStateSkill()
{
	ChangeState(FGsContentsMode::InGame::ContentsSkill);
}

void FGsContentsManagerGame::OnStateOption()
{
	ChangeState(FGsContentsMode::InGame::ContentsOption);
}

void FGsContentsManagerGame::OnStateQuest()
{
	ChangeState(FGsContentsMode::InGame::ContentsQuest);
}

void FGsContentsManagerGame::OnStateQuestBoard()
{
	ChangeState(FGsContentsMode::InGame::ContentsQuestBoard);
}

void FGsContentsManagerGame::OnStateContinentalMap()
{
	ChangeState(FGsContentsMode::InGame::ContentsContinentalMap);
}

void FGsContentsManagerGame::OnStateRegionMap()
{
	ChangeState(FGsContentsMode::InGame::ContentsRegionMap);
}

void FGsContentsManagerGame::OnStateFairy()
{
	ChangeState(FGsContentsMode::InGame::ContentsFairy);
}

void FGsContentsManagerGame::OnStateNpcShop()
{
	ChangeState(FGsContentsMode::InGame::ContentsNpcShop);
}

void FGsContentsManagerGame::OnStateGuild()
{
	ChangeState(FGsContentsMode::InGame::ContentsGuild);
}

void FGsContentsManagerGame::OnStateCostume()
{
	ChangeState(FGsContentsMode::InGame::ContentsCostume);
}

void FGsContentsManagerGame::OnStateMail()
{
	ChangeState(FGsContentsMode::InGame::ContentsMail);
}

void FGsContentsManagerGame::OnStateItemCollection()
{
	ChangeState(FGsContentsMode::InGame::ContentsItemCollection);
}

void FGsContentsManagerGame::OnStateDungeon()
{
	ChangeState(FGsContentsMode::InGame::ContentsDungeon);
}

void FGsContentsManagerGame::OnStateNpcDepot()
{
	ChangeState(FGsContentsMode::InGame::ContentsNpcDepot);
}

void FGsContentsManagerGame::OnStateMarket()
{
	ChangeState(FGsContentsMode::InGame::ContentsMarket);
}

void FGsContentsManagerGame::OnStateCraft()
{
	const FGsGameStateCraft* GameStateCraft = GetContentsStateObject<FGsGameStateCraft>(FGsContentsMode::ContentsCraft);
	if (nullptr != GameStateCraft && GameStateCraft->CanEnter(true))
	{
		ChangeState(FGsContentsMode::InGame::ContentsCraft);
	}
}

void FGsContentsManagerGame::OnMultiLevelRank()
{
	ChangeState(FGsContentsMode::InGame::ContentsMultiLevelRank);
}

void FGsContentsManagerGame::OnCommunity()
{
	ChangeState(FGsContentsMode::InGame::ContentsCommunity);
}

void FGsContentsManagerGame::OnRecoverToDefaultState(const IGsMessageParam*)
{
	// 주의: State간 이동 시 창이 닫히고 새 창이 열리는 과정에서, 스택이 비워져 이 로직을 타게되어
	// PrevState Exit -> HUD Enter -> NextState Enter 로 비정상적인 스테이트 변환 상황 발생함(C2URWQ-2391).
	// 따라서 현재 ChangeState 중일 경우 HUD로 변환하지 않도록 리턴처리 함
	if (IsChangingState())
	{
		return;
	}

	ChangeState(FGsContentsMode::InGame::ContentsHud);
}

void FGsContentsManagerGame::OnOfflinePlay()
{
	ChangeState(FGsContentsMode::InGame::ContentsOfflinePlay);
}

void FGsContentsManagerGame::OnStateBMShop()
{
	FGsBMShopManager* shopMgr = GBMShop();
	if (shopMgr->IsUseHiveProductPlatform() && false == shopMgr->IsValidHiveProduct())
	{
		// 재요청 시도
		shopMgr->CheckHiveProductData(true);
		return;
	}
	ChangeState(FGsContentsMode::InGame::ContentsBMShop);
}

void FGsContentsManagerGame::OnStateRanking()
{
	ChangeState(FGsContentsMode::InGame::ContentsRanking);
}

void FGsContentsManagerGame::OnStateAchievement()
{
	ChangeState(FGsContentsMode::InGame::ContentsAchievement);
}

void FGsContentsManagerGame::OnStateMonsterCollection()
{
	ChangeState(FGsContentsMode::InGame::ContentsMonsterCollection);
}

void FGsContentsManagerGame::OnStateTitle()
{
	ChangeState(FGsContentsMode::ContentsTitle);
}

void FGsContentsManagerGame::OnStateGameStatistics()
{
	ChangeState(FGsContentsMode::ContentsGameStatistics);
}

void FGsContentsManagerGame::OnStateUserProfile()
{
	ChangeState(FGsContentsMode::ContentsUserProfile);
}

void FGsContentsManagerGame::OnStateBoss()
{
	ChangeState(FGsContentsMode::ContentsBoss);
}

void FGsContentsManagerGame::OnStatePKBook()
{
	ChangeState(FGsContentsMode::ContentsPKBook);
}

void FGsContentsManagerGame::OnStateRepair()
{
	ChangeState(FGsContentsMode::ContentsRepair);
}

void FGsContentsManagerGame::OnStateRecompose()
{
	ChangeState(FGsContentsMode::ContentsRecompose);
}

void FGsContentsManagerGame::OnStateKeyBoardMapping()
{
	ChangeState(FGsContentsMode::ContentsKeyBoardMapping);
}

void FGsContentsManagerGame::OnStateInvade()
{
	ChangeState(FGsContentsMode::ContentsInvade);
}

void FGsContentsManagerGame::OnStateSeal()
{
	ChangeState(FGsContentsMode::ContentsSeal);
}

void FGsContentsManagerGame::OnStateBattlePass()
{
	ChangeState(FGsContentsMode::ContentsBattlePass);
}

void FGsContentsManagerGame::OnStateSiegeWarfare()
{
	ChangeState(FGsContentsMode::ContentsSiegeWarfare);
}

void FGsContentsManagerGame::OnStateExchangeCostume()
{
	ChangeState(FGsContentsMode::ContentsExchangeCostume);
}

void FGsContentsManagerGame::OnStateSpiritShot()
{
	ChangeState(FGsContentsMode::ContentsSpiritShot);
}

void FGsContentsManagerGame::OnStateAgitDungeon()
{
	ChangeState(FGsContentsMode::ContentsAgitDungeon);
}

FGsContentsMode::InGame FGsContentsManagerGame::GetCurrentContentsMode() const
{
	if(auto currState = GetCurrentState().Pin())
	{
		return currState->GetType();
	}

	return FGsContentsMode::InGame::GameMax;
}
void FGsContentsManagerGame::OnStateStarLegacy()
{
	ChangeState(FGsContentsMode::ContentsStarLegacy);
}