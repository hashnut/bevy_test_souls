#include "GsGameStateBoss.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Boss/GsFieldBossHandler.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "Message/MessageParam/GsBossMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "UI/UILib/Define/GsUIDefine.h"
#include "UI/UIContent/Window/Boss/GsUIWindowBossInfo.h"
#include "UI/UIContent/Popup/Boss/GsUIPopupBossContribution.h"

#include "EventProgress/GsEventProgressDefine.h"

#include "Data/GsDataContainManager.h"

#include "ActorEx/GsActorBossScene.h"
#include "ActorEx/GsCharacterNonPlayer.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"

#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Net/GsNetSendServiceBoss.h"
#include "Shared/Client/SharedEnums/SharedBossEnum.h"
#include "GameObject/Boss/GsWorldBossHandler.h"
#include "DataSchema/WorldBoss/GsSchemaWorldBossData.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "../Message/GsMessageContents.h"
#include "../Management/GsScopeHolder.h"


FGsGameStateBoss::FGsGameStateBoss() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsBoss)
{
	
}

FGsGameStateBoss::~FGsGameStateBoss()
{

}

void FGsGameStateBoss::Enter()
{
	FGsContentsGameBase::Enter();

	RegisterMessages();
	OpenBossUI();
	SetNavigationBar(CurrencyType::NONE);
}

void FGsGameStateBoss::Exit()
{
	UnregisterMessages();

	CloseBossUI();

	if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
	{
		eventProgressManager->OnReqFinishEvent(EGsEventProgressType::BOSS_ROOM);
	}

	FGsContentsGameBase::Exit();
}


void FGsGameStateBoss::RegisterMessages()
{
	//bind message
	if (FGsMessageHolder* msg = GMessage())
	{

	}
}

void FGsGameStateBoss::UnregisterMessages()
{
	MBoss& bossManager = GMessage()->GetBoss();
	for (TPair<MessageBoss, FDelegateHandle>& handler : _bossDelegates)
	{
		bossManager.Remove(handler);
	}

	MUI& uiManager = GMessage()->GetUI();
	for (TPair<MessageUI, FDelegateHandle>& handler : _uiDelegates)
	{
		uiManager.Remove(handler);
	}
}

void FGsGameStateBoss::OpenBossUI()
{
	// UI 출력
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->SetHideFlags(EGsUIHideFlags::STATE_SCENE_WINDOW);
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(WINDOW_BOSS_INFO);
	}
}

void FGsGameStateBoss::CloseBossUI()
{
	_popupBossInfoUI.Reset();	

	_popupBossContribution.Reset();
	GUI()->CloseByKeyName(POPUP_BOSS_CONTRIBUTION);

	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->ClearHideFlags(EGsUIHideFlags::STATE_SCENE_WINDOW);
	}
}

void FGsGameStateBoss::ClearSelect()
{
	SetSelectedBossData();
	SetSelectedBossData();
	SetShowMutant();
}

void FGsGameStateBoss::SelectFirstFieldBoss(bool inShowMutant)
{
	ClearSelectBossData();
	_selectBossType = EGsBossTab::FieldBoss;
	_isShowMutant = inShowMutant;		

	TArray<const struct FGsSchemaFieldBossData*> fieldBossArray;
	TryGetFieldBossArray(fieldBossArray);
	const FGsSchemaFieldBossData** find = fieldBossArray.FindByPredicate([&](const FGsSchemaFieldBossData* fieldBossDataRow)->bool {
		return fieldBossDataRow->bossType == BossType::COMPETITION;
		});

	if (find)
	{
		const FGsSchemaFieldBossData* bossData = FGsFieldBossHandler::FindFieldBossData((*find)->bossId);
		if (bossData)
		{
			SetSelectedBossData(bossData);
		}

		const FGsSchemaFieldBossData* mutantData = FGsFieldBossHandler::FindFieldBossData((*find)->mutantApear.fieldbossDataId);
		if (bossData)
		{
			SetSelectedMutantData(mutantData);
		}		
	}

	SaveSelectBoss();

	if (_selectedBossData)
	{				
		FGsUIMsgParamSelectBoss param(_selectedBossData, _selectedMutantData, _isShowMutant);
		GMessage()->GetUI().SendMessage(MessageUI::INVALID_BOSS, &param);

		FGsNetSendServiceBoss::SendReqCompetitionFieldBossLastDead(_selectedBossData->bossId);
	}
}

void FGsGameStateBoss::SelectedFieldBoss(FieldBossId inBossId, bool inShowMutant)
{
	//clear boss id
	ClearSelectBossData();
	_selectBossType = EGsBossTab::FieldBoss;
	bool isShowMutant = inShowMutant;

	const FGsSchemaFieldBossData* bossData = FGsFieldBossHandler::FindFieldBossData(inBossId);
	if (nullptr == bossData)
	{
		GSLOG(Error, TEXT("boss id - missing : %lld"), (int64)inBossId);
		return;
	}

	if (bossData)
	{
		//변종 몬스터인가?
		if (bossData->bossType == BossType::COMPETITION_MUTANT
			|| bossData->bossType == BossType::COOPERATION_MUTANT)
		{
			//변종 몬스터라면 실제 보여줘야하는 보스 정보를 설정 
			bossData = FGsFieldBossHandler::FindFieldBossDataByMutantBossId(inBossId);
			isShowMutant = true;
		}

		SetSelectedBossData(bossData);
	}

	SetShowMutant(isShowMutant);

	const FGsSchemaFieldBossData* mutantData = FGsFieldBossHandler::FindFieldBossData(bossData->mutantApear.fieldbossDataId);
	SetSelectedMutantData(mutantData);	

	SaveSelectBoss();
	
	if(_selectedBossData)
	{		
		FGsUIMsgParamSelectBoss param(_selectedBossData, _selectedMutantData, _isShowMutant);
		GMessage()->GetUI().SendMessage(MessageUI::INVALID_BOSS, &param);

		FGsNetSendServiceBoss::SendReqCompetitionFieldBossLastDead(_selectedBossData->bossId);
	}
}

void FGsGameStateBoss::SelectedBossByMap(int32 inLevelId, bool inShowMutant)
{
	TArray<const FGsSchemaFieldBossData*> bossDataArray = FGsFieldBossHandler::GetFieldBossDataByLevelId(inLevelId);
	const FGsSchemaFieldBossData** findBoss = bossDataArray.FindByPredicate([&](const FGsSchemaFieldBossData* bossData)->bool {
		return bossData->bossType == BossType::COOPERATION;
		});

	if (nullptr == findBoss
	|| nullptr == (*findBoss))
	{
		GSLOG(Warning, TEXT("No boss in level - level id : %d"), inLevelId);		
		return;
	}

	SelectedFieldBoss((*findBoss)->bossId, inShowMutant);
}

void FGsGameStateBoss::InvalidateBoss()
{
	FGsUIMsgParamSelectBoss param(_selectedBossData, _selectedMutantData, _isShowMutant);
	GMessage()->GetUI().SendMessage(MessageUI::INVALID_BOSS, &param);
}

void FGsGameStateBoss::InvalidateDeathTime()
{
	FGsUIMsgParamFieldBossDeathTime param(_lastDeadTime);
	GMessage()->GetUI().SendMessage(MessageUI::INVALID_FEILD_BOSS_DEATH_TIME, &param);
}

void FGsGameStateBoss::SaveSelectBoss()
{
	_savedBossType = _selectBossType;
	_savedBossData = _selectedBossData;
	_savedMutantData = _selectedMutantData;
	_savedIsShowMutant = _isShowMutant;

	_saveWorldBossData = _selectWorldBossData;
}

void FGsGameStateBoss::RecoverySelectBoss()
{
	_selectBossType = _savedBossType;
	_selectedBossData = _savedBossData;
	_selectedMutantData = _savedMutantData;
	_isShowMutant = _savedIsShowMutant;

	FGsUIMsgParamSelectBoss param(_selectedBossData, _selectedMutantData, _isShowMutant);
	GMessage()->GetUI().SendMessage(MessageUI::INVALID_BOSS, &param);
}

void FGsGameStateBoss::ClearSelectBossData()
{
	_selectedMutantData = nullptr;
	_selectedMutantData = nullptr;
	_savedBossData = nullptr;
	_savedMutantData = nullptr;
}

void FGsGameStateBoss::SelectFirstWorldBoss()
{
	ClearSelectBossData();	
	_selectBossType = EGsBossTab::WorldBoss;
	
	TArray<const FGsSchemaWorldBossData*> worldBossArray = GetWorldBossArray();
	if (worldBossArray.Num() > 0)
	{
		if (worldBossArray[0])
		{
			SetSelectedWorldBossData(worldBossArray[0]);
		}
	}

	SaveSelectBoss();

	if (_selectedBossData)
	{
		FGsNetSendServiceBoss::SendReqWorldBossInfo(_selectWorldBossData->id);			
	}
}

void FGsGameStateBoss::SelectedWorldBoss(WorldBossId inWorldBossId)
{
	//clear boss id
	ClearSelectBossData();

	_selectBossType = EGsBossTab::WorldBoss;
	
	const FGsSchemaWorldBossData* bossData = FGsWorldBossHandler::FindWorldBossData(inWorldBossId);
	if (nullptr == bossData)
	{
		GSLOG(Error, TEXT("boss id - missing : %lld"), (int64)inWorldBossId);
		return;
	}

	if (bossData)
	{
		SetSelectedWorldBossData(bossData);
	}

	SaveSelectBoss();

	if (_selectWorldBossData)
	{
		FGsNetSendServiceBoss::SendReqWorldBossInfo(inWorldBossId);
	}
}

void FGsGameStateBoss::OpenSelectedWorldBoss(const MapId InMapId, const CreatureId InCreatureId)
{
	FGsUnlockManager* unlockManager = GSUnlock();
	if (nullptr == unlockManager)
		return;

	// 던전 컨텐츠 잠금 확인 및 잠겼을 경우 티커 출력
	if (false == unlockManager->IsContentsUnlock(EGsUnlockContentsType::BOSS, true))
		return;

	WorldBossId CurrentWorldBossId = -1;	

	const UGsTableWorldBossData* WorldBossDataTable = Cast<UGsTableWorldBossData>(FGsSchemaWorldBossData::GetStaticTable());
	TArray<const FGsSchemaWorldBossData*> WorldBossDatas;
	WorldBossDataTable->GetAllRows(WorldBossDatas);
	for (const FGsSchemaWorldBossData* WorldBossData : WorldBossDatas)
	{
		if (nullptr == WorldBossData)
		{
			GSLOG(Error, TEXT("nullptr == WorldBossData"));
			continue;
		}

		const FGsSchemaMapData* MapData = WorldBossData->mapId.GetRow();
		const FGsSchemaNpcData* NpcData = WorldBossData->bossNpcId.GetRow();
		if (nullptr == MapData || nullptr == NpcData)
		{
			continue;
		}

		if (MapData->id == InMapId && NpcData->id == InCreatureId)
		{
			CurrentWorldBossId = StaticCast<WorldBossId>(WorldBossData->id);
		}
	}

	if (0 <= CurrentWorldBossId)
	{
		const UGsUIManager* UIManager = GUI();
		FGsMessageHolder* MessageHolder = GMessage();
		if (nullptr != UIManager && nullptr != MessageHolder)
		{
			MessageHolder->GetContents().SendMessage(MessageContents::ACTIVE_BOSS);
			if (UIManager->IsActiveWidget(WINDOW_BOSS_INFO))
			{
				FGsGameStateBoss* GameStateBoss = FGsGameStateBoss::GetInstance();
				if (nullptr != GameStateBoss)
				{
					GameStateBoss->SelectedWorldBoss(CurrentWorldBossId);
				}
			}
		}
	}
	else
	{
		GSLOG(Error, TEXT("Invalid CurrentWorldBossId, InMapId:%d, InCreatureId:%d"), InMapId, InCreatureId);
	}
}

bool FGsGameStateBoss::TryGetFieldBossArray(TArray<const struct FGsSchemaFieldBossData*>& outFieldBossArray)
{
	outFieldBossArray.Empty();
	const UGsTable* fieldBossTable = FGsSchemaFieldBossData::GetStaticTable();
	if (fieldBossTable)
	{
		const UGsTableFieldBossData* castFieldBossTable = Cast<const UGsTableFieldBossData>(fieldBossTable);
		if (castFieldBossTable)
		{
			castFieldBossTable->GetAllRows(outFieldBossArray);
		}
	}

	FGsGameDataManager* gameData = GGameData();
	outFieldBossArray.Sort([&](const FGsSchemaFieldBossData& source, const FGsSchemaFieldBossData& dest)->bool {
		int32 sourceBossId = source.bossId;
		int32 destBossId = dest.bossId;
		int32 sourceFixedLevel = source.bossNpcId.GetRow()->fixedLevel;
		int32 destFixedLevel = dest.bossNpcId.GetRow()->fixedLevel;

		if (gameData->IsBossBookmark(sourceBossId)
			&& gameData->IsBossBookmark(destBossId))
		{
			// 즐겨찾기 된거 끼리도 레벨 오름차순 정렬 요청
			// https://jira.com2us.com/jira/browse/C2URWQ-6921
			return sourceFixedLevel < destFixedLevel;
		}
		else if (false == gameData->IsBossBookmark(sourceBossId)
			&& gameData->IsBossBookmark(destBossId))
		{
			return false;
		}
		else if (gameData->IsBossBookmark(sourceBossId)
			&& false == gameData->IsBossBookmark(destBossId))
		{
			return true;
		}
		
		return sourceFixedLevel < destFixedLevel;
		});

	return outFieldBossArray.Num() > 0;
}

TArray<const FGsSchemaWorldBossData*> FGsGameStateBoss::GetWorldBossArray()
{
	TArray<const FGsSchemaWorldBossData*> worldBossDataArray;
	const UGsTable* worldBossTable = FGsSchemaWorldBossData::GetStaticTable();
	if (worldBossTable)
	{
		const UGsTableWorldBossData* castWorldBossTable = Cast<const UGsTableWorldBossData>(worldBossTable);
		if (castWorldBossTable)
		{
			castWorldBossTable->GetAllRows(worldBossDataArray);
		}
	}

	return worldBossDataArray;
}

FGsGameStateBoss* FGsGameStateBoss::GetInstance()
{
	if (FGsGameFlowManager* mode = GMode())
	{
		if (FGsGameFlowGame* gameFlow = mode->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				return contents->GetContentsStateObject<FGsGameStateBoss>(FGsContentsMode::InGame::ContentsBoss);
			}
		}
	}

	return nullptr;
}
