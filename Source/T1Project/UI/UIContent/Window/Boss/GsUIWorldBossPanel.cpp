#include "GsUIWorldBossPanel.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Management/GsMessageHolder.h"
#include "UMG/Public/Components/ListViewBase.h"
#include "UMG/Public/Components/Widget.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "GsUIWindowBossInfo.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "GameObject/Boss/GsFieldBossHandler.h"
#include "UI/UIContent/Popup/Boss/GsBossInfoListViewEntry.h"
#include "Message/MessageParam/GsBossMessageParam.h"
#include "UMG/Public/Components/TextBlock.h"
#include "GsUIWorldBossDetailInfoPanel.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Popup/Boss/GsUIPopupBossContribution.h"
#include "Net/GsNetSendServiceBoss.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UI/UIContent/Popup/Boss/GsWorldBossInfoListViewEntry.h"
#include "DataSchema/WorldBoss/GsSchemaWorldBossData.h"
#include "Engine/Texture2D.h"
#include "Engine/AssetManager.h"
#include "UTIL/GsTimeUtil.h"
#include "GameObject/Boss/GsWorldBossHandler.h"
#include "GameObject/Boss/GsBossHandler.h"
#include "GameFlow/Stage/StageGame/GsStageGameWorldBoss.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/Stage/StageGame/GsStageManagerGame.h"
#include "GameFlow/Stage/GsStageMode.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UTIL/GsLevelUtil.h"

const FTextKey WOLRD_BOSS_TEXT_KEY = FTextKey(TEXT("WorldBossText"));
const FTextKey WORLD_BOSS_NOTIC_ALREADY_ENTER = FTextKey(TEXT("Notice_AlreadyEnter"));

void UGsUIWorldBossPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//world boss button
	_btnWorldBossEnter->OnClicked.AddDynamic(this, &UGsUIWorldBossPanel::OnClickEnterWorldBoss);	
}

void UGsUIWorldBossPanel::NativeConstruct()
{
	Super::NativeConstruct();

	_isCanEnter = false;
	CheckShowTimerType();

	//bind message
	if (FGsMessageHolder* msg = GMessage())
	{
		_bossDelegates.Emplace(msg->GetBoss().AddUObject(MessageBoss::OPEN_BOSS_CONTRIBUTION_POPUP
			, this, &UGsUIWorldBossPanel::OnOpenBossKillContribution));

		//보스 윈도우 갱신 콜백
		_uiDelegates.Emplace(msg->GetUI().AddUObject(MessageUI::INVALID_BOSS
			, this, &UGsUIWorldBossPanel::OnSelectBoss));
	}
		
	_bossEntryDataArray.Empty();
	
	_bossImage->SetVisibility(ESlateVisibility::Hidden);
	_bossListView->SetScrollbarVisibility(ESlateVisibility::Hidden);

	FText::FindText(FIELD_BOSS_TEXT_KEY, UI_TEXT_NPERCENT, _ui_text_nPercent);
	FText::FindText(INVADE_TEXT_KEY, UI_TEXT_CAN_USE_FUNCTION, _ui_text_canUseFunction);
	FText::FindText(INVADE_TEXT_KEY, UI_TEXT_CAN_TELEPORT, _ui_text_canTeleport);	
	FText::FindText(WOLRD_BOSS_TEXT_KEY, WORLD_BOSS_NOTIC_ALREADY_ENTER, _ui_text_noticAlreadyEnter);
	FText::FindText(WOLRD_BOSS_TEXT_KEY, TEXT("Notice_CanNotEnter_InGuildDungeon"), _ui_text_notice_canNotEnter_inGuildDungeon);
	FText::FindText(WOLRD_BOSS_TEXT_KEY, TEXT("Notice_CanNotEnter_InvadeAttack"), _ui_text_notice_canNotEnter_invadeAttack);	
	
	Clear();
	InvalidBossList();
}

void UGsUIWorldBossPanel::NativeDestruct()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_bossDelegates.Num() > 0)
		{
			for (auto iter : _bossDelegates)
			{
				msg->GetBoss().Remove(iter);
			}
			_bossDelegates.Empty();
		}

		if (_uiDelegates.Num() > 0)
		{
			for (auto iter : _uiDelegates)
			{
				msg->GetUI().Remove(iter);
			}
			_uiDelegates.Empty();
		}
	}

	_enterTimer.Stop(GetWorld());
	_enterTimer.Clear();

	_bossEntryDataArray.Empty();
	Clear();

	Super::NativeDestruct();
}

void UGsUIWorldBossPanel::InvalidBossList()
{
	_bossEntryDataArray = CreateBossArray();
	if (0 == _bossEntryDataArray.Num())
	{
		return;
	}

	UGsWorldBossInfoListViewEntryData** bossInfo = _bossEntryDataArray.FindByPredicate([&](UGsWorldBossInfoListViewEntryData* data)->bool {
		if (_selectWorldBossData)
		{
			return data->_bossData->id == _selectWorldBossData->id;
		}

		return false;
		});

	_bossListView->SetListItems(_bossEntryDataArray);

	if (bossInfo)
	{
		_bossListView->SetSelectedItem(*bossInfo);
	}
}

void UGsUIWorldBossPanel::Hide()
{
	_enterTimer.Stop(GetWorld());
	_enterTimer.Clear();
}

void UGsUIWorldBossPanel::OnSelectBoss(const struct IGsMessageParam* inParam)
{
	const FGsUIMsgParamSelectBoss* param = inParam->Cast<const FGsUIMsgParamSelectBoss>();
	if (nullptr == param)
	{
		return;
	}

	if (EGsBossTab::WorldBoss != param->_bossType)
	{
		return;
	}

	if (false == IsNeedToInvalid(param))
	{
		StartRemainTime();
		return;
	}

	Clear();	

	_selectWorldBossData = param->_worldBossData;
	_bossSpawnTime = param->_bossSpawnTime;
	_bossRoomOpenTime = param->_bossRoomOpenTime;
	_leftRewardCount = param->_leftRewardCount;

	SelectWorldBoss(_selectWorldBossData, _leftRewardCount);
	StartRemainTime();
}

bool UGsUIWorldBossPanel::IsNeedToInvalid(const FGsUIMsgParamSelectBoss* inParam)
{
	EGsBossTab bossTab = inParam->_bossType;

	switch (inParam->_bossType)
	{
	case EGsBossTab::WorldBoss:
	{
		if (inParam->_worldBossData != _selectWorldBossData
			|| inParam->_bossRoomOpenTime != _bossRoomOpenTime
			|| inParam->_bossSpawnTime != _bossSpawnTime)
		{
			return true;
		}
	}
		break;
	}

	return false;
}

TArray<UGsWorldBossInfoListViewEntryData*> UGsUIWorldBossPanel::CreateBossArray()
{
	TArray<UGsWorldBossInfoListViewEntryData*> bossInfoArray;
	const UGsTable* table = FGsSchemaWorldBossData::GetStaticTable();
	const UGsTableWorldBossData* bossTable = Cast<UGsTableWorldBossData>(table);
	if (nullptr == bossTable)
	{
		return bossInfoArray;
	}

	FGsGameDataManager* gameData = GGameData();
	if (nullptr == gameData)
	{
		return bossInfoArray;
	}

	const TMap<FName, uint8*>* bossMap;
	bossTable->GetRowMap(bossMap);
	int32 bossId;
	UGsWorldBossInfoListViewEntryData* newBossInfoParma = nullptr;
	const FGsSchemaWorldBossData* bossInfo = nullptr;
	TArray<const FGsSchemaNpcData*> bossNpcDataArray;
	_bossDataArray.Empty();
	for (const TPair<FName, uint8*>& pair : (*bossMap))
	{
		bossId = FCString::Atoi(*pair.Key.ToString());
		bossInfo = reinterpret_cast<const FGsSchemaWorldBossData*>(pair.Value);
		if (nullptr == bossInfo
			|| nullptr == bossInfo->mapId.GetRow())
		{
			continue;
		}

		newBossInfoParma = NewObject<UGsWorldBossInfoListViewEntryData>();
		newBossInfoParma->_bossData = bossInfo;		
		newBossInfoParma->_level = UGsLevelUtil::GetSpawnLevel(bossInfo->bossNpcId.GetRow(), bossInfo->mapId.GetRow()->id);

		_bossDataArray.Add(bossInfo);
		bossInfoArray.Add(newBossInfoParma);
	}

	bossInfoArray.Sort([&](const UGsWorldBossInfoListViewEntryData& source, const UGsWorldBossInfoListViewEntryData& dest)->bool {
		int32 sourceBossId = source._bossData->id;
		int32 destBossId = dest._bossData->id;
		return sourceBossId < destBossId;
		});

	return bossInfoArray;
}

void UGsUIWorldBossPanel::OnClickEnterWorldBoss()
{	
	if (GLevel()->GetCurrentMapInfo()->mapType == MapType::GUILD_DUNGEON)
	{		
		FGsUIHelper::TrayMessageTicker(_ui_text_notice_canNotEnter_inGuildDungeon);
		return;
	}

	if (GGameData()->IsInvadeWorld())
	{
		FGsUIHelper::TrayMessageTicker(_ui_text_notice_canNotEnter_invadeAttack);
		return;
	}

	const FGsSchemaUnlockContents* unlockData = _selectWorldBossData->unlockContentsId.GetRow();
	if (unlockData
		&& false == GSUnlock()->IsContentsUnlock(unlockData->id))
	{
		FText message;
		GSUnlock()->MakeStringUnLockCondition(_selectWorldBossData->unlockContentsId.GetRow()->id
			, _selectWorldBossData->unlockContentsId.GetRow()->lockType, message);

		FGsUIHelper::TrayMessageTicker(message);
		return;
	}

	if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
	{
		if (FGsStageManagerGame* stageMgr = gameFlow->GetStageManagerGame())
		{
			if (stageMgr->GetCurrentStageMode() == FGsStageMode::Game::WORLD_BOSS)
			{
				if(auto worldBossStage = StaticCastSharedPtr<FGsStageGameWorldBoss>(stageMgr->GetCurrentState().Pin()))
				{
					if (_selectWorldBossData->id == worldBossStage->GetWorldBossId())
					{
						FGsUIHelper::TrayMessageTicker(_ui_text_noticAlreadyEnter);
						return;
					}
				}				
			}
		}
	}

	FGsBossHandler::GetInstance()->GetWorldBossHandler()->WarpToWorldBoss(_selectWorldBossData->id);
}

void UGsUIWorldBossPanel::OnOpenBossKillContribution(const IGsMessageParam* inParam)
{
	const FGsResultDefeatFieldBossMsgParam* param = static_cast<const FGsResultDefeatFieldBossMsgParam*>(inParam);

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(POPUP_BOSS_CONTRIBUTION);
	if (false == widget.IsValid())
	{		
		return;
	}

	UGsUIPopupBossContribution* contribution = Cast<UGsUIPopupBossContribution>(widget.Get());
	if (nullptr == contribution)
	{
		return;
	}
	
	const struct FGsSchemaFieldBossData* bossData = FGsFieldBossHandler::FindFieldBossData(param->_bossId);
	contribution->InitContribution(bossData, param->_resultDepeatBoss);
}

void UGsUIWorldBossPanel::InvalidateBossInfo()
{
	InvalidNickName();		
	InvalidBossImage();	
	InvalidDetailPanel();
}

void UGsUIWorldBossPanel::SelectWorldBoss(const struct FGsSchemaWorldBossData* inBossInfo, int32 inLeftRewardCount)
{
	_selectWorldBossData = inBossInfo;
	_leftRewardCount = inLeftRewardCount;

	//select npc
	UGsWorldBossInfoListViewEntryData** findEntryData = _bossEntryDataArray.FindByPredicate([&](const UGsWorldBossInfoListViewEntryData* entryData)->bool {
		return entryData->_bossData == inBossInfo;
		});
	if (nullptr == findEntryData)
	{
		return;
	}

	UGsWorldBossInfoListViewEntryData* entryData = (*findEntryData);

	_bossListView->SetItemSelection(entryData, true);
	int32 index = _bossListView->GetIndexForItem(entryData);
	_bossListView->NavigateToIndex(index);

	InvalidateBossInfo();
}

void UGsUIWorldBossPanel::InvalidNickName()
{
	_nickNameTextBlock->SetText(_selectWorldBossData->nickName);
	_nameTextBlock->SetText(_selectWorldBossData->bossNpcId.GetRow()->nameText);
}


void UGsUIWorldBossPanel::InvalidBossImage()
{
	if (false == _selectWorldBossData->bossImg.IsValid())
	{
		return;
	}

	UTexture2D* image = Cast<UTexture2D>(UAssetManager::GetStreamableManager().LoadSynchronous(_selectWorldBossData->bossImg));	
	if (image)
	{
		_bossImage->SetBrushFromTexture(image);
		_bossImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UGsUIWorldBossPanel::StartRemainTime()
{
	if (nullptr == _selectWorldBossData)
	{
		return;
	}

	//다음 몬스터 출현 시간 얻어온다. 	
		
#if WORLD_BOSS_DEBUG
	GSLOG(Log, TEXT("Show next boss room open remain time - boss id : %lld\tmap open time : %s\tspawn time : %s")
		, (int64)_selectWorldBossData->id
		, *_bossRoomOpenTime.ToString()
		, *_bossSpawnTime.ToString());
#endif

	_timePanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);	

	_isCanEnter = _bossRoomOpenTime > _bossSpawnTime;
	CheckShowTimerType();
	if (false == _isCanEnter)
	{	
#if WORLD_BOSS_DEBUG
		GSLOG(Log, TEXT("Show next boss room open remain time - boss id : %lld\tmap open time : %s")
			, (int64)_selectWorldBossData->id
			, *_bossRoomOpenTime.ToString());
#endif
		FDateTime time = _bossRoomOpenTime + FTimespan(0, 0, 0, 1);
		_enterTimer.Start(GetWorld(), time, 0.5f
			, [this]() {
				FTimespan remainTime = _bossRoomOpenTime - FGsTimeSyncUtil::GetClientNowDateTime();								
				if (0 == remainTime.GetTotalSeconds())
				{
					_timePanel->SetVisibility(ESlateVisibility::Hidden);
				}
				FString remainTimeText = FGsDateTimer::GetRemainTimeText(remainTime);
				_remainTimeTextBlock->SetText(FText::FromString(remainTimeText));
			}
			, [this]() {				
				FGsNetSendServiceBoss::SendReqWorldBossInfo(_selectWorldBossData->id);
			});
	}
	else
	{		
#if WORLD_BOSS_DEBUG
		GSLOG(Log, TEXT("Show boss spawn remain time - boss id : %lld\tboss spawn time : % s")
			, (int64)_selectWorldBossData->id
			, *_bossSpawnTime.ToString());
#endif

		FDateTime time = _bossSpawnTime + FTimespan(0, 0, 0, 1);
		_enterTimer.Start(GetWorld(), time, 0.5f
			, [this]() {
				FTimespan remainTime = _bossSpawnTime - FGsTimeSyncUtil::GetClientNowDateTime();
				if (0 == remainTime.GetTotalSeconds())
				{
					_timePanel->SetVisibility(ESlateVisibility::Hidden);
				}
				FString remainTimeText = FGsDateTimer::GetRemainTimeText(remainTime);
				_remainTimeTextBlock->SetText(FText::FromString(remainTimeText));
			}
			, [this]() {				
				FGsNetSendServiceBoss::SendReqWorldBossInfo(_selectWorldBossData->id);
			});
	}

	InvalidRemainTime();
}


void UGsUIWorldBossPanel::CheckShowTimerType()
{
	if (false == _isCanEnter)
	{
		_timeSwitcher->SetActiveWidgetIndex(1);
		_btnWorldBossEnter->SetIsEnabled(false);		
	}
	else
	{
		_timeSwitcher->SetActiveWidgetIndex(0);
		_btnWorldBossEnter->SetIsEnabled(true);
	}
}

void UGsUIWorldBossPanel::InvalidRemainTime()
{
	FString remainTime = _enterTimer.GetRemainTimeText();
	_remainTimeTextBlock->SetText(FText::FromString(remainTime));
}

void UGsUIWorldBossPanel::InvalidDetailPanel()
{
	_worldBossDetailPanel->SetWorldBossInfo(_selectWorldBossData, _leftRewardCount);
}

void UGsUIWorldBossPanel::Clear()
{
	_selectWorldBossData = nullptr;

	_nickNameTextBlock->SetText(FText::GetEmpty());
	_nameTextBlock->SetText(FText::GetEmpty());
	_bossImage->SetBrushFromTexture(nullptr);

	_worldBossDetailPanel->SetWorldBossInfo(_selectWorldBossData, _leftRewardCount);
}