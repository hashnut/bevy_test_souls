#include "GsGameFlowGame.h"

#include "T1Project.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Net/GsNetSendService.h"

#include "Stage/StageGame/GsStageManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "Map/GsSchemaMapData.h"

#include "Message/GsMessageStage.h"
#include "Message/GsMessageGameObject.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "GameObject/Fence/GsFence.h"
#include "GameObject/Fence/GsTownChaosFence.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "ActorEx/GsCharacterBase.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"

#include "Item/GsItemManager.h"
#include "Shared/Client/SharedEnums/SharedUnlockEnum.h"



void FGsGameFlowGame::Enter()
{
	if (nullptr == _stageManager)
	{
		_stageManager = new FGsStageManagerGame();
	}
	_stageManager->InitState();
	
	if (nullptr == _contentsGameManager)
	{
		_contentsGameManager = new FGsContentsManagerGame();
	}
	_contentsGameManager->InitState();

	// DelegateHandle을 Exit시 지우므로, 인게임에서 로비로 재 진입 시 등록이 필요하다. 
	InitMessageHandler();
	SetPlayer();
}

void FGsGameFlowGame::Exit()
{
	FGsMessageHolder* msg = GMessage();

	MStage& stageMsgHandler = msg->GetStage();
	for (MsgStageHandle& elem : _listStageDelegate)
	{
		stageMsgHandler.Remove(elem);
	}
	_listStageDelegate.Empty();

	MGameObject& msgObject = msg->GetGameObject();
	for (MsgGameObjHandle& elem : _litObjectDelegate)
	{
		msgObject.Remove(elem);
	}
	_litObjectDelegate.Empty();

	if (nullptr != _stageManager)
	{
		_stageManager->RemoveAll();
		delete _stageManager;
		_stageManager = nullptr;
	}

	if (nullptr != _contentsGameManager)
	{
		_contentsGameManager->RemoveAll();
		delete _contentsGameManager;
		_contentsGameManager = nullptr;
	}

	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->RemoveWhenFlowChanged();
	}

	if (FGsGuildManager* guildManager = GGuild())
	{
		guildManager->RemoveWhenFlowChanged();
	}

	if (FGsGameDataManager* gameDataManager = GGameData())
	{
		gameDataManager->OnExitGameFlow();
	}

	if (UGsItemManager* itemMgr = GItem())
	{
		itemMgr->ResetData();
	}

	if (FGsBMShopManager* bmShopMgr = GBMShop())
	{
		bmShopMgr->ResetShopData();
	}

	_localPlayer.Reset();
}

void FGsGameFlowGame::Update(float In_deltaTime)
{
	if (_contentsGameManager)
	{
		_contentsGameManager->Update(In_deltaTime);
	}
}

void FGsGameFlowGame::InitMessageHandler()
{
	for (MsgStageHandle& elem : _listStageDelegate)
	{
		_listStageDelegate.Remove(elem);
	}
	 _listStageDelegate.Empty();

	FGsMessageHolder* msg = GMessage();

	MStage& stageMsgHandler = msg->GetStage();

	// 타운으로 갈때		
	_listStageDelegate.Emplace(stageMsgHandler.AddRaw(MessageStage::ENTER_TOWN,
		this, &FGsGameFlowGame::OnEnterTown));

	// 필드로 갈때
	_listStageDelegate.Emplace(stageMsgHandler.AddRaw(MessageStage::ENTER_FIELD,
		this, &FGsGameFlowGame::OnEnterField));

	// 던전으로 갈때
	_listStageDelegate.Emplace(stageMsgHandler.AddRaw(MessageStage::ENTER_DUNGEON,
		this, &FGsGameFlowGame::OnEnterDungeon));

	_listStageDelegate.Emplace(stageMsgHandler.AddRaw(MessageStage::ENTER_WORLD_BOSS,
		this, &FGsGameFlowGame::OnEnterWorldBoss));

	//Enter guild dungeon
	_listStageDelegate.Emplace(stageMsgHandler.AddRaw(MessageStage::ENTER_GUILD_DUNGEON,
		this, &FGsGameFlowGame::OnEnterGuildDungeon));

	// PvP컨텐츠 채널로 갈 때
	_listStageDelegate.Emplace(stageMsgHandler.AddRaw(MessageStage::ENTER_PVP_CONTENT,
		this, &FGsGameFlowGame::OnEnterPvPContent));

	MGameObject& msgObject = msg->GetGameObject();
	_litObjectDelegate.Emplace(msgObject.AddRaw(MessageGameObject::TERRITORY_ENTERED,
		this, &FGsGameFlowGame::OnTerritoryUpdate));
	_litObjectDelegate.Emplace(msgObject.AddRaw(MessageGameObject::TERRITORY_LEFT,
		this, &FGsGameFlowGame::OnTerritoryUpdate));

	_litObjectDelegate.Emplace(msgObject.AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE,
		this, &FGsGameFlowGame::OnLocalPlayerSpawnComplete));
	_litObjectDelegate.Emplace(msgObject.AddRaw(MessageGameObject::LOCAL_REVIVE,
		this, &FGsGameFlowGame::OnLocalPlayerSpawnComplete));
}

void FGsGameFlowGame::OnReconnectionEnd()
{
	if (_stageManager)
	{
		_stageManager->OnReconnectionEnd();
	}

	if (_contentsGameManager)
	{
		_contentsGameManager->OnReconnectionEnd();
	}
}

void FGsGameFlowGame::OnExitGame()
{
	// TEXT: 게임을 나가시겠습니까?
	FText msg;
	FText::FindText(TEXT("SystemText"), TEXT("AskExitGame"), msg);

	FGsUIHelper::PopupSystemYesNo(msg, [](bool bYes)
		{
			if (bYes)
			{
				// 로비로 가기 전 마무리 작업 용도
				GMessage()->GetSystem().SendMessage(MessageSystem::BEFORE_GAME_TO_LOBBY);
				// 로비로 가기 패킷 전송
				FGsNetSendService::SendGatewayReturnToLobby();
			}
		});
}

void FGsGameFlowGame::OnQuitGame()
{
	// 게임 종료 팝업 띄우기
	FGsUIHelper::PopupQuitGame();
}

int32 FGsGameFlowGame::GetContentStateIndex() const
{
	return (nullptr == _contentsGameManager)? -1 : static_cast<int32>(_contentsGameManager->GetCurrentContentsMode());
}

void FGsGameFlowGame::OnEnterTown()
{
	if (nullptr == _stageManager)
		return;

	_stageManager->ChangeState(FGsStageMode::Game::TOWN);
}

void FGsGameFlowGame::OnEnterSafetyZone()
{
	if (nullptr == _stageManager)
		return;

	_stageManager->ChangeState(FGsStageMode::Game::SAFETY_ZONE);
}

void FGsGameFlowGame::OnEnterChaos()
{
	if (nullptr == _stageManager)
		return;

	_stageManager->ChangeState(FGsStageMode::Game::CHAOS);
}

void FGsGameFlowGame::OnEnterField()
{
	if (nullptr == _stageManager)
		return;

	_stageManager->ChangeState(FGsStageMode::Game::FIELD);
}

void FGsGameFlowGame::OnEnterDungeon()
{
	if (nullptr == _stageManager)
		return;

	_stageManager->ChangeState(FGsStageMode::Game::DUNGEON);


	// 기존 FGsEliteDungeonState Enter시는 서버 패킷 받는 시점이라
	// GsGameFlowGame::OnChangeLevel 보다 먼저 일어남
	// FGsStageGameDungeon::Enter 에서 처리하면 시련의 전당 1층에서 2층으로 갈때 호출안됨...
	// 호출 될때마다 처리하기
	// https://jira.com2us.com/jira/browse/C2URWQ-8568
	if (MapType::ELITE_DUNGEON_1 == GLevel()->GetMapType())
	{
		if (FGsMessageHolder* messageMgr = GMessage())
		{
			FGsUIMsgParamBool param(true);
			messageMgr->GetGameObject().SendMessage(MessageGameObject::MINIMAP_ELITE_DUNGEON_ACTIVE, &param);
		}
	}
}

void FGsGameFlowGame::OnEnterBattleArena()
{
	if (nullptr == _stageManager)
		return;

	_stageManager->ChangeState(FGsStageMode::Game::DUNGEON);
}

void FGsGameFlowGame::OnEnterGuildDungeon()
{
	if (nullptr == _stageManager)
		return;

	_stageManager->ChangeState(FGsStageMode::Game::GUILD_DUNGEON);
}

void FGsGameFlowGame::OnEnterArenaReady()
{
	if (nullptr == _stageManager)
		return;

	_stageManager->ChangeState(FGsStageMode::Game::ARENA_READY);
}

void FGsGameFlowGame::OnEnterArena()
{
	if (nullptr == _stageManager)
		return;

	_stageManager->ChangeState(FGsStageMode::Game::ARENA);
}

void FGsGameFlowGame::OnEnterNonPK()
{
	if (nullptr == _stageManager)
		return;

	_stageManager->ChangeState(FGsStageMode::Game::NON_PK);
}

void FGsGameFlowGame::OnEnterPvPContent()
{
	if (nullptr == _stageManager)
		return;

	_stageManager->ChangeState(FGsStageMode::Game::PVP_CONTENT);
}

void FGsGameFlowGame::OnEnterWorldBoss()
{
	if (nullptr == _stageManager)
		return;

	_stageManager->ChangeState(FGsStageMode::Game::WORLD_BOSS);
}

void FGsGameFlowGame::OnEnterAgitDungeon()
{
	if (_stageManager)
	{
		_stageManager->ChangeState(FGsStageMode::Game::AGIT_DUNGEON);
	}
}

void FGsGameFlowGame::OnChangeLevel()
{	
	SetPlayer();
	SetPlayerTerritory();

	GMessage()->GetSound().SendMessage(MessageSound::PLAY_BGM, nullptr);

	// 1. 1번 슬롯 캐릭터가 시련의 전당에서 캐릭터 선택창으로 나감
	// 2. 2번 슬롯 캐릭터(일반필드) 입장
	// 3. 즐겨찾기 버튼이 안나오는 이슈
	// MINIMAP_CONTRIBUTION_BUTTON_ACTIVE( FGsStageGameGuildDungeon, FGsStageGameAgitDungeon : Enter, Exit)
	// MINIMAP_ELITE_DUNGEON_ACTIVE ( FGsEliteDungeonState : Enter, Exit)
	// 일때만 미니맵 옆 사이드 메뉴 갱신이 일어나기 때문임
	// 레벨 변경(모든 스테이지 변경)시 필드 타입으로 초기화후
	// 각 컨텐츠 스테이지 안에서 다시 해당 껄로 변경하기
	// https://jira.com2us.com/jira/browse/C2URWQ-8568

	if (FGsMessageHolder* messageMgr = GMessage())
	{
		FGsUIMsgParamBool param(false);
		messageMgr->GetGameObject().SendMessage(MessageGameObject::MINIMAP_ELITE_DUNGEON_ACTIVE, &param);
	}

	if (const FGsSchemaMapData* mapData = GLevel()->GetCurrentMapInfo())
	{
		switch (mapData->mapType)
		{
			break;
		case MapType::PUBLIC_FIELD:
		case MapType::INTER_FIELD:
			OnEnterField();
			break;
		case MapType::INSTANCE_SINGLE:		
			OnEnterField();
			break;
		case MapType::INSTANCE_FIELD:
			OnEnterField();
			break;
		case MapType::PUBLIC_DUNGEON:
		case MapType::PARTY_DUNGEON:
		case MapType::ELITE_DUNGEON_1:
			OnEnterDungeon();
			break;
		case MapType::GUILD_WAR:
			OnEnterBattleArena();
			break;
		case MapType::GUILD_DUNGEON:
			OnEnterGuildDungeon();
			break;
		case MapType::WORLD_BOSS:
			OnEnterWorldBoss();
			break;
		case MapType::AGIT_DUNGEON_RAID:
			OnEnterAgitDungeon();
			break;
		case MapType::NONE:
		default:
			OnEnterField();
			break;
		}

		// 2023/9/6 PKT - Inter Field추가로 해당 Field로 UnLock Contents 검사를 진행 해야 함. 추후 컨텐츠가 늘어 날 수록 이 부분 방대해질 여지 있음.
		if (FGsUnlockManager* unlockMgr = GSUnlock())
		{
			if (MapType::INTER_FIELD == mapData->mapType)
			{
				unlockMgr->LockContentsByClient(UnlockCategory::INTER_FIELD);
			}
			else
			{
				unlockMgr->UnlockContentsByClient(UnlockCategory::INTER_FIELD);
			}
		}		
	}
}

//펜스에 Priority가 존재하기 때문에 펜스에 들어갔다 나갔다고 해서 들어갔다 나갔다 한 펜스를 기준으로 Stage를 변경하면 안됨
//변경된 순간에 현재 펜스 Priority가 높은 펜스를 기준으로 변경해야함 
//예 : 마을 펜스에 있다가 PK 존 펜스로 들어가는 경우, 마을 펜스가 PK 존보다 높기 때문에 마을 펜스를 벗어나지 않는 이상, PK 존 Stage와 UI로 들어가면 안됨
void FGsGameFlowGame::OnTerritoryUpdate(const IGsMessageParam* InParam)
{
	if (nullptr == _stageManager)
		return;
	
	FGsFenceHandler* fenceHandler = GFence();
	if (nullptr == fenceHandler)
	{
		return;
	}
	
	//아직 펜스가 남아 있다면 곂친 서버 펜스 중 Priority가 가장 높은 펜스의 Stage로 변경
	FGsFence* firstPriorityFence = nullptr;
	if (false == fenceHandler->TryGetPlayerTopPriorityServerFence(firstPriorityFence))
	{		
		OnChangeLevel();
		return;
	}

	TerritoryType serverFenceType = firstPriorityFence->GetTerritoryType();
	SetPlayer();
	SetPlayerTerritory(serverFenceType);

	switch (serverFenceType)
	{
	case  TerritoryType::TOWN:
	{
		if (FGsStageMode::Game::TOWN != _stageManager->GetCurrentStageMode())
		{
			firstPriorityFence->Enter();
			OnEnterTown();
		}
	}
	break;
	case  TerritoryType::SAFE_ZONE:
	{
		if (FGsStageMode::Game::SAFETY_ZONE != _stageManager->GetCurrentStageMode())
		{
			firstPriorityFence->Enter();
			OnEnterSafetyZone();
		}
	}
	break;
	case  TerritoryType::ARENA_READY:
	{
		if (FGsStageMode::Game::ARENA_READY != _stageManager->GetCurrentStageMode())
		{
			firstPriorityFence->Enter();
			OnEnterArenaReady();
		}
	}
	break;
	case  TerritoryType::ARENA:
	{
		if (FGsStageMode::Game::ARENA != _stageManager->GetCurrentStageMode())
		{
			firstPriorityFence->Enter();
			OnEnterArena();
		}
	}
	break;
	case  TerritoryType::NON_PK:
	{
		if (FGsStageMode::Game::NON_PK != _stageManager->GetCurrentStageMode())
		{
			firstPriorityFence->Enter();
			OnEnterNonPK();
		}
	}
	break;
	case TerritoryType::TOWN_CHAOS:
	{
		const FGsTownChaosFence* townChaos = static_cast<const FGsTownChaosFence*>(firstPriorityFence);
		if (townChaos)
		{
			if (townChaos->IsChaos())
			{
				if (FGsStageMode::Game::CHAOS != _stageManager->GetCurrentStageMode())
				{
					firstPriorityFence->Enter();
					OnEnterChaos();
				}
			}
			else
			{
				if (FGsStageMode::Game::TOWN != _stageManager->GetCurrentStageMode())
				{
					firstPriorityFence->Enter();
					OnEnterTown();
				}
			}
		}
	}
	break;
	}
}

void FGsGameFlowGame::SetPlayer()
{
	if (false == _localPlayer.IsValid())
	{		
		if (UGsGameObjectManager* gameObjectManager = GSGameObject())
		{
			_localPlayer = Cast<UGsGameObjectLocalPlayer>(gameObjectManager->FindObject(EGsGameObjectType::LocalPlayer));
		}		
	}
}

void FGsGameFlowGame::SetPlayerTerritory(TerritoryType inTerritory /*= TerritoryType::NONE*/)
{
	if (_localPlayer.IsValid())
	{
		FGsGameObjectDataLocalPlayer* localPlayerData = static_cast<FGsGameObjectDataLocalPlayer*>(_localPlayer->GetData());
		if (localPlayerData)
		{
			localPlayerData->SetTerritoryType(inTerritory);
		}
	}
}

#if RECONNECT_DEBUG
FString FGsGameFlowGame::ToString()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	if (_stageManager->GetCurrentState().IsValid()
		&& _contentsGameManager->GetCurrentState().IsValid())
	{
		return FString::Printf(TEXT("FGsGameFlowGame - FGsStageMode : %d\tFGsContentsMode : %d")
			, (int32)_stageManager->GetCurrentState().Pin()->GetType()
			, (int32)_contentsGameManager->GetCurrentState().Pin()->GetType());
	}	

	return TEXT("FGsGameFlowGame - no state");
}
#endif

void FGsGameFlowGame::OnLocalPlayerSpawnComplete(const IGsMessageParam* InParam)
{
	OnChangeLevel();
}

void FGsGameFlowGame::OnBackToTitle()
{
	// TEXT: 게임을 나가시겠습니까?
	FText msg;
	FText::FindText(TEXT("SystemText"), TEXT("AskQuitGame"), msg);

	FGsUIHelper::PopupSystemYesNo(msg, [](bool bYes)
	{
		if (bYes)
		{
			TSharedPtr<FGsNetBase> active = GNet()->GetActive();
			if (active.IsValid())
			{
				active->Disconnect();
			}

			GMessage()->GetSystem().SendMessage(MessageSystem::BACKTO_LOGIN);			
		}
	});
}

void FGsGameFlowGame::OnRestartGame()
{
	FGsUIHelper::ForcedHideBlockUI(EGsHideBlockUIFactor::RESTART_GAME);
	FGsUIHelper::TrayDimmed(false);
	GNet()->OnExitGame();
	GMessage()->GetSystem().SendMessage(MessageSystem::BACKTO_LOGIN);
}