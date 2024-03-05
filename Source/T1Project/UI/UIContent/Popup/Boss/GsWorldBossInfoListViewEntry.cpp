// Fill out your copyright notice in the Description page of Project Settings.


#include "GsWorldBossInfoListViewEntry.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/Button.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateBoss.h"
#include "GsTable.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsDungeonManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsBossMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "UI/UIContent/Window/Boss/GsUIWindowBossInfo.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Net/GsNetSendServiceBoss.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "DataSchema/Dungeon/GsSchemaDungeonData.h"
#include "Dungeon/GsDungeonGroupBase.h"
#include "Dungeon/GsDungeonData.h"
#include "Dungeon/GsDungeonHelper.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "GameObject/Boss/GsBossHandler.h"
#include "UMG/Public/Components/RichTextBlock.h"
#include "Engine/AssetManager.h"
#include "Message/GsMessageContents.h"
#include "DataSchema/Unlock/GsSchemaUnlockContents.h"

void UGsWorldBossInfoListViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_button->OnClicked.AddDynamic(this, &UGsWorldBossInfoListViewEntry::OnClickListViewEntry);	
}

void UGsWorldBossInfoListViewEntry::NativeConstruct()
{
	Super::NativeConstruct();

	FText::FindText(FIELD_BOSS_TEXT_KEY, BOSS_SPAWN_AREA_TEXT_KEY, _levelLimitText);
	FText::FindText(TEXT("WorldMapText"), TEXT("MonLevel"), _levelFormat);

	_activeEffect->SetVisibility(ESlateVisibility::Collapsed);

	if (FGsMessageHolder* msg = GMessage())
	{
		_uiDelegateArray.Emplace(msg->GetUI().AddUObject(MessageUI::INVALID_BOSS
		, this, &UGsWorldBossInfoListViewEntry::OnSelectBossInfo));
		_msgUnlockHandleList.Emplace(msg->GetUnlock().AddUObject(MessageContentUnlock::INVALIDATE_ALL_UNLOCK
			, this, &UGsWorldBossInfoListViewEntry::OnInvalidUnlock));
		_bossDelegateArray.Emplace(msg->GetBoss().AddUObject(MessageBoss::INFORM_WORLD_BOSS_OPEN
			, this, &UGsWorldBossInfoListViewEntry::OnBossRoomReady));
	}
}

void UGsWorldBossInfoListViewEntry::NativeDestruct()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		MBoss& boss = msg->GetBoss();
		if (_bossDelegateArray.Num() > 0)
		{
			for (auto iter : _bossDelegateArray)
			{
				boss.Remove(iter);
			}
			_bossDelegateArray.Empty();
		}

		MUI& ui = msg->GetUI();
		if (_uiDelegateArray.Num() > 0)
		{
			for (auto iter : _uiDelegateArray)
			{
				ui.Remove(iter);
			}
			_uiDelegateArray.Empty();
		}

		MUnlock& msgUnlock = msg->GetUnlock();
		if (_msgUnlockHandleList.Num() > 0)
		{
			for (auto iter : _msgUnlockHandleList)
			{
				msgUnlock.Remove(iter);
			}
			_msgUnlockHandleList.Empty();
		}
	}

	_enterTimer.Stop(GetWorld());
	_enterTimer.Clear();

	Super::NativeDestruct();
}

void UGsWorldBossInfoListViewEntry::NativeOnListItemObjectSet(class UObject* inListItemObject)
{
	UGsWorldBossInfoListViewEntryData* data = Cast<UGsWorldBossInfoListViewEntryData>(inListItemObject);
	if (nullptr == data)
	{
		return;
	}

	_bossData = data->_bossData;
	_level = data->_level;

	const FGsSchemaWorldBossData* selectedBossData = FGsGameStateBoss::GetInstance()->GetSelectedWorldBossData();

	InvalidBossInfo();
	InvalidSelect(selectedBossData);

	CheckActiveTimer();
}

void UGsWorldBossInfoListViewEntry::OnClickListViewEntry()
{	
	FGsGameStateBoss::GetInstance()->SelectedWorldBoss(_bossData->id);
}

void UGsWorldBossInfoListViewEntry::InvalidBossInfo()
{
	//get boss npc id			
	const FGsSchemaNpcData* npcData = _bossData->bossNpcId.GetRow();
	if (npcData)
	{
		_bossNameTextBlock->SetText(npcData->nameText);
		_levelTextBlock->SetText(FText::Format(_levelFormat, _level));

		//set boss icon		
		UPaperSprite* image = Cast<UPaperSprite>(UAssetManager::GetStreamableManager().LoadSynchronous(_bossData->portraitImg));
		_bossIcon->SetBrushFromAtlasInterface(image);
	}

	//get area name
	{
		if (const FGsSchemaMapData* mapData = _bossData->mapId.GetRow())
		{
			FString areaName = _bossData->mapId.GetRow()->MapStringId.ToString();
			_areaNameTextBlock->SetText(FText::FromString(areaName));
		}	
	}

	InvalidLock();	
}

void UGsWorldBossInfoListViewEntry::InvalidLock()
{
	if (nullptr == _bossData)
	{
		_lockPanel->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	const FGsSchemaUnlockContents* contents = _bossData->unlockContentsId.GetRow();
	if (nullptr == contents)
	{
		_lockPanel->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	int32 contentsId = contents->id;
	if (GSUnlock()->IsContentsUnlock(contentsId))
	{
		_lockPanel->SetVisibility(ESlateVisibility::Collapsed);
	
	}
	else
	{
		_lockPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		FText message;
		GSUnlock()->MakeStringUnLockCondition(contentsId, contents->lockType, message);
		_lockDescriptionRichTextBlock->SetText(message);
	}		
}

void UGsWorldBossInfoListViewEntry::CheckActiveTimer()
{
	InvalidActiveEffect();

	FGsWorldBossHandler* worldBossHandler = FGsBossHandler::GetInstance()->GetWorldBossHandler();
	worldBossHandler->TryGetWorldBossInfo(_bossData->id, _bossInfo);
	if (worldBossHandler->IsActiveWorldBoss(_bossData->id))
	{
		_enterTimer.Start(GetWorld(), _bossInfo._mapOpenTime, 0.5f
			, nullptr
			, [this]() {
				InvalidActiveEffect();
			});
	}
}

void UGsWorldBossInfoListViewEntry::InvalidActiveEffect()
{
	if (FGsBossHandler::GetInstance()->GetWorldBossHandler()->IsActiveWorldBoss(_bossData->id))
	{
		_activeEffect->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_activeEffect->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsWorldBossInfoListViewEntry::OnSelectBossInfo(const IGsMessageParam* inParam)
{
	if (nullptr == inParam)
	{
		return;
	}

	const FGsUIMsgParamSelectBoss* selectBoss = inParam->Cast<const FGsUIMsgParamSelectBoss>();
	if (selectBoss)
	{
		InvalidSelect(selectBoss->_worldBossData);
	}

	CheckActiveTimer();
}

void UGsWorldBossInfoListViewEntry::OnBossRoomReady(const struct IGsMessageParam* inParam)
{
	CheckActiveTimer();
}

void UGsWorldBossInfoListViewEntry::OnInvalidUnlock(const struct IGsMessageParam* inParam)
{
	InvalidLock();
}

void UGsWorldBossInfoListViewEntry::InvalidSelect(const struct FGsSchemaWorldBossData* inData)
{
	if (inData == _bossData)
	{
		_selectImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		_selectImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

const FGsSchemaWorldBossData* UGsWorldBossInfoListViewEntry::GetBossData() const
{
	return _bossData;
}

const FGsSchemaNpcData* UGsWorldBossInfoListViewEntry::GetNpcData() const
{
	return _bossData->bossNpcId.GetRow();
}
