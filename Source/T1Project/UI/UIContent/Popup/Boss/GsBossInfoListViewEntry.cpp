// Fill out your copyright notice in the Description page of Project Settings.


#include "GsBossInfoListViewEntry.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/Button.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateBoss.h"
#include "GameObject/Boss/GsFieldBossHandler.h"
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

void UGsBossInfoListViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_button->OnClicked.AddDynamic(this, &UGsBossInfoListViewEntry::OnClickListViewEntry);
	_bookmarkToggleButton->OnClicked.AddDynamic(this, &UGsBossInfoListViewEntry::OnClickBookmark);
}

void UGsBossInfoListViewEntry::NativeConstruct()
{
	Super::NativeConstruct();
	
	FText::FindText(FIELD_BOSS_TEXT_KEY, BOSS_SPAWN_AREA_TEXT_KEY, _levelLimitText);

	if (FGsMessageHolder* msg = GMessage())
	{
		_uiDelegateArray.Emplace(msg->GetUI().AddUObject(MessageUI::INVALID_BOSS
		, this, &UGsBossInfoListViewEntry::OnSelectBossInfo));
	}
}

void UGsBossInfoListViewEntry::NativeDestruct()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_bossDelegateArray.Num() > 0)
		{
			for (auto iter : _bossDelegateArray)
			{
				msg->GetBoss().Remove(iter);
			}
			_bossDelegateArray.Empty();
		}

		if (_uiDelegateArray.Num() > 0)
		{
			for (auto iter : _uiDelegateArray)
			{
				msg->GetUI().Remove(iter);
			}
			_uiDelegateArray.Empty();
		}
	}

	Super::NativeDestruct();
}

void UGsBossInfoListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	UGsBossInfoListViewEntryData* data = Cast<UGsBossInfoListViewEntryData>(inListItemObject);
	if (nullptr == data)
	{
		return;
	}

	_bossData = data->_bossData;
	_mutantBossData = data->_mutantBossData;
	_parent = data->_parent;
	const FGsSchemaFieldBossData* selectedBossData = FGsGameStateBoss::GetInstance()->GetSelectedBossData();

	InvalidBossInfo();
	InvalidSelect(selectedBossData);
	InvalidBookmark();
}

void UGsBossInfoListViewEntry::OnClickListViewEntry()
{
	FGsGameStateBoss::GetInstance()->SelectedFieldBoss(_bossData->bossId);	
}

void UGsBossInfoListViewEntry::InvalidBossInfo()
{
	//get boss npc id			
	const FGsSchemaNpcData* npcData = _bossData->bossNpcId.GetRow();
	if (npcData)
	{
		_bossNameTextBlock->SetText(npcData->nameText);

		//set boss icon
		UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(npcData->shapeId.GetRow()->imageRegionMap);
		_bossIcon->SetBrushFromAtlasInterface(spriteImg);
	}

	//get area name
	{
		if (const FGsSchemaMapData* mapData = _bossData->mapId.GetRow())
		{
			FString areaName = _bossData->mapId.GetRow()->MapStringId.ToString();

			const FGsDungeonGroupBase* groupData = GSDungeon()->FindGroupDataByMapId(mapData->id);
			const FGsDungeonData* dungeonData = GSDungeon()->FindDungeonDataByMapId(mapData->id);			
			if (groupData && dungeonData)
			{
				const FGsSchemaDungeonData* dungeonTableData = dungeonData->GetSchema();
				const FGsSchemaDungeonGroupData* groupTableData = groupData->GetSchemaData();
				if (dungeonTableData && groupTableData)
				{					
					if (EGsDungeonGroupType::Level == groupTableData->dungeonGroupType)
					{
						const FText levelLimitText = FText::Format(_levelLimitText, dungeonTableData->unlockLevel, FText::FromString(areaName));						
						_areaNameTextBlock->SetText(levelLimitText);
					}			
					else
					{
						_areaNameTextBlock->SetText(FText::FromString(areaName));
					}
				}				
			}
			else
			{			
				_areaNameTextBlock->SetText(FText::FromString(areaName));
			}
		}	
	}
}

void UGsBossInfoListViewEntry::InvalidBookmark()
{
	int32 bossId = _bossData->bossId;
	bool isContain = GGameData()->GetUserData()->bossBookMarkArray.Contains(bossId);
	_bookmarkToggleButton->SetIsSelected(isContain);
}

void UGsBossInfoListViewEntry::OnSelectBossInfo(const IGsMessageParam* inParam)
{
	if (nullptr == inParam)
	{
		return;
	}

	const FGsUIMsgParamSelectBoss* selectBoss = inParam->Cast<const FGsUIMsgParamSelectBoss>();
	if (selectBoss)
	{
		InvalidSelect(selectBoss->_fieldBoss);
	}
}

void UGsBossInfoListViewEntry::InvalidSelect(const struct FGsSchemaFieldBossData* inData)
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

void UGsBossInfoListViewEntry::OnClickBookmark()
{
	int32 bossId = _bossData->bossId;
	bool isContain = GGameData()->GetUserData()->bossBookMarkArray.Contains(bossId);
	if (isContain)
	{
		GGameData()->RemoveBossBookmark(bossId);
	}
	else
	{
		GGameData()->AddBossBookmark(bossId);
	}	

	InvalidBookmark();

	//if (_parent.IsValid())
	//{
	//	_parent->InvalidBossList();
	//}

	FGsNetSendServiceBoss::SendReqUpdateFieldBossBookmark(GGameData()->GetUserData()->bossBookMarkArray);
}

const FGsSchemaFieldBossData* UGsBossInfoListViewEntry::GetBossData() const
{
	return _bossData;
}

const FGsSchemaNpcData* UGsBossInfoListViewEntry::GetNpcData() const
{
	return _bossData->bossNpcId.GetRow();
}
